#include "common/Logger.h"
#include <iostream>
#include <list>
#include <string>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#ifndef SCRIPT_LOADER_HEADER
#define SCRIPT_LOADER_HEADER
#define SLArg std::list<std::string>

class Lua_State {
public:
  Lua_State() : m_state(lua_open()) {}

  ~Lua_State() { lua_close(m_state); }

  inline operator lua_State *() { return m_state; }

private:
  lua_State *m_state;
};

class ScriptLoader {
public:
  ScriptLoader();

  ~ScriptLoader();

  void call(std::string);

  void call(std::string, SLArg);

  void focusVar(std::string);

  int load(std::string);

  int loadText(std::string);

  int loadLib(std::string);

  int addProc(lua_CFunction, void *, const std::string);

  int getTableValue_int(std::string);

  int getValue_int(std::string);

  std::string getTableValue_str(std::string);

private:
  Lua_State m_state;
};

#endif