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
#include "Lua_CPXE.h"

#include "../Main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../AutPI.h"

#include "../Entity.h"
#include "../EntityLoad.h"

static int lua_CpxeGetCustomVal(lua_State* L)
{
	NPCHAR* npc = *(NPCHAR**)luaL_checkudata(L, 1, "NpcMeta");
	int type = (int)luaL_checknumber(L, 2);
	int num = 0;

	if (type == 0)
		num = CustomNpcValues(npc).CustomValueA;
	else if (type == 1)
		num = CustomNpcValues(npc).CustomValueB;
	else if (type == 2)
		num = CustomNpcValues(npc).CustomValueC;
	else if (type == 3)
		num = CustomNpcValues(npc).CustomValueD;
	else if (type == 4)
		num = CustomNpcValues(npc).CustomValueE;
	else if (type == 5)
		num = CustomNpcValues(npc).CustomValueF;
	else
		num = 0;

	lua_pushnumber(L, num);

	return 1;
}

FUNCTION_TABLE CpxeFunctionTable[FUNCTION_TABLE_CPXE_SIZE] =
{
	{"Get", lua_CpxeGetCustomVal}
};