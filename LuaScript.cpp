#include "LuaScript.h"

LuaScript::LuaScript(const std::string& filename) {
    L = luaL_newstate();
    if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
        std::cout<<"Error: failed to load ("<<filename<<")"<<std::endl;
		L = 0;
    }
}

LuaScript::~LuaScript() {
	if(L) lua_close(L);
}

void LuaScript::printError(const std::string& variableName, const std::string& reason) {
	std::cout<<"Error: can't get ["<<variableName<<"]. "<<reason<<std::endl;
}

std::vector<int> LuaScript::getIntVector(const std::string& name) {
    std::vector<int> v;
    lua_getglobal(L, name.c_str());
    if(lua_isnil(L, -1)) { // array is not found
        return std::vector<int>();
    }
    lua_pushnil(L);
    while(lua_next(L, -2)) {
        v.push_back((int)lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    clean();
    return v;
}