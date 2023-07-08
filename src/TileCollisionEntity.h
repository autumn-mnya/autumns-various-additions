#pragma once

#include <Windows.h>

#include "cave_story.h"

void JudgeHitNpCharJumpThru(NPCHAR* npc, int x, int y);
void JudgeHitNpCharJumpThruUpsideDown(NPCHAR* npc, int x, int y);
void JudgeHitNpCharJumpThruSidewayRight(NPCHAR* npc, int x, int y);
void JudgeHitNpCharJumpThruSidewayLeft(NPCHAR* npc, int x, int y);

void Replacement_HitNpCharMap(void);