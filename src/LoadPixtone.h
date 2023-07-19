#pragma once

#include <Windows.h>

#include "cave_story.h"

#define MaxPixTonePath 256

extern char default_pixtoneFolder[MaxPixTonePath];
extern char global_pixtoneFolder[MaxPixTonePath];

void Reset_PixToneFolder();
void Replacement_LoadGenericData_Pixtone_Sprintf(char* b, const char* f, int pt);
void LoadUserCustomPixtoneData(const char* folder);