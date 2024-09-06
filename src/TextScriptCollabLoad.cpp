#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "TextScriptCustomLoad.h"

#include "main.h"

#include "mod_loader.h"
#include "mod_loader_hooks.h"
#include "ModSettings.h"
#include "ASMLoader.h"
#include "cave_story.h"
#include "Collectables.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "Frame.h"
#include "Game.h"
#include "GenericLoad.h"
#include "MyChar.h"
#include "MycParam.h"
#include "Profile.h"
#include "Respawn.h"
#include "Stage.h"
#include "SurfaceDefines.h"
#include "TextScript.h"
#include "TextScriptVAR.h"

bool setting_enable_custom_armsitem_location = false;
bool setting_enable_custom_head_location = false;

const char* defaultArmsItemTSCName = "ArmsItem";
const char* defaultHeadTSCName = "Head";

char CustomArmsItemTSCLocation[CustomTscMaxPath] = "ArmsItem";
char CustomHeadTSCLocation[CustomTscMaxPath] = "Head";

// Replace this functional *if* "Load Custom Head.tsc location" is enabled
// Consider replacing the "sprintf" call instead, it'll be safer..

void Reset_CustomScriptNames()
{
	if (setting_enable_custom_armsitem_location == true)
	{
		if (!(strcmp(CustomArmsItemTSCLocation, defaultArmsItemTSCName) == 0)) // Only reload scripts if they arent the same as the defaults
		{
			memset(CustomArmsItemTSCLocation, 0, sizeof(CustomArmsItemTSCLocation));
			strcpy(CustomArmsItemTSCLocation, defaultArmsItemTSCName);
		}
	}

	if (setting_enable_custom_head_location == true)
	{
		if (!(strcmp(CustomHeadTSCLocation, defaultHeadTSCName) == 0)) // Only reload scripts if they arent the same as the defaults
		{
			memset(CustomHeadTSCLocation, 0, sizeof(CustomHeadTSCLocation));
			strcpy(CustomHeadTSCLocation, defaultHeadTSCName);
		}
	}
}

// 0x421777
void Replacement_LoadTextScript_Stage_sprintf_Call(char path[MAX_PATH])
{
	Freeware_sprintf(path, "%s\\%s%s", gDataPath, CustomHeadTSCLocation, ".tsc"); // Split the name + file extension
}

// 0x401D51
void Replacement_CampLoop_LoadTextScript2(const char* name)
{
	// We need to replace this for custom ArmsItem.tsc files!
	char path[MAX_PATH];
	sprintf(path, "%s%s", CustomArmsItemTSCLocation, ".tsc"); // Split the name + file extension
	LoadTextScript2(path);
}

void InitMod_ScriptReplacements()
{
	if (setting_enable_custom_head_location == true)
		ModLoader_WriteCall((void*)0x421777, (void*)Replacement_LoadTextScript_Stage_sprintf_Call); // Head.tsc
	if (setting_enable_custom_armsitem_location == true)
		ModLoader_WriteCall((void*)0x401D51, (void*)Replacement_CampLoop_LoadTextScript2); // ArmsItem.tsc
}