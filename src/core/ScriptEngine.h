#pragma once
#include "Entity.h"
#include <string>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

class ScriptEngine {
public:
    static bool init();
    static void shutdown();

    static void bindEntity(Entity* entity);
    static bool loadScript(const std::string& path);
    static void callUpdate(float deltaTime);

private:
    static lua_State* s_lua;

    static int lua_getPositionX(lua_State* L);
    static int lua_getPositionY(lua_State* L);
    static int lua_setPositionX(lua_State* L);
    static int lua_setPositionY(lua_State* L);
    static int lua_getSizeX(lua_State* L);
    static int lua_getSizeY(lua_State* L);
};