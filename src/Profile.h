#pragma once

#include <Windows.h>

#include "BKG.h"
#include "cave_story.h"
#include "Collectables.h"
#include "TextScript.h"

#define ProfileCodeSize 16
extern bool isLoadingSave;

// struct padding exists ! make sure these all get evenly divided by four..
typedef struct CustomProfileData
{
	char code[ProfileCodeSize];
	char imgFolder[ImgFolderSize];
	char bkgTxT[bkgTxTSize];
	int playerMoney;
	COLLECTABLES pCollectables;
	int enable_collect_a;
	int enable_collect_b;
	int enable_collect_c;
	int enable_collect_d;
	int enable_collect_e;
} CustomProfileData;

extern CustomProfileData profile;

void Replacement_SaveProfile_LastMemcpy_Call(void* dst, const void* src, size_t size);
void Replacement_SaveProfile_fwrite_Call(void* buf, size_t eleS, size_t eleC, FILE* fp);
void Replacement_LoadProfile_fclose_Call(FILE* fp);
void Replacement_LoadProfile_ClearFade_Call();