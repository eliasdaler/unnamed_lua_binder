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
    void printError(const std::string& variableName, const std::string& reason);

    template<typename T>
    T get(const std::string& variableName) {
      level = 0;
      if(!L) {
        printError(variableName, "Script is not loaded");
        return lua_getdefault<T>();
      }
      
      bool success = lua_gettostack<T>(variableName);

      T result;
      if(success) {
        result = lua_get<T>(variableName);  
      } else {
        result = lua_getdefault<T>();
      }
      //std::cout<<"Pushing this many objects from stack:"<<level+1<<std::endl;
      lua_pop(L, level + 1); // pop all existing elements from stack     

      return result;
    }

    template <typename T>
    bool lua_gettostack(const std::string& variableName) {
      std::string var = "";
        for(unsigned int i = 0; i < variableName.size(); i++) {
          if(variableName.at(i) == '.') {
            if(level == 0) {
              lua_getglobal(L, var.c_str());
            } else {
              lua_getfield(L, -1, var.c_str());
            }
            
            if(lua_isnil(L, -1)) {
              printError(variableName, var + " is not defined");
              return false;
            } else {
              var = "";
              level++;
            }
          } else {
            var += variableName.at(i);
          }
        }
        if(level == 0) {
          lua_getglobal(L, var.c_str());
        } else {
          lua_getfield(L, -1, var.c_str());
        }
        if(lua_isnil(L, -1)) {
            printError(variableName, var + " is not defined");
            return false;
        }

        return true;       
    }

    // Generic get
    template<typename T>
    T lua_get(const std::string& variableName) {
      return 0;
    }

    template<typename T>
    T lua_getdefault() {
      return 0;
    }
   
private:
    lua_State* L;
    std::string filename;
    int level;
};

 // Specializations

template <> 
inline bool LuaScript::lua_get<bool>(const std::string& variableName) {
    return (bool)lua_toboolean(L, -1);
}

template <> 
inline float LuaScript::lua_get<float>(const std::string& variableName) {
    if(!lua_isnumber(L, -1)) {
      printError(variableName, "Not a number");
    }
    return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaScript::lua_get<int>(const std::string& variableName) {
    if(!lua_isnumber(L, -1)) {
      printError(variableName, "Not a number");
    }
    return (int)lua_tonumber(L, -1);
}

template <>
inline std::string LuaScript::lua_get<std::string>(const std::string& variableName) {
    std::string s = "null";
    if(lua_isstring(L, -1)) {
      s = std::string(lua_tostring(L, -1));
    } else {
      printError(variableName, "Not a string");
    }
    return s;
}

template<>
inline std::string LuaScript::lua_getdefault<std::string>() {
  return "null";
}

#endif