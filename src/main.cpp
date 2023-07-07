#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Main.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ASMPatches.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "MyChar.h"

// Paths
char gModulePath[MAX_PATH];
char gDataPath[MAX_PATH];

// Options (none exist in Settings.ini currently)

// Function that kills the player (I don't have a place to put this at the moment)
void PlayerDeath()
{
	PlaySoundObject(17, SOUND_MODE_PLAY);
	gMC->cond = 0;
	SetDestroyNpChar(gMC->x, gMC->y, 10 * 0x200, 0x40);
	StartTextScript(40);
}

// Inits anything relating to entities. The main thing are the 3 ModLoader_WriteJump's -- These replace every function that uses the Npc Table, and instead we also insert our new table!
void InitMod_Entity()
{
	InitCustomEntityPatch();
	ModLoader_WriteJump((void*)0x46FA00, (void*)Replacement_ActNpChar);
	ModLoader_WriteJump((void*)0x46FAB0, (void*)Replacement_ChangeNpCharByEvent);
	ModLoader_WriteJump((void*)0x46FD10, (void*)Replacement_ChangeCheckableNpCharByEvent);
}

// Loads the new surface files (We can't go above 40, but we can use the unused ones. Except 3 and 4 as they are used in the netplay dll!)
void InitMod_Sprites()
{
	ModLoader_WriteCall((void*)0x411546, (void*)Replacement_StageImageSurfaceCall);
}


// Init MyChar replacement
void InitMod_MyChar()
{
	InitMyCharPhysicsPatch();
	InitCustomMyCharPatch();
}

// For settings related things, this function will be used.
void InitMod_Settings()
{
	// Walljumps
	setting_walljumps_enabled = ModLoader_GetSettingBool("Walljumps Enabled", false);
	setting_walljumps_flag_enabled = ModLoader_GetSettingBool("Walljumps Enable on Flag", false);
	setting_walljumps_flag = ModLoader_GetSettingInt("Walljumps Flag", 6500);
	setting_walljump_horizontal_speed = ModLoader_GetSettingInt("Walljumps Horizontal Speed", 1000);
	setting_walljump_water_horizontal_speed = ModLoader_GetSettingInt("Underwater Walljumps Horizontal Speed", 300);
	setting_walljump_jump_height = ModLoader_GetSettingInt("Walljumps Jump Momentum", 1113);
	setting_walljump_water_jump_height = ModLoader_GetSettingInt("Underwater Walljumps Jump Momentum", 556);
	setting_walljump_sliding_speed = ModLoader_GetSettingInt("Wallsliding Speed", 554);
	
	// Extra Jumps
	setting_extrajump_jump_height = ModLoader_GetSettingInt("Extra Jump Momentum", 1280);
	setting_extrajump_water_jump_height = ModLoader_GetSettingInt("Underwater Extra Jump Momentum", 640);

	// Double Jump
	setting_doublejump_enabled = ModLoader_GetSettingBool("Double Jump Enabled", false);
	setting_doublejump_flag_enabled = ModLoader_GetSettingBool("Double Jump Enable on Flag", false);
	setting_doublejump_flag = ModLoader_GetSettingInt("Double Jump Flag", 6501);

	// Physics + Run button
	setting_physics_ini_enabled = ModLoader_GetSettingBool("Physics in Settings.ini", false);
	setting_run_button_enabled = ModLoader_GetSettingBool("Enable Run Button", false);
	setting_run_button_flag_enabled = ModLoader_GetSettingBool("Enable Run Button on Flag", false);
	setting_run_button_flag = ModLoader_GetSettingInt("Run Button Flag", 6502);

	setting_physics_max_dash = ModLoader_GetSettingInt("Max Walking Speed", 812);
	setting_physics_max_move = ModLoader_GetSettingInt("Max Top Movement Speed", 1535);
	setting_physics_gravity1 = ModLoader_GetSettingInt("Gravity", 80);
	setting_physics_gravity2 = ModLoader_GetSettingInt("Rising Gravity", 32);
	setting_physics_jump = ModLoader_GetSettingInt("Jump Momentum", 1280);
	setting_physics_dash1 = ModLoader_GetSettingInt("Walking Acceleration", 85);
	setting_physics_dash2 = ModLoader_GetSettingInt("Air Control", 32);
	setting_physics_resist = ModLoader_GetSettingInt("Friction", 51);

	setting_physics_water_max_dash = ModLoader_GetSettingInt("Underwater Max Walking Speed", 406);
	setting_physics_water_max_move = ModLoader_GetSettingInt("Underwater Top Movement Speed", 767);
	setting_physics_water_gravity1 = ModLoader_GetSettingInt("Underwater Gravity", 40);
	setting_physics_water_gravity2 = ModLoader_GetSettingInt("Underwater Rising Gravity", 16);
	setting_physics_water_jump = ModLoader_GetSettingInt("Underwater Jump Momentum", 640);
	setting_physics_water_dash1 = ModLoader_GetSettingInt("Underwater Walking Acceleration", 42);
	setting_physics_water_dash2 = ModLoader_GetSettingInt("Underwater Air Control", 16);
	setting_physics_water_resist = ModLoader_GetSettingInt("Underwater Friction", 25);

	setting_running_speed = ModLoader_GetSettingInt("Max Running Speed", 1218);
}


// Init the whole mod
void InitMod(void)
{
	// Get executable's path
	GetModuleFileNameA(NULL, gModulePath, MAX_PATH);
	PathRemoveFileSpecA(gModulePath);

	// Get path of the data folder
	strcpy(gDataPath, gModulePath);
	strcat(gDataPath, "\\data");

	// The 4 calls above setup gDataPath and gModulePath so they can be used

	// Init settings set by the user
	InitMod_Settings();

	// Functions
	InitMod_Entity();
	InitMod_Sprites();
	InitMod_MyChar();
	
	// debug putmylife
	ModLoader_WriteJump((void*)0x41A1D0, (void*)Replacement_PutMyLife);
}
