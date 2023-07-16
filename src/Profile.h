#pragma once

#include <Windows.h>

#include "BKG.h"
#include "cave_story.h"
#include "Collectables.h"
#include "TextScript.h"

#define ProfileCodeSize 16
extern bool isLoadingSave;

// struct padding exists ! make sure these all get evenly divided by four..
typedef struct CustomProfileData
{
	char code[ProfileCodeSize];
	char imgFolder[ImgFolderSize];
	char bkgTxT[bkgTxTSize];
	int playerMoney;
	COLLECTABLES pCollectables;
	// Collectables enabled (ints instead of bools to deal with struct padding lol)
	int enable_collect_a;
	int enable_collect_b;
	int enable_collect_c;
	int enable_collect_d;
	int enable_collect_e;
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
} CustomProfileData;

extern CustomProfileData profile;

void Replacement_SaveProfile_LastMemcpy_Call(void* dst, const void* src, size_t size);
void Replacement_SaveProfile_fwrite_Call(void* buf, size_t eleS, size_t eleC, FILE* fp);
void Replacement_LoadProfile_fclose_Call(FILE* fp);
void Replacement_LoadProfile_ClearFade_Call();
void Replacement_LoadProfile_ClearFade_Call();
void Replacement_InitializeGame_ClearArmsData_Call();