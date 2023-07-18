#pragma once

#include <Windows.h>

#include "cave_story.h"

#include "mod_loader_hooks.h"

#define ImgFolderSize 48
#define ImgNameSize 0x32

#define StageTblMaxPath 120
#define NpcTblMaxPath 120

// Booleans
extern bool setting_enable_money_code;
extern bool setting_money_disable_enemy_money_drops;
extern bool setting_money_disable_exp_drops;
extern bool setting_enable_mim_mod;
extern bool setting_disable_tsc_encryption;
// Money Hud
extern int setting_money_hud_x;
extern int setting_money_hud_y;
extern int setting_money_hud_x_number_offset;
// Booster Fuel
extern int booster_08_fuel;
extern int booster_20_fuel;

extern char TSC_IMG_Folder[ImgFolderSize];

// Custom stage/npc tbl paths
extern char stageTblPath[StageTblMaxPath];
extern char npcTblPath[NpcTblMaxPath];

void ResetCollabPaths();
void Mod_WriteBoosterFuel();
void InitMod_TSC();