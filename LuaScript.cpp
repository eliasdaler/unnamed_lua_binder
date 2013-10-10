#include "LuaScript.h"

LuaScript::LuaScript(const std::string& filename) {
    L = luaL_newstate();
    if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
        std::cout<<"Error: script not loaded"<<filename<<")"<<std::endl;
    }
}

LuaScript::~LuaScript() {
    lua_close(L);
}