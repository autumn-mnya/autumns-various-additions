#pragma once

#include <Windows.h>

#include "BKG.h"
#include "cave_story.h"
#include "TextScript.h"

typedef struct CustomProfileData
{
	char code[11];
	char imgFolder[ImgFolderSize];
	char saveBkName[MAX_PATH];
	int saveBkType;
	SUBKG saveBkList[0x400];
} CustomProfileData;

extern CustomProfileData profile;

void Replacement_SaveProfile_LastMemcpy_Call(void* dst, const void* src, size_t size);
void Replacement_SaveProfile_fwrite_Call(void* buf, size_t eleS, size_t eleC, FILE* fp);
void Replacement_LoadProfile_fclose_Call(FILE* fp);
void Replacement_LoadProfile_ClearFade_Call();