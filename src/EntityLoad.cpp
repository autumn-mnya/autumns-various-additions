#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "EntityLoad.h"

#include "ModSettings.h"
#include "cave_story.h"
#include "main.h"
#include "Collectables.h"
#include "Entity.h"
#include "EntityTable.h"
#include "TextScript.h"

// This file loads certain things relating to entities, but does not hold entity code itself.

bool setting_enable_collab_npc_table = false;

CNPCHAR gCustomNPC[NPC_MAX];

const char* const gPassCustomPixEve = "AUT";

const char* const cpxeFileName = "cpxe";

const char* const gDefaultNpcTableName = "npc.tbl";

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
				if (gNPC[n].x < gMC.x)
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
				if (gNPC[n].x < gMC.x)
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

void SetNormalExp(int x, int y, int exp)
{
	int n;
	int sub_exp;

	n = 0x100;
	while (exp)
	{
		while (n < NPC_MAX && gNPC[n].cond)
			++n;

		if (n == NPC_MAX)
			break;

		memset(&gNPC[n], 0, sizeof(NPCHAR));

		if (exp >= 20)
		{
			exp -= 20;
			sub_exp = 20;
		}
		else if (exp >= 5)
		{
			exp -= 5;
			sub_exp = 5;
		}
		else if (exp >= 1)
		{
			exp -= 1;
			sub_exp = 1;
		}

		gNPC[n].cond |= 0x80;
		gNPC[n].direct = 0;
		gNPC[n].code_char = 1;
		gNPC[n].x = x;
		gNPC[n].y = y;
		gNPC[n].bits = (*gNpcTable)[gNPC[n].code_char].bits;
		gNPC[n].exp = sub_exp;

		SetUniqueParameter(&gNPC[n]);
	}
}

void SetMoneyObjects(int x, int y, int exp)
{
	int n;
	int sub_exp;

	n = 0x100;
	while (exp)
	{
		while (n < NPC_MAX && gNPC[n].cond)
			++n;

		if (n == NPC_MAX)
			break;

		memset(&gNPC[n], 0, sizeof(NPCHAR));

		if (exp >= 20)
		{
			exp -= 20;
			sub_exp = 20;
		}
		else if (exp >= 5)
		{
			exp -= 5;
			sub_exp = 5;
		}
		else if (exp >= 1)
		{
			exp -= 1;
			sub_exp = 1;
		}

		gNPC[n].cond |= 0x80;
		gNPC[n].direct = 0;
		gNPC[n].code_char = 459;
		gNPC[n].x = x;
		gNPC[n].y = y;
		gNPC[n].bits = (*gNpcTable)[gNPC[n].code_char].bits;
		gNPC[n].exp = sub_exp;

		SetUniqueParameter(&gNPC[n]);
	}
}

void Replacement_SetExpObjects(int x, int y, int exp)
{
	// For now !!!! this will change
	if (setting_money_disable_exp_drops == false)
	{
		SetNormalExp(x, y, exp);
		if (setting_money_division)
			SetMoneyObjects(x, y, exp / 2);
		else
			SetMoneyObjects(x, y, exp);
	}
	else
	{
		if (setting_money_division)
			SetMoneyObjects(x, y, exp / 2);
		else
			SetMoneyObjects(x, y, exp);
	}
}

void Replacement_HitMyCharNpChar(void)
{
	int i;
	int hit = 0;

	if (!(gMC.cond & 0x80) || gMC.cond & 2)
		return;

	for (i = 0; i < NPC_MAX; ++i)
	{
		if (!(gNPC[i].cond & 0x80))
			continue;

		hit = 0;

		if (gNPC[i].bits & NPC_SOLID_SOFT)
		{
			hit = JudgeHitMyCharNPC(&gNPC[i]);
			gMC.flag |= hit;
		}
		else if (gNPC[i].bits & NPC_SOLID_HARD)
		{
			hit = JudgeHitMyCharNPC4(&gNPC[i]);
			gMC.flag |= hit;
		}
		else
		{
			hit = JudgeHitMyCharNPC3(&gNPC[i]);
		}

		// Special NPCs (pickups)
		if (hit != 0 && gNPC[i].code_char == 1)
		{
			PlaySoundObject(14, SOUND_MODE_PLAY);
			AddExpMyChar(gNPC[i].exp);
			if (setting_exp_acts_as_money)
			{
				if (setting_money_division)
				{
					if (gNPC[i].exp != 1)
						AddMoney(gNPC[i].exp / 2);
					else
						AddMoney(gNPC[i].exp);
				}
				else
					AddMoney(gNPC[i].exp);
			}
			gNPC[i].cond = 0;
		}

		if (hit != 0 && gNPC[i].code_char == 86)
		{
			PlaySoundObject(42, SOUND_MODE_PLAY);
			AddBulletMyChar(gNPC[i].code_event, gNPC[i].exp);
			gNPC[i].cond = 0;
		}

		if (hit != 0 && gNPC[i].code_char == 87)
		{
			PlaySoundObject(20, SOUND_MODE_PLAY);
			AddLifeMyChar(gNPC[i].exp);
			gNPC[i].cond = 0;
		}

		if (hit != 0 && gNPC[i].code_char == 459)
		{
			PlaySoundObject(14, SOUND_MODE_PLAY);
			AddMoney(gNPC[i].exp);
			gNPC[i].cond = 0;
		}

		// Run event on contact
		if (!(g_GameFlags & 4) && hit != 0 && gNPC[i].bits & NPC_EVENT_WHEN_TOUCHED)
			StartTextScript(gNPC[i].code_event);

		// NPC damage
		if (g_GameFlags & 2 && !(gNPC[i].bits & NPC_INTERACTABLE))
		{
			if (gNPC[i].bits & NPC_REAR_AND_TOP_DONT_HURT)
			{
				if (hit & 4 && gNPC[i].xm < 0)
					DamageMyChar(gNPC[i].damage);
				if (hit & 1 && gNPC[i].xm > 0)
					DamageMyChar(gNPC[i].damage);
				if (hit & 8 && gNPC[i].ym < 0)
					DamageMyChar(gNPC[i].damage);
				if (hit & 2 && gNPC[i].ym > 0)
					DamageMyChar(gNPC[i].damage);
			}
			else if (hit != 0 && gNPC[i].damage && !(g_GameFlags & 4))
			{
				DamageMyChar(gNPC[i].damage);
			}
		}

		// Interaction
		if (!(g_GameFlags & 4) && hit != 0 && gMC.cond & 1 && gNPC[i].bits & NPC_INTERACTABLE)
		{
			StartTextScript(gNPC[i].code_event);
			gMC.xm = 0;
			gMC.ques = FALSE;
		}
	}

	// Create question mark when NPC hasn't been interacted with
	if (gMC.ques)
		SetCaret(gMC.x, gMC.y, CARET_QUESTION_MARK, DIR_LEFT);
}

// Custom Functions

void BossExplosionAtNpc(int event)
{
	int n;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if ((gNPC[n].cond & 0x80) && gNPC[n].code_event == event)
		{
			SetFlash(gNPC[n].x, gNPC[n].y, FLASH_MODE_EXPLOSION);
		}
	}
}

// Collab

void LoadCustomNpcTable(const char* name)
{
	char path[MAX_PATH];

	// Try to load npc.tbl
	if (name != NULL)
	{
		if (setting_collab_enabled == true)
			sprintf(path, "%s\\%s\\%s%s", gDataPath, setting_collab_name, name, ".npc.tbl");
		else
			sprintf(path, "%s\\%s%s", gDataPath, name, ".npc.tbl");
	}
	else
		sprintf(path, "%s\\%s", gDataPath, gDefaultNpcTableName);

	ReleaseNpcTable();
	LoadNpcTable(path);
}