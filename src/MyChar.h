#pragma once

#include <Windows.h>

#include "cave_story.h"

extern int current_jumps;

extern bool setting_walljumps_enabled;
extern bool setting_walljumps_flag_enabled;
extern bool setting_doublejump_enabled;
extern bool setting_doublejump_flag_enabled;
extern int setting_doublejump_flag;
extern int setting_walljumps_flag;
extern int setting_walljump_jump_height;
extern int setting_walljump_sliding_speed;
extern int setting_extrajump_jump_height;

void ActMyChar_Normal_Custom(BOOL bKey);