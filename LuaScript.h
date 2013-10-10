#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <string>
#include <vector>
#include <iostream>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
# include "lualib.h"
}

class LuaScript {
public:
    LuaScript(const std::string& filename);
    ~LuaScript();
    void close();
    std::vector<short int> getShortIntVector(const std::string& name);
    std::vector<std::string> getTableKeys(const std::string& name);

    template<typename T>
    T get(const std::string& variableName) {
      int level = 0;
      std::string var = "";
      for(unsigned int i = 0; i < variableName.size(); i++) {
        if(variableName.at(i) == '.') {
          if(level == 0) {
            lua_getglobal(L, var.c_str());
          } else {
            lua_getfield(L, -1, var.c_str());
          }
          var = "";
          level++;
        } else {
          var += variableName.at(i);
        }
      }
      if(level == 0) {
        lua_getglobal(L, var.c_str());
      } else {
          lua_getfield(L, -1, var.c_str());
      }

      T result = lua_get<T>(variableName);
      lua_pop(L, level + 1); // pop all existing elements from stack
      return result;
    }

    // Generic get
    template<typename T>
    T lua_get(const std::string& variableName) {
      return 0;
    }
   
private:
    lua_State* L;
    std::string filename;
};

 // Specializations

template <> 
inline bool LuaScript::lua_get<bool>(const std::string& variableName) {
    return (bool)lua_toboolean(L, -1);
}

template <> 
inline float LuaScript::lua_get<float>(const std::string& variableName) {
    if(!lua_isnumber(L, -1)) {
      std::cout<<"Error: not a number ("<<variableName<<")"<<std::endl;
    }
    return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaScript::lua_get<int>(const std::string& variableName) {
    if(!lua_isnumber(L, -1)) {
      std::cout<<"Error: not a number ("<<variableName<<")"<<std::endl;
    }
    return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaScript::lua_get<std::string>(const std::string&    variableName) {
    std::string s = "null";
    if(lua_isstring(L, -1)) {
      s = std::string(lua_tostring(L, -1));
    } else {
      std::cout<<"Error: can't parse string ("<<variableName<<")."<<std::endl;
    }
    return s;
}
#endif