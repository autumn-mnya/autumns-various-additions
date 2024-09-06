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

// Version 1.0.7

extern int setting_physics_boost20_accel_up;
extern int setting_physics_boost20_accel_left;
extern int setting_physics_boost20_accel_right;
extern int setting_physics_boost20_accel_down;
extern int setting_physics_boost20_accel_up_no_key;

extern int setting_physics_boost08_accel_add;
extern int setting_physics_boost08_accel_max_speed;

extern int setting_physics_max_speed_left;
extern int setting_physics_max_speed_right;
extern int setting_physics_max_speed_up;
extern int setting_physics_max_speed_down;

extern int setting_physics_water_max_speed_left;
extern int setting_physics_water_max_speed_right;
extern int setting_physics_water_max_speed_up;
extern int setting_physics_water_max_speed_down;

// Version 1.0.9

extern bool setting_double_jump_wall_jump_refresh;

// Version 1.1.0
extern int max_coyote;
extern int coyote_frames;
extern bool coyote_spent;
extern int coyote_spent_frames;

extern int max_buffer;
extern int jump_buffer;
extern bool buffer_timer;
extern bool buffer_can_jump;
extern bool walljumps_can_be_buffered;

extern int jump_sound_effect;

void Set_Version107_Physics();
void Replacement_InitMyChar_memset_Call(void* dst, int val, size_t size);
int GetPlayerPhysicsValue(int x);
void SetPlayerPhysicsValues(int x, int y);
void Replacement_PutMyChar_PutChar_Call(const RECT* r, int a, int b, const RECT* r2, SurfaceID s);
void AutumnsVariousAdditionsExtraJumpUI();
void SetCustomMyCharTGT();
void setPlayerPhysics(BOOL bKey, Physics* physics);
void ActMyChar_Normal_Custom(BOOL bKey);
void SetPlayerCamera(BOOL bKey);