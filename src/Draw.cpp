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
#include "Game.h"
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
	SetFadeMask();
	ResetTSC_Image();
	BKG_ResetBackgrounds();
}

void Replacement_ModeTitle_InitStar_Call()
{
	InitStar();
	ResetTSC_Image();
}