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


#include "../Draw.h"
#include "../Main.h"
#include "../MycParam.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../Entity.h"
#include "../MyChar.h"
#include "../Profile.h"
#include "../TextScript.h"

#include "../AutPI.h"

#include "Lua_BKG.h"
#include "Lua_Collectables.h"
#include "Lua_CPXE.h"
#include "Lua_Frame.h"
#include "Lua_Money.h"
#include "Lua_Physics.h"
#include "Lua_Respawn.h"
#include "Lua_Var.h"

#define gL GetLuaL()
const char* LuaModName = "AVA";
#define FUNCTION_TABLE_AVA_SIZE 10

static int lua_KillPlayer(lua_State* L)
{
	int event = 40;

	if (lua_gettop(L) >= 1) {
		event = (int)luaL_checknumber(L, 1);
	}

	PlayerDeathWithEvent(event);
	return 0;
}

static int lua_GetMimVal(lua_State* L)
{
	lua_pushnumber(L, mim_num);

	return 1;
}

static int lua_GetWallboosting(lua_State* L)
{
	if (entity_IsWallboosting)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

static int lua_GetIcewalled(lua_State* L)
{
	if (entity_IsIceWalled)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

static int lua_AvaOnWall(lua_State* L)
{
	if (onWall)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

static int lua_GetCurrentJumps(lua_State* L)
{
	lua_pushnumber(L, current_jumps);

	return 1;
}

static int lua_GetLoadingSave(lua_State* L)
{
	if (isLoadingSave)
		lua_pushboolean(L, 1);
	else
		lua_pushboolean(L, 0);

	return 1;
}

static int lua_SetMimVal(lua_State* L)
{
	int num = (int)luaL_checknumber(L, 1);

	mim_num = num;

	return 0;
}

void SetImageFolder(const char* string)
{
	char path[ImgFolderSize];

	if (path[0] == 0)
		memset(TSC_IMG_Folder, 0, sizeof(TSC_IMG_Folder));
	else
	{
		memcpy(TSC_IMG_Folder, path, sizeof(TSC_IMG_Folder));
		strcpy(path, string);
		strcat(TSC_IMG_Folder, "\\");
	}
}

void SetImage(const char* string)
{
	char name[ImgNameSize];
	memset(name, 0, sizeof(name));
	strcpy(name, string);
	LoadTSC_Image(name);
}

static int lua_IMGFolder(lua_State* L)
{
	const char* string = luaL_checkstring(L, 1);
	SetImageFolder(string);
	return 0;
}

static int lua_IMGSet(lua_State* L)
{
	const char* string = luaL_checkstring(L, 1);
	SetImage(string);
	return 0;
}

FUNCTION_TABLE AvaFunctionTable[FUNCTION_TABLE_AVA_SIZE] =
{
	{"KillPlayer", lua_KillPlayer},
	{"GetMim", lua_GetMimVal},
	{"SetMim", lua_SetMimVal},
	{"OnWallbooster", lua_GetWallboosting},
	{"OnIcewall", lua_GetIcewalled},
	{"OnWall", lua_AvaOnWall},
	{"GetAirJumps", lua_GetCurrentJumps},
	{"LoadingSave", lua_GetLoadingSave},
	{"SetIMG", lua_IMGSet},
	{"SetIMGFolder", lua_IMGFolder}
};

void SetAVAGlobalString()
{
	
}

void PushAVAMetadata()
{

}

void SetAVALua()
{
	PushFunctionTable(gL, "AVA", AvaFunctionTable, FUNCTION_TABLE_AVA_SIZE, TRUE);
	PushFunctionTable(gL, "BKG", BkgFunctionTable, FUNCTION_TABLE_BKG_SIZE, TRUE);
	PushFunctionTable(gL, "Collectable", CollectablesFunctionTable, FUNCTION_TABLE_COLLECTABLES_SIZE, TRUE);
	PushFunctionTable(gL, "CPXE", CpxeFunctionTable, FUNCTION_TABLE_CPXE_SIZE, TRUE);
	PushFunctionTable(gL, "CameraEx", CameraExFunctionTable, FUNCTION_TABLE_CAMERAEX_SIZE, TRUE);
	PushFunctionTable(gL, "Money", MoneyFunctionTable, FUNCTION_TABLE_MONEY_SIZE, TRUE);
	PushFunctionTable(gL, "Respawn", RespawnFunctionTable, FUNCTION_TABLE_RESPAWN_SIZE, TRUE);
	PushFunctionTable(gL, "Physics", PhysicsFunctionTable, FUNCTION_TABLE_PHYSICS_SIZE, TRUE);
	PushFunctionTable(gL, "Var", VarFunctionTable, FUNCTION_TABLE_VAR_SIZE, TRUE);
}