#pragma once

#include <Windows.h>

#include "cave_story.h"

extern bool setting_enable_life_drain;
extern bool setting_enable_life_regen;

extern int setting_life_drain_flag;
extern int setting_life_regen_flag;

extern int setting_life_drain_wait;
extern int setting_life_regen_wait;

extern int setting_life_drain_amount;
extern int setting_life_regen_amount;

void PlayerDeath();
void PlayerDeathWithEvent(int event);
void RemoveMaxLifeMyChar(int val);
void SetMaxLifeMyChar(int val);
void AutumnsVariousAdditionsPlayerHud();