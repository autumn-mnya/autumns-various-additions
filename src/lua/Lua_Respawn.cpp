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
#include "Lua_Respawn.h"

#include "../Main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../AutPI.h"

#include "../Respawn.h"

static int lua_SetRespawn(lua_State* L)
{
	int x = (int)luaL_checknumber(L, 1);
	int y = (int)luaL_checknumber(L, 2);
	SetRespawnPoint(x, y);
	return 0;
}

static int lua_RespawnPlayer(lua_State* L)
{
	RespawnPlayer();
	return 0;
}

static int lua_RevivePlayer(lua_State* L)
{
	int life = setting_revive_hp;

	if (lua_gettop(L) >= 1) {
		life = (int)luaL_checknumber(L, 1);
	}

	RevivePlayerWithValue(life);
	return 0;
}

static int lua_IsRespawning(lua_State* L)
{
	if (PlayerIsRespawning != FALSE)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

FUNCTION_TABLE RespawnFunctionTable[FUNCTION_TABLE_RESPAWN_SIZE] =
{
	{"Set", lua_SetRespawn},
	{"Run", lua_RespawnPlayer},
	{"IsRespawning", lua_IsRespawning},
	{"Revive", lua_RevivePlayer}
};