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


#include "../Main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../AutPI.h"

#include "Lua_Respawn.h"
#include "Lua_Var.h"

#define gL GetLuaL()
const char* LuaModName = "AVA";

static int lua_AvaTest(lua_State* L)
{
	const char* string = luaL_checkstring(L, 1);
	printf("%s\n", string);
	return 0;
}

void SetAVAGlobalString()
{
	
}

void PushAVAMetadata()
{

}

void SetAVALua()
{
	PushFunctionTable(gL, "AVA", RespawnFunctionTable, FUNCTION_TABLE_RESPAWN_SIZE, TRUE);
	PushFunctionTable(gL, "AVA", VarFunctionTable, FUNCTION_TABLE_VAR_SIZE, TRUE);
}