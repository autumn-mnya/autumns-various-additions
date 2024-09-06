#pragma once

#include <Windows.h>

#include "cave_story.h"
#include "Collectables.h"
#include "LoadPixtone.h"
#include "ModSettings.h"
#include "Profile.h"
#include "SurfaceDefines.h"
#include "TextScript.h"
#include "TextScriptCustomLoad.h"
#include "TextScriptVAR.h"

#define ProfileCodeSize 16
extern bool isLoadingSave;

// struct padding exists ! make sure these all get evenly divided by four..
typedef struct CustomProfileData
{
	char code[ProfileCodeSize];
	char imgFolder[ImgFolderSize];
	int playerMoney;
	COLLECTABLES pCollectables;
	// Collectables enabled (ints instead of bools to deal with struct padding lol)
	int enable_collect_a;
	int enable_collect_b;
	int enable_collect_c;
	int enable_collect_d;
	int enable_collect_e;
	// Collectables Position / Offset
	int collectables_a_x_pos;
	int collectables_a_y_pos;
	int collectables_a_x_offset;
	int collectables_b_x_pos;
	int collectables_b_y_pos;
	int collectables_b_x_offset;
	int collectables_c_x_pos;
	int collectables_c_y_pos;
	int collectables_c_x_offset;
	int collectables_d_x_pos;
	int collectables_d_y_pos;
	int collectables_d_x_offset;
	int collectables_e_x_pos;
	int collectables_e_y_pos;
	int collectables_e_x_offset;
	// <PHY physics
	int phy_physics_max_dash;
	int phy_physics_max_move;
	int phy_physics_gravity1;
	int phy_physics_gravity2;
	int phy_physics_dash1;
	int phy_physics_dash2;
	int phy_physics_resist;
	int phy_physics_jump;
	int phy_physics_water_max_dash;
	int phy_physics_water_max_move;
	int phy_physics_water_gravity1;
	int phy_physics_water_gravity2;
	int phy_physics_water_dash1;
	int phy_physics_water_dash2;
	int phy_physics_water_resist;
	int phy_physics_water_jump;
	int phy_walljump_horizontal_speed;
	int phy_walljump_jump_height;
	int phy_walljump_sliding_speed;
	int phy_walljump_water_horizontal_speed;
	int phy_walljump_water_jump_height;
	int phy_extrajump_jump_height;
	int phy_extrajump_water_jump_height;
	int phy_running_speed;
	int phy_bounce_speed;
	// <VAR
	int varData[TSC_VAR_MAX];
	// <MIM
	int mim_num;
	// Booster Fuel
	int booster_08_fuel;
	int booster_20_fuel;
	// custom npc table
	char npc_tbl[NpcTblMaxPath];
	// Surface Names
	char surfaceName_0_Title[MaxSurfaceName];
	char surfaceName_5_Image[MaxSurfaceName];
	char surfaceName_6_Fade[MaxSurfaceName];
	char surfaceName_8_ItemImage[MaxSurfaceName];
	char surfaceName_11_Arms[MaxSurfaceName];
	char surfaceName_12_ArmsImage[MaxSurfaceName];
	char surfaceName_14_StageImage[MaxSurfaceName];
	char surfaceName_16_MyChar[MaxSurfaceName];
	char surfaceName_17_Bullet[MaxSurfaceName];
	char surfaceName_19_Caret[MaxSurfaceName];
	char surfaceName_20_NpcSym[MaxSurfaceName];
	char surfaceName_23_NpcRegu[MaxSurfaceName];
	char surfaceName_24_AutumnUI[MaxSurfaceName];
	char surfaceName_25_AutumnObjects[MaxSurfaceName];
	char surfaceName_26_TextBox[MaxSurfaceName];
	char surfaceName_27_Face[MaxSurfaceName];
	char surfaceName_38_AutumnItems[MaxSurfaceName];
	char surfaceName_39_AutumnCharacters[MaxSurfaceName];
	// Head.tsc / ArmsItem.tsc
	char armsitem_tsc[CustomTscMaxPath];
	char head_tsc[CustomTscMaxPath];
	// PixTone folder path
	char pixtoneFolder[MaxPixTonePath];
	// Version 1.0.7 Additions
	int phy_physics_boost20_accel_up;
	int phy_physics_boost20_accel_left;
	int phy_physics_boost20_accel_right;
	int phy_physics_boost20_accel_down;
	int phy_physics_boost20_accel_up_no_key;

	int phy_physics_boost08_accel_add;
	int phy_physics_boost08_accel_max_speed;

	int phy_physics_max_speed_left;
	int phy_physics_max_speed_right;
	int phy_physics_max_speed_up;
	int phy_physics_max_speed_down;

	int phy_physics_water_max_speed_left;
	int phy_physics_water_max_speed_right;
	int phy_physics_water_max_speed_up;
	int phy_physics_water_max_speed_down;
} CustomProfileData;

void SaveAvaFile();
void LoadAvaFile();
void Replacement_LoadProfile_InitMyChar_Call();
void Replacement_LoadProfile_TransferStage_Call(int w, int x, int y, int z);
void AvaLoadProfileInit();
void Replacement_InitializeGame_TransferStage_Call(int w, int x, int y, int z);
void Replacement_InitializeGame_ClearArmsData_Call();