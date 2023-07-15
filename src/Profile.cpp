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
#include "File.h"
#include "TextScript.h"

const char* const gAutumnProfileCode = "AutumnHazel";
CustomProfileData profile;
FILE* LoadProfileFp;

// 0x41D213
void Replacement_SaveProfile_LastMemcpy_Call(void* dst, const void* src, size_t size)
{
	Freeware_memcpy(dst, src, size); // gFlagNPC

	// Custom Save below

	memset(&profile, 0, sizeof(profile)); // Reset this when doing the last memcpy
	memcpy(profile.code, gAutumnProfileCode, sizeof(profile.code));
	memcpy(profile.imgFolder, TSC_IMG_Folder, sizeof(profile.imgFolder));
	memcpy(profile.saveBkName, backgroundName, sizeof(profile.saveBkName));
	profile.saveBkType = backgroundType;
	profile.saveBkCount = numBks;
	memcpy(profile.saveBkList, bkList, sizeof(profile.saveBkList));

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
	int i = 0;

	Freeware_fread(profile.code, 11, 1, fp);

	if (memcmp(profile.code, gAutumnProfileCode, 11) == 0)
	{
		memset(&profile, 0, sizeof(CustomProfileData));
		Freeware_fread(&profile.imgFolder, ImgFolderSize, 1, fp);
		Freeware_fread(&profile.saveBkName, MAX_PATH, 1, fp);
		profile.saveBkType = Freeware_ReadBE32(fp);
		profile.saveBkCount = Freeware_ReadBE32(fp);
		for (i = 0; i < BKGCount; ++i)
		{
			profile.saveBkList[i].isActive = Freeware_ReadBE32(fp);
			profile.saveBkList[i].bmX = Freeware_ReadBE32(fp);
			profile.saveBkList[i].bmY = Freeware_ReadBE32(fp);
			profile.saveBkList[i].bmW = Freeware_ReadBE32(fp);
			profile.saveBkList[i].bmH = Freeware_ReadBE32(fp);
			profile.saveBkList[i].repX = Freeware_ReadBE32(fp);
			profile.saveBkList[i].repY = Freeware_ReadBE32(fp);
			profile.saveBkList[i].xDist = Freeware_ReadBE32(fp);
			profile.saveBkList[i].yDist = Freeware_ReadBE32(fp);
			profile.saveBkList[i].type = Freeware_ReadBE32(fp);
			profile.saveBkList[i].xm = Freeware_ReadBE64(fp);
			profile.saveBkList[i].ym = Freeware_ReadBE64(fp);
			profile.saveBkList[i].xCount = Freeware_ReadBE32(fp);
			profile.saveBkList[i].yCount = Freeware_ReadBE32(fp);
			profile.saveBkList[i].spriteNum = Freeware_ReadBE32(fp);
			profile.saveBkList[i].animFrame = Freeware_ReadBE32(fp);
			profile.saveBkList[i].animSpeed = Freeware_ReadBE32(fp);
			profile.saveBkList[i].counter = Freeware_ReadBE32(fp);
			profile.saveBkList[i].x = Freeware_ReadBE64(fp);
			profile.saveBkList[i].y = Freeware_ReadBE64(fp);
			profile.saveBkList[i].bkgXval = Freeware_ReadBE64(fp);
			profile.saveBkList[i].bkgYval = Freeware_ReadBE64(fp);
		}

		Freeware_memcpy(bkList, profile.saveBkList, sizeof(bkList)); // set bklist
	}

	// Close the file
	Freeware_fclose(fp);

	// Set Custom Data here
	strcpy(TSC_IMG_Folder, profile.imgFolder);

	memset(backgroundName, 0, sizeof(backgroundName));
	strcpy(backgroundName, profile.saveBkName);
	backgroundType = profile.saveBkType;
	numBks = profile.saveBkCount;
}