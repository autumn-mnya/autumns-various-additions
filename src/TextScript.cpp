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
#include "ModSettings.h"
#include "ASMLoader.h"
#include "BKG.h"
#include "cave_story.h"
#include "Collectables.h"
#include "CollabFlag.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "Frame.h"
#include "Game.h"
#include "GenericLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "MycParam.h"
#include "Profile.h"
#include "Respawn.h"
#include "Stage.h"
#include "SurfaceDefines.h"
#include "TextScriptCollabLoad.h"
#include "TextScriptVAR.h"

// Booleans
bool setting_enable_money_code = false;
bool setting_money_disable_enemy_money_drops = false;
bool setting_money_disable_exp_drops = false;
bool setting_enable_mim_mod = true;
bool setting_disable_tsc_encryption = false;

// Money HUD
int setting_money_hud_x = 8;
int setting_money_hud_y = 48;
int setting_money_hud_x_number_offset = 16;

// Booster Fuel
int booster_08_fuel = 50;
int booster_20_fuel = 50;

// Text for different things
char TSC_IMG_Folder[ImgFolderSize];

// Stage/Npc table paths
char stageTblPath[StageTblMaxPath];
char npcTblPath[NpcTblMaxPath];

// We replace the <TRA transfer stage call so that we can set a respawn point
// 0x422E09
void Replacement_TextScript_TransferStage_Call(int w, int x, int y, int z)
{
	bSetRespawn = TRUE;
	TransferStage(w, x, y, z);
}

void SetCustomWindowTitle(char* window_name)
{
	sprintf(window_name, "%s", window_name);
	SetWindowTextA(ghWnd, window_name);
}

// Get 4 digit number from TSC data
int Replacement_GetTextScriptNo(int a)
{
	int b = 0;
	char most_significant = gTS->data[a++];
	if (most_significant == 'V') // Variables
	{
		int var = 0;
		var += (gTS->data[a++] - '0') * 100;
		var += (gTS->data[a++] - '0') * 10;
		var += gTS->data[a] - '0';
		b = GetVariable(var);
	}
	else // Default behaviour
	{
		b += (most_significant - '0') * 1000;
		b += (gTS->data[a++] - '0') * 100;
		b += (gTS->data[a++] - '0') * 10;
		b += gTS->data[a] - '0';
	}

	return b;
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
	if (strncmp(where + 1, "TA2", 3) == 0) //TrAnsport 2 (No Respawn)
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		w = GetTextScriptNo(gTS->p_read + 9);
		x = GetTextScriptNo(gTS->p_read + 14);
		y = GetTextScriptNo(gTS->p_read + 19);

		bSetRespawn = FALSE;
		if (!TransferStage(z, w, x, y))
		{
			return enum_ESCRETURN_exit;
		}
	}
	else if (strncmp(where + 1, "TRM", 3) == 0) //TRansport Momentum
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		w = GetTextScriptNo(gTS->p_read + 9);
		x = GetTextScriptNo(gTS->p_read + 14);
		y = GetTextScriptNo(gTS->p_read + 19);

		int xm = gMC->xm;
		int ym = gMC->ym;

		bSetRespawn = TRUE;
		if (!TransferStage(z, w, x, y))
		{
			return enum_ESCRETURN_exit;
		}

		// Restore player velocity
		gMC->xm = xm;
		gMC->ym = ym;
	}
	else if (strncmp(where + 1, "TM2", 3) == 0) //Transport Momentum 2 (No Respawn)
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		w = GetTextScriptNo(gTS->p_read + 9);
		x = GetTextScriptNo(gTS->p_read + 14);
		y = GetTextScriptNo(gTS->p_read + 19);

		int xm = gMC->xm;
		int ym = gMC->ym;

		bSetRespawn = FALSE; // Do NOT set a respawn point
		if (!TransferStage(z, w, x, y))
		{
			return enum_ESCRETURN_exit;
		}

		// Restore player velocity
		gMC->xm = xm;
		gMC->ym = ym;
	}
	else if (strncmp(where + 1, "TRX", 3) == 0) //TRansport keep X:Y (buggy)
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		w = GetTextScriptNo(gTS->p_read + 9);

		int trx_x = gMC->x / 0x200 / 0x10;
		int trx_y = gMC->y / 0x200 / 0x10;

		bSetRespawn = TRUE;
		if (!TransferStage(z, w, trx_x, trx_y))
		{
			return enum_ESCRETURN_exit;
		}
	}
	else if (strncmp(where + 1, "TX2", 3) == 0) //Transport keep X:Y (buggy, No Respawn)
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		w = GetTextScriptNo(gTS->p_read + 9);

		int trx_x = gMC->x / 0x200 / 0x10;
		int trx_y = gMC->y / 0x200 / 0x10;

		bSetRespawn = FALSE; // Do NOT set a respawn point
		if (!TransferStage(z, w, trx_x, trx_y))
		{
			return enum_ESCRETURN_exit;
		}
	}
	else if (strncmp(where + 1, "MS4", 3) == 0) // MeSsage box 4
	{
		ClearTextLine();
		gTS->flags |= 0x01;
		gTS->flags &= ~0x32;
		if (gTS->flags & 0x40)
			gTS->flags |= 0x10;
		gTS->face = 0;
		gTS->p_read += 4;
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
		char CustomWindowName[MAX_PATH];
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
		char CustomScriptName[0x20] = "";
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
	else if (strncmp(where + 1, "CSF", 3) == 0) // Change Surface File (This command needs a rewrite!)
	{
		char surfaceId[0x30];
		gTS->p_read += 4;

		memset(surfaceId, 0, sizeof(surfaceId));
		
		GetTextScriptString(surfaceId);

		switch (atoi(surfaceId))
		{
			default:
				printf("Invalid Surface ID");
				break; // don't do anything if the ID was invalid
			case 0:
				memset(surfaceName_0_Title, 0, sizeof(surfaceName_0_Title));
				GetTextScriptString(surfaceName_0_Title);
				SwapSurfaces(surfaceName_0_Title, SURFACE_ID_TITLE);
				break;
			case 6:
				memset(surfaceName_6_Fade, 0, sizeof(surfaceName_6_Fade));
				GetTextScriptString(surfaceName_6_Fade);
				SwapSurfaces(surfaceName_6_Fade, SURFACE_ID_FADE);
				break;
			case 8:
				memset(surfaceName_8_ItemImage, 0, sizeof(surfaceName_8_ItemImage));
				GetTextScriptString(surfaceName_8_ItemImage);
				SwapSurfaces(surfaceName_8_ItemImage, SURFACE_ID_ITEM_IMAGE);
				break;
			case 11:
				memset(surfaceName_11_Arms, 0, sizeof(surfaceName_11_Arms));
				GetTextScriptString(surfaceName_11_Arms);
				SwapSurfaces(surfaceName_11_Arms, SURFACE_ID_ARMS);
				break;
			case 12:
				memset(surfaceName_12_ArmsImage, 0, sizeof(surfaceName_12_ArmsImage));
				GetTextScriptString(surfaceName_12_ArmsImage);
				SwapSurfaces(surfaceName_12_ArmsImage, SURFACE_ID_ARMS_IMAGE);
				break;
			case 14:
				memset(surfaceName_14_StageImage, 0, sizeof(surfaceName_14_StageImage));
				GetTextScriptString(surfaceName_14_StageImage);
				SwapSurfaces(surfaceName_14_StageImage, SURFACE_ID_STAGE_ITEM);
				break;
			case 16:
				memset(surfaceName_16_MyChar, 0, sizeof(surfaceName_16_MyChar));
				GetTextScriptString(surfaceName_16_MyChar);
				SwapSurfaces(surfaceName_16_MyChar, SURFACE_ID_MY_CHAR);
				break;
			case 17:
				memset(surfaceName_17_Bullet, 0, sizeof(surfaceName_17_Bullet));
				GetTextScriptString(surfaceName_17_Bullet);
				SwapSurfaces(surfaceName_17_Bullet, SURFACE_ID_BULLET);
				break;
			case 19:
				memset(surfaceName_19_Caret, 0, sizeof(surfaceName_19_Caret));
				GetTextScriptString(surfaceName_19_Caret);
				SwapSurfaces(surfaceName_19_Caret, SURFACE_ID_CARET);
				break;
			case 20:
				memset(surfaceName_20_NpcSym, 0, sizeof(surfaceName_20_NpcSym));
				GetTextScriptString(surfaceName_20_NpcSym);
				SwapSurfaces(surfaceName_20_NpcSym, SURFACE_ID_NPC_SYM);
				break;
			case 23:
				memset(surfaceName_23_NpcRegu, 0, sizeof(surfaceName_23_NpcRegu));
				GetTextScriptString(surfaceName_23_NpcRegu);
				SwapSurfaces(surfaceName_23_NpcRegu, SURFACE_ID_NPC_REGU);
				break;
			case 24:
				memset(surfaceName_24_AutumnUI, 0, sizeof(surfaceName_24_AutumnUI));
				GetTextScriptString(surfaceName_24_AutumnUI);
				SwapSurfaces(surfaceName_24_AutumnUI, SURFACE_ID_AUTUMN_HUD);
				break;
			case 25:
				memset(surfaceName_25_AutumnObjects, 0, sizeof(surfaceName_25_AutumnObjects));
				GetTextScriptString(surfaceName_25_AutumnObjects);
				SwapSurfaces(surfaceName_25_AutumnObjects, SURFACE_ID_AUTUMN_OBJECTS);
				break;
			case 26:
				memset(surfaceName_26_TextBox, 0, sizeof(surfaceName_26_TextBox));
				GetTextScriptString(surfaceName_26_TextBox);
				SwapSurfaces(surfaceName_26_TextBox, SURFACE_ID_TEXT_BOX);
				break;
			case 27:
				memset(surfaceName_27_Face, 0, sizeof(surfaceName_27_Face));
				GetTextScriptString(surfaceName_27_Face);
				SwapSurfaces(surfaceName_27_Face, SURFACE_ID_FACE);
				break;
			case 38:
				memset(surfaceName_38_AutumnItems, 0, sizeof(surfaceName_38_AutumnItems));
				GetTextScriptString(surfaceName_38_AutumnItems);
				SwapSurfaces(surfaceName_38_AutumnItems, SURFACE_ID_AUTUMN_ITEMS);
				break;
			case 39:
				memset(surfaceName_39_AutumnCharacters, 0, sizeof(surfaceName_39_AutumnCharacters));
				GetTextScriptString(surfaceName_39_AutumnCharacters);
				SwapSurfaces(surfaceName_39_AutumnCharacters, SURFACE_ID_AUTUMN_CHARACTERS);
				break;
		}
	}
	else if (strncmp(where + 1, "CBK", 3) == 0) // Change BacKground (Vanilla)
	{
		char back[32];
		char id[32];
		gTS->p_read += 4;

		memset(back, 0, sizeof(back));
		memset(id, 0, sizeof(id));

		GetTextScriptString(back); // the background image
		GetTextScriptString(id); // the background type

		InitBack(back, atoi(id));
	}
	else if (strncmp(where + 1, "CTS", 3) == 0) // Change TileSet
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
	else if (strncmp(where + 1, "CXP", 3) == 0) // Collectable X Position
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		SetCollectablePosition(x, y, false);

		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "CYP", 3) == 0) // Collectable Y Position
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		SetCollectablePosition(x, y, true);

		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "CXO", 3) == 0) // Collectable X Offset
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		SetCollectableXOffset(x, y);

		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "PHY", 3) == 0) // set PHYsics
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		SetPlayerPhysics(x, y);

		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "IPH", 3) == 0) // Init PHysics
	{
		InitMyCharPhysics();
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "VAR", 3) == 0) // VARiable (Set variable X to Y)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);

		SetVariable(x, y);

		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "VA+", 3) == 0) // VAriable + (Add number Y to variable X)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		SetVariable(x, GetVariable(x) + y);
		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "VA-", 3) == 0) // VAriable - (Subtract number Y to variable X)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		SetVariable(x, GetVariable(x) - y);
		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "VA*", 3) == 0) // VAriable * (Multiply number Y to variable X)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		SetVariable(x, GetVariable(x) * y);
		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "VA/", 3) == 0) // VAriable / (Divide number Y to variable X)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		SetVariable(x, GetVariable(x) / y);
		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "VAJ", 3) == 0) // VAriable Jump (Compares number W to Number X (not variables) using compare method Y. Jump to event Z if successful)
	{
		w = GetTextScriptNo(gTS->p_read + 4);
		x = GetTextScriptNo(gTS->p_read + 9);
		y = GetTextScriptNo(gTS->p_read + 14);
		z = GetTextScriptNo(gTS->p_read + 19);

		BOOL success = FALSE;

		switch (y)
		{
			default:
				success = FALSE;
				break;

			case 0:
				success = w == x;
				break;

			case 1:
				success = w != x;
				break;

			case 2:
				success = w > x;
				break;

			case 3:
				success = w < x;
				break;

			case 4:
				success = w >= x;
				break;

			case 5:
				success = w <= x;
				break;
		}

		if (success)
			JumpTextScript(z);
		else
			gTS->p_read += 23;
	}
	else if (strncmp(where + 1, "VAZ", 3) == 0) // VAriable Zero (Sets Y variables to 0, starting at X)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		for (int j = 0; j < y; ++j) {
			SetVariable(x + j, 0);
		}
		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "VND", 3) == 0) // Variable raNDom (Generates a random # between X and Y, and puts the result in variable Z)
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		z = GetTextScriptNo(gTS->p_read + 14);
		SetVariable(z, Random(x, y));
		gTS->p_read += 18;
	}
	else if (strncmp(where + 1, "VIJ", 3) == 0) // Variable Increment Jump (Increments variable W, if W >= X, X is set to 0 and jumps to event Y)
	{
		w = GetTextScriptNo(gTS->p_read + 4);
		x = GetTextScriptNo(gTS->p_read + 9);
		y = GetTextScriptNo(gTS->p_read + 14);
		SetVariable(w, GetVariable(w) + 1);
		if (GetVariable(w) >= x)
		{
			SetVariable(w, 0);
			JumpTextScript(y);
		}
		else
			gTS->p_read += 18;
	}
	else if (strncmp(where + 1, "PAT", 3) == 0) // PATch load
	{
		char patch[MAX_PATH];
		gTS->p_read += 4;

		memset(patch, 0, sizeof(patch));

		GetTextScriptString(patch); // the background image

		ApplyPatches(patch);
	}
	else if (strncmp(where + 1, "UPT", 3) == 0) // UnPaTch
	{
		UnpatchMemory();
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "RSP", 3) == 0) // ReSPawn
	{
		RespawnPlayer();
	}
	else if (strncmp(where + 1, "SRP", 3) == 0) // Set ResPawn
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		SetRespawnPoint(x, y);
		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "AMC", 3) == 0) // ArMs Clear
	{
		ClearArmsData();
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "ITC", 3) == 0) // ITem Clear
	{
		ClearItemData();
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "BLO", 3) == 0) // BLOw up
	{
		w = GetTextScriptNo(gTS->p_read + 4);
		BossExplosionAtNpc(w);
		gTS->p_read += 8;
	}
	else if (strncmp(where + 1, "PS-", 3) == 0) // Portal Slot -
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		SubPermitStage(x);
		gTS->p_read += 8;
	}
	else if (strncmp(where + 1, "PSC", 3) == 0) // Portal Slot Clear
	{
		ClearPermitStage();
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "BFS", 3) == 0) // Booster Fuel Set
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		z = GetTextScriptNo(gTS->p_read + 14);
		switch (x)
		{
			default:
			case 0:
				if (z == 0)
					booster_08_fuel = y;
				else if (z == 1)
					booster_08_fuel += y;
				else if (z == 2)
					booster_08_fuel -= y;
				break;

			case 1:
			case 2: // This makes more logical sense for some people I think?
				if (z == 0)
					booster_20_fuel = y;
				else if (z == 1)
					booster_20_fuel += y;
				else if (z == 2)
					booster_20_fuel -= y;
				break;

			case 8:
				if (z == 0)
					booster_08_fuel = y;
				else if (z == 1)
					booster_08_fuel += y;
				else if (z == 2)
					booster_08_fuel -= y;
				break;
		}
		Mod_WriteBoosterFuel();
		gTS->p_read += 18;
	}
	else if (strncmp(where + 1, "STT", 3) == 0) // STage Table
	{
		gTS->p_read += 4;

		memset(stageTblPath, 0, sizeof(stageTblPath));

		GetTextScriptString(stageTblPath);
		LoadStageTable(stageTblPath);
	}
	else if (strncmp(where + 1, "NPT", 3) == 0) // NPc Table
	{
		gTS->p_read += 4;

		memset(npcTblPath, 0, sizeof(npcTblPath));

		GetTextScriptString(npcTblPath);
		LoadCustomNpcTable(npcTblPath);
	}
	else if (strncmp(where + 1, "HSC", 3) == 0) // Head SCript
	{
		char headPath[CustomTscMaxPath];
		gTS->p_read += 4;
		memset(headPath, 0, sizeof(headPath));
		GetTextScriptString(headPath);
		strcpy(CustomHeadTSCLocation, headPath);
	}
	else if (strncmp(where + 1, "ASC", 3) == 0) // Armsitem SCript
	{
		char invPath[CustomTscMaxPath];
		gTS->p_read += 4;
		memset(invPath, 0, sizeof(invPath));
		GetTextScriptString(invPath);
		strcpy(CustomArmsItemTSCLocation, invPath);
	}
	else if (strncmp(where + 1, "PXT", 3) == 0) // PiXTone folder
	{
		char pixtoneFolder[MaxPixTonePath];
		gTS->p_read += 4;
		memset(pixtoneFolder, 0, sizeof(pixtoneFolder));
		GetTextScriptString(pixtoneFolder);
		strcpy(global_pixtoneFolder, pixtoneFolder);
		if (strcmp(pixtoneFolder, "0") == 0) // if it's just "0" we load the default folder instead.
			LoadUserCustomPixtoneData(NULL);
		else
			LoadUserCustomPixtoneData(global_pixtoneFolder);
	}
	else if (strncmp(where + 1, "RSF", 3) == 0) // Reset SurFaces
	{
		ResetCustomGenericData();
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "CFE", 3) == 0) // Collab Flag Enable
	{
		enable_collab_flags = 1;
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "CFD", 3) == 0) // Collab Flag Disable
	{
		enable_collab_flags = 0;
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "ICF", 3) == 0) // Init Collab Flag
	{
		InitCollabFlags();
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "ICD", 3) == 0) // Init Collab Data
	{
		InitMyCollabData();
		gTS->p_read += 4;
	}
	else if (strncmp(where + 1, "CLN", 3) == 0) // CoLlab Name
	{
		char collabName[CollabNameMaxPath];
		gTS->p_read += 4;
		memset(collabName, 0, sizeof(collabName));
		GetTextScriptString(collabName);
		strcpy(setting_collab_name, collabName);
	}
	else if (strncmp(where + 1, "ICN", 3) == 0) // Init Collab Name
	{
		InitCollabName();
		gTS->p_read += 4;
	}
	else
		return 0;
	
	regs->eip = CSJ_tsc_done;
	return 1;
}

// Different hook for disabling <MIM command (as it is the most commonly used hack tbh)
static int CustomTSC_MIM(MLHookCPURegisters* regs, void* ud)
{
	(void)ud;
	int w, x, y, z;

	char* where = TextScriptBuffer + gTS->p_read;
	if (where[0] != '<')
		return 0;
	if (strncmp(where + 1, "MIM", 3) == 0) // MIMiga mask
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		mim_num = x;
		gTS->p_read += 8;
	}
	else
		return 0;

	regs->eip = CSJ_tsc_done;
	return 1;
}

void ResetCollabPaths()
{
	memset(stageTblPath, 0, sizeof(stageTblPath));
	memset(npcTblPath, 0, sizeof(npcTblPath));
}

// If you want to disable this for some reason, we just set this to be an empty function..
void Replacement_EncryptionBinaryData2(unsigned char* pData, long size)
{

}

void Mod_WriteBoosterFuel()
{
	ModLoader_WriteLong((void*)0x4157A7, booster_08_fuel); // Booster 0.8 Fuel
	ModLoader_WriteLong((void*)0x4157BD, booster_20_fuel); // Booster 2.0 Fuel
}

void InitMod_TSC()
{
	// Collab Tables
	ModLoader_WriteCall((void*)0x41D407, (void*)Replacement_LoadProfile_InitMyChar_Call);
	// Script Replacements (Head.tsc / ArmsItem.tsc)
	InitMod_ScriptReplacements();
	// Collab Flags loading
	InitMod_CollabFlagLoading();
	// Respawning the player
	ModLoader_WriteCall((void*)0x41D419, (void*)Replacement_LoadProfile_TransferStage_Call);
	ModLoader_WriteCall((void*)0x41D59A, (void*)Replacement_InitializeGame_TransferStage_Call);
	ModLoader_WriteCall((void*)0x40F770, (void*)Replacement_ModeOpening_SetFrameTargetMyChar_Call);
	ModLoader_WriteCall((void*)0x422E09, (void*)Replacement_TextScript_TransferStage_Call);

	// <VAR
	ModLoader_WriteJump((void*)0x421900, (void*)Replacement_GetTextScriptNo);

	// TSC commands
	ModLoader_AddStackableHook(CSH_tsc_start, CustomTextScriptCommands, (void*)0);

	// <MIM command
	if (setting_enable_mim_mod)
		ModLoader_AddStackableHook(CSH_tsc_start, CustomTSC_MIM, (void*)0);

	if (setting_disable_tsc_encryption)
		ModLoader_WriteJump((void*)0x4215C0, (void*)Replacement_EncryptionBinaryData2);

	// Booster Fuel amounts (only when MyChar code is enabled)
	if (setting_enable_mychar)
		Mod_WriteBoosterFuel();
}