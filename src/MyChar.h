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
extern int onWall;

extern bool setting_walljumps_enabled;
extern bool setting_walljumps_flag_enabled;
extern bool setting_doublejump_enabled;
extern bool setting_doublejump_flag_enabled;
extern int setting_doublejump_flag;
extern int setting_walljump_horizontal_speed;
extern int setting_walljump_water_horizontal_speed;
extern int setting_walljumps_flag;
extern int setting_walljump_jump_height;
extern int setting_walljump_water_jump_height;
extern int setting_walljump_sliding_speed;
extern int setting_extrajump_jump_height;
extern int setting_extrajump_water_jump_height;

// physics
extern bool setting_physics_ini_enabled;
extern bool setting_run_button_enabled;
extern bool setting_run_button_flag_enabled;
extern int setting_run_button_flag;

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

extern int setting_running_speed;

extern bool setting_ice_particles;

extern int setting_jump_arrow_x_offset;
extern int setting_jump_arrow_y_offset;

extern bool setting_jump_arrow_x_offset_negative;
extern bool setting_jump_arrow_y_offset_negative;

extern int setting_max_jump_arrow_display;

extern int custom_camera_offset_x;
extern int custom_camera_offset_y;
extern int custom_index_x;
extern int custom_index_y;
extern int custom_tgt_x;
extern int custom_tgt_y;

extern bool is_direction_locked;
extern int lock_direction;

extern int mim_num;
extern int mim_player_size_width;
extern int mim_player_size_height;

void Replacement_InitMyChar_memset_Call(void* dst, int val, size_t size);
void SetPlayerPhysics(int x, int y);
void Replacement_PutMyChar_PutChar_Call(const RECT* r, int a, int b, const RECT* r2, SurfaceID s);
void Replacement_PutMyChar_Call(int fx, int fy);
void Replacement_SetMyCharPosition_InitStar_Call();
void setPlayerPhysics(BOOL bKey, Physics* physics);
void ActMyChar_Normal_Custom(BOOL bKey);
void SetPlayerCamera(BOOL bKey);