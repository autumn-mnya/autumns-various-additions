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
#include "Lua_Money.h"

#include "../Main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../AutPI.h"

#include "../Collectables.h"

static int lua_GetMoney(lua_State* L)
{
	lua_pushnumber(L, playerMoney);

	return 1;
}

static int lua_SetMoney(lua_State* L)
{
	int num = (int)luaL_checknumber(L, 1);

	playerMoney = num;

	return 0;
}

FUNCTION_TABLE MoneyFunctionTable[FUNCTION_TABLE_MONEY_SIZE] =
{
	{"Get", lua_GetMoney},
	{"Set", lua_SetMoney}
};