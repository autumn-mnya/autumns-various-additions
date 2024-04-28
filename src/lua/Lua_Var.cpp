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
#include "Lua_Var.h"

#include "../Main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../AutPI.h"

#include "../TextScriptVar.h"

static int lua_GetVariable(lua_State* L)
{
	int no = (int)luaL_checknumber(L, 1);

	lua_pushnumber(L, GetVariable(no));

	return 1;
}

static int lua_SetVariable(lua_State* L)
{
	int var = (int)luaL_checknumber(L, 1);
	int val = (int)luaL_checknumber(L, 2);
	SetVariable(var, val);

	return 0;
}

FUNCTION_TABLE VarFunctionTable[FUNCTION_TABLE_VAR_SIZE] =
{
	{"Get", lua_GetVariable},
	{"Set", lua_SetVariable}
};