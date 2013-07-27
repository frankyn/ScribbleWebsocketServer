#include <iostream>
#include <list>
#include <string>
#include "ScriptLoader.h"

int test ( lua_State * state ) {
	std::string str = lua_tostring ( state, -1 );
	std::cout<< "LUA ARG: " << str << std::endl;
	lua_pushstring ( state, "TESTING: test() lua to c call" );
	return 1;
}

int main ( ) {
	try {
		ScriptLoader scriptloader;
		SLArg args;
		args.push_back ( "HELLO" );
		args.push_back ( "Hello" );
		scriptloader.addProc ( test, NULL, "test" );
		scriptloader.loadLib ( "testlib.lua" );
		scriptloader.load ( "test.lua" );
		scriptloader.call ( "init" );
		scriptloader.call ( "onMessage", args );
		//Check if stack is saved for runtime variables so we can run an update on the code and not have to create a new ScriptLoader
		scriptloader.load ( "test2.lua" );
		scriptloader.call ( "check" );
	} catch ( std::string e ) {
		std::cout<<"Log:"<<e <<std::endl;
	} catch ( ... ) {
		std::cout<<"Error: "<<std::endl;
	}
	return 0;
};
