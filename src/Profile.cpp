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
#include "TextScript.h"

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
	profile.enable_collect_a = enable_collectables_a;
	profile.enable_collect_b = enable_collectables_b;
	profile.enable_collect_c = enable_collectables_c;
	profile.enable_collect_d = enable_collectables_d;
	profile.enable_collect_e = enable_collectables_e;

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
		Freeware_fread(&profile.enable_collect_a, 4, 1, fp);
		Freeware_fread(&profile.enable_collect_b, 4, 1, fp);
		Freeware_fread(&profile.enable_collect_c, 4, 1, fp);
		Freeware_fread(&profile.enable_collect_d, 4, 1, fp);
		Freeware_fread(&profile.enable_collect_e, 4, 1, fp);
	}

	// Close the file
	Freeware_fclose(fp);

	// Set Custom Data here
	strcpy(TSC_IMG_Folder, profile.imgFolder);
	strcpy(bkgTxT_Global, profile.bkgTxT);
	playerMoney = profile.playerMoney;
	memcpy(&gCollectables, &profile.pCollectables, sizeof(COLLECTABLES)); // write collectables
	enable_collectables_a = profile.enable_collect_a; // the enabler in question
	enable_collectables_b = profile.enable_collect_b;
	enable_collectables_c = profile.enable_collect_c;
	enable_collectables_d = profile.enable_collect_d;
	enable_collectables_e = profile.enable_collect_e;
}