#pragma once

#include <Windows.h>

#include "cave_story.h"

#define TSC_VAR_MAX 300 // max amount of variables

extern int varData[TSC_VAR_MAX];

int GetVariable(int id);
void SetVariable(int id, int value);
void InitTSCVariables();
void InitVarData();