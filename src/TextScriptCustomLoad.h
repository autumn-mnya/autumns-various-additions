#pragma once

#include <Windows.h>

#include "cave_story.h"

#define CustomTscMaxPath 256

extern bool setting_enable_custom_armsitem_location;
extern bool setting_enable_custom_head_location;

extern const char* defaultArmsItemTSCName;
extern const char* defaultHeadTSCName;

extern char CustomArmsItemTSCLocation[CustomTscMaxPath];
extern char CustomHeadTSCLocation[CustomTscMaxPath];

void Reset_CustomScriptNames();
void InitMod_ScriptReplacements();