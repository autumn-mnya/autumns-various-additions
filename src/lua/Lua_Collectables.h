#pragma once

#include <Windows.h>
#include "../cave_story.h"

extern "C"
{
#include <lua.h>
}

#include "Lua.h"
#include "../AutPI.h"

#define FUNCTION_TABLE_COLLECTABLES_SIZE 2
extern FUNCTION_TABLE CollectablesFunctionTable[FUNCTION_TABLE_COLLECTABLES_SIZE];