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
#include "Lua_Frame.h"

#include "../Main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../AutPI.h"

#include "../Frame.h"

static int lua_SetCustomCamPos(lua_State* L)
{
    int x = (int)luaL_checknumber(L, 1);
    int y = (int)luaL_checknumber(L, 2);
    int w = 16;

    // Check if the third argument exists
    if (lua_gettop(L) >= 3) {
        w = (int)luaL_checknumber(L, 3);
    }

    SetFrameTargetMyCharOffset(w, 0, x, y);

    return 0;
}

static int lua_SetTargetCoords(lua_State* L)
{
    int x = (int)luaL_checknumber(L, 1);
    int y = (int)luaL_checknumber(L, 2);
    int w = 16;

    // Check if the third argument exists
    if (lua_gettop(L) >= 3) {
        w = (int)luaL_checknumber(L, 3);
    }

    SetFrameTargetCoordinate(w, x * 0x200 * 0x10, y * 0x200 * 0x10);

    return 0;
}

FUNCTION_TABLE CameraExFunctionTable[FUNCTION_TABLE_CAMERAEX_SIZE] =
{
	{"SetOffset", lua_SetCustomCamPos},
	{"SetTargetCoords", lua_SetTargetCoords}
};