#include <iostream>
#include <string>
#include <list>
#include "../common/Logger.h"
extern "C" {
        #include <lua5.1/lua.h>
        #include <lua5.1/lualib.h>
        #include <lua5.1/lauxlib.h>
}


#ifndef SCRIPT_LOADER_HEADER
#define SCRIPT_LOADER_HEADER
#define SLArg std::list<std::string>
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
		int loadText ( std::string );
		int loadLib ( std::string );
		int addProc ( lua_CFunction, void * , const std::string );
		void call ( std::string );
		void call ( std::string, SLArg );
	private:			
		Lua_State state;
		
};

#endif
