#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "EntityLoad.h"

#include "cave_story.h"
#include "main.h"
#include "Entity.h"
#include "EntityTable.h"

CNPCHAR gCustomNPC[NPC_MAX];

const char* const gPassPixEve = "PXE";
const char* const gPassCustomPixEve = "AUT";

const char* const cpxeFileName = "cpxe";

BOOL LoadCustomEvent(const char* path_event, int npc_count)
{
	char code[4];

	// Get path of .pxe
	char* cpathevent = new char[MAX_PATH]();

	// Get path of .cpxe
	int slength = sprintf(cpathevent, "%s\\%s", gDataPath, path_event);

	memcpy(cpathevent + slength - 3, cpxeFileName, 4);

	FILE* fp = fopen(cpathevent, "rb");

	delete[] cpathevent;

	// If the file doesn't exist, quit early
	if (fp == NULL)
		return TRUE;

	// Read Header
	fread(code, 1, 4, fp);
	if (memcmp(code, gPassCustomPixEve, 3) != 0)
	{
		fclose(fp);
		return FALSE;
	}

	// Reset struct to null
	memset(gCustomNPC, 0, sizeof(CNPCHAR) * NPC_MAX);

	// Read .cpxe data
	fread(gCustomNPC + 170, sizeof(CNPCHAR), npc_count, fp);

	// Return to TransferStage inside the exe
	return TRUE;
}

void Replacement_ActNpChar(void)
{
	int i;
	int code_char;

	for (i = 0; i < NPC_MAX; ++i)
	{
		if (gNPC[i].cond & 0x80)
		{
			code_char = gNPC[i].code_char;

			if (code_char <= 360)
				gpNpcFuncTbl[code_char](&gNPC[i]);
			else
				gpEntityFuncTbl[code_char - 361](&gNPC[i]);

			if (gNPC[i].shock)
				--gNPC[i].shock;
		}
	}
}

void Replacement_ChangeNpCharByEvent(int code_event, int code_char, int dir)
{
	int n;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if ((gNPC[n].cond & 0x80) && gNPC[n].code_event == code_event)
		{
			gNPC[n].bits &= ~(NPC_SOLID_SOFT | NPC_IGNORE_TILE_44 | NPC_INVULNERABLE | NPC_IGNORE_SOLIDITY | NPC_BOUNCY | NPC_SHOOTABLE | NPC_SOLID_HARD | NPC_REAR_AND_TOP_DONT_HURT | NPC_SHOW_DAMAGE);	// Clear these flags
			gNPC[n].code_char = code_char;
			gNPC[n].bits |= (*gNpcTable)[gNPC[n].code_char].bits;
			gNPC[n].exp = (*gNpcTable)[gNPC[n].code_char].exp;
			SetUniqueParameter(&gNPC[n]);
			gNPC[n].cond |= 0x80;
			gNPC[n].act_no = 0;
			gNPC[n].act_wait = 0;
			gNPC[n].count1 = 0;
			gNPC[n].count2 = 0;
			gNPC[n].ani_no = 0;
			gNPC[n].ani_wait = 0;
			gNPC[n].xm = 0;
			gNPC[n].ym = 0;

			if (dir == 5)
			{
				// Another empty case that has to exist for the same assembly to be generated
			}
			else if (dir == 4)
			{
				if (gNPC[n].x < gMC->x)
					gNPC[n].direct = 2;
				else
					gNPC[n].direct = 0;
			}
			else
			{
				gNPC[n].direct = dir;
			}

			if (code_char <= 360)
				gpNpcFuncTbl[code_char](&gNPC[n]);
			else
				gpEntityFuncTbl[code_char - 361](&gNPC[n]);
		}
	}
}

void Replacement_ChangeCheckableNpCharByEvent(int code_event, int code_char, int dir)
{
	int n;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if (!(gNPC[n].cond & 0x80) && gNPC[n].code_event == code_event)
		{
			gNPC[n].bits &= ~(NPC_SOLID_SOFT | NPC_IGNORE_TILE_44 | NPC_INVULNERABLE | NPC_IGNORE_SOLIDITY | NPC_BOUNCY | NPC_SHOOTABLE | NPC_SOLID_HARD | NPC_REAR_AND_TOP_DONT_HURT | NPC_SHOW_DAMAGE);	// Clear these flags
			gNPC[n].bits |= NPC_INTERACTABLE;
			gNPC[n].code_char = code_char;
			gNPC[n].bits |= (*gNpcTable)[gNPC[n].code_char].bits;
			gNPC[n].exp = (*gNpcTable)[gNPC[n].code_char].exp;
			SetUniqueParameter(&gNPC[n]);
			gNPC[n].cond |= 0x80;
			gNPC[n].act_no = 0;
			gNPC[n].act_wait = 0;
			gNPC[n].count1 = 0;
			gNPC[n].count2 = 0;
			gNPC[n].ani_no = 0;
			gNPC[n].ani_wait = 0;
			gNPC[n].xm = 0;
			gNPC[n].ym = 0;

			if (dir == 5)
			{
				// Another empty case that has to exist for the same assembly to be generated
			}
			else if (dir == 4)
			{
				if (gNPC[n].x < gMC->x)
					gNPC[n].direct = 2;
				else
					gNPC[n].direct = 0;
			}
			else
			{
				gNPC[n].direct = (signed char)dir;
			}

			if (code_char <= 360)
				gpNpcFuncTbl[code_char](&gNPC[n]);
			else
				gpEntityFuncTbl[code_char - 361](&gNPC[n]);
		}
	}
}