#include "ScriptLoader.h"

ScriptLoader::ScriptLoader () { 

}

ScriptLoader::~ScriptLoader() { 	
	//Lua_State is closed by the container class.
	//Look at the header file.
}

int ScriptLoader::load ( std::string filename ) { 
	this->i = 0;
    luaL_openlibs ( state );

    luaL_loadfile ( state , filename.c_str() );
    lua_pcall ( state , 0, 0, 0 );	

	lua_getglobal ( state, "init" );
	lua_pcall ( state, 0, 0, 0 );

	return 0;
}		

int ScriptLoader::addProc ( lua_CFunction fn, void * arg, const std::string fn_name ) {
	//lua_register ( state, fn_name.c_str(), fn);
	//lua_pushstring( state, fn_name.c_str());
	lua_pushlightuserdata ( state,  arg);
    lua_pushcclosure( state, fn, 1);
    lua_setglobal( state, fn_name.c_str());
	return 1;
}

std::string ScriptLoader::call ( std::string func_name, std::string input ) {
	std::string response;

	lua_getglobal ( state , func_name.c_str() ) ;
	
    lua_pushstring ( state , input.c_str() );
    lua_pcall ( state, 1, 1, 0 );
	
	response = std::string ( lua_tostring ( state , -1 ) );

	return response;
}

