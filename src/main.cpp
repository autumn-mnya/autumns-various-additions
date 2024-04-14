#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Main.h"
#include "ModSettings.h"

#include "AutPI.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ASMPatches.h"
#include "ASMLoader.h"
#include "BKG.h"
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
#include "PauseScreen.h"
#include "Profile.h"
#include "Stage.h"
#include "SurfaceDefines.h"
#include "TextScript.h"
#include "TileCollisionBoss.h"
#include "TileCollisionBullet.h"
#include "TileCollisionEntity.h"
#include "TileCollisionMyChar.h"

#include "ModInit.h"

#include "fmod/fmodAudio.h"

// Paths
char gPatchesPath[MAX_PATH];
char gBkgPath[MAX_PATH];
char gAudioPath[MAX_PATH];
char gAVAPath[MAX_PATH];
char gAVAConfigPath[MAX_PATH];

const char* audioDirectory = "data\\audio\\Desktop";

// Inits anything relating to entities. The main thing are the 3 ModLoader_WriteJump's -- These replace every function that uses the Npc Table, and instead we also insert our new table!
void InitMod_Entity()
{
	InitCustomEntityPatch();
	ModLoader_WriteJump((void*)0x46FA00, (void*)Replacement_ActNpChar);
	ModLoader_WriteJump((void*)0x46FAB0, (void*)Replacement_ChangeNpCharByEvent);
	ModLoader_WriteJump((void*)0x46FD10, (void*)Replacement_ChangeCheckableNpCharByEvent);
	// This only gets replaced if TSC is loaded
	if (setting_enable_text_script_code)
	{
		if (setting_enable_money_code == true)
		{
			ModLoader_WriteJump((void*)0x419030, (void*)Replacement_HitMyCharNpChar);

			if (setting_money_disable_enemy_money_drops == false)
				ModLoader_WriteJump((void*)0x46F2B0, (void*)Replacement_SetExpObjects);
		}

		// Replace NPCs when using <MIM
		if (setting_enable_mim_mod)
		{
			ModLoader_WriteJump((void*)0x43CDE0, (void*)Replacement_ActNpc111);
			ModLoader_WriteJump((void*)0x43D0A0, (void*)Replacement_ActNpc112);
			ModLoader_WriteJump((void*)0x445660, (void*)Replacement_ActNpc150);
		}
	}

	// This sets "isLoadingSave" to false after we're done loading, and after entities are loaded
	RegisterActionElement(AutumnsVariousAdditionsModeActionAct);
}

// Loads the new surface files (We can't go above 40, but we can use the unused ones. Except 3 and 4 as they are used in the netplay dll!)
void InitMod_Sprites()
{
	Replacement_LoadGenericData_Calls();
	InitMod_PreLaunch_GenericLoad();
	RegisterInitElement(AutumnsVariousAdditionsModeActionInit);
}

void InitMod_SFX()
{
	ModLoader_WriteCall((void*)0x411DEF, (void*)Replacement_LoadGenericData_Pixtone_Sprintf);
}

// Init MyChar replacement
void InitMod_MyChar()
{
	ModLoader_WriteCall((void*)0x416B22, (void*)Replacement_SetMyCharPosition_InitStar_Call);
	ModLoader_WriteCall((void*)0x414B5F, (void*)Replacement_InitMyChar_memset_Call); // for initmychar related things, we replace the memset call and do our own thing
	InitMyCharPhysicsPatch();
	InitCustomMyCharPatch();
	if (setting_enable_text_script_code && setting_enable_mim_mod) // Only if TSC + <MIM is enabled
		ModLoader_WriteCall((void*)0x415535, (void*)Replacement_PutMyChar_PutChar_Call);

	InitMod_PreLaunch_PhysicsSettings(); // Init Settings
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
	ModLoader_WriteCall((void*)0x41D213, (void*)Replacement_SaveProfile_LastMemcpy_Call); // autpi
	ModLoader_WriteCall((void*)0x41D22D, (void*)Replacement_SaveProfile_fwrite_Call); // autpi
	ModLoader_WriteCall((void*)0x41D353, (void*)Replacement_LoadProfile_fclose_Call); // autpi
	isLoadingSave = false;
}

void InitMod_GameUI()
{
	if (legacy_extra_jumps_ui == true)
		RegisterAbovePlayerElement(AutumnsVariousAdditionsExtraJumpUI);
	else
		RegisterPlayerHudElement(AutumnsVariousAdditionsExtraJumpUI);

	RegisterPlayerHudElement(AutumnsVariousAdditionsPlayerHud);
}

// Init TSC <IMG related calls
void InitMod_TSCImage()
{
	RegisterOpeningInitElement(AutumnsVariousAdditionsModeOpeningInit);
	RegisterTitleInitElement(AutumnsVariousAdditionsModeTitleInit);
	ModLoader_WriteCall((void*)0x41D508, (void*)Replacement_LoadProfile_ClearFade_Call);
	ModLoader_WriteCall((void*)0x41D576, (void*)Replacement_InitializeGame_ClearArmsData_Call); // AUTPI
	RegisterOpeningBelowTextBoxElement(AutumnsVariousAdditions_ModeOpening_TextBoxBelow);
	RegisterOpeningAboveTextBoxElement(AutumnsVariousAdditionsTextBoxAbove);
	RegisterBelowTextBoxElement(AutumnsVariousAdditions_ModeAction_TextBoxBelow);
	RegisterAboveTextBoxElement(AutumnsVariousAdditionsTextBoxAbove);
}

void FModUpdate()
{
	FmodStudioObj->update();
}
void ReleaseFModAudio()
{
	ReleaseFmod();
}

void InitFMOD_MusicCalls()
{
	ModLoader_WriteCall((void*)0x40F756, (void*)Replacement_ModeOpening_ChangeMusic_Silence_Call);
	ModLoader_WriteCall((void*)0x40FE81, (void*)Replacement_ModeTitle_ChangeMusic_RunningHell_Call);
	ModLoader_WriteCall((void*)0x40FE96, (void*)Replacement_ModeTitle_ChangeMusic_TorokosTheme_Call);
	ModLoader_WriteCall((void*)0x40FEAB, (void*)Replacement_ModeTitle_ChangeMusic_White_Call);
	ModLoader_WriteCall((void*)0x40FEC0, (void*)Replacement_ModeTitle_ChangeMusic_Safety_Call);
	ModLoader_WriteCall((void*)0x40FECC, (void*)Replacement_ModeTitle_ChangeMusic_CaveStory_Call);
	ModLoader_WriteCall((void*)0x41038C, (void*)Replacement_ModeTitle_ChangeMusic_Silence_Call);
}

void InitMod_FMOD()
{
	fmod_Init();
	fmod_LoadBanks();
	InitFMOD_MusicCalls();

	RegisterOpeningActionElement(FModUpdate);
	RegisterTitleActionElement(FModUpdate);
	RegisterActionElement(FModUpdate);
	RegisterReleaseElement(ReleaseFModAudio);
	RegisterInitializeGameInitElement(FModClearEventNames);
}

// Init TSC <BKG related calls
// AutPI is not used for this function.
void InitMod_TSCBkg()
{
	ModLoader_WriteCall((void*)0x402339, (void*)Replacement_InitBack_ReloadBitmap_File_Call); // Release Surface + MakeSurface_File instead of reloading bitmap
	ModLoader_WriteCall((void*)0x40F881, (void*)Replacement_ModeOpening_PutStageBack_Call);
	ModLoader_WriteCall((void*)0x40F8D1, (void*)Replacement_ModeOpening_PutFront_Call);
	ModLoader_WriteCall((void*)0x410643, (void*)Replacement_ModeAction_PutStageBack_Call);
	ModLoader_WriteCall((void*)0x4106C3, (void*)Replacement_ModeAction_PutFront_Call);
	if (setting_external_stage_tbl_support == false) // Only replace this if stage.tbl support is disabled
		RegisterTransferStageInitElement(AutumnsVariousAdditionsTransferStageInit);
	memset(bkgTxT_Global, 0, sizeof(bkgTxT_Global));

	// Get path of the Bkg folder
	strcpy(gBkgPath, gDataPath);
	strcat(gBkgPath, "\\bkg");
}

void InitMod_ExternalStageTable()
{
	RegisterPreModeElement(AutumnsVariousAdditionsStageTableInit);
	ModLoader_WriteJump((void*)0x420BE0, (void*)Replacement_TransferStage); // TransferStage replaced for stage.tbl changing
}

void InitMod_TeleporterMenuFix()
{
	ModLoader_WriteCall((void*)0x41DB16, (void*)PutBitmap4);
}

void InitMod_DisableWindowRect()
{
	ModLoader_WriteCall((void*)0x40F710, (void*)Replacement_SaveWindowRect);
}

void InitMod_DisableErrorLog()
{
	ModLoader_WriteJump((void*)0x410DE0, (void*)Replacement_ErrorLog);
}

void InitMod_AutumnConfigDefaults()
{
	ModLoader_WriteCall((void*)0x4124E0, (void*)Replacement_DefaultConfigData);
}

// Init the whole mod
void InitMod(void)
{
	LoadAutPiDll();
	// Get executable's path
	GetModuleFileNameA(NULL, gModulePath, MAX_PATH);
	PathRemoveFileSpecA(gModulePath);

	// Get path of the data folder
	strcpy(gDataPath, gModulePath);
	strcat(gDataPath, "\\data");

	// Get path of the audio folder
	strcpy(gAudioPath, gModulePath);
	strcat(gAudioPath, "\\");
	strcat(gAudioPath, audioDirectory);

	// Get path of the ava folder
	strcpy(gAVAPath, gModulePath);
	strcat(gAVAPath, "\\ava");

	// Get path of the ava config folder

	strcpy(gAVAConfigPath, gAVAPath);
	strcat(gAVAConfigPath, "\\config");

	// The 4 calls above setup gDataPath and gModulePath so they can be used

	// Init settings set by the user
	//InitMod_Settings();
	InitSettingsRevamp();

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
		InitMod_PreLaunch_CollectablesEnabled();
		InitMod_PreLaunch_CollectablesPositioning();
		InitMod_PreLaunch_BoosterFuel();

		// Get path of the patches folder if asm loader is enabled
		if (setting_enable_asm_loader)
		{
			strcpy(gPatchesPath, gDataPath);
			strcat(gPatchesPath, "\\patches");
		}

		if (setting_external_stage_tbl_support == true)
			InitMod_ExternalStageTable();
	}

	if (setting_collab_enabled)
		InitMod_PreLaunch_CollabName();

	if (setting_enable_ui)
		InitMod_GameUI();

	if (setting_enable_savedata_code)
		InitMod_SaveData();

	// Fix Teleporter menu flicker
	if (setting_enable_teleporter_bugfix)
		InitMod_TeleporterMenuFix();

	// Disable window rect saving by default because tbh it REALLY annoys me for some reason??
	if (setting_disable_window_rect_saving)
		InitMod_DisableWindowRect();

	// Disable error.log
	if (setting_disable_errorlog_saving)
		InitMod_DisableErrorLog();

	if (setting_enable_default_config_options)
		InitMod_AutumnConfigDefaults();

	if (setting_enable_fmod)
		InitMod_FMOD();
}