#include "ScriptEngine.h"
#include <iostream>

lua_State* ScriptEngine::s_lua = nullptr;
static Entity* s_boundEntity = nullptr;

bool ScriptEngine::init() {
    s_lua = luaL_newstate();
    luaL_openlibs(s_lua);

    lua_register(s_lua, "getPositionX", lua_getPositionX);
    lua_register(s_lua, "getPositionY", lua_getPositionY);
    lua_register(s_lua, "setPositionX", lua_setPositionX);
    lua_register(s_lua, "setPositionY", lua_setPositionY);
    lua_register(s_lua, "getSizeX",     lua_getSizeX);
    lua_register(s_lua, "getSizeY",     lua_getSizeY);

    return true;
}

void ScriptEngine::shutdown() {
    if (s_lua) {
        lua_close(s_lua);
        s_lua = nullptr;
    }
}

void ScriptEngine::bindEntity(Entity* entity) {
    s_boundEntity = entity;
}

bool ScriptEngine::loadScript(const std::string& path) {
    if (luaL_dofile(s_lua, path.c_str()) != LUA_OK) {
        std::cerr << "Erro ao carregar script: " << lua_tostring(s_lua, -1) << std::endl;
        lua_pop(s_lua, 1);
        return false;
    }
    return true;
}

void ScriptEngine::callUpdate(float deltaTime) {
    lua_getglobal(s_lua, "update");
    if (lua_isfunction(s_lua, -1)) {
        lua_pushnumber(s_lua, deltaTime);
        if (lua_pcall(s_lua, 1, 0, 0) != LUA_OK) {
            std::cerr << "Erro no script update: " << lua_tostring(s_lua, -1) << std::endl;
            lua_pop(s_lua, 1);
        }
    } else {
        lua_pop(s_lua, 1);
    }
}

int ScriptEngine::lua_getPositionX(lua_State* L) {
    if (s_boundEntity) lua_pushnumber(L, s_boundEntity->transform.position.x);
    else lua_pushnumber(L, 0);
    return 1;
}

int ScriptEngine::lua_getPositionY(lua_State* L) {
    if (s_boundEntity) lua_pushnumber(L, s_boundEntity->transform.position.y);
    else lua_pushnumber(L, 0);
    return 1;
}

int ScriptEngine::lua_setPositionX(lua_State* L) {
    if (s_boundEntity) s_boundEntity->transform.position.x = (float)lua_tonumber(L, 1);
    return 0;
}

int ScriptEngine::lua_setPositionY(lua_State* L) {
    if (s_boundEntity) s_boundEntity->transform.position.y = (float)lua_tonumber(L, 1);
    return 0;
}

int ScriptEngine::lua_getSizeX(lua_State* L) {
    if (s_boundEntity) lua_pushnumber(L, s_boundEntity->transform.size.x);
    else lua_pushnumber(L, 0);
    return 1;
}

int ScriptEngine::lua_getSizeY(lua_State* L) {
    if (s_boundEntity) lua_pushnumber(L, s_boundEntity->transform.size.y);
    else lua_pushnumber(L, 0);
    return 1;
}