#include "LuaScript.h"

LuaScript::LuaScript(const std::string& filename) {
    L = luaL_newstate();
    if (luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
        //game_system.log->write("Script not loaded:" +  filename, LogManager::ERR);
    } //else game_system.log->write("Script loaded:" + filename);
    luaL_openlibs(L);
    this->filename = filename;
}

LuaScript::~LuaScript() {
    lua_close(L);
}

void LuaScript::close() {
    lua_close(L);
}

std::vector<short int> LuaScript::getShortIntVector(const std::string& name) {
    std::vector<short int> v;
    lua_getglobal(L, name.c_str());
    lua_pushnil(L);
    while(lua_next(L, -2)) {
        v.push_back((short int)lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    return v;
}

std::vector<std::string> LuaScript::getTableKeys(const std::string& name) {
    std::string code = 
        "function getKeys(name) "
        "s = \"\""
        "for k, v in pairs(_G[name]) do "
        "    s = s..k..\",\" "
        "    end "
        "return s "
        "end"; // function for getting table keys
    luaL_loadstring(L, 
        code.c_str()); // execute code
    lua_pcall(L,0,0,0);
    lua_getglobal(L, "getKeys"); // get function
    lua_pushstring(L, name.c_str());
    lua_pcall(L, 1 , 1, 0); // execute function
    std::string test = lua_tostring(L, -1);
    std::vector<std::string> strings;
    std::string temp = "";
    for(unsigned int i = 0; i < test.size(); i++) {     
        if(test.at(i) != ',') {
            temp += test.at(i);
        } else {
            strings.push_back(temp);
            temp= "";
        }
    }
    lua_pop(L, 2);
    return strings;
}