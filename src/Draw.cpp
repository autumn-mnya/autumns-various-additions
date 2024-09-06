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
#include "ModSettings.h"
#include "cave_story.h"
#include "Collectables.h"
#include "Entity.h"
#include "Game.h"
#include "GenericLoad.h"
#include "MyChar.h"
#include "Profile.h"
#include "SurfaceDefines.h"
#include "TextScript.h"

RECT rcTSCImage = { 0, 0, 1000, 1000 };

int setting_show_img_on_top_flag = 6503;

char TSC_IMG_Name[ImgNameSize];

const char* const defaultFolderPath = "";

void SwapSurfaces(const char* n, int no)
{
	ReleaseSurface(no);
	MakeSurface_File(n, no);
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

	strcpy(surfaceName_5_Image, imgPath);
}

void ResetTSC_Image()
{
	memset(TSC_IMG_Folder, 0, sizeof(TSC_IMG_Folder));
	LoadTSC_Image("0");
}

static int& mag = *reinterpret_cast<int*>(0x48F914); // Window magnification
static LPDIRECTDRAWSURFACE& backbuffer = *reinterpret_cast<LPDIRECTDRAWSURFACE*>(0x49D384);
static auto& surf = *reinterpret_cast<LPDIRECTDRAWSURFACE(*)[40]>(0x49D388);