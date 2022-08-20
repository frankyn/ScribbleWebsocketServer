#include <iostream>
#include <list>
#include <string>
#include "ScriptLoader.h"

int test(lua_State *state) {
    std::string str = lua_tostring(state, -1);
    std::cout << "LUA ARG: " << str << std::endl;
    lua_pushstring(state, "TESTING: test() lua to c call");
    return 1;
}

int main() {
    ScriptLoader scriptLoader;
    SLArg args;
    args.push_back("HELLO");
    args.push_back("Hello");
    scriptLoader.addProc(test, NULL, "test");
    scriptLoader.loadLib("testlib.lua");
    scriptLoader.load("test.lua");
    scriptLoader.call("init");
    scriptLoader.call("onMessage", args);

    //Check if stack is saved for runtime variables
    // so we can run an update on the code and not
    // have to create a new ScriptLoader
    scriptLoader.load("test2.lua");
    scriptLoader.call("check");

    return 0;
};
