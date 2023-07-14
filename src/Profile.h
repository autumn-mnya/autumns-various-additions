#pragma once

#include <Windows.h>

#include "cave_story.h"
#include "TextScript.h"

typedef struct CustomProfileData
{
	char code[11];
	char imgFolder[ImgFolderSize];
} CustomProfileData;

void Replacement_SaveProfile_LastMemcpy_Call(void* dst, const void* src, size_t size);
void Replacement_SaveProfile_fwrite_Call(void* buf, size_t eleS, size_t eleC, FILE* fp);
void Replacement_LoadProfile_fclose_Call(FILE* fp);
void Replacement_LoadProfile_ClearFade_Call();