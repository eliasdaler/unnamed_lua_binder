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