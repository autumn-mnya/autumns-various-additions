#pragma once

#include <Windows.h>
#include "cave_story.h"

extern char gModulePath[MAX_PATH];
extern char gDataPath[MAX_PATH];
extern char gBkgPath[MAX_PATH];

void PlayerDeath();
void InitCollectablesEnabled();
void InitMyCharPhysics();