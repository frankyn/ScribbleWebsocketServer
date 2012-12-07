/**
 *    Copyright (C) 2012 10gen Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mongo/s/cluster_constants.h"

namespace mongo {

    const string ConfigNS::shard = "config.shards";
    BSONField<string> ShardFields::name("_id");
    BSONField<string> ShardFields::host("host");
    BSONField<bool> ShardFields::draining("draining");
    BSONField<long long> ShardFields::maxSize("maxSize");

    const string ConfigNS::database = "config.databases";
    BSONField<string> DatabaseFields::name("_id");
    BSONField<bool> DatabaseFields::partitioned("partitioned");
    BSONField<string> DatabaseFields::primary("primary");
    BSONField<string> DatabaseFields::DEPRECATED_name("name");
    BSONField<bool> DatabaseFields::DEPRECATED_sharded("sharded");
    BSONField<string> DatabaseFields::NEW_shard("shard");
    BSONField<bool> DatabaseFields::NEW_draining("draining");
    BSONField<bool> DatabaseFields::NEW_scatterCollections("scatterCollections");

    const string ConfigNS::collection = "config.collections";
    BSONField<string> CollectionFields::name("_id");
    BSONField<string> CollectionFields::shard("shard");
    BSONField<BSONObj> CollectionFields::key("key");
    BSONField<bool> CollectionFields::unique("unique");
    BSONField<Date_t> CollectionFields::lastmod("lastmod");
    BSONField<bool> CollectionFields::dropped("dropped");
    BSONField<bool> CollectionFields::noBalance("noBalance");

    const string ConfigNS::chunk = "config.chunks";
    BSONField<string> ChunkFields::name("_id");
    BSONField<string> ChunkFields::ns("ns");
    BSONField<BSONObj> ChunkFields::min("min");
    BSONField<BSONObj> ChunkFields::max("max");
    BSONField<string> ChunkFields::lastmod("lastmod");
    BSONField<string> ChunkFields::shard("shard");
    BSONField<bool> ChunkFields::jumbo("jumbo");
    BSONField<OID> ChunkFields::lastmodEpoch("lastmodEpoch");
    BSONField<BSONArray> ChunkFields::NEW_lastmod("lastmod");

    const string ConfigNS::tag = "config.tags";
    BSONField<string> TagFields::ns("ns");
    BSONField<string> TagFields::tag("tag");
    BSONField<BSONObj> TagFields::min("min");
    BSONField<BSONObj> TagFields::max("max");

    // ============  below not yet hooked  ============

    const string ConfigNS::mongos = "config.mongos";
    BSONField<string> MongosFields::UNHOOKED_name("_id");
    BSONField<Date_t> MongosFields::UNHOOKED_ping("ping");
    BSONField<int> MongosFields::UNHOOKED_up("up");
    BSONField<bool> MongosFields::UNHOOKED_waiting("waiting");

    const string ConfigNS::settings = "config.settings";
    BSONField<int> SettingFields::UNHOOKED_name("_id");
    BSONField<string> SettingFields::UNHOOKED_value("value");

    const string ConfigNS::changelog = "config.changelog";
    BSONField<string> ChangelogFields::UNHOOKED_name("_id");
    BSONField<string> ChangelogFields::UNHOOKED_server("server");
    BSONField<string> ChangelogFields::UNHOOKED_clientAddr("clientAddr");
    BSONField<Date_t> ChangelogFields::UNHOOKED_time("time");
    BSONField<string> ChangelogFields::UNHOOKED_what("what");
    BSONField<string> ChangelogFields::UNHOOKED_ns("ns");
    BSONField<string> ChangelogFields::UNHOOKED_details("details");

    const string ConfigNS::locks = "config.locks";
    BSONField<string> LockFields::UNHOOKED_name("_id");
    BSONField<int> LockFields::UNHOOKED_state("state");
    BSONField<Date_t> LockFields::UNHOOKED_ts("ts");
    BSONField<string> LockFields::UNHOOKED_who("who");

    const string ConfigNS::lockpings = "config.lockpings";
    BSONField<string> LockPingFields::UNHOOKED_name("_id");
    BSONField<Date_t> LockPingFields::UNHOOKED_ping("ping");

} // namespace mongo
