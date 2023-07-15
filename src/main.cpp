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
#include "BKG.h"
#include "Collectables.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "MycParam.h"
#include "PauseScreen.h"
#include "Profile.h"
#include "TextScript.h"
#include "TileCollisionBoss.h"
#include "TileCollisionBullet.h"
#include "TileCollisionEntity.h"
#include "TileCollisionMyChar.h"

// Paths
char gModulePath[MAX_PATH];
char gDataPath[MAX_PATH];
char gBkgPath[MAX_PATH];

bool init_collectables_a_enabled = false;
bool init_collectables_b_enabled = false;
bool init_collectables_c_enabled = false;
bool init_collectables_d_enabled = false;
bool init_collectables_e_enabled = false;

// Function that kills the player (I don't have a place to put this at the moment)
void PlayerDeath()
{
	PlaySoundObject(17, SOUND_MODE_PLAY);
	gMC->cond = 0;
	SetDestroyNpChar(gMC->x, gMC->y, 10 * 0x200, 0x40);
	StartTextScript(40);
}

void InitMod_CollectablesEnabled()
{
	init_collectables_a_enabled = enable_collectables_a;
	init_collectables_b_enabled = enable_collectables_b;
	init_collectables_c_enabled = enable_collectables_c;
	init_collectables_d_enabled = enable_collectables_d;
	init_collectables_e_enabled = enable_collectables_e;
}

// Inits anything relating to entities. The main thing are the 3 ModLoader_WriteJump's -- These replace every function that uses the Npc Table, and instead we also insert our new table!
void InitMod_Entity()
{
	InitCustomEntityPatch();
	ModLoader_WriteJump((void*)0x46FA00, (void*)Replacement_ActNpChar);
	ModLoader_WriteJump((void*)0x46FAB0, (void*)Replacement_ChangeNpCharByEvent);
	ModLoader_WriteJump((void*)0x46FD10, (void*)Replacement_ChangeCheckableNpCharByEvent);
	// This only gets replaced if TSC is loaded
	if (setting_enable_text_script_code && setting_enable_money_code && setting_money_disable_enemy_money_drops == false)
	{
		ModLoader_WriteJump((void*)0x419030, (void*)Replacement_HitMyCharNpChar);
		ModLoader_WriteJump((void*)0x46F2B0, (void*)Replacement_SetExpObjects);
	}
}

// Loads the new surface files (We can't go above 40, but we can use the unused ones. Except 3 and 4 as they are used in the netplay dll!)
void InitMod_Sprites()
{
	ModLoader_WriteCall((void*)0x41143D, (void*)Replacement_StageImageSurfaceCall);
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

void InitMod_SaveData()
{
	ModLoader_WriteCall((void*)0x41D213, (void*)Replacement_SaveProfile_LastMemcpy_Call);
	ModLoader_WriteCall((void*)0x41D22D, (void*)Replacement_SaveProfile_fwrite_Call);
	ModLoader_WriteCall((void*)0x41D353, (void*)Replacement_LoadProfile_fclose_Call);
	isLoadingSave = false;
}

void InitMod_GameUI()
{
	ModLoader_WriteCall((void*)0x410683, (void*)Replacement_PutMyChar_Call);
	ModLoader_WriteCall((void*)0x410856, (void*)Replacement_PutActiveArmsList_Call);
}

void InitMod_TSCImage()
{
	ModLoader_WriteCall((void*)0x40F778, (void*)Replacement_ModeOpening_SetFadeMask_Call);
	ModLoader_WriteCall((void*)0x40FD80, (void*)Replacement_ModeTitle_InitStar_Call);
	ModLoader_WriteCall((void*)0x41D508, (void*)Replacement_LoadProfile_ClearFade_Call);
	ModLoader_WriteCall((void*)0x41D576, (void*)Replacement_InitializeGame_ClearArmsData_Call);
	ModLoader_WriteCall((void*)0x40F91F, (void*)Replacement_ModeOpening_PutTextScript_Call); // Mode Opening
	ModLoader_WriteCall((void*)0x41086F, (void*)Replacement_ModeAction_PutTextScript_Call); // Mode Action
}

void InitMod_TSCBkg()
{
	ModLoader_WriteCall((void*)0x402339, (void*)Replacement_InitBack_ReloadBitmap_File_Call); // Release Surface + MakeSurface_File instead of reloading bitmap
	ModLoader_WriteCall((void*)0x40F871, (void*)Replacement_ModeOpening_PutBack_Call);
	ModLoader_WriteCall((void*)0x40F8D1, (void*)Replacement_ModeOpening_PutFront_Call);
	ModLoader_WriteCall((void*)0x410633, (void*)Replacement_ModeAction_PutBack_Call);
	ModLoader_WriteCall((void*)0x4106C3, (void*)Replacement_ModeAction_PutFront_Call);
	ModLoader_WriteCall((void*)0x420EB5, (void*)Replacement_TransferStage_ResetFlash_Call);
	memset(bkgTxT_Global, 0, sizeof(bkgTxT_Global));
}

void InitMod_ASMPatches()
{
	// Random ASM Patches that arent related to any of the other Init functions go here
}

void InitCollectablesEnabled()
{
	enable_collectables_a = init_collectables_a_enabled;
	enable_collectables_b = init_collectables_b_enabled;
	enable_collectables_c = init_collectables_c_enabled;
	enable_collectables_d = init_collectables_d_enabled;
	enable_collectables_e = init_collectables_e_enabled;
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
	{
		InitMod_TSC();
		InitMod_TSCImage();
		InitMod_TSCBkg();
		InitMod_CollectablesEnabled();

		// Get path of the Bkg folder
		strcpy(gBkgPath, gDataPath);
		strcat(gBkgPath, "\\bkg");
	}

	if (setting_enable_ui)
		InitMod_GameUI();

	if (setting_enable_savedata_code)
		InitMod_SaveData();

	// Fix Teleporter menu flicker
	if (setting_enable_teleporter_bugfix)
		ModLoader_WriteCall((void*)0x41DB16, (void*)PutBitmap4);

	// debug testing hud
	// ModLoader_WriteJump((void*)0x41A1D0, Replacement_Debug_PutMyLife);

	/*
	InitMod_ASMPatches();
	*/
}
