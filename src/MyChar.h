#pragma once

#include <Windows.h>

#include "cave_story.h"

struct Physics {
	int jump;
	int dash1;
	int resist;
	int gravity1;
	int dash2;
	int max_dash;
	int gravity2;
	int max_move;
};

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

// physics

extern int setting_physics_water_max_dash;
extern int setting_physics_water_max_move;
extern int setting_physics_water_gravity1;
extern int setting_physics_water_gravity2;
extern int setting_physics_water_jump;
extern int setting_physics_water_dash1;
extern int setting_physics_water_dash2;
extern int setting_physics_water_resist;

extern int setting_physics_max_dash;
extern int setting_physics_max_move;
extern int setting_physics_gravity1;
extern int setting_physics_gravity2;
extern int setting_physics_jump;
extern int setting_physics_dash1;
extern int setting_physics_dash2;
extern int setting_physics_resist;

void setPlayerPhysics(BOOL bKey, Physics* physics);
void ActMyChar_Normal_Custom(BOOL bKey);