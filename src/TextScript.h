#pragma once

#include <Windows.h>

#include "cave_story.h"

#include "mod_loader_hooks.h"

#define ImgFolderSize 48
#define ImgNameSize 0x32

extern bool setting_enable_money_code;
extern bool setting_money_disable_enemy_money_drops;
extern bool setting_money_disable_exp_drops;
extern int setting_money_hud_x;
extern int setting_money_hud_y;
extern int setting_money_hud_x_number_offset;

extern char TSC_IMG_Folder[ImgFolderSize];

void InitMod_TSC();