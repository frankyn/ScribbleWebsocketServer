#include <iostream>
#include <string>
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
		std::string onmessage ( std::string );
		std::string onconnect ( int );
		std::string ondisconnect ( int );
	private:			
		Lua_State state;
		int i;

		
};

#endif
