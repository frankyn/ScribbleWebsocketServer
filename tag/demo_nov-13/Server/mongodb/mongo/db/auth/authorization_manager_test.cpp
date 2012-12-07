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

/**
 * Unit tests of the AuthorizationManager type.
 */

#include "mongo/db/auth/authorization_manager.h"

#include "mongo/base/status.h"
#include "mongo/db/auth/external_state_mock.h"
#include "mongo/db/jsobj.h"
#include "mongo/unittest/unittest.h"

#define ASSERT_NULL(EXPR) ASSERT_FALSE(EXPR)
#define ASSERT_NON_NULL(EXPR) ASSERT_TRUE(EXPR)

namespace mongo {
namespace {

    TEST(AuthorizationManagerTest, AcquirePrivilegeAndCheckAuthorization) {
        Principal* principal = new Principal("Spencer");
        ActionSet actions;
        actions.addAction(ActionType::insert);
        AcquiredPrivilege writePrivilege(Privilege("test", actions), principal);
        AcquiredPrivilege allDBsWritePrivilege(Privilege("*", actions), principal);
        ExternalStateMock* externalState = new ExternalStateMock();
        AuthorizationManager authManager(externalState);

        ASSERT_NULL(authManager.checkAuthorization("test", ActionType::insert));
        externalState->setReturnValueForShouldIgnoreAuthChecks(true);
        ASSERT_EQUALS("special",
                      authManager.checkAuthorization("test", ActionType::insert)->getName());
        externalState->setReturnValueForShouldIgnoreAuthChecks(false);
        ASSERT_NULL(authManager.checkAuthorization("test", ActionType::insert));

        ASSERT_EQUALS(ErrorCodes::UserNotFound,
                      authManager.acquirePrivilege(writePrivilege).code());
        authManager.addAuthorizedPrincipal(principal);
        ASSERT_OK(authManager.acquirePrivilege(writePrivilege));
        ASSERT_EQUALS(principal, authManager.checkAuthorization("test", ActionType::insert));

        ASSERT_NULL(authManager.checkAuthorization("otherDb", ActionType::insert));
        ASSERT_OK(authManager.acquirePrivilege(allDBsWritePrivilege));
        ASSERT_EQUALS(principal, authManager.checkAuthorization("otherDb", ActionType::insert));
    }

    TEST(AuthorizationManagerTest, GrantInternalAuthorization) {
        ExternalStateMock* externalState = new ExternalStateMock();
        AuthorizationManager authManager(externalState);

        ASSERT_NULL(authManager.checkAuthorization("test", ActionType::insert));
        ASSERT_NULL(authManager.checkAuthorization("test", ActionType::replSetHeartbeat));

        authManager.grantInternalAuthorization();

        ASSERT_NON_NULL(authManager.checkAuthorization("test", ActionType::insert));
        ASSERT_NON_NULL(authManager.checkAuthorization("test", ActionType::replSetHeartbeat));
    }

    TEST(AuthorizationManagerTest, GetPrivilegesFromPrivilegeDocument) {
        Principal* principal = new Principal("Spencer");
        BSONObj invalid;
        BSONObj readWrite = BSON("user" << "Spencer" << "pwd" << "passwordHash");
        BSONObj readOnly = BSON("user" << "Spencer" << "pwd" << "passwordHash" <<
                                "readOnly" << true);

        PrivilegeSet privilegeSet;
        ASSERT_EQUALS(ErrorCodes::UnsupportedFormat,
                      AuthorizationManager::buildPrivilegeSet("test",
                                                               principal,
                                                               invalid,
                                                               &privilegeSet).code());

        ASSERT_OK(AuthorizationManager::buildPrivilegeSet("test",
                                                           principal,
                                                           readOnly,
                                                           &privilegeSet));
        ASSERT_NULL(privilegeSet.getPrivilegeForAction("test", ActionType::insert));
        ASSERT_NON_NULL(privilegeSet.getPrivilegeForAction("test", ActionType::find));

        ASSERT_OK(AuthorizationManager::buildPrivilegeSet("test",
                                                           principal,
                                                           readWrite,
                                                           &privilegeSet));
        ASSERT_NON_NULL(privilegeSet.getPrivilegeForAction("test", ActionType::find));
        ASSERT_NON_NULL(privilegeSet.getPrivilegeForAction("test", ActionType::insert));
        ASSERT_NON_NULL(privilegeSet.getPrivilegeForAction("test", ActionType::userAdmin));
        ASSERT_NON_NULL(privilegeSet.getPrivilegeForAction("test", ActionType::compact));
        ASSERT_NULL(privilegeSet.getPrivilegeForAction("test", ActionType::shutdown));
        ASSERT_NULL(privilegeSet.getPrivilegeForAction("test", ActionType::addShard));

        ASSERT_NULL(privilegeSet.getPrivilegeForAction("admin", ActionType::find));
        ASSERT_NULL(privilegeSet.getPrivilegeForAction("*", ActionType::find));
        ASSERT_OK(AuthorizationManager::buildPrivilegeSet("admin",
                                                           principal,
                                                           readOnly,
                                                           &privilegeSet));
        ASSERT_NON_NULL(privilegeSet.getPrivilegeForAction("admin", ActionType::find));
        ASSERT_NON_NULL(privilegeSet.getPrivilegeForAction("*", ActionType::find));

        ASSERT_NULL(privilegeSet.getPrivilegeForAction("admin", ActionType::insert));
        ASSERT_NULL(privilegeSet.getPrivilegeForAction("*", ActionType::insert));
        ASSERT_OK(AuthorizationManager::buildPrivilegeSet("admin",
                                                           principal,
                                                           readWrite,
                                                           &privilegeSet));
        ASSERT_NON_NULL(privilegeSet.getPrivilegeForAction("admin", ActionType::insert));
        ASSERT_NON_NULL(privilegeSet.getPrivilegeForAction("*", ActionType::insert));
    }

}  // namespace
}  // namespace mongo
