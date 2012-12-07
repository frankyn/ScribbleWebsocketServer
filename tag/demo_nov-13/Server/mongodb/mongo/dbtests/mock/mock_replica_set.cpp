/*    Copyright 2012 10gen Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "mongo/dbtests/mock/mock_replica_set.h"

#include "mongo/db/repl/rs_member.h"
#include "mongo/dbtests/mock/mock_dbclient_connection.h"

#include <sstream>

using mongo::BSONObjBuilder;

using std::map;
using std::string;
using std::vector;

namespace mongo_test {
    MockReplicaSet::ReplSetConnHook::ReplSetConnHook(MockReplicaSet* replSet):
            _replSet(replSet) {
    }

    MockReplicaSet::ReplSetConnHook::~ReplSetConnHook() {
    }

    mongo::DBClientBase* MockReplicaSet::ReplSetConnHook::connect(
            const mongo::ConnectionString& connString,
            std::string& errmsg,
            double socketTimeout) {
        vector<mongo::HostAndPort> servers = connString.getServers();
        verify(servers.size() == 1);

        const string serverName = servers.front().toString(true);
        return new MockDBClientConnection(_replSet->getNode(serverName));
    }

    MockReplicaSet::MockReplicaSet(const string& setName, size_t nodes):
            _setName(setName),
            _connStringHook(this) {
        std::vector<mongo::ReplSetConfig::MemberCfg> replConfig;

        for (size_t n = 0; n < nodes; n++) {
            std::stringstream str;
            str << "$" << setName << n << ":27017";
            const string hostName(str.str());

            if (n == 0) {
                _primaryHost = hostName;
            }
            else {
                _secondaryHosts.push_back(hostName);
            }

            _nodeMap[hostName] = new MockRemoteDBServer(hostName);

            mongo::ReplSetConfig::MemberCfg config;
            config.h = mongo::HostAndPort(hostName);
            replConfig.push_back(config);
        }

        setConfig(replConfig);
    }

    MockReplicaSet::~MockReplicaSet() {
        for (std::map<string, MockRemoteDBServer*>::iterator iter = _nodeMap.begin();
                iter != _nodeMap.end(); ++iter) {
            delete iter->second;
        }
    }

    string MockReplicaSet::getSetName() const {
        return _setName;
    }

    string MockReplicaSet::getConnectionString() const {
        std::stringstream str;
        str << _setName;
        str << "/";

        std::map<string, MockRemoteDBServer*>::const_iterator iter = _nodeMap.begin();
        while (iter != _nodeMap.end()) {
            str << iter->second->getServerAddress();
            ++iter;

            if (iter != _nodeMap.end()) {
                str << ",";
            }
        }

        return str.str();
    }

    vector<mongo::HostAndPort> MockReplicaSet::getHosts() const {
        vector<mongo::HostAndPort> list;

        for (std::map<string, MockRemoteDBServer*>::const_iterator iter = _nodeMap.begin();
                iter != _nodeMap.end(); ++iter) {
            list.push_back(mongo::HostAndPort(iter->second->getServerAddress()));
        }

        return list;
    }

    string MockReplicaSet::getPrimary() const {
        return _primaryHost;
    }

    const vector<string>& MockReplicaSet::getSecondaries() const {
        return _secondaryHosts;
    }

    mongo::ConnectionString::ConnectionHook* MockReplicaSet::getConnectionHook() {
        return &_connStringHook;
    }

    MockRemoteDBServer* MockReplicaSet::getNode(const string& hostName) {
        return _nodeMap[hostName];
    }

    const vector<mongo::ReplSetConfig::MemberCfg>& MockReplicaSet::getReplConfig() const {
        return _replConfig;
    }

    void MockReplicaSet::setConfig(const vector<mongo::ReplSetConfig::MemberCfg>& newConfig) {
        _replConfig = newConfig;
        mockIsMasterCmd();
        mockReplSetGetStatusCmd();
    }

    void MockReplicaSet::kill(const string& hostName) {
        verify(_nodeMap.count(hostName) == 1);
        _nodeMap[hostName]->shutdown();
    }

    void MockReplicaSet::restore(const string& hostName) {
        verify(_nodeMap.count(hostName) == 1);
        _nodeMap[hostName]->reboot();
    }

    void MockReplicaSet::mockIsMasterCmd() {
        // Copied from ReplSetImpl::_fillIsMaster
        for (vector<mongo::ReplSetConfig::MemberCfg>::iterator iter = _replConfig.begin();
                iter != _replConfig.end(); ++iter) {
            const string hostName(iter->h.toString(true));
            BSONObjBuilder builder;
            builder.append("setName", _setName);

            const bool isPrimary = hostName == getPrimary();
            builder.append("ismaster", isPrimary);
            builder.append("secondary", !isPrimary);

            {
                // TODO: add passives & arbiters
                vector<string> hostList;
                hostList.push_back(getPrimary());
                for (vector<string>::const_iterator secIter = getSecondaries().begin();
                                secIter != getSecondaries().end(); ++secIter) {
                    hostList.push_back(*secIter);
                }

                builder.append("hosts", hostList);
            }

            builder.append("primary", getPrimary());

            if (iter->arbiterOnly) {
                builder.append("arbiterOnly", true);
            }

            if (iter->priority == 0 && !iter->arbiterOnly) {
                builder.append("passive", true);
            }

            if (iter->slaveDelay) {
                builder.append("slaveDelay", iter->slaveDelay);
            }

            if (iter->hidden) {
                builder.append("hidden", true);
            }

            if (!iter->buildIndexes) {
                builder.append("buildIndexes", false);
            }

            if(!iter->tags.empty()) {
                BSONObjBuilder tagBuilder;
                for(map<string, string>::const_iterator tagIter = iter->tags.begin();
                        tagIter != iter->tags.end(); tagIter++) {
                    tagBuilder.append(tagIter->first, tagIter->second);
                }

                builder.append("tags", tagBuilder.done());
            }

            builder.append("me", hostName);
            builder.append("ok", true);

            getNode(hostName)->setCommandReply("ismaster", builder.done());
        }
    }

    int MockReplicaSet::getState(const std::string& host) const {
        if (host == getPrimary()) {
            return static_cast<int>(mongo::MemberState::RS_PRIMARY);
        }
        else {
            return static_cast<int>(mongo::MemberState::RS_SECONDARY);
        }
    }

    void MockReplicaSet::mockReplSetGetStatusCmd() {
        // Copied from ReplSetImpl::_summarizeStatus
        for (std::map<string, MockRemoteDBServer*>::iterator nodeIter = _nodeMap.begin();
                        nodeIter != _nodeMap.end(); ++nodeIter) {
            MockRemoteDBServer* node = nodeIter->second;
            vector<mongo::BSONObj> hostsField;

            BSONObjBuilder fullStatBuilder;

            {
                BSONObjBuilder selfStatBuilder;
                selfStatBuilder.append("name", node->getServerAddress());
                selfStatBuilder.append("health", 1.0);
                selfStatBuilder.append("state", getState(node->getServerAddress()));

                selfStatBuilder.append("self", true);
                // TODO: _id, stateStr, uptime, optime, optimeDate, maintenanceMode, errmsg

                hostsField.push_back(selfStatBuilder.obj());
            }

            for (std::map<string, MockRemoteDBServer*>::iterator hostNodeIter = _nodeMap.begin();
                    hostNodeIter != _nodeMap.end(); ++hostNodeIter) {
                MockRemoteDBServer* hostNode = hostNodeIter->second;

                if (hostNode == node) {
                    continue;
                }

                BSONObjBuilder hostMemberBuilder;

                // TODO: _id, stateStr, uptime, optime, optimeDate, lastHeartbeat, pingMs
                // errmsg, authenticated

                hostMemberBuilder.append("name", hostNode->getServerAddress());
                const double health = hostNode->isRunning() ? 1.0 : 0.0;
                hostMemberBuilder.append("health", health);
                hostMemberBuilder.append("state", getState(hostNode->getServerAddress()));

                hostsField.push_back(hostMemberBuilder.obj());
            }

            sort(hostsField.begin(), hostsField.end());

            // TODO: syncingTo

            fullStatBuilder.append("set", _setName);
            fullStatBuilder.appendTimeT("date", time(0));
            fullStatBuilder.append("myState", getState(node->getServerAddress()));
            fullStatBuilder.append("members", hostsField);
            fullStatBuilder.append("ok", true);

            node->setCommandReply("replSetGetStatus", fullStatBuilder.done());
        }
    }
}
