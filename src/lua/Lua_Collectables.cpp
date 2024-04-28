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
#include "Lua_Collectables.h"

#include "../Main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../AutPI.h"

#include "../Collectables.h"

static int lua_GetCollectable(lua_State* L)
{
	int type = (int)luaL_checknumber(L, 1);

	lua_pushnumber(L, GetCollectable(type));

	return 1;
}

static int lua_SetCollectable(lua_State* L)
{
	int type = (int)luaL_checknumber(L, 1);
	int num = (int)luaL_checknumber(L, 2);

	SetCollectables(type, num);

	return 0;
}

FUNCTION_TABLE CollectablesFunctionTable[FUNCTION_TABLE_COLLECTABLES_SIZE] =
{
	{"Get", lua_GetCollectable},
	{"Set", lua_SetCollectable}
};