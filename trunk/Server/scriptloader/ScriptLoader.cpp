#include "ScriptLoader.h"

ScriptLoader::ScriptLoader () { 
    luaL_openlibs ( state );
}

ScriptLoader::~ScriptLoader() { 	
	//Lua_State is closed by the container class.
	//Look at the header file.
}

int ScriptLoader::load ( std::string filename ) { 
	Log ( "ScriptLoader: LOAD " + filename );
    luaL_loadfile ( state , filename.c_str() );
    int err_num = lua_pcall ( state, 0, 0, 0 );
    if ( err_num != 0 ) {
		std::string err = lua_tostring ( state, -1 );
		if ( err.empty () ) {
			err = "unrecongnized Lua error";;
		}
		Log ( "ScriptLoader: LOAD ERROR " + err );
	}
	return 0;
}		

int ScriptLoader::loadText ( std::string script ) {
	Log ( "ScriptLoader: SCRIPT [Not provided due to length]" );
	luaL_loadstring ( state, script.c_str() );
	int err_num = lua_pcall ( state, 0, 0, 0 );
	if ( err_num != 0 ) {
		std::string err = lua_tostring ( state, -1 );
		if ( err.empty () ) {
			err = "unrecongnized Lua error";;
		}
		Log ( "ScriptLoader: LOAD ERROR " + err );
	} 
	return 0;
}

int ScriptLoader::loadLib ( std::string filename ) {
	Log ( "ScriptLoader: LOAD LIB " + filename );
	int err_num = luaL_dofile ( state, filename.c_str() );
	
	if ( err_num != 0 ) {
		std::string err = lua_tostring ( state, -1 );
		if ( err.empty () ) {
			err = "unrecongnized Lua error";;
		}
		Log ( "ScriptLoader: LOAD LIB ERROR " + err );
	} 
	return 0;
}

int ScriptLoader::addProc ( lua_CFunction fn, void * arg, const std::string fn_name ) {
	Log ( "ScriptLoader: REGISTER " + fn_name );
	lua_pushlightuserdata ( state,  arg);
    lua_pushcclosure( state, fn, 1);
    lua_setglobal( state, fn_name.c_str());
	return 1;
}

void ScriptLoader::call ( std::string func_name ) {
	//Log ( "ScriptLoader: CALL " + func_name );
	lua_getglobal ( state , func_name.c_str() ) ;
    int err_num = lua_pcall ( state, 0, 0, 0 );
	
	if ( err_num != 0 ) {
		std::string err = lua_tostring ( state, -1 );
		if ( err.empty () ) {
			err = "unrecongnized Lua error";;
		}
		Log ( "ScriptLoader: CALL ERROR " + err );
	}
}

void ScriptLoader::call ( std::string func_name, SLArg args ) {
	//Log ( "ScriptLoader: CALL " + func_name );
	std::string response;

	lua_getglobal ( state , func_name.c_str() ) ;
	
	for ( std::list<std::string>::iterator it = args.begin(); it!=args.end(); it++ ) {
		lua_pushstring ( state , it->c_str() );	
	}

    int err_num = lua_pcall ( state, args.size(), 1, 0 );
	if ( err_num != 0 ) {
		std::string err = lua_tostring ( state, -1 );
		if ( err.empty () ) {
			err = "unrecongnized Lua error";;
		}
		Log ( "ScriptLoader: CALL ERROR " + err );
	}
}

