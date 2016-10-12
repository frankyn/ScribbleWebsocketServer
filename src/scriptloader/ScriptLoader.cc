#include "ScriptLoader.h"

ScriptLoader::ScriptLoader() { 
    luaL_openlibs(m_state);
}

ScriptLoader::~ScriptLoader() {   
  //Lua_State is closed by the container class.
  //Look at the header file.
}

int ScriptLoader::load ( std::string filename ) { 
  Log("ScriptLoader: LOAD " + filename);
  luaL_loadfile(m_state, filename.c_str());
  int err_num = lua_pcall(m_state, 0, 0, 0);
  
  if (err_num != 0) {
    std::string err = lua_tostring(m_state, -1);
 
    if (err.empty()) {
      err = "unrecongnized Lua error";
    }
    Log("ScriptLoader: LOAD ERROR " + err);
  }

  return 0;
}   

int ScriptLoader::loadText(std::string script) {
  Log("ScriptLoader: SCRIPT [Not provided due to length]");
  luaL_loadstring(m_state, script.c_str());
  int err_num = lua_pcall(m_state, 0, 0, 0);

  if (err_num != 0) {
    std::string err = lua_tostring(m_state, -1);

    if (err.empty()) {
      err = "unrecongnized Lua error";
    }

    Log("ScriptLoader: LOAD ERROR " + err);
  } 
  return 0;

}

int ScriptLoader::loadLib(std::string filename) {
  Log("ScriptLoader: LOAD LIB " + filename);
  int err_num = luaL_dofile(m_state, filename.c_str());
  
  if (err_num != 0) {
    std::string err = lua_tostring(m_state, -1);

    if (err.empty()) {
      err = "unrecongnized Lua error";
    }

    Log("ScriptLoader: LOAD LIB ERROR " + err);
  } 

  return 0;
}

int ScriptLoader::addProc(lua_CFunction fn, 
                          void * arg, 
                          const std::string fn_name) {
  Log("ScriptLoader: REGISTER " + fn_name);
  lua_pushlightuserdata(m_state, arg);
  lua_pushcclosure( m_state, fn, 1);
  lua_setglobal( m_state, fn_name.c_str());

  return 1;
}

void ScriptLoader::call(std::string func_name) {
  Log("ScriptLoader: CALL " + func_name);
  lua_getglobal(m_state, func_name.c_str());
  int err_num = lua_pcall ( m_state, 0, 0, 0 );
  
  if (err_num != 0) {
    std::string err = lua_tostring(m_state, -1);

    if (err.empty()) {
      err = "unrecongnized Lua error";
    }

    Log("ScriptLoader: CALL ERROR " + err);
  }
}

void ScriptLoader::call(std::string func_name, SLArg args) {
  Log("ScriptLoader: CALL " + func_name + " (..args..)");
  std::string response;

  lua_getglobal(m_state, func_name.c_str());
  
  for (SLArg::iterator it = args.begin(); it!=args.end(); ++it) {
    lua_pushstring(m_state, it->c_str()); 
  }

  int err_num = lua_pcall(m_state, args.size(), 0, 0);
  if (err_num != 0) {
    std::string err = lua_tostring(m_state, -1);

    if (err.empty ()) {
      err = "unrecongnized Lua error";
    }

    Log("ScriptLoader: CALL ERROR " + err);
  }
}

void ScriptLoader::focusVar(std::string var_name) {
  lua_getglobal(m_state, var_name.c_str());

  if (!lua_istable(m_state, -1)) {
    Log("ScriptLoader: Value not found");
  }
}

std::string ScriptLoader::getTableValue_str(std::string key) {
  std::string result;
  lua_pushstring(m_state, key.c_str());
  lua_gettable(m_state, -2);  /* get Table[key] */

  if (!lua_isstring(m_state, -1)) {
    Log ( "ScriptLoader: value not a string" );   
    return result;
  }

  result = lua_tostring(m_state, -1);
  lua_pop(m_state, 1);  /* remove string */

  return result;
}

int ScriptLoader::getTableValue_int(std::string key) {
  int result=-1;

  lua_pushstring(m_state, key.c_str());
  lua_gettable(m_state, -2);  /* get Table[key] */

  if (!lua_isnumber(m_state, -1)) {
    Log ( "ScriptLoader: value not a number" );   

    return result;
  }

  result = lua_tonumber(m_state, -1);
  lua_pop(m_state, 1);  /* remove number */

  return result;
}

int ScriptLoader::getValue_int(std::string key) {
  focusVar(key);

  if ( !lua_isnumber(m_state, -1)) {
    Log("ScriptLoader: value not a number");
    return -1;
  } 

  return lua_tonumber(m_state, -1);
}

