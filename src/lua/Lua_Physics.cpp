#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "Lua.h"
#include "Lua_Physics.h"

#include "../Main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../AutPI.h"

#include "../MyChar.h"

static int lua_SetPhysics(lua_State* L)
{
	int x = (int)luaL_checknumber(L, 1);
	int y = (int)luaL_checknumber(L, 2);
	SetPlayerPhysicsValues(x, y);
	return 0;
}

static int lua_GetPhysics(lua_State* L)
{
	int x = (int)luaL_checknumber(L, 1);

	lua_pushnumber(L, GetPlayerPhysicsValue(x));

	return 1;
}

FUNCTION_TABLE PhysicsFunctionTable[FUNCTION_TABLE_PHYSICS_SIZE] =
{
	{"Set", lua_SetPhysics},
	{"Get", lua_GetPhysics}
};