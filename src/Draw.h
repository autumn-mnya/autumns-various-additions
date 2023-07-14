#pragma once

#include <Windows.h>

#include "cave_story.h"
#include "TextScript.h"


extern int setting_show_img_on_top_flag;
extern char TSC_IMG_Name[ImgNameSize];

void ReplaceSurfaceID(const char* n, SurfaceID no);
void Replacement_StageImageSurfaceCall(int x, int y, int s, BOOL b);
void LoadTSC_Image(char* name);
void Replacement_ModeOpening_SetFadeMask_Call();
void Replacement_ModeTitle_InitStar_Call();
void Replacement_LoadProfile_ClearFade_Call();
void Replacement_InitializeGame_ClearArmsData_Call();
void Replacement_ModeOpening_PutTextScript_Call();
void Replacement_ModeAction_PutTextScript_Call();
void Replacement_Debug_PutMyLife(BOOL flash); // for testing stuff lol