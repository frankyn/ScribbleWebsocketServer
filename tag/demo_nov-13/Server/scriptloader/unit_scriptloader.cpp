#include <iostream>
#include "ScriptLoader.h"

int test ( lua_State * state ) {
	std::string str = lua_tostring ( state, -1 );
	lua_pushstring ( state, "TESTING: test() lua to c call" );
	return 1;
}

int main ( ) {
	try {
		ScriptLoader scriptloader;
		scriptloader.load ( "test.lua" );
		scriptloader.addProc ( test, "test" );
		std::cout<<scriptloader.call ( "onmessage", "Frank is cool" )<<std::endl;
	} catch ( ... ) {
		std::cout<<"Error"<<std::endl;
	}
	return 0;
};
