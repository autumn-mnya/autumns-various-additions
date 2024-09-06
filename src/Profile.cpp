#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Profile.h"

#include "main.h"
#include "ModInit.h"
#include "cave_story.h"
#include "Collectables.h"
#include "Draw.h"
#include "EntityLoad.h"
#include "GenericLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "Respawn.h"
#include "Stage.h"
#include "SurfaceDefines.h"
#include "TextScript.h"
#include "TextScriptCustomLoad.h"
#include "TileCollisionMyChar.h"

const char* gAvaSaveFileName = "Profile.AVA.dat";
const char* gAutumnProfileCode = "AutumnMnyaHazel";

bool isLoadingSave = false;

void SaveAvaFile()
{
	FILE* fp;
	CustomProfileData profile;

	char path[MAX_PATH];

	// Get path
	sprintf(path, "%s\\%s", gSavePath, gAvaSaveFileName);

	fp = fopen(path, "wb");
	if (fp == NULL)
		return;

	memset(&profile, 0, sizeof(CustomProfileData));

	memcpy(profile.code, gAutumnProfileCode, sizeof(profile.code));
	memcpy(profile.imgFolder, TSC_IMG_Folder, sizeof(profile.imgFolder));
	profile.playerMoney = playerMoney;
	memcpy(&profile.pCollectables, &gCollectables, sizeof(COLLECTABLES));
	// Collectables enabled on screen
	profile.enable_collect_a = enable_collectables_a;
	profile.enable_collect_b = enable_collectables_b;
	profile.enable_collect_c = enable_collectables_c;
	profile.enable_collect_d = enable_collectables_d;
	profile.enable_collect_e = enable_collectables_e;
	// Collectables Position
	profile.collectables_a_x_pos = collectables_a_x_pos;
	profile.collectables_a_y_pos = collectables_a_y_pos;
	profile.collectables_a_x_offset = collectables_a_x_offset;
	profile.collectables_b_x_pos = collectables_b_x_pos;
	profile.collectables_b_y_pos = collectables_b_y_pos;
	profile.collectables_b_x_offset = collectables_b_x_offset;
	profile.collectables_c_x_pos = collectables_c_x_pos;
	profile.collectables_c_y_pos = collectables_c_y_pos;
	profile.collectables_c_x_offset = collectables_c_x_offset;
	profile.collectables_d_x_pos = collectables_d_x_pos;
	profile.collectables_d_y_pos = collectables_d_y_pos;
	profile.collectables_d_x_offset = collectables_d_x_offset;
	profile.collectables_e_x_pos = collectables_e_x_pos;
	profile.collectables_e_y_pos = collectables_e_y_pos;
	profile.collectables_e_x_offset = collectables_e_x_offset;
	// <PHY
	profile.phy_physics_max_dash = setting_physics_max_dash;
	profile.phy_physics_max_move = setting_physics_max_move;
	profile.phy_physics_gravity1 = setting_physics_gravity1;
	profile.phy_physics_gravity2 = setting_physics_gravity2;
	profile.phy_physics_dash1 = setting_physics_dash1;
	profile.phy_physics_dash2 = setting_physics_dash2;
	profile.phy_physics_resist = setting_physics_resist;
	profile.phy_physics_jump = setting_physics_jump;
	profile.phy_physics_water_max_dash = setting_physics_water_max_dash;
	profile.phy_physics_water_max_move = setting_physics_water_max_move;
	profile.phy_physics_water_gravity1 = setting_physics_water_gravity1;
	profile.phy_physics_water_gravity2 = setting_physics_water_gravity2;
	profile.phy_physics_water_dash1 = setting_physics_water_dash1;
	profile.phy_physics_water_dash2 = setting_physics_water_dash2;
	profile.phy_physics_water_resist = setting_physics_water_resist;
	profile.phy_physics_water_jump = setting_physics_water_jump;
	profile.phy_walljump_horizontal_speed = setting_walljump_horizontal_speed;
	profile.phy_walljump_jump_height = setting_walljump_jump_height;
	profile.phy_walljump_sliding_speed = setting_walljump_sliding_speed;
	profile.phy_walljump_water_horizontal_speed = setting_walljump_water_horizontal_speed;
	profile.phy_walljump_water_jump_height = setting_walljump_water_jump_height;
	profile.phy_extrajump_jump_height = setting_extrajump_jump_height;
	profile.phy_extrajump_water_jump_height = setting_extrajump_water_jump_height;
	profile.phy_running_speed = setting_running_speed;
	profile.phy_bounce_speed = setting_bounce_speed;
	memcpy(&profile.varData, varData, sizeof(varData));
	profile.mim_num = mim_num;
	profile.booster_08_fuel = booster_08_fuel;
	profile.booster_20_fuel = booster_20_fuel;
	// Set Custom Tables
	memcpy(profile.npc_tbl, npcTblPath, sizeof(profile.npc_tbl));
	// Set <CSF surfaces
	memcpy(profile.surfaceName_0_Title, surfaceName_0_Title, sizeof(profile.surfaceName_0_Title));
	memcpy(profile.surfaceName_5_Image, surfaceName_5_Image, sizeof(profile.surfaceName_5_Image));
	memcpy(profile.surfaceName_6_Fade, surfaceName_6_Fade, sizeof(profile.surfaceName_6_Fade));
	memcpy(profile.surfaceName_8_ItemImage, surfaceName_8_ItemImage, sizeof(profile.surfaceName_8_ItemImage));
	memcpy(profile.surfaceName_11_Arms, surfaceName_11_Arms, sizeof(profile.surfaceName_11_Arms));
	memcpy(profile.surfaceName_12_ArmsImage, surfaceName_12_ArmsImage, sizeof(profile.surfaceName_12_ArmsImage));
	memcpy(profile.surfaceName_14_StageImage, surfaceName_14_StageImage, sizeof(profile.surfaceName_14_StageImage));
	// Skip Loading
	memcpy(profile.surfaceName_16_MyChar, surfaceName_16_MyChar, sizeof(profile.surfaceName_16_MyChar));
	memcpy(profile.surfaceName_17_Bullet, surfaceName_17_Bullet, sizeof(profile.surfaceName_17_Bullet));
	memcpy(profile.surfaceName_19_Caret, surfaceName_19_Caret, sizeof(profile.surfaceName_19_Caret));
	memcpy(profile.surfaceName_20_NpcSym, surfaceName_20_NpcSym, sizeof(profile.surfaceName_20_NpcSym));
	memcpy(profile.surfaceName_23_NpcRegu, surfaceName_23_NpcRegu, sizeof(profile.surfaceName_23_NpcRegu));
	memcpy(profile.surfaceName_24_AutumnUI, surfaceName_24_AutumnUI, sizeof(profile.surfaceName_24_AutumnUI));
	memcpy(profile.surfaceName_25_AutumnObjects, surfaceName_25_AutumnObjects, sizeof(profile.surfaceName_25_AutumnObjects));
	memcpy(profile.surfaceName_26_TextBox, surfaceName_26_TextBox, sizeof(profile.surfaceName_26_TextBox));
	memcpy(profile.surfaceName_27_Face, surfaceName_27_Face, sizeof(profile.surfaceName_27_Face));
	memcpy(profile.surfaceName_38_AutumnItems, surfaceName_38_AutumnItems, sizeof(profile.surfaceName_38_AutumnItems));
	memcpy(profile.surfaceName_39_AutumnCharacters, surfaceName_39_AutumnCharacters, sizeof(profile.surfaceName_39_AutumnCharacters));
	// Set Head.tsc/ArmsItem.tsc data
	memcpy(profile.armsitem_tsc, CustomArmsItemTSCLocation, sizeof(profile.armsitem_tsc));
	memcpy(profile.head_tsc, CustomHeadTSCLocation, sizeof(profile.head_tsc));
	// PixTone folder
	memcpy(profile.pixtoneFolder, global_pixtoneFolder, sizeof(profile.pixtoneFolder));
	// Version 1.0.7
	profile.phy_physics_boost20_accel_up = setting_physics_boost20_accel_up;
	profile.phy_physics_boost20_accel_left = setting_physics_boost20_accel_left;
	profile.phy_physics_boost20_accel_right = setting_physics_boost20_accel_right;
	profile.phy_physics_boost20_accel_down = setting_physics_boost20_accel_down;
	profile.phy_physics_boost20_accel_up_no_key = setting_physics_boost20_accel_up_no_key;

	profile.phy_physics_boost08_accel_add = setting_physics_boost08_accel_add;
	profile.phy_physics_boost08_accel_max_speed = setting_physics_boost08_accel_max_speed;

	profile.phy_physics_max_speed_left = setting_physics_max_speed_left;
	profile.phy_physics_max_speed_right = setting_physics_max_speed_right;
	profile.phy_physics_max_speed_up = setting_physics_max_speed_up;
	profile.phy_physics_max_speed_down = setting_physics_max_speed_down;

	profile.phy_physics_water_max_speed_left = setting_physics_water_max_speed_left;
	profile.phy_physics_water_max_speed_right = setting_physics_water_max_speed_right;
	profile.phy_physics_water_max_speed_up = setting_physics_water_max_speed_up;
	profile.phy_physics_water_max_speed_down = setting_physics_water_max_speed_down;

	// Write to file
	fwrite(&profile, sizeof(CustomProfileData), 1, fp);

	fclose(fp);

	return;
}

CustomProfileData setDataProfile;

void LoadAvaFile()
{
	isLoadingSave = true;

	FILE* fp;
	CustomProfileData profile;
	char path[MAX_PATH];

	// Get path
	sprintf(path, "%s\\%s", gSavePath, gAvaSaveFileName);

	// Open file
	fp = fopen(path, "rb");
	if (fp == NULL)
		return;

	// Read data
	fseek(fp, 0, SEEK_SET);
	memset(&profile, 0, sizeof(CustomProfileData));
	fread(&profile, sizeof(CustomProfileData), 1, fp);
	fclose(fp);

	printf("Trying to do this :(\n");

	strcpy(TSC_IMG_Folder, profile.imgFolder);

	// custom npc table
	strcpy(npcTblPath, profile.npc_tbl);

	// <CSF surface names
	strcpy(surfaceName_0_Title, profile.surfaceName_0_Title);
	strcpy(surfaceName_5_Image, profile.surfaceName_5_Image);
	strcpy(surfaceName_6_Fade, profile.surfaceName_6_Fade);
	strcpy(surfaceName_8_ItemImage, profile.surfaceName_8_ItemImage);
	strcpy(surfaceName_11_Arms, profile.surfaceName_11_Arms);
	strcpy(surfaceName_12_ArmsImage, profile.surfaceName_12_ArmsImage);
	strcpy(surfaceName_14_StageImage, profile.surfaceName_14_StageImage);
	strcpy(surfaceName_16_MyChar, profile.surfaceName_16_MyChar);
	strcpy(surfaceName_17_Bullet, profile.surfaceName_17_Bullet);
	strcpy(surfaceName_19_Caret, profile.surfaceName_19_Caret);
	strcpy(surfaceName_20_NpcSym, profile.surfaceName_20_NpcSym);
	strcpy(surfaceName_23_NpcRegu, profile.surfaceName_23_NpcRegu);
	strcpy(surfaceName_24_AutumnUI, profile.surfaceName_24_AutumnUI);
	strcpy(surfaceName_25_AutumnObjects, profile.surfaceName_25_AutumnObjects);
	strcpy(surfaceName_26_TextBox, profile.surfaceName_26_TextBox);
	strcpy(surfaceName_27_Face, profile.surfaceName_27_Face);
	strcpy(surfaceName_38_AutumnItems, profile.surfaceName_38_AutumnItems);
	strcpy(surfaceName_39_AutumnCharacters, profile.surfaceName_39_AutumnCharacters);

	// Head.tsc / ArmsItem.tsc
	strcpy(CustomArmsItemTSCLocation, profile.armsitem_tsc);
	strcpy(CustomHeadTSCLocation, profile.head_tsc);

	// PixTone path
	strcpy(global_pixtoneFolder, profile.pixtoneFolder);

	setDataProfile = profile;

	return;
}

// 0x41D407
void Replacement_LoadProfile_InitMyChar_Call()
{
	InitMyChar();
	LoadCustomGenericData(); // Load custom surfaces

	// Load PixTone data
	if (setting_enable_sound_effect_code)
	{
		if (strcmp(default_pixtoneFolder, global_pixtoneFolder) == 0)
			LoadUserCustomPixtoneData(NULL);
		else
			LoadUserCustomPixtoneData(global_pixtoneFolder);
	}
}

// This sets the data for everything in LoadProfile, AFTER it runs InitMyChar and other stuff (it makes more sense!!)
void SetProfileData()
{
	playerMoney = setDataProfile.playerMoney;
	memcpy(&gCollectables, &setDataProfile.pCollectables, sizeof(COLLECTABLES)); // write collectables
	// set enable collectables booleans to the save file ones
	enable_collectables_a = setDataProfile.enable_collect_a;
	enable_collectables_b = setDataProfile.enable_collect_b;
	enable_collectables_c = setDataProfile.enable_collect_c;
	enable_collectables_d = setDataProfile.enable_collect_d;
	enable_collectables_e = setDataProfile.enable_collect_e;
	// set collectables position values
	collectables_a_x_pos = setDataProfile.collectables_a_x_pos;
	collectables_a_y_pos = setDataProfile.collectables_a_y_pos;
	collectables_a_x_offset = setDataProfile.collectables_a_x_offset;
	collectables_b_x_pos = setDataProfile.collectables_b_x_pos;
	collectables_b_y_pos = setDataProfile.collectables_b_y_pos;
	collectables_b_x_offset = setDataProfile.collectables_b_x_offset;
	collectables_c_x_pos = setDataProfile.collectables_c_x_pos;
	collectables_c_y_pos = setDataProfile.collectables_c_y_pos;
	collectables_c_x_offset = setDataProfile.collectables_c_x_offset;
	collectables_d_x_pos = setDataProfile.collectables_d_x_pos;
	collectables_d_y_pos = setDataProfile.collectables_d_y_pos;
	collectables_d_x_offset = setDataProfile.collectables_d_x_offset;
	collectables_e_x_pos = setDataProfile.collectables_e_x_pos;
	collectables_e_y_pos = setDataProfile.collectables_e_y_pos;
	collectables_e_x_offset = setDataProfile.collectables_e_x_offset;
	// set <PHY values to the save file ones
	setting_physics_max_dash = setDataProfile.phy_physics_max_dash;
	setting_physics_max_move = setDataProfile.phy_physics_max_move;
	setting_physics_gravity1 = setDataProfile.phy_physics_gravity1;
	setting_physics_gravity2 = setDataProfile.phy_physics_gravity2;
	setting_physics_dash1 = setDataProfile.phy_physics_dash1;
	setting_physics_dash2 = setDataProfile.phy_physics_dash2;
	setting_physics_resist = setDataProfile.phy_physics_resist;
	setting_physics_jump = setDataProfile.phy_physics_jump;
	setting_physics_water_max_dash = setDataProfile.phy_physics_water_max_dash;
	setting_physics_water_max_move = setDataProfile.phy_physics_water_max_move;
	setting_physics_water_gravity1 = setDataProfile.phy_physics_water_gravity1;
	setting_physics_water_gravity2 = setDataProfile.phy_physics_water_gravity2;
	setting_physics_water_dash1 = setDataProfile.phy_physics_water_dash1;
	setting_physics_water_dash2 = setDataProfile.phy_physics_water_dash2;
	setting_physics_water_resist = setDataProfile.phy_physics_water_resist;
	setting_physics_water_jump = setDataProfile.phy_physics_water_jump;
	setting_walljump_horizontal_speed = setDataProfile.phy_walljump_horizontal_speed;
	setting_walljump_jump_height = setDataProfile.phy_walljump_jump_height;
	setting_walljump_sliding_speed = setDataProfile.phy_walljump_sliding_speed;
	setting_walljump_water_horizontal_speed = setDataProfile.phy_walljump_water_horizontal_speed;
	setting_walljump_water_jump_height = setDataProfile.phy_walljump_water_jump_height;
	setting_extrajump_jump_height = setDataProfile.phy_extrajump_jump_height;
	setting_extrajump_water_jump_height = setDataProfile.phy_extrajump_water_jump_height;
	setting_running_speed = setDataProfile.phy_running_speed;
	setting_bounce_speed = setDataProfile.phy_bounce_speed;
	// set <VAR values
	memcpy(&varData, setDataProfile.varData, sizeof(varData));
	// set <MIM value
	mim_num = setDataProfile.mim_num;
	// set booster fuel values
	booster_08_fuel = setDataProfile.booster_08_fuel;
	booster_20_fuel = setDataProfile.booster_20_fuel;
	Mod_WriteBoosterFuel();
	// Version 1.0.7
	setting_physics_boost20_accel_up = setDataProfile.phy_physics_boost20_accel_up;
	setting_physics_boost20_accel_left = setDataProfile.phy_physics_boost20_accel_left;
	setting_physics_boost20_accel_right = setDataProfile.phy_physics_boost20_accel_right;
	setting_physics_boost20_accel_down = setDataProfile.phy_physics_boost20_accel_down;
	setting_physics_boost20_accel_up_no_key = setDataProfile.phy_physics_boost20_accel_up_no_key;
	setting_physics_boost08_accel_add = setDataProfile.phy_physics_boost08_accel_add;
	setting_physics_boost08_accel_max_speed = setDataProfile.phy_physics_boost08_accel_max_speed;
	setting_physics_max_speed_left = setDataProfile.phy_physics_max_speed_left;
	setting_physics_max_speed_right = setDataProfile.phy_physics_max_speed_right;
	setting_physics_max_speed_up = setDataProfile.phy_physics_max_speed_up;
	setting_physics_max_speed_down = setDataProfile.phy_physics_max_speed_down;
	setting_physics_water_max_speed_left = setDataProfile.phy_physics_water_max_speed_left;
	setting_physics_water_max_speed_right = setDataProfile.phy_physics_water_max_speed_right;
	setting_physics_water_max_speed_up = setDataProfile.phy_physics_water_max_speed_up;
	setting_physics_water_max_speed_down = setDataProfile.phy_physics_water_max_speed_down;
	Set_Version107_Physics();
}

// 0x41D419
void Replacement_LoadProfile_TransferStage_Call(int w, int x, int y, int z)
{
	if (setting_enable_custom_npc_table)
	{
		if (!(npcTblPath[0] == 0))
			LoadCustomNpcTable(npcTblPath);
	}

	bSetRespawn = TRUE;
	TransferStage(w, x, y, z);
}

void AvaLoadProfileInit()
{
	SetProfileData(); // Set profile data
	Stage_SetRespawn(); // Set respawn point (this should actually work now?)

	ResetTSC_Image();
}

// 0x41D59A
void Replacement_InitializeGame_TransferStage_Call(int w, int x, int y, int z)
{
	bSetRespawn = TRUE;
	TransferStage(w, x, y, z);
}

void Replacement_InitializeGame_ClearArmsData_Call()
{
	ResetCustomGenericData(); // Reload Surfaces that were changed if they dont match default surface names
	Reset_CustomScriptNames(); // Reset custom script names on new game
	Reset_PixToneFolder(); // reset pixtone folder
	ResetTablePaths();
	ResetTSC_Image();
	ClearArmsData();
}