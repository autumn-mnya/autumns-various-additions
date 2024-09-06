#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Main.h"
#include "ModInit.h"
#include "ModSettings.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ASMPatches.h"
#include "ASMLoader.h"
#include "Collectables.h"
#include "Config.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "Game.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpcReplacements.h"
#include "Profile.h"
#include "Stage.h"
#include "SurfaceDefines.h"
#include "TextScript.h"
#include "TileCollisionBoss.h"
#include "TileCollisionBullet.h"
#include "TileCollisionEntity.h"
#include "TileCollisionMyChar.h"

bool init_collectables_a_enabled = false;
bool init_collectables_b_enabled = false;
bool init_collectables_c_enabled = false;
bool init_collectables_d_enabled = false;
bool init_collectables_e_enabled = false;

// Init settings
int init_physics_max_dash = 812;
int init_physics_max_move = 1535;
int init_physics_gravity1 = 80;
int init_physics_gravity2 = 32;
int init_physics_dash1 = 85;
int init_physics_dash2 = 32;
int init_physics_resist = 51;
int init_physics_jump = 1280;
int init_physics_water_max_dash = 406;
int init_physics_water_max_move = 767;
int init_physics_water_gravity1 = 40;
int init_physics_water_gravity2 = 16;
int init_physics_water_dash1 = 42;
int init_physics_water_dash2 = 16;
int init_physics_water_resist = 25;
int init_physics_water_jump = 640;
int init_walljump_horizontal_speed = 1000;
int init_walljump_jump_height = 1113;
int init_walljump_sliding_speed = 554;
int init_walljump_water_horizontal_speed = 300;
int init_walljump_water_jump_height = 556;
int init_extrajump_jump_height = 1280;
int init_extrajump_water_jump_height = 640;
int init_running_speed = 1218;
int init_bounce_speed = 1535;

int init_collectables_a_x_pos = 8;
int init_collectables_b_x_pos = 8;
int init_collectables_c_x_pos = 8;
int init_collectables_d_x_pos = 8;
int init_collectables_e_x_pos = 8;
int init_collectables_a_y_pos = 56;
int init_collectables_b_y_pos = 64;
int init_collectables_c_y_pos = 72;
int init_collectables_d_y_pos = 80;
int init_collectables_e_y_pos = 88;
int init_collectables_a_x_offset = 0;
int init_collectables_b_x_offset = 0;
int init_collectables_c_x_offset = 0;
int init_collectables_d_x_offset = 0;
int init_collectables_e_x_offset = 0;

// Booster fuel
int init_booster_08_fuel = 50;
int init_booster_20_fuel = 50;

// Version 1.0.7 physics
int init_physics_boost20_accel_up = -1535;
int init_physics_boost20_accel_left = -1535;
int init_physics_boost20_accel_right = 1535;
int init_physics_boost20_accel_down = 1535;
int init_physics_boost20_accel_up_no_key = -1535;

int init_physics_boost08_accel_add = 32;
int init_physics_boost08_accel_max_speed = -1024;

int init_physics_max_speed_left = -1535;
int init_physics_max_speed_right = 1535;
int init_physics_max_speed_up = -1535;
int init_physics_max_speed_down = 1535;

int init_physics_water_max_speed_left = -1535;
int init_physics_water_max_speed_right = 1535;
int init_physics_water_max_speed_up = -1535;
int init_physics_water_max_speed_down = 1535;

// GenericLoad paths

char init_surfaceName_0_Title[MaxSurfaceName] = "Title";
char init_surfaceName_5_Image[MaxSurfaceName] = "Image\\0";
char init_surfaceName_6_Fade[MaxSurfaceName] = "Fade";
char init_surfaceName_8_ItemImage[MaxSurfaceName] = "ItemImage";
char init_surfaceName_11_Arms[MaxSurfaceName] = "Arms";
char init_surfaceName_12_ArmsImage[MaxSurfaceName] = "ArmsImage";
char init_surfaceName_14_StageImage[MaxSurfaceName] = "StageImage";
char init_surfaceName_15_Loading[MaxSurfaceName] = "Loading";
char init_surfaceName_16_MyChar[MaxSurfaceName] = "MyChar";
char init_surfaceName_17_Bullet[MaxSurfaceName] = "Bullet";
char init_surfaceName_19_Caret[MaxSurfaceName] = "Caret";
char init_surfaceName_20_NpcSym[MaxSurfaceName] = "Npc\\NpcSym";
char init_surfaceName_23_NpcRegu[MaxSurfaceName] = "Npc\\NpcRegu";
char init_surfaceName_24_AutumnUI[MaxSurfaceName] = "AutumnUI";
char init_surfaceName_25_AutumnObjects[MaxSurfaceName] = "Npc\\NpcAutumnObj";
char init_surfaceName_26_TextBox[MaxSurfaceName] = "TextBox";
char init_surfaceName_27_Face[MaxSurfaceName] = "Face";
char init_surfaceName_38_AutumnItems[MaxSurfaceName] = "Autumn";
char init_surfaceName_39_AutumnCharacters[MaxSurfaceName] = "Npc\\NpcAutumnChar";

// These functions set "init" values so that we can know what set variables were on boot of the game.

void InitMod_PreLaunch_CollectablesEnabled()
{
	init_collectables_a_enabled = enable_collectables_a;
	init_collectables_b_enabled = enable_collectables_b;
	init_collectables_c_enabled = enable_collectables_c;
	init_collectables_d_enabled = enable_collectables_d;
	init_collectables_e_enabled = enable_collectables_e;
}

void InitMod_PreLaunch_CollectablesPositioning()
{
	init_collectables_a_x_pos = collectables_a_x_pos;
	init_collectables_b_x_pos = collectables_b_x_pos;
	init_collectables_c_x_pos = collectables_c_x_pos;
	init_collectables_d_x_pos = collectables_d_x_pos;
	init_collectables_e_x_pos = collectables_e_x_pos;
	init_collectables_a_y_pos = collectables_a_y_pos;
	init_collectables_b_y_pos = collectables_b_y_pos;
	init_collectables_c_y_pos = collectables_c_y_pos;
	init_collectables_d_y_pos = collectables_d_y_pos;
	init_collectables_e_y_pos = collectables_e_y_pos;
	init_collectables_a_x_offset = collectables_a_x_offset;
	init_collectables_b_x_offset = collectables_b_x_offset;
	init_collectables_c_x_offset = collectables_c_x_offset;
	init_collectables_d_x_offset = collectables_d_x_offset;
	init_collectables_e_x_offset = collectables_e_x_offset;
}

void InitMod_PreLaunch_PhysicsSettings()
{
	init_physics_max_dash = setting_physics_max_dash;
	init_physics_max_move = setting_physics_max_move;
	init_physics_gravity1 = setting_physics_gravity1;
	init_physics_gravity2 = setting_physics_gravity2;
	init_physics_dash1 = setting_physics_dash1;
	init_physics_dash2 = setting_physics_dash2;
	init_physics_resist = setting_physics_resist;
	init_physics_jump = setting_physics_jump;
	init_physics_water_max_dash = setting_physics_water_max_dash;
	init_physics_water_max_move = setting_physics_water_max_move;
	init_physics_water_gravity1 = setting_physics_water_gravity1;
	init_physics_water_gravity2 = setting_physics_water_gravity2;
	init_physics_water_dash1 = setting_physics_water_dash1;
	init_physics_water_dash2 = setting_physics_water_dash2;
	init_physics_water_resist = setting_physics_water_resist;
	init_physics_water_jump = setting_physics_water_jump;
	init_walljump_horizontal_speed = setting_walljump_horizontal_speed;
	init_walljump_jump_height = setting_walljump_jump_height;
	init_walljump_sliding_speed = setting_walljump_sliding_speed;
	init_walljump_water_horizontal_speed = setting_walljump_water_horizontal_speed;
	init_walljump_water_jump_height = setting_walljump_water_jump_height;
	init_extrajump_jump_height = setting_extrajump_jump_height;
	init_extrajump_water_jump_height = setting_extrajump_water_jump_height;
	init_running_speed = setting_running_speed;
	init_bounce_speed = setting_bounce_speed;
	// Version 1.0.7
	init_physics_boost20_accel_up = setting_physics_boost20_accel_up;
	init_physics_boost20_accel_left = setting_physics_boost20_accel_left;
	init_physics_boost20_accel_right = setting_physics_boost20_accel_right;
	init_physics_boost20_accel_down = setting_physics_boost20_accel_down;
	init_physics_boost20_accel_up_no_key = setting_physics_boost20_accel_up_no_key;

	init_physics_boost08_accel_add = setting_physics_boost08_accel_add;
	init_physics_boost08_accel_max_speed = setting_physics_boost08_accel_max_speed;

	init_physics_max_speed_left = setting_physics_max_speed_left;
	init_physics_max_speed_right = setting_physics_max_speed_right;
	init_physics_max_speed_up = setting_physics_max_speed_up;
	init_physics_max_speed_down = setting_physics_max_speed_down;

	init_physics_water_max_speed_left = setting_physics_water_max_speed_left;
	init_physics_water_max_speed_right = setting_physics_water_max_speed_right;
	init_physics_water_max_speed_up = setting_physics_water_max_speed_up;
	init_physics_water_max_speed_down = setting_physics_water_max_speed_down;
}

void InitMod_PreLaunch_BoosterFuel()
{
	init_booster_08_fuel = booster_08_fuel;
	init_booster_20_fuel = booster_20_fuel;
}

void InitMod_PreLaunch_GenericLoad()
{
	strcpy(init_surfaceName_0_Title, surfaceName_0_Title);
	strcpy(init_surfaceName_5_Image, surfaceName_5_Image);
	strcpy(init_surfaceName_6_Fade, surfaceName_6_Fade);
	strcpy(init_surfaceName_8_ItemImage, surfaceName_8_ItemImage);
	strcpy(init_surfaceName_11_Arms, surfaceName_11_Arms);
	strcpy(init_surfaceName_12_ArmsImage, surfaceName_12_ArmsImage);
	strcpy(init_surfaceName_14_StageImage, surfaceName_14_StageImage);
	strcpy(init_surfaceName_15_Loading, surfaceName_15_Loading);
	strcpy(init_surfaceName_16_MyChar, surfaceName_16_MyChar);
	strcpy(init_surfaceName_17_Bullet, surfaceName_17_Bullet);
	strcpy(init_surfaceName_19_Caret, surfaceName_19_Caret);
	strcpy(init_surfaceName_20_NpcSym, surfaceName_20_NpcSym);
	strcpy(init_surfaceName_23_NpcRegu, surfaceName_23_NpcRegu);
	strcpy(init_surfaceName_24_AutumnUI, surfaceName_24_AutumnUI);
	strcpy(init_surfaceName_25_AutumnObjects, surfaceName_25_AutumnObjects);
	strcpy(init_surfaceName_26_TextBox, surfaceName_26_TextBox);
	strcpy(init_surfaceName_27_Face, surfaceName_27_Face);
	strcpy(init_surfaceName_38_AutumnItems, surfaceName_38_AutumnItems);
	strcpy(init_surfaceName_39_AutumnCharacters, surfaceName_39_AutumnCharacters);
}

// Functions below here reset the set values to the values they were on boot of the game

void InitCollectablesEnabled()
{
	enable_collectables_a = init_collectables_a_enabled;
	enable_collectables_b = init_collectables_b_enabled;
	enable_collectables_c = init_collectables_c_enabled;
	enable_collectables_d = init_collectables_d_enabled;
	enable_collectables_e = init_collectables_e_enabled;
}

void InitCollectablesPositioning()
{
	collectables_a_x_pos = init_collectables_a_x_pos;
	collectables_b_x_pos = init_collectables_b_x_pos;
	collectables_c_x_pos = init_collectables_c_x_pos;
	collectables_d_x_pos = init_collectables_d_x_pos;
	collectables_e_x_pos = init_collectables_e_x_pos;
	collectables_a_y_pos = init_collectables_a_y_pos;
	collectables_b_y_pos = init_collectables_b_y_pos;
	collectables_c_y_pos = init_collectables_c_y_pos;
	collectables_d_y_pos = init_collectables_d_y_pos;
	collectables_e_y_pos = init_collectables_e_y_pos;
	collectables_a_x_offset = init_collectables_a_x_offset;
	collectables_b_x_offset = init_collectables_b_x_offset;
	collectables_c_x_offset = init_collectables_c_x_offset;
	collectables_d_x_offset = init_collectables_d_x_offset;
	collectables_e_x_offset = init_collectables_e_x_offset;
}

void InitMyCharPhysics()
{
	setting_physics_max_dash = init_physics_max_dash;
	setting_physics_max_move = init_physics_max_move;
	setting_physics_gravity1 = init_physics_gravity1;
	setting_physics_gravity2 = init_physics_gravity2;
	setting_physics_dash1 = init_physics_dash1;
	setting_physics_dash2 = init_physics_dash2;
	setting_physics_resist = init_physics_resist;
	setting_physics_jump = init_physics_jump;
	setting_physics_water_max_dash = init_physics_water_max_dash;
	setting_physics_water_max_move = init_physics_water_max_move;
	setting_physics_water_gravity1 = init_physics_water_gravity1;
	setting_physics_water_gravity2 = init_physics_water_gravity2;
	setting_physics_water_dash1 = init_physics_water_dash1;
	setting_physics_water_dash2 = init_physics_water_dash2;
	setting_physics_water_resist = init_physics_water_resist;
	setting_physics_water_jump = init_physics_water_jump;
	setting_walljump_horizontal_speed = init_walljump_horizontal_speed;
	setting_walljump_jump_height = init_walljump_jump_height;
	setting_walljump_sliding_speed = init_walljump_sliding_speed;
	setting_walljump_water_horizontal_speed = init_walljump_water_horizontal_speed;
	setting_walljump_water_jump_height = init_walljump_water_jump_height;
	setting_extrajump_jump_height = init_extrajump_jump_height;
	setting_extrajump_water_jump_height = init_extrajump_water_jump_height;
	setting_running_speed = init_running_speed;
	setting_bounce_speed = init_bounce_speed;
	// Version 1.0.7
	setting_physics_boost20_accel_up = init_physics_boost20_accel_up;
	setting_physics_boost20_accel_left = init_physics_boost20_accel_left;
	setting_physics_boost20_accel_right = init_physics_boost20_accel_right;
	setting_physics_boost20_accel_down = init_physics_boost20_accel_down;
	setting_physics_boost20_accel_up_no_key = init_physics_boost20_accel_up_no_key;
	setting_physics_boost08_accel_add = init_physics_boost08_accel_add;
	setting_physics_boost08_accel_max_speed = init_physics_boost08_accel_max_speed;
	setting_physics_max_speed_left = init_physics_max_speed_left;
	setting_physics_max_speed_right = init_physics_max_speed_right;
	setting_physics_max_speed_up = init_physics_max_speed_up;
	setting_physics_max_speed_down = init_physics_max_speed_down;
	setting_physics_water_max_speed_left = init_physics_water_max_speed_left;
	setting_physics_water_max_speed_right = init_physics_water_max_speed_right;
	setting_physics_water_max_speed_up = init_physics_water_max_speed_up;
	setting_physics_water_max_speed_down = init_physics_water_max_speed_down;
	Set_Version107_Physics();
}

void InitMyCharBoostFuel()
{
	booster_08_fuel = init_booster_08_fuel;
	booster_20_fuel = init_booster_20_fuel;
	Mod_WriteBoosterFuel();
}

void InitGameSurfaces()
{
	strcpy(surfaceName_0_Title, init_surfaceName_0_Title);
	strcpy(surfaceName_5_Image, init_surfaceName_5_Image);
	strcpy(surfaceName_6_Fade, init_surfaceName_6_Fade);
	strcpy(surfaceName_8_ItemImage, init_surfaceName_8_ItemImage);
	strcpy(surfaceName_11_Arms, init_surfaceName_11_Arms);
	strcpy(surfaceName_12_ArmsImage, init_surfaceName_12_ArmsImage);
	strcpy(surfaceName_14_StageImage, init_surfaceName_14_StageImage);
	strcpy(surfaceName_15_Loading, init_surfaceName_15_Loading);
	strcpy(surfaceName_16_MyChar, init_surfaceName_16_MyChar);
	strcpy(surfaceName_17_Bullet, init_surfaceName_17_Bullet);
	strcpy(surfaceName_19_Caret, init_surfaceName_19_Caret);
	strcpy(surfaceName_20_NpcSym, init_surfaceName_20_NpcSym);
	strcpy(surfaceName_23_NpcRegu, init_surfaceName_23_NpcRegu);
	strcpy(surfaceName_24_AutumnUI, init_surfaceName_24_AutumnUI);
	strcpy(surfaceName_25_AutumnObjects, init_surfaceName_25_AutumnObjects);
	strcpy(surfaceName_26_TextBox, init_surfaceName_26_TextBox);
	strcpy(surfaceName_27_Face, init_surfaceName_27_Face);
	strcpy(surfaceName_38_AutumnItems, init_surfaceName_38_AutumnItems);
	strcpy(surfaceName_39_AutumnCharacters, init_surfaceName_39_AutumnCharacters);
}