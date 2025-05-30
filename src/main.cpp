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
#include "API_Pause.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ASMPatches.h"
#include "ASMLoader.h"
#include "Collectables.h"
#include "Config.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "EntityTable.h"
#include "Game.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "LoadPixtone.h"
#include "InitMod.h"
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

#include "ModInit.h"

#include "lua/lua.h"

// Paths
char gPatchesPath[MAX_PATH];
char gAudioPath[MAX_PATH];
char gAVAPath[MAX_PATH];
char gAVAConfigPath[MAX_PATH];
char gSavePath[MAX_PATH];

int gCurrentGameMode = 0;

void SetModeOpening()
{
	gCurrentGameMode = 1;
}

void SetModeTitle()
{
	gCurrentGameMode = 2;
}

void SetModeAction()
{
	gCurrentGameMode = 3;
}

static int Callback_Resume(OptionsMenu* parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	PlaySoundObject(18, SOUND_MODE_PLAY);
	return enum_ESCRETURN_continue;
}

void AVAPauseEntries()
{
	// For a later date --> I can now add
	// add_pause_entry(GetOptionsMain(), "AVA TEST", Callback_Resume, NULL, NULL, 0, FALSE, GetNumEntriesAddedMain());
}

void ReleaseData()
{
	if (autpiDLL != nullptr)
		free(autpiDLL);
	if (pauseDLL != nullptr)
		free(pauseDLL);
}

// Init the whole mod
void InitMod(void)
{
	LoadAutPiDll();
	RegisterReleaseElement(ReleaseData); // Release dlls once finished with them
	// Get executable's path
	GetModuleFileNameA(NULL, gModulePath, MAX_PATH);
	PathRemoveFileSpecA(gModulePath);

	// Get path of the data folder
	strcpy(gDataPath, gModulePath);
	strcat(gDataPath, "\\data");

	// Get path of the ava folder
	strcpy(gAVAPath, gDataPath);
	strcat(gAVAPath, "\\ava");

	// Get path of the ava config folder

	strcpy(gAVAConfigPath, gAVAPath);
	strcat(gAVAConfigPath, "\\config");

	// Get path of the savedata folder

	strcpy(gSavePath, gModulePath);
	strcat(gSavePath, "\\savedata");

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

	if (setting_enable_text_script_code)
	{
		InitMod_TSC();
		InitMod_TSCImage();
		InitMod_PreLaunch_CollectablesEnabled();
		InitMod_PreLaunch_CollectablesPositioning();
		InitMod_PreLaunch_BoosterFuel();

		// Get path of the patches folder if asm loader is enabled
		if (setting_enable_asm_loader)
		{
			strcpy(gPatchesPath, gDataPath);
			strcat(gPatchesPath, "\\patches");
		}
	}

	if (setting_enable_ui)
		InitMod_GameUI();

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

	RegisterLuaPreGlobalModCSElement(SetAVAGlobalString);
	RegisterLuaMetadataElement(PushAVAMetadata);
	RegisterLuaFuncElement(SetAVALua);

	// jank way of getting mode
	RegisterOpeningInitElement(SetModeOpening);
	RegisterTitleInitElement(SetModeTitle);
	RegisterInitElement(SetModeAction);

	// register init dialogue choices (for resetting while mid <CHO)
	RegisterOpeningInitElement(InitMod_DialogueChoice);
	RegisterTitleInitElement(InitMod_DialogueChoice);
	RegisterInitElement(InitMod_DialogueChoice);

	if (pause_menu_ava_enabled)
	{
		LoadPauseMenuDll();
		RegisterPreModeElement(AVAPauseEntries);
	}
}