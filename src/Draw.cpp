#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Draw.h"

#include "main.h"

#include "mod_loader.h"
#include "BKG.h"
#include "cave_story.h"
#include "Collectables.h"
#include "Entity.h"
#include "MyChar.h"
#include "Profile.h"
#include "TextScript.h"

// Doesn't support widescreen
RECT rcTSCImage = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

int setting_show_img_on_top_flag = 6503;

char TSC_IMG_Name[ImgNameSize];

const char* const defaultFolderPath = "";

// Replace MakeSurfaceFile with another call
void ReplaceSurfaceID(const char* n, SurfaceID no)
{
	MakeSurface_File(n, no);
}

void SwapSurfaces(const char* n, int no)
{
	ReleaseSurface(no);
	MakeSurface_File(n, no);
}

void Replacement_StageImageSurfaceCall(const char* n, SurfaceID s)
{
	MakeSurface_File("Image\\0", SURFACE_ID_TSC_IMG);
	MakeSurface_File("AutumnUI", SURFACE_ID_AUTUMN_HUD);
	MakeSurface_File("Npc\\NpcAutumnObj", SURFACE_ID_AUTUMN_OBJECTS);
	MakeSurface_File("Autumn", SURFACE_ID_AUTUMN_ITEMS);
	MakeSurface_File("Npc\\NpcAutumnChar", SURFACE_ID_AUTUMN_CHARACTERS);
	MakeSurface_File(n, s);
}

void LoadTSC_Image(char* name)
{
	char imgPath[MAX_PATH];

	// Reset name before we copy it
	memset(TSC_IMG_Name, 0, sizeof(TSC_IMG_Name));
	strcpy(TSC_IMG_Name, name);

	// Load TSC image
	memset(imgPath, 0, sizeof(imgPath));
	strcpy(imgPath, "Image\\");
	
	// Only use a subfolder is the path isnt null
	if (strlen(TSC_IMG_Folder) != NULL)
	{
		strcat(imgPath, TSC_IMG_Folder);
		strcat(imgPath, "\\");
	}

	strcat(imgPath, name);

	ReleaseSurface(SURFACE_ID_TSC_IMG);
	MakeSurface_File(imgPath, SURFACE_ID_TSC_IMG);
}

void ResetTSC_Image()
{
	memset(TSC_IMG_Folder, 0, sizeof(TSC_IMG_Folder));
	LoadTSC_Image("0");
}

// <IMG resetting calls
void Replacement_ModeOpening_SetFadeMask_Call()
{
	InitCollectablesEnabled();
	SetFadeMask();
	ResetTSC_Image();
	BKG_ResetBackgrounds();
}

void Replacement_ModeTitle_InitStar_Call()
{
	InitCollectablesEnabled();
	InitStar();
	ResetTSC_Image();
}

// Put <IMG on screen

void Replacement_ModeOpening_PutTextScript_Call()
{
	// Show <IMG behind textbox when flag isnt set
	if (!(GetNPCFlag(setting_show_img_on_top_flag)))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);

	PutTextScript();

	// Show <IMG infront of textbox when flag is set
	if (GetNPCFlag(setting_show_img_on_top_flag))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);
}

void Replacement_ModeAction_PutTextScript_Call()
{
	// Show <IMG behind textbox when flag isnt set
	if (!(GetNPCFlag(setting_show_img_on_top_flag)))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);

	PutTextScript();

	// Show <IMG infront of textbox when flag is set
	if (GetNPCFlag(setting_show_img_on_top_flag))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);
}

void Replacement_Debug_PutMyLife(BOOL flash)
{
	RECT rcCase = { 0, 40, 232, 48 };
	RECT rcLife = { 0, 24, 232, 32 };
	RECT rcBr = { 0, 32, 232, 40 };

	if (flash == TRUE && gMC->shock / 2 % 2)
		return;

	if (gMC->lifeBr < gMC->life)
		gMC->lifeBr = gMC->life;

	if (gMC->lifeBr > gMC->life)
	{
		if (++gMC->lifeBr_count > 30)
			--gMC->lifeBr;
	}
	else
	{
		gMC->lifeBr_count = 0;
	}

	// Draw bar
	rcCase.right = 64;
	rcLife.right = ((gMC->life * 40) / gMC->max_life) - 1;
	rcBr.right = ((gMC->lifeBr * 40) / gMC->max_life) - 1;

	PutBitmap3(&grcGame, 16, 40, &rcCase, SURFACE_ID_TEXT_BOX);
	PutBitmap3(&grcGame, 40, 40, &rcBr, SURFACE_ID_TEXT_BOX);
	PutBitmap3(&grcGame, 40, 40, &rcLife, SURFACE_ID_TEXT_BOX);
	PutNumber4(8, 40, gMC->lifeBr, FALSE);
}