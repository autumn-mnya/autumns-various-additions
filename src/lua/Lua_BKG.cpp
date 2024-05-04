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
#include "Lua_BKG.h"

#include "../Main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../ModSettings.h"

#include "../AutPI.h"

#include "../BKG.h"

static int lua_GetIsActive(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushboolean(L, bkList[index].isActive);
    return 1;
}

static int lua_GetBmX(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].bmX);
    return 1;
}

static int lua_GetBmY(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].bmY);
    return 1;
}

static int lua_GetBmW(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].bmW);
    return 1;
}

static int lua_GetBmH(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].bmH);
    return 1;
}

static int lua_GetRepX(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].repX);
    return 1;
}

static int lua_GetRepY(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].repY);
    return 1;
}

static int lua_GetXDist(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].xDist);
    return 1;
}

static int lua_GetYDist(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].yDist);
    return 1;
}

static int lua_GetType(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].type);
    return 1;
}

static int lua_GetXm(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].xm);
    return 1;
}

static int lua_GetYm(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].ym);
    return 1;
}

static int lua_GetSpriteNum(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].spriteNum);
    return 1;
}

static int lua_GetAnimFrame(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].animFrame);
    return 1;
}

static int lua_GetAnimSpeed(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].animSpeed);
    return 1;
}

static int lua_GetCounter(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].counter);
    return 1;
}

static int lua_GetX(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].x);
    return 1;
}

static int lua_GetY(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].y);
    return 1;
}

static int lua_GetBkgXval(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].bkgXval);
    return 1;
}

static int lua_GetBkgYval(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    lua_pushnumber(L, bkList[index].bkgYval);
    return 1;
}

static int lua_SetIsActive(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].isActive = lua_toboolean(L, 2);
    return 0;
}

static int lua_SetBmX(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].bmX = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetBmY(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].bmY = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetBmW(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].bmW = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetBmH(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].bmH = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetRepX(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].repX = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetRepY(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].repY = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetXDist(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].xDist = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetYDist(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].yDist = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetType(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].type = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetXm(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].xm = luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetYm(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].ym = luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetSpriteNum(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].spriteNum = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetAnimFrame(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].animFrame = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetAnimSpeed(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].animSpeed = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetCounter(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].counter = (int)luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetX(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].x = luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetY(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].y = luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetBkgXval(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].bkgXval = luaL_checknumber(L, 2);
    return 0;
}

static int lua_SetBkgYval(lua_State* L) {
    int index = (int)luaL_checknumber(L, 1);
    bkList[index].bkgYval = luaL_checknumber(L, 2);
    return 0;
}

FUNCTION_TABLE BkgFunctionTable[FUNCTION_TABLE_BKG_SIZE] =
{
    {"GetActive", lua_GetIsActive},
    {"GetBkXOffset", lua_GetBmX},
    {"GetBkYOffset", lua_GetBmY},
    {"GetBkWidth", lua_GetBmW},
    {"GetBkHeight", lua_GetBmH},
    {"GetRepeatX", lua_GetRepX},
    {"GetRepeatY", lua_GetRepY},
    {"GetRepeatGapX", lua_GetXDist},
    {"GetRepeatGapY", lua_GetYDist},
    {"GetType", lua_GetType},
    {"GetXSpeed", lua_GetXm},
    {"GetYSpeed", lua_GetYm},
    {"GetFrameCount", lua_GetSpriteNum},
    {"GetAniFrame", lua_GetAnimFrame},
    {"GetAniSpeed", lua_GetAnimSpeed},
    {"GetMapX", lua_GetBkgXval},
    {"GetMapY", lua_GetBkgYval},
    {"SetActive", lua_SetIsActive},
    {"SetBkXOffset", lua_SetBmX},
    {"SetBkYOffset", lua_SetBmY},
    {"SetBkWidth", lua_SetBmW},
    {"SetBkHeight", lua_SetBmH},
    {"SetRepeatX", lua_SetRepX},
    {"SetRepeatY", lua_SetRepY},
    {"SetRepeatGapX", lua_SetXDist},
    {"SetRepeatGapY", lua_SetYDist},
    {"SetType", lua_SetType},
    {"SetXSpeed", lua_SetXm},
    {"SetYSpeed", lua_SetYm},
    {"SetFrameCount", lua_SetSpriteNum},
    {"SetAniFrame", lua_SetAnimFrame},
    {"SetAniSpeed", lua_SetAnimSpeed},
    {"SetMapX", lua_SetBkgXval},
    {"SetMapY", lua_SetBkgYval}
};