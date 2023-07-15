#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "TextScript.h"

#include "main.h"

#include "mod_loader.h"
#include "mod_loader_hooks.h"
#include "BKG.h"
#include "cave_story.h"
#include "Collectables.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "Frame.h"
#include "MyChar.h"
#include "MycParam.h"

bool setting_enable_money_code = false;
bool setting_money_disable_enemy_money_drops = false;
bool setting_money_disable_exp_drops = false;

int setting_money_hud_x = 8;
int setting_money_hud_y = 48;
int setting_money_hud_x_number_offset = 16;

char CustomWindowName[MAX_PATH] = "";
char CustomScriptName[0x20] = "";
char TSC_IMG_Folder[ImgFolderSize];

void SetCustomWindowTitle(char* window_name)
{
	sprintf(window_name, "%s", window_name);
	SetWindowTextA(ghWnd, window_name);
}

// JakeV wrote this in "TSC Extended"
void GetTextScriptString(char returnData[])
{
	int i = 0;
	while (gTS->data[gTS->p_read] != '$') {
		returnData[i] = gTS->data[gTS->p_read];
		gTS->p_read++;
		i++;
	}
	//Insert the null terminator overtop the $
	returnData[i] = '\0';
	//Skip over the '$'
	gTS->p_read++;
}

static int CustomTextScriptCommands(MLHookCPURegisters* regs, void* ud)
{
	(void)ud;
	int w, x, y, z;

	char* where = TextScriptBuffer + gTS->p_read;
	if (where[0] != '<')
		return 0;
	if (strncmp(where + 1, "TRM", 3) == 0) //TRansport Momentum
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		w = GetTextScriptNo(gTS->p_read + 9);
		x = GetTextScriptNo(gTS->p_read + 14);
		y = GetTextScriptNo(gTS->p_read + 19);

		int xm = gMC->xm;
		int ym = gMC->ym;

		if (!TransferStage(z, w, x, y))
		{
			return enum_ESCRETURN_exit;
		}

		// Restore player velocity
		gMC->xm = xm;
		gMC->ym = ym;
	}
	if (strncmp(where + 1, "TRX", 3) == 0) //TRansport keep X:Y (buggy)
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		w = GetTextScriptNo(gTS->p_read + 9);

		int trx_x = gMC->x / 0x200 / 0x10;
		int trx_y = gMC->y / 0x200 / 0x10;

		if (!TransferStage(z, w, trx_x, trx_y))
		{
			return enum_ESCRETURN_exit;
		}
	}
	else if (strncmp(where + 1, "FN2", 3) == 0) // Focus on Npc 2 (FON but with index_x and index_y in use?)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		SetFrameTargetNpCharWithMyCharIndex(x, y);
		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "FM2", 3) == 0) // Focus on Me 2 ~ (Wait, Mode, OffsetX, OffsetY)
	{
		w = GetTextScriptNo(gTS->p_read + 4);
		x = GetTextScriptNo(gTS->p_read + 9);
		y = GetTextScriptNo(gTS->p_read + 14);
		z = GetTextScriptNo(gTS->p_read + 19);
		SetFrameTargetMyCharOffset(w, x, y, z);
		gTS->p_read += 23;
	}
	else if (strncmp(where + 1, "LDR", 3) == 0) // Lock Direction (only when using <FM2)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		switch (x)
		{
			default:
				is_direction_locked = false;
				break;

			case 1:
				is_direction_locked = true;
				lock_direction = y;
				break;
		}

		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "FOC", 3) == 0) // Focus on Coordinate
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		z = GetTextScriptNo(gTS->p_read + 14);
		SetFrameTargetCoordinate(x, y * 0x200 * 0x10, z * 0x200 * 0x10);
		gTS->p_read += 18;
	}
	else if (strncmp(where + 1, "RNJ", 3) == 0) // Random Number Jump
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		z = GetTextScriptNo(gTS->p_read + 9);
		JumpTextScript(Random(x, z));
	}
	else if (strncmp(where + 1, "SWN", 3) == 0) // Set Window Name
	{
		gTS->p_read += 4;
		memset(CustomWindowName, 0, sizeof(CustomWindowName));
		GetTextScriptString(CustomWindowName);
		SetCustomWindowTitle(CustomWindowName);
	}
	else if (strncmp(where + 1, "ML-", 3) == 0) // Max Life -
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		RemoveMaxLifeMyChar(z);
		gTS->p_read += 8;
	}
	else if (strncmp(where + 1, "LSC", 3) == 0) // Load tsc SCript
	{
		char path[MAX_PATH];
		char path_dir[20];

		gTS->p_read += 4;
		memset(CustomScriptName, 0, sizeof(CustomScriptName));
		GetTextScriptString(CustomScriptName);

		// Get path
		strcpy(path_dir, "Stage");

		sprintf(path, "%s\\%s.tsc", path_dir, CustomScriptName);

		LoadTextScript_Stage(path);
		JumpTextScript(0);
	}
	else if (strncmp(where + 1, "WCL", 3) == 0) // Write CaLl (Jank)
	{
		char address[8];
		char address2[8];

		gTS->p_read += 4;

		memset(address, 0, sizeof(address));
		memset(address2, 0, sizeof(address2));

		GetTextScriptString(address);
		GetTextScriptString(address2);

		ModLoader_WriteCall((void*)strtol(address, NULL, 0x10), (void*)strtol(address2, NULL, 0x10));
	}
	else if (strncmp(where + 1, "FNJ", 3) == 0) // Flag Not Jump
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		z = GetTextScriptNo(gTS->p_read + 9);

		if (!(GetNPCFlag(x)))
			JumpTextScript(z);
		else
			gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "SNJ", 3) == 0) // Skipflag Not Jump
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		z = GetTextScriptNo(gTS->p_read + 9);

		if (!(GetSkipFlag(x)))
			JumpTextScript(z);
		else
			gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "IMF", 3) == 0) // IMage Folder
	{
		char path[ImgFolderSize];

		gTS->p_read += 4;

		memset(path, 0, sizeof(path));

		GetTextScriptString(path);
		
		if (path[0] == 0)
			memset(TSC_IMG_Folder, 0, sizeof(TSC_IMG_Folder));
		else
		{
			memcpy(TSC_IMG_Folder, path, sizeof(TSC_IMG_Folder));
			strcat(TSC_IMG_Folder, "\\");
		}
	}
	else if (strncmp(where + 1, "IMG", 3) == 0) // IMaGe display
	{
		char name[ImgNameSize];

		gTS->p_read += 4;

		memset(name, 0, sizeof(name));

		GetTextScriptString(name);
		LoadTSC_Image(name);
	}
	else if (strncmp(where + 1, "BKG", 3) == 0) // BacKGround
	{
		char bkPath[bkgTxTSize];
		gTS->p_read += 4;

		memset(bkPath, 0, sizeof(bkPath));

		GetTextScriptString(bkPath);
		BKG_LoadBackground(bkPath);
	}
	else if (strncmp(where + 1, "BKP", 3) == 0) // BacKground Parameter
	{
		w = GetTextScriptNo(gTS->p_read + 4);
		x = GetTextScriptNo(gTS->p_read + 9);
		y = GetTextScriptNo(gTS->p_read + 14);

		BKG_SetParameter(w, x, y); // Set parameter X for layer W to value Y
		gTS->p_read += 18;
	}
	else if (strncmp(where + 1, "BKR", 3) == 0) // BacKground Reset
	{
		BKG_ResetBackgrounds();
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "BKE", 3) == 0) // BacKground Enable
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		bkList[z].isActive = true;
		gTS->p_read += 8;
	}
	else if (strncmp(where + 1, "BKD", 3) == 0) // BacKground Disable
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		bkList[z].isActive = false;
		gTS->p_read += 8;
	}
	else if (strncmp(where + 1, "BUY", 3) == 0) // BUY something (with money enabled)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		if (playerMoney < x) // if player money is less than X, jump event
			JumpTextScript(y);
		else
		{
			playerMoney -= x;
			gTS->p_read += 13;
		}
	}
	else if (strncmp(where + 1, "BY2", 3) == 0) // BuY something 2 (with money enabled)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		if (playerMoney >= x) // if player money is greater than or equal to X, jump event
		{
			playerMoney -= x;
			JumpTextScript(y);
		}
		else
			gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "SEL", 3) == 0) // SELl (Add money when money is enabled)
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		playerMoney += z;
		gTS->p_read += 8;
	}
	else if (strncmp(where + 1, "SL=", 3) == 0) // SeLl = (Set money to Z when money is enabled)
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		playerMoney = z;
		gTS->p_read += 8;
	}
	else if (strncmp(where + 1, "CSF", 3) == 0) // Change Surface Folder
	{
		char surfaceId[0x30];
		char surfacePath[0x30];
		gTS->p_read += 4;

		memset(surfaceId, 0, sizeof(surfaceId));
		memset(surfacePath, 0, sizeof(surfacePath));

		GetTextScriptString(surfaceId);
		GetTextScriptString(surfacePath);

		SwapSurfaces(surfacePath, atoi(surfaceId));
	}
	else if (strncmp(where + 1, "CTS", 3) == 0)
	{
		// Get path
		char path[MAX_PATH];
		char path_dir[20];
		char parts[32];
		gTS->p_read += 4;

		memset(path, 0, sizeof(path));
		memset(path_dir, 0, sizeof(path_dir));
		memset(parts, 0, sizeof(parts));

		GetTextScriptString(parts); // the tileset we want to load

		strcpy(path_dir, "Stage");

		// Load tileset
		sprintf(path, "%s\\Prt%s", path_dir, parts);
		ReleaseSurface(SURFACE_ID_LEVEL_TILESET); // release
		MakeSurface_File(path, SURFACE_ID_LEVEL_TILESET); // make surface!

		sprintf(path, "%s\\%s.pxa", path_dir, parts);
		LoadAttributeData(path);
	}
	else if (strncmp(where + 1, "CO+", 3) == 0) // Collectable Jump
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		AddCollectables(x, y);

		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "CO-", 3) == 0) // Collectable Jump
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		RemoveCollectables(x, y);

		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "COJ", 3) == 0) // Collectable Jump
	{
		int collectable;

		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		z = GetTextScriptNo(gTS->p_read + 14);

		switch (x)
		{
			default:
			case 0:
				collectable = gCollectables.numA;
				break;

			case 1:
				collectable = gCollectables.numB;
				break;

			case 2:
				collectable = gCollectables.numC;
				break;

			case 3:
				collectable = gCollectables.numD;
				break;

			case 4:
				collectable = gCollectables.numE;
				break;
		}

		if (collectable >= y)
			JumpTextScript(z);
		else
			gTS->p_read += 18;
	}
	else if (strncmp(where + 1, "COE", 3) == 0) // COllectable Enable
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		EnableCollectable(z);
		gTS->p_read += 8;
	}
	else if (strncmp(where + 1, "COD", 3) == 0) // COllectable Disable
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		DisableCollectable(z);
		gTS->p_read += 8;
	}
	else
		return 0;
	
	regs->eip = CSJ_tsc_done;
	return 1;
}

void InitMod_TSC()
{
	ModLoader_AddStackableHook(CSH_tsc_start, CustomTextScriptCommands, (void*)0);
}