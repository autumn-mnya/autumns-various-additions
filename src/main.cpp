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
	setting_walljump_jump_height = ModLoader_GetSettingInt("Walljumps Jump Momentum", 1280);
	setting_walljump_sliding_speed = ModLoader_GetSettingInt("Wallsliding Speed", 554);
	
	// Extra Jumps
	setting_extrajump_jump_height = ModLoader_GetSettingInt("Extra Jump Momentum", 1280);

	// Double Jump
	setting_doublejump_enabled = ModLoader_GetSettingBool("Double Jump Enabled", false);
	setting_doublejump_flag_enabled = ModLoader_GetSettingBool("Double Jump Enable on Flag", false);
	setting_doublejump_flag = ModLoader_GetSettingInt("Double Jump Flag", 6501);
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
