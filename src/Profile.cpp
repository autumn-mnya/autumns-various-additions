#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Profile.h"

#include "main.h"
#include "BKG.h"
#include "cave_story.h"
#include "Collectables.h"
#include "Draw.h"
#include "MyChar.h"
#include "TextScript.h"
#include "TileCollisionMyChar.h"

const char* const gAutumnProfileCode = "AutumnMnyaHazel";
CustomProfileData profile;
FILE* LoadProfileFp;
bool isLoadingSave = false;

// 0x41D213
void Replacement_SaveProfile_LastMemcpy_Call(void* dst, const void* src, size_t size)
{
	Freeware_memcpy(dst, src, size); // gFlagNPC

	// Custom Save below

	memset(&profile, 0, sizeof(profile)); // Reset this when doing the last memcpy
	memcpy(profile.code, gAutumnProfileCode, sizeof(profile.code));
	memcpy(profile.imgFolder, TSC_IMG_Folder, sizeof(profile.imgFolder));
	memcpy(profile.bkgTxT, bkgTxT_Global, sizeof(profile.bkgTxT));
	profile.playerMoney = playerMoney;
	memcpy(&profile.pCollectables, &gCollectables, sizeof(COLLECTABLES));
	// Collectables enabled on screen
	profile.enable_collect_a = enable_collectables_a;
	profile.enable_collect_b = enable_collectables_b;
	profile.enable_collect_c = enable_collectables_c;
	profile.enable_collect_d = enable_collectables_d;
	profile.enable_collect_e = enable_collectables_e;
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

	// Write new save code after this
}

// 0x41D22D
void Replacement_SaveProfile_fwrite_Call(void* buf, size_t eleS, size_t eleC, FILE* fp)
{
	Freeware_fwrite(buf, eleS, eleC, fp);

	// Write the whole struct
	Freeware_fwrite(&profile, sizeof(CustomProfileData), 1, fp);
}

// 0x41D353 
void Replacement_LoadProfile_fclose_Call(FILE* fp)
{
	// Set this up if needed
	LoadProfileFp = fp;
	isLoadingSave = true;

	Freeware_fread(profile.code, ProfileCodeSize, 1, fp);

	if (memcmp(profile.code, gAutumnProfileCode, ProfileCodeSize) == 0)
	{
		memset(&profile, 0, sizeof(CustomProfileData));
		Freeware_fread(&profile.imgFolder, ImgFolderSize, 1, fp);
		Freeware_fread(&profile.bkgTxT, bkgTxTSize, 1, fp);
		Freeware_fread(&profile.playerMoney, 4, 1, fp);
		Freeware_fread(&profile.pCollectables, sizeof(COLLECTABLES), 1, fp);
		// read savefile "collectables are enabled" booleans
		Freeware_fread(&profile.enable_collect_a, 4, 1, fp);
		Freeware_fread(&profile.enable_collect_b, 4, 1, fp);
		Freeware_fread(&profile.enable_collect_c, 4, 1, fp);
		Freeware_fread(&profile.enable_collect_d, 4, 1, fp);
		Freeware_fread(&profile.enable_collect_e, 4, 1, fp);
		// read savefile <PHY ints
		Freeware_fread(&profile.phy_physics_max_dash, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_max_move, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_gravity1, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_gravity2, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_dash1, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_dash2, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_resist, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_jump, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_water_max_dash, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_water_max_move, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_water_gravity1, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_water_gravity2, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_water_dash1, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_water_dash2, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_water_resist, 4, 1, fp);
		Freeware_fread(&profile.phy_physics_water_jump, 4, 1, fp);
		Freeware_fread(&profile.phy_walljump_horizontal_speed, 4, 1, fp);
		Freeware_fread(&profile.phy_walljump_jump_height, 4, 1, fp);
		Freeware_fread(&profile.phy_walljump_sliding_speed, 4, 1, fp);
		Freeware_fread(&profile.phy_walljump_water_horizontal_speed, 4, 1, fp);
		Freeware_fread(&profile.phy_walljump_water_jump_height, 4, 1, fp);
		Freeware_fread(&profile.phy_extrajump_jump_height, 4, 1, fp);
		Freeware_fread(&profile.phy_extrajump_water_jump_height, 4, 1, fp);
		Freeware_fread(&profile.phy_running_speed, 4, 1, fp);
		Freeware_fread(&profile.phy_bounce_speed, 4, 1, fp);
	}

	// Close the file
	Freeware_fclose(fp);

	// We set some of the profile data here because I honestly don't want to touch any of the code later lol ,
	strcpy(TSC_IMG_Folder, profile.imgFolder);
	strcpy(bkgTxT_Global, profile.bkgTxT);
}

// This sets the data for everything in LoadProfile, AFTER it runs InitMyChar and other stuff (it makes more sense!!)
void SetProfileData()
{
	// Set Custom Data here
	playerMoney = profile.playerMoney;
	memcpy(&gCollectables, &profile.pCollectables, sizeof(COLLECTABLES)); // write collectables
	// set enable collectables booleans to the save file ones
	enable_collectables_a = profile.enable_collect_a;
	enable_collectables_b = profile.enable_collect_b;
	enable_collectables_c = profile.enable_collect_c;
	enable_collectables_d = profile.enable_collect_d;
	enable_collectables_e = profile.enable_collect_e;
	// set <PHY values to the save file ones
	setting_physics_max_dash = profile.phy_physics_max_dash;
	setting_physics_max_move = profile.phy_physics_max_move;
	setting_physics_gravity1 = profile.phy_physics_gravity1;
	setting_physics_gravity2 = profile.phy_physics_gravity2;
	setting_physics_dash1 = profile.phy_physics_dash1;
	setting_physics_dash2 = profile.phy_physics_dash2;
	setting_physics_resist = profile.phy_physics_resist;
	setting_physics_jump = profile.phy_physics_jump;
	setting_physics_water_max_dash = profile.phy_physics_water_max_dash;
	setting_physics_water_max_move = profile.phy_physics_water_max_move;
	setting_physics_water_gravity1 = profile.phy_physics_water_gravity1;
	setting_physics_water_gravity2 = profile.phy_physics_water_gravity2;
	setting_physics_water_dash1 = profile.phy_physics_water_dash1;
	setting_physics_water_dash2 = profile.phy_physics_water_dash2;
	setting_physics_water_resist = profile.phy_physics_water_resist;
	setting_physics_water_jump = profile.phy_physics_water_jump;
	setting_walljump_horizontal_speed = profile.phy_walljump_horizontal_speed;
	setting_walljump_jump_height = profile.phy_walljump_jump_height;
	setting_walljump_sliding_speed = profile.phy_walljump_sliding_speed;
	setting_walljump_water_horizontal_speed = profile.phy_walljump_water_horizontal_speed;
	setting_walljump_water_jump_height = profile.phy_walljump_water_jump_height;
	setting_extrajump_jump_height = profile.phy_extrajump_jump_height;
	setting_extrajump_water_jump_height = profile.phy_extrajump_water_jump_height;
	setting_running_speed = profile.phy_running_speed;
	setting_bounce_speed = profile.phy_bounce_speed;
}

void Replacement_LoadProfile_ClearFade_Call()
{
	SetProfileData(); // Set profile data

	// this needs to NOT reset if we're loading
	if (isLoadingSave == false)
		BKG_ResetBackgrounds();
	else
		isLoadingSave = false;

	ResetTSC_Image();
	ClearFade();
}

void Replacement_InitializeGame_ClearArmsData_Call()
{
	InitCollectablesEnabled();
	ClearArmsData();
	ResetTSC_Image();
	BKG_ResetBackgrounds();
	memset(bkgTxT_Global, 0, sizeof(bkgTxT_Global));
}