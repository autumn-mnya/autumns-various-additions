#pragma once

#include <Windows.h>
#include "../cave_story.h"

extern "C"
{
#include <lua.h>
}

#include "Lua.h"
#include "../AutPI.h"

#define FUNCTION_TABLE_VAR_SIZE 2
extern FUNCTION_TABLE VarFunctionTable[FUNCTION_TABLE_VAR_SIZE];