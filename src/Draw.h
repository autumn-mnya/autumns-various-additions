#pragma once

#include <Windows.h>

#include "cave_story.h"
#include "TextScript.h"

extern RECT rcTSCImage;

extern int setting_show_img_on_top_flag;

extern char TSC_IMG_Name[ImgNameSize];

void ReplaceSurfaceID(const char* n, SurfaceID no);
void SwapSurfaces(const char* n, int no);
void Replacement_StageImageSurfaceCall(const char* n, SurfaceID s);
void LoadTSC_Image(char* name);
void ResetTSC_Image();
void Replacement_ModeOpening_SetFadeMask_Call();
void Replacement_ModeTitle_InitStar_Call();
void Replacement_Debug_PutMyLife(BOOL flash); // for testing stuff lol