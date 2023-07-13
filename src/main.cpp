#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Main.h"
#include "ModSettings.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ASMPatches.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "PauseScreen.h"
#include "TextScript.h"
#include "TileCollisionBoss.h"
#include "TileCollisionBullet.h"
#include "TileCollisionEntity.h"
#include "TileCollisionMyChar.h"

// Paths
char gModulePath[MAX_PATH];
char gDataPath[MAX_PATH];

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

void InitMod_SFX()
{
	ModLoader_WriteCall((void*)0x411DEF, (void*)Replacement_LoadGenericData_Pixtone_Sprintf);
}

// Init MyChar replacement
void InitMod_MyChar()
{
	ModLoader_WriteCall((void*)0x416B22, (void*)Replacement_SetMyCharPosition_InitStar_Call);
	InitMyCharPhysicsPatch();
	InitCustomMyCharPatch();
}

void InitMod_TileCollision()
{
	ModLoader_WriteJump((void*)0x402B30, (void*)Replacement_JudgeHitBulletBlock2);
	ModLoader_WriteJump((void*)0x403740, (void*)Replacement_HitBulletMap);
	ModLoader_WriteJump((void*)0x417E40, (void*)Replacement_HitMyCharMap);
	ModLoader_WriteJump((void*)0x471160, (void*)Replacement_HitNpCharMap);
	ModLoader_WriteJump((void*)0x473080, (void*)Replacement_HitBossMap);
}

void InitMod_PauseScreen()
{
	ModLoader_WriteJump((void*)0x40DD70, (void*)Call_Pause);
}

void InitMod_GameUI()
{
	ModLoader_WriteCall((void*)0x410683, (void*)Replacement_PutMyChar_Call);
}

void InitMod_ASMPatches()
{
	// Random ASM Patches that arent related to any of the other Init functions go here
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

	if (setting_enable_entity)
		InitMod_Entity();

	if (setting_enable_surfaces)
		InitMod_Sprites();

	if (setting_enable_sound_effect_code)
		InitMod_SFX();

	if (setting_enable_mychar)
		InitMod_MyChar();

	if (setting_enable_tilecollision)
		InitMod_TileCollision();

	if (setting_enable_pause_screen)
		InitMod_PauseScreen();

	if (setting_enable_text_script_code)
		InitMod_TSC();

	if (setting_enable_ui)
		InitMod_GameUI();

	// debug testing hud
	// ModLoader_WriteJump((void*)0x41A1D0, Replacement_Debug_PutMyLife);

	/*
	InitMod_ASMPatches();
	*/
}
