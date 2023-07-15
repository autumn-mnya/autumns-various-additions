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
	}

	// Close the file
	Freeware_fclose(fp);

	// Set Custom Data here
	strcpy(TSC_IMG_Folder, profile.imgFolder);
	strcpy(bkgTxT_Global, profile.bkgTxT);

	memset(backgroundName, 0, sizeof(backgroundName));
	/*
	strcpy(backgroundName, profile.saveBkName);
	backgroundType = profile.saveBkType;
	numBks = profile.saveBkCount;
	*/
}