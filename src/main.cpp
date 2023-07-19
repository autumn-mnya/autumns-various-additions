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

// Paths
char gModulePath[MAX_PATH];
char gDataPath[MAX_PATH];
char gPatchesPath[MAX_PATH];
char gBkgPath[MAX_PATH];

bool init_collectables_a_enabled = false;
bool init_collectables_b_enabled = false;
bool init_collectables_c_enabled = false;
bool init_collectables_d_enabled = false;
bool init_collectables_e_enabled = false;

// Init settings
int init_physics_max_dash = 812;
int init_physics_max_move = 1535;
int init_physics_gravity1 = 80;
int init_physics_gravity2 = 32;
int init_physics_dash1 = 85;
int init_physics_dash2 = 32;
int init_physics_resist = 51;
int init_physics_jump = 1280;
int init_physics_water_max_dash = 406;
int init_physics_water_max_move = 767;
int init_physics_water_gravity1 = 40;
int init_physics_water_gravity2 = 16;
int init_physics_water_dash1 = 42;
int init_physics_water_dash2 = 16;
int init_physics_water_resist = 25;
int init_physics_water_jump = 640;
int init_walljump_horizontal_speed = 1000;
int init_walljump_jump_height = 1113;
int init_walljump_sliding_speed = 554; 
int init_walljump_water_horizontal_speed = 300;
int init_walljump_water_jump_height = 556;
int init_extrajump_jump_height = 1280;
int init_extrajump_water_jump_height = 640;
int init_running_speed = 1218;
int init_bounce_speed = 1535;

int init_collectables_a_x_pos = 8;
int init_collectables_b_x_pos = 8;
int init_collectables_c_x_pos = 8;
int init_collectables_d_x_pos = 8;
int init_collectables_e_x_pos = 8;
int init_collectables_a_y_pos = 56;
int init_collectables_b_y_pos = 64;
int init_collectables_c_y_pos = 72;
int init_collectables_d_y_pos = 80;
int init_collectables_e_y_pos = 88;
int init_collectables_a_x_offset = 0;
int init_collectables_b_x_offset = 0;
int init_collectables_c_x_offset = 0;
int init_collectables_d_x_offset = 0;
int init_collectables_e_x_offset = 0;

// Booster fuel
int init_booster_08_fuel = 50;
int init_booster_20_fuel = 50;

// GenericLoad paths

char init_surfaceName_0_Title[MaxSurfaceName] = "Title";
char init_surfaceName_5_Image[MaxSurfaceName] = "Image\\0";
char init_surfaceName_6_Fade[MaxSurfaceName] = "Fade";
char init_surfaceName_8_ItemImage[MaxSurfaceName] = "ItemImage";
char init_surfaceName_11_Arms[MaxSurfaceName] = "Arms";
char init_surfaceName_12_ArmsImage[MaxSurfaceName] = "ArmsImage";
char init_surfaceName_14_StageImage[MaxSurfaceName] = "StageImage";
char init_surfaceName_15_Loading[MaxSurfaceName] = "Loading";
char init_surfaceName_16_MyChar[MaxSurfaceName] = "MyChar";
char init_surfaceName_17_Bullet[MaxSurfaceName] = "Bullet";
char init_surfaceName_19_Caret[MaxSurfaceName] = "Caret";
char init_surfaceName_20_NpcSym[MaxSurfaceName] = "Npc\\NpcSym";
char init_surfaceName_23_NpcRegu[MaxSurfaceName] = "Npc\\NpcRegu";
char init_surfaceName_24_AutumnUI[MaxSurfaceName] = "AutumnUI";
char init_surfaceName_25_AutumnObjects[MaxSurfaceName] = "Npc\\NpcAutumnObj";
char init_surfaceName_26_TextBox[MaxSurfaceName] = "TextBox";
char init_surfaceName_27_Face[MaxSurfaceName] = "Face";
char init_surfaceName_38_AutumnItems[MaxSurfaceName] = "Autumn";
char init_surfaceName_39_AutumnCharacters[MaxSurfaceName] = "Npc\\NpcAutumnChar";

// Function that kills the player (I don't have a place to put this at the moment)
void PlayerDeath()
{
	PlaySoundObject(17, SOUND_MODE_PLAY);
	gMC->cond = 0;
	SetDestroyNpChar(gMC->x, gMC->y, 10 * 0x200, 0x40);
	StartTextScript(40);
}

// This gets run to make sure we reset this on new game basically
void InitMod_PreLaunch_CollectablesEnabled()
{
	init_collectables_a_enabled = enable_collectables_a;
	init_collectables_b_enabled = enable_collectables_b;
	init_collectables_c_enabled = enable_collectables_c;
	init_collectables_d_enabled = enable_collectables_d;
	init_collectables_e_enabled = enable_collectables_e;
}

// This gets run to make sure we reset this on new game basically
void InitMod_PreLaunch_CollectablesPositioning()
{
	init_collectables_a_x_pos = collectables_a_x_pos;
	init_collectables_b_x_pos = collectables_b_x_pos;
	init_collectables_c_x_pos = collectables_c_x_pos;
	init_collectables_d_x_pos = collectables_d_x_pos;
	init_collectables_e_x_pos = collectables_e_x_pos;
	init_collectables_a_y_pos = collectables_a_y_pos;
	init_collectables_b_y_pos = collectables_b_y_pos;
	init_collectables_c_y_pos = collectables_c_y_pos;
	init_collectables_d_y_pos = collectables_d_y_pos;
	init_collectables_e_y_pos = collectables_e_y_pos;
	init_collectables_a_x_offset = collectables_a_x_offset;
	init_collectables_b_x_offset = collectables_b_x_offset;
	init_collectables_c_x_offset = collectables_c_x_offset;
	init_collectables_d_x_offset = collectables_d_x_offset;
	init_collectables_e_x_offset = collectables_e_x_offset;
}

// This gets run to make sure we reset this on new game basically
void InitMod_PreLaunch_PhysicsSettings()
{
	init_physics_max_dash = setting_physics_max_dash;
	init_physics_max_move = setting_physics_max_move;
	init_physics_gravity1 = setting_physics_gravity1;
	init_physics_gravity2 = setting_physics_gravity2;
	init_physics_dash1 = setting_physics_dash1;
	init_physics_dash2 = setting_physics_dash2;
	init_physics_resist = setting_physics_resist;
	init_physics_jump = setting_physics_jump;
	init_physics_water_max_dash = setting_physics_water_max_dash;
	init_physics_water_max_move = setting_physics_water_max_move;
	init_physics_water_gravity1 = setting_physics_water_gravity1;
	init_physics_water_gravity2 = setting_physics_water_gravity2;
	init_physics_water_dash1 = setting_physics_water_dash1;
	init_physics_water_dash2 = setting_physics_water_dash2;
	init_physics_water_resist = setting_physics_water_resist;
	init_physics_water_jump = setting_physics_water_jump;
	init_walljump_horizontal_speed = setting_walljump_horizontal_speed;
	init_walljump_jump_height = setting_walljump_jump_height;
	init_walljump_sliding_speed = setting_walljump_sliding_speed;
	init_walljump_water_horizontal_speed = setting_walljump_water_horizontal_speed;
	init_walljump_water_jump_height = setting_walljump_water_jump_height;
	init_extrajump_jump_height = setting_extrajump_jump_height;
	init_extrajump_water_jump_height = setting_extrajump_water_jump_height;
	init_running_speed = setting_running_speed;
	init_bounce_speed = setting_bounce_speed;
}

void InitMod_PreLaunch_BoosterFuel()
{
	init_booster_08_fuel = booster_08_fuel;
	init_booster_20_fuel = booster_20_fuel;
}

void InitMod_PreLaunch_GenericLoad()
{
	strcpy(init_surfaceName_0_Title, surfaceName_0_Title);
	strcpy(init_surfaceName_5_Image, surfaceName_5_Image);
	strcpy(init_surfaceName_6_Fade, surfaceName_6_Fade);
	strcpy(init_surfaceName_8_ItemImage, surfaceName_8_ItemImage);
	strcpy(init_surfaceName_11_Arms, surfaceName_11_Arms);
	strcpy(init_surfaceName_12_ArmsImage, surfaceName_12_ArmsImage);
	strcpy(init_surfaceName_14_StageImage, surfaceName_14_StageImage);
	strcpy(init_surfaceName_15_Loading, surfaceName_15_Loading);
	strcpy(init_surfaceName_16_MyChar, surfaceName_16_MyChar);
	strcpy(init_surfaceName_17_Bullet, surfaceName_17_Bullet);
	strcpy(init_surfaceName_19_Caret, surfaceName_19_Caret);
	strcpy(init_surfaceName_20_NpcSym, surfaceName_20_NpcSym);
	strcpy(init_surfaceName_23_NpcRegu, surfaceName_23_NpcRegu);
	strcpy(init_surfaceName_24_AutumnUI, surfaceName_24_AutumnUI);
	strcpy(init_surfaceName_25_AutumnObjects, surfaceName_25_AutumnObjects);
	strcpy(init_surfaceName_26_TextBox, surfaceName_26_TextBox);
	strcpy(init_surfaceName_27_Face, surfaceName_27_Face);
	strcpy(init_surfaceName_38_AutumnItems, surfaceName_38_AutumnItems);
	strcpy(init_surfaceName_39_AutumnCharacters, surfaceName_39_AutumnCharacters);
}

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
		if (setting_enable_money_code && setting_money_disable_enemy_money_drops == false)
		{
			ModLoader_WriteJump((void*)0x419030, (void*)Replacement_HitMyCharNpChar);
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
	ModLoader_WriteCall((void*)0x410564, (void*)Replacement_ModeAction_ActValueView_Call);
}

// Loads the new surface files (We can't go above 40, but we can use the unused ones. Except 3 and 4 as they are used in the netplay dll!)
void InitMod_Sprites()
{
	Replacement_LoadGenericData_Calls();
	InitMod_PreLaunch_GenericLoad();
	ModLoader_WriteCall((void*)0x41043B, (void*)Replacement_ModeAction_InitMyChar_Call); // run generic load function in InitMyChar
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

// Init TSC <IMG related calls
void InitMod_TSCImage()
{
	ModLoader_WriteCall((void*)0x40F778, (void*)Replacement_ModeOpening_SetFadeMask_Call);
	ModLoader_WriteCall((void*)0x40FD80, (void*)Replacement_ModeTitle_InitStar_Call);
	ModLoader_WriteCall((void*)0x41D508, (void*)Replacement_LoadProfile_ClearFade_Call);
	ModLoader_WriteCall((void*)0x41D576, (void*)Replacement_InitializeGame_ClearArmsData_Call);
	ModLoader_WriteCall((void*)0x40F91F, (void*)Replacement_ModeOpening_PutTextScript_Call); // Mode Opening
	ModLoader_WriteCall((void*)0x41086F, (void*)Replacement_ModeAction_PutTextScript_Call); // Mode Action
}

// Init TSC <BKG related calls
void InitMod_TSCBkg()
{
	ModLoader_WriteCall((void*)0x402339, (void*)Replacement_InitBack_ReloadBitmap_File_Call); // Release Surface + MakeSurface_File instead of reloading bitmap
	ModLoader_WriteCall((void*)0x40F871, (void*)Replacement_ModeOpening_PutBack_Call);
	ModLoader_WriteCall((void*)0x40F8D1, (void*)Replacement_ModeOpening_PutFront_Call);
	ModLoader_WriteCall((void*)0x410633, (void*)Replacement_ModeAction_PutBack_Call);
	ModLoader_WriteCall((void*)0x4106C3, (void*)Replacement_ModeAction_PutFront_Call);
	if (setting_external_stage_tbl_support == false) // Only replace this if stage.tbl support is disabled
		ModLoader_WriteCall((void*)0x420EB5, (void*)Replacement_TransferStage_ResetFlash_Call);
	memset(bkgTxT_Global, 0, sizeof(bkgTxT_Global));

	// Get path of the Bkg folder
	strcpy(gBkgPath, gDataPath);
	strcat(gBkgPath, "\\bkg");
}

void InitMod_ExternalStageTable()
{
	ModLoader_WriteCall((void*)0x40F67C, (void*)Replacement_Game_InitTextScript2_Call);
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

void InitMod_AutumnConfigDefaults()
{
	ModLoader_WriteCall((void*)0x4124E0, (void*)Replacement_DefaultConfigData);
}

// Change collectables back to the originals
void InitCollectablesEnabled()
{
	enable_collectables_a = init_collectables_a_enabled;
	enable_collectables_b = init_collectables_b_enabled;
	enable_collectables_c = init_collectables_c_enabled;
	enable_collectables_d = init_collectables_d_enabled;
	enable_collectables_e = init_collectables_e_enabled;
}

void InitCollectablesPositioning()
{
	collectables_a_x_pos = init_collectables_a_x_pos;
	collectables_b_x_pos = init_collectables_b_x_pos;
	collectables_c_x_pos = init_collectables_c_x_pos;
	collectables_d_x_pos = init_collectables_d_x_pos;
	collectables_e_x_pos = init_collectables_e_x_pos;
	collectables_a_y_pos = init_collectables_a_y_pos;
	collectables_b_y_pos = init_collectables_b_y_pos;
	collectables_c_y_pos = init_collectables_c_y_pos;
	collectables_d_y_pos = init_collectables_d_y_pos;
	collectables_e_y_pos = init_collectables_e_y_pos;
	collectables_a_x_offset = init_collectables_a_x_offset;
	collectables_b_x_offset = init_collectables_b_x_offset;
	collectables_c_x_offset = init_collectables_c_x_offset;
	collectables_d_x_offset = init_collectables_d_x_offset;
	collectables_e_x_offset = init_collectables_e_x_offset;
}

// Change mychar physics back to their originals
void InitMyCharPhysics()
{
	setting_physics_max_dash = init_physics_max_dash;
	setting_physics_max_move = init_physics_max_move;
	setting_physics_gravity1 = init_physics_gravity1;
	setting_physics_gravity2 = init_physics_gravity2;
	setting_physics_dash1 = init_physics_dash1;
	setting_physics_dash2 = init_physics_dash2;
	setting_physics_resist = init_physics_resist;
	setting_physics_jump = init_physics_jump;
	setting_physics_water_max_dash = init_physics_water_max_dash;
	setting_physics_water_max_move = init_physics_water_max_move;
	setting_physics_water_gravity1 = init_physics_water_gravity1;
	setting_physics_water_gravity2 = init_physics_water_gravity2;
	setting_physics_water_dash1 = init_physics_water_dash1;
	setting_physics_water_dash2 = init_physics_water_dash2;
	setting_physics_water_resist = init_physics_water_resist;
	setting_physics_water_jump = init_physics_water_jump;
	setting_walljump_horizontal_speed = init_walljump_horizontal_speed;
	setting_walljump_jump_height = init_walljump_jump_height;
	setting_walljump_sliding_speed = init_walljump_sliding_speed;
	setting_walljump_water_horizontal_speed = init_walljump_water_horizontal_speed;
	setting_walljump_water_jump_height = init_walljump_water_jump_height;
	setting_extrajump_jump_height = init_extrajump_jump_height;
	setting_extrajump_water_jump_height = init_extrajump_water_jump_height;
	setting_running_speed = init_running_speed;
	setting_bounce_speed = init_bounce_speed;
}

void InitMyCharBoostFuel()
{
	booster_08_fuel = init_booster_08_fuel;
	booster_20_fuel = init_booster_20_fuel;
	Mod_WriteBoosterFuel();
}

void InitGameSurfaces()
{
	strcpy(surfaceName_0_Title, init_surfaceName_0_Title);
	strcpy(surfaceName_5_Image, init_surfaceName_5_Image);
	strcpy(surfaceName_6_Fade, init_surfaceName_6_Fade);
	strcpy(surfaceName_8_ItemImage, init_surfaceName_8_ItemImage);
	strcpy(surfaceName_11_Arms, init_surfaceName_11_Arms);
	strcpy(surfaceName_12_ArmsImage, init_surfaceName_12_ArmsImage);
	strcpy(surfaceName_14_StageImage, init_surfaceName_14_StageImage);
	strcpy(surfaceName_15_Loading, init_surfaceName_15_Loading);
	strcpy(surfaceName_16_MyChar, init_surfaceName_16_MyChar);
	strcpy(surfaceName_17_Bullet, init_surfaceName_17_Bullet);
	strcpy(surfaceName_19_Caret, init_surfaceName_19_Caret);
	strcpy(surfaceName_20_NpcSym, init_surfaceName_20_NpcSym);
	strcpy(surfaceName_23_NpcRegu, init_surfaceName_23_NpcRegu);
	strcpy(surfaceName_24_AutumnUI, init_surfaceName_24_AutumnUI);
	strcpy(surfaceName_25_AutumnObjects, init_surfaceName_25_AutumnObjects);
	strcpy(surfaceName_26_TextBox, init_surfaceName_26_TextBox);
	strcpy(surfaceName_27_Face, init_surfaceName_27_Face);
	strcpy(surfaceName_38_AutumnItems, init_surfaceName_38_AutumnItems);
	strcpy(surfaceName_39_AutumnCharacters, init_surfaceName_39_AutumnCharacters);
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

	if (setting_enable_default_config_options)
		InitMod_AutumnConfigDefaults();
}