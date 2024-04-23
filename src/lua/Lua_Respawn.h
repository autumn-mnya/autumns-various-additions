#pragma once

#include <Windows.h>
#include "../cave_story.h"

extern "C"
{
#include <lua.h>
}

#include "Lua.h"
#include "../AutPI.h"

#define FUNCTION_TABLE_RESPAWN_SIZE 2
extern FUNCTION_TABLE RespawnFunctionTable[FUNCTION_TABLE_RESPAWN_SIZE];