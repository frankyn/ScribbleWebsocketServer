#include <iostream>
#include <string>
#include <map>

extern "C" {
        #include <lua5.1/lua.h>
        #include <lua5.1/lualib.h>
        #include <lua5.1/lauxlib.h>
}


#ifndef SCRIPT_LOADER_HEADER
#define SCRIPT_LOADER_HEADER

class Lua_State {		
		lua_State * L;
	public:  
		Lua_State ( ) : L ( lua_open ( ) ) { }
		~Lua_State ( ) { lua_close ( L ); }
	
		inline operator lua_State*( ) {
			return L;
		}

};

class ScriptLoader {
	public:
		ScriptLoader();
		~ScriptLoader();
		
		int load ( std::string );
		int addProc ( lua_CFunction, void * , const std::string );
		std::string call ( std::string, std::string );
	private:			
		Lua_State state;
		int i;
		
};

#endif
