#pragma once

#include <Windows.h>
#include "../cave_story.h"

extern "C"
{
#include <lua.h>
}

#include "Lua.h"
#include "../AutPI.h"

#define FUNCTION_TABLE_CPXE_SIZE 1
extern FUNCTION_TABLE CpxeFunctionTable[FUNCTION_TABLE_CPXE_SIZE];