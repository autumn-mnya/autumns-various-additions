#pragma once

#include <Windows.h>
#include "../cave_story.h"

extern "C"
{
#include <lua.h>
}

#include "Lua.h"
#include "../AutPI.h"

#define FUNCTION_TABLE_BKG_SIZE 34
extern FUNCTION_TABLE BkgFunctionTable[FUNCTION_TABLE_BKG_SIZE];