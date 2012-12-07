#include "ScriptLoader.h"

ScriptLoader::ScriptLoader () { 
};

ScriptLoader::~ScriptLoader() { 
};

int ScriptLoader::load ( std::string filename ) { 
	this->i = 0;
        luaL_openlibs ( state );

        luaL_loadfile ( state , filename.c_str() );
        lua_pcall ( state , 0, 0, 0 );	
	
	lua_getglobal ( state, "init" );
	lua_pcall ( state, 0, 0, 0 );

	return 0;

}		

std::string ScriptLoader::onmessage ( std::string input ) { 
	std::string response;	

	lua_getglobal ( state , "onmessage");
	
        lua_pushstring ( state , input.c_str() );
        lua_pcall ( state, 1, 1, 0 );
	response = std::string ( lua_tostring ( state , -1 ) );

	return response;
}

std::string ScriptLoader::onconnect ( int descriptor ) {
	std::string response;
	lua_getglobal ( state , "onconnect" );

	lua_pushnumber ( state , descriptor );
        lua_pcall ( state, 1, 1, 0 );

	response = std::string ( lua_tostring ( state , -1 ) );
	return response;
}

std::string ScriptLoader::ondisconnect ( int descriptor ) { 
	std::string response;
	lua_getglobal ( state , "ondisconnect" );
	
	lua_pushnumber ( state, descriptor );
        lua_pcall ( state, 1, 1, 0);

	response = std::string ( lua_tostring (state , -1 ) );
	return response;
}
