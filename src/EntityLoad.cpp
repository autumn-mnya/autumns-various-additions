#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "EntityLoad.h"

#include "cave_story.h"
#include "main.h"
#include "Entity.h"

const char* const gPassPixEve = "PXE";

// Npc function table
const NPCFUNCTION gpNpcFuncTbl[] = {
	ActNpc000,
	ActNpc001,
	ActNpc002,
	ActNpc003,
	ActNpc004,
	ActNpc005,
	ActNpc006,
	ActNpc007,
	ActNpc008,
	ActNpc009,
	ActNpc010,
	ActNpc011,
	ActNpc012,
	ActNpc013,
	ActNpc014,
	ActNpc015,
	ActNpc016,
	ActNpc017,
	ActNpc018,
	ActNpc019,
	ActNpc020,
	ActNpc021,
	ActNpc022,
	ActNpc023,
	ActNpc024,
	ActNpc025,
	ActNpc026,
	ActNpc027,
	ActNpc028,
	ActNpc029,
	ActNpc030,
	ActNpc031,
	ActNpc032,
	ActNpc033,
	ActNpc034,
	ActNpc035,
	ActNpc036,
	ActNpc037,
	ActNpc038,
	ActNpc039,
	ActNpc040,
	ActNpc041,
	ActNpc042,
	ActNpc043,
	ActNpc044,
	ActNpc045,
	ActNpc046,
	ActNpc047,
	ActNpc048,
	ActNpc049,
	ActNpc050,
	ActNpc051,
	ActNpc052,
	ActNpc053,
	ActNpc054,
	ActNpc055,
	ActNpc056,
	ActNpc057,
	ActNpc058,
	ActNpc059,
	ActNpc060,
	ActNpc061,
	ActNpc062,
	ActNpc063,
	ActNpc064,
	ActNpc065,
	ActNpc066,
	ActNpc067,
	ActNpc068,
	ActNpc069,
	ActNpc070,
	ActNpc071,
	ActNpc072,
	ActNpc073,
	ActNpc074,
	ActNpc075,
	ActNpc076,
	ActNpc077,
	ActNpc078,
	ActNpc079,
	ActNpc080,
	ActNpc081,
	ActNpc082,
	ActNpc083,
	ActNpc084,
	ActNpc085,
	ActNpc086,
	ActNpc087,
	ActNpc088,
	ActNpc089,
	ActNpc090,
	ActNpc091,
	ActNpc092,
	ActNpc093,
	ActNpc094,
	ActNpc095,
	ActNpc096,
	ActNpc097,
	ActNpc098,
	ActNpc099,
	ActNpc100,
	ActNpc101,
	ActNpc102,
	ActNpc103,
	ActNpc104,
	ActNpc105,
	ActNpc106,
	ActNpc107,
	ActNpc108,
	ActNpc109,
	ActNpc110,
	ActNpc111,
	ActNpc112,
	ActNpc113,
	ActNpc114,
	ActNpc115,
	ActNpc116,
	ActNpc117,
	ActNpc118,
	ActNpc119,
	ActNpc120,
	ActNpc121,
	ActNpc122,
	ActNpc123,
	ActNpc124,
	ActNpc125,
	ActNpc126,
	ActNpc127,
	ActNpc128,
	ActNpc129,
	ActNpc130,
	ActNpc131,
	ActNpc132,
	ActNpc133,
	ActNpc134,
	ActNpc135,
	ActNpc136,
	ActNpc137,
	ActNpc138,
	ActNpc139,
	ActNpc140,
	ActNpc141,
	ActNpc142,
	ActNpc143,
	ActNpc144,
	ActNpc145,
	ActNpc146,
	ActNpc147,
	ActNpc148,
	ActNpc149,
	ActNpc150,
	ActNpc151,
	ActNpc152,
	ActNpc153,
	ActNpc154,
	ActNpc155,
	ActNpc156,
	ActNpc157,
	ActNpc158,
	ActNpc159,
	ActNpc160,
	ActNpc161,
	ActNpc162,
	ActNpc163,
	ActNpc164,
	ActNpc165,
	ActNpc166,
	ActNpc167,
	ActNpc168,
	ActNpc169,
	ActNpc170,
	ActNpc171,
	ActNpc172,
	ActNpc173,
	ActNpc174,
	ActNpc175,
	ActNpc176,
	ActNpc177,
	ActNpc178,
	ActNpc179,
	ActNpc180,
	ActNpc181,
	ActNpc182,
	ActNpc183,
	ActNpc184,
	ActNpc185,
	ActNpc186,
	ActNpc187,
	ActNpc188,
	ActNpc189,
	ActNpc190,
	ActNpc191,
	ActNpc192,
	ActNpc193,
	ActNpc194,
	ActNpc195,
	ActNpc196,
	ActNpc197,
	ActNpc198,
	ActNpc199,
	ActNpc200,
	ActNpc201,
	ActNpc202,
	ActNpc203,
	ActNpc204,
	ActNpc205,
	ActNpc206,
	ActNpc207,
	ActNpc208,
	ActNpc209,
	ActNpc210,
	ActNpc211,
	ActNpc212,
	ActNpc213,
	ActNpc214,
	ActNpc215,
	ActNpc216,
	ActNpc217,
	ActNpc218,
	ActNpc219,
	ActNpc220,
	ActNpc221,
	ActNpc222,
	ActNpc223,
	ActNpc224,
	ActNpc225,
	ActNpc226,
	ActNpc227,
	ActNpc228,
	ActNpc229,
	ActNpc230,
	ActNpc231,
	ActNpc232,
	ActNpc233,
	ActNpc234,
	ActNpc235,
	ActNpc236,
	ActNpc237,
	ActNpc238,
	ActNpc239,
	ActNpc240,
	ActNpc241,
	ActNpc242,
	ActNpc243,
	ActNpc244,
	ActNpc245,
	ActNpc246,
	ActNpc247,
	ActNpc248,
	ActNpc249,
	ActNpc250,
	ActNpc251,
	ActNpc252,
	ActNpc253,
	ActNpc254,
	ActNpc255,
	ActNpc256,
	ActNpc257,
	ActNpc258,
	ActNpc259,
	ActNpc260,
	ActNpc261,
	ActNpc262,
	ActNpc263,
	ActNpc264,
	ActNpc265,
	ActNpc266,
	ActNpc267,
	ActNpc268,
	ActNpc269,
	ActNpc270,
	ActNpc271,
	ActNpc272,
	ActNpc273,
	ActNpc274,
	ActNpc275,
	ActNpc276,
	ActNpc277,
	ActNpc278,
	ActNpc279,
	ActNpc280,
	ActNpc281,
	ActNpc282,
	ActNpc283,
	ActNpc284,
	ActNpc285,
	ActNpc286,
	ActNpc287,
	ActNpc288,
	ActNpc289,
	ActNpc290,
	ActNpc291,
	ActNpc292,
	ActNpc293,
	ActNpc294,
	ActNpc295,
	ActNpc296,
	ActNpc297,
	ActNpc298,
	ActNpc299,
	ActNpc300,
	ActNpc301,
	ActNpc302,
	ActNpc303,
	ActNpc304,
	ActNpc305,
	ActNpc306,
	ActNpc307,
	ActNpc308,
	ActNpc309,
	ActNpc310,
	ActNpc311,
	ActNpc312,
	ActNpc313,
	ActNpc314,
	ActNpc315,
	ActNpc316,
	ActNpc317,
	ActNpc318,
	ActNpc319,
	ActNpc320,
	ActNpc321,
	ActNpc322,
	ActNpc323,
	ActNpc324,
	ActNpc325,
	ActNpc326,
	ActNpc327,
	ActNpc328,
	ActNpc329,
	ActNpc330,
	ActNpc331,
	ActNpc332,
	ActNpc333,
	ActNpc334,
	ActNpc335,
	ActNpc336,
	ActNpc337,
	ActNpc338,
	ActNpc339,
	ActNpc340,
	ActNpc341,
	ActNpc342,
	ActNpc343,
	ActNpc344,
	ActNpc345,
	ActNpc346,
	ActNpc347,
	ActNpc348,
	ActNpc349,
	ActNpc350,
	ActNpc351,
	ActNpc352,
	ActNpc353,
	ActNpc354,
	ActNpc355,
	ActNpc356,
	ActNpc357,
	ActNpc358,
	ActNpc359,
	ActNpc360,
};

NPC_TABLE* gNpcTable;

BOOL Replacement_LoadEntityTable(const char* path)
{
	FILE* fp;
	int n;
	size_t size;
	int num;

	size = GetFileSizeLong(path);
	if (size == INVALID_FILE_SIZE)
		return FALSE;

	num = (int)(size / 0x18);

	gNpcTable = (NPC_TABLE*)malloc(num * sizeof(NPC_TABLE));
	if (gNpcTable == NULL)
		return FALSE;

	fp = fopen(path, "rb");
	if (fp == NULL)
	{
		free(gNpcTable);
		gNpcTable = NULL;
		return FALSE;
	}

	for (n = 0; n < num; ++n) // bits
		fread(&gNpcTable[n].bits, 2, 1, fp);
	for (n = 0; n < num; ++n) // life
		fread(&gNpcTable[n].life, 2, 1, fp);
	for (n = 0; n < num; ++n) // surf
		fread(&gNpcTable[n].surf, 1, 1, fp);
	for (n = 0; n < num; ++n) // destroy_voice
		fread(&gNpcTable[n].destroy_voice, 1, 1, fp);
	for (n = 0; n < num; ++n) // hit_voice
		fread(&gNpcTable[n].hit_voice, 1, 1, fp);
	for (n = 0; n < num; ++n) // size
		fread(&gNpcTable[n].size, 1, 1, fp);
	for (n = 0; n < num; ++n) // exp
		fread(&gNpcTable[n].exp, 4, 1, fp);
	for (n = 0; n < num; ++n) // damage
		fread(&gNpcTable[n].damage, 4, 1, fp);
	for (n = 0; n < num; ++n) // hit
		fread(&gNpcTable[n].hit, 4, 1, fp);
	for (n = 0; n < num; ++n) // view
		fread(&gNpcTable[n].view, 4, 1, fp);

	fclose(fp);
	return TRUE;
}

void ReleaseEntityTable(void)
{
	if (gNpcTable != NULL)
	{
		free(gNpcTable);
		gNpcTable = NULL;
	}
}

void Replacement_InitNpChar(void)
{
	memset(gNPC, 0, sizeof(gNPC));
}

BOOL Replacement_LoadEvent(const char* path_event)
{
	int i, n;
	FILE* fp;
	int count;
	char code[4];
	EVENT eve;

	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gDataPath, path_event);

	fp = fopen(path, "rb");
	if (fp == NULL)
		return FALSE;

	// Read "PXE" check
	fread(code, 1, 4, fp);
	if (memcmp(code, gPassPixEve, 3) != 0)
	{
		// The original game forgot to close the file here
		fclose(fp);
		return FALSE;
	}

	// Get amount of NPCs
	fread(&count, 4, 1, fp);

	// Load NPCs
	memset(gNPC, 0, sizeof(gNPC));

	n = 170;
	for (i = 0; i < count; ++i)
	{
		// Get data from file
		fread(&eve, sizeof(EVENT), 1, fp);

		// Set NPC parameters
		gNPC[n].direct = (eve.bits & NPC_SPAWN_IN_OTHER_DIRECTION) ? 2 : 0;
		gNPC[n].code_char = eve.code_char;
		gNPC[n].code_event = eve.code_event;
		gNPC[n].code_flag = eve.code_flag;
		gNPC[n].x = eve.x * 0x10 * 0x200;
		gNPC[n].y = eve.y * 0x10 * 0x200;
		gNPC[n].bits = eve.bits;
		gNPC[n].bits |= gNpcTable[gNPC[n].code_char].bits;
		gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
		SetUniqueParameter(&gNPC[n]);

		// Check flags
		if (gNPC[n].bits & NPC_APPEAR_WHEN_FLAG_SET)
		{
			if (GetNPCFlag(gNPC[n].code_flag) == TRUE)
				gNPC[n].cond |= 0x80;
		}
		else if (gNPC[n].bits & NPC_HIDE_WHEN_FLAG_SET)
		{
			if (GetNPCFlag(gNPC[n].code_flag) == FALSE)
				gNPC[n].cond |= 0x80;
		}
		else
		{
			gNPC[n].cond = 0x80;
		}

		// Increase index
		++n;
	}

	fclose(fp);
	return TRUE;
}

void Replacement_SetNpChar(int code_char, int x, int y, int xm, int ym, int dir, NPCHAR* npc, int start_index)
{
	int n = start_index;
	while (n < NPC_MAX && gNPC[n].cond)
		++n;

	if (n == NPC_MAX)
		return;

	// Set NPC parameters
	memset(&gNPC[n], 0, sizeof(NPCHAR));
	gNPC[n].cond |= 0x80;
	gNPC[n].direct = dir;
	gNPC[n].code_char = code_char;
	gNPC[n].x = x;
	gNPC[n].y = y;
	gNPC[n].xm = xm;
	gNPC[n].ym = ym;
	gNPC[n].pNpc = npc;
	gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
	SetUniqueParameter(&gNPC[n]);
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

			gpNpcFuncTbl[code_char](&gNPC[i]);

			if (gNPC[i].shock)
				--gNPC[i].shock;
		}
	}
}

void Replacement_SetExpObjects(int x, int y, int exp)
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
		gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
		gNPC[n].exp = sub_exp;

		SetUniqueParameter(&gNPC[n]);
	}
}

BOOL Replacement_SetBulletObject(int x, int y, int val)
{
	int n;
	int bullet_no;	// The Linux debug data claims there's a 3-line gap between this and the next variable declaration. Just enough space for an 'if' statement.

	// if (/* unknown */)
	{	// This is necessary for accurate ASM (stack frame layout)
		int tamakazu_ari[10];
		int t = 0;

		memset(tamakazu_ari, 0, sizeof(tamakazu_ari));

		for (n = 0; n < 8; ++n)
		{
			switch (gArmsData[n].code)
			{
			case 5:
				tamakazu_ari[t++] = 0;
				break;

			case 10:
				tamakazu_ari[t++] = 1;
				break;

			default:
				tamakazu_ari[t] = 0;
				break;
			}
		}

		if (t == 0)
			return FALSE;

		n = Random(1, 10 * t);
		bullet_no = tamakazu_ari[n % t];

		n = 0x100;
		while (n < NPC_MAX && gNPC[n].cond)
			++n;

		if (n == NPC_MAX)
			return FALSE;

		memset(&gNPC[n], 0, sizeof(NPCHAR));
		gNPC[n].cond |= 0x80;
		gNPC[n].direct = 0;
		gNPC[n].code_event = bullet_no;
		gNPC[n].code_char = 86;
		gNPC[n].x = x;
		gNPC[n].y = y;
		gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
		gNPC[n].exp = val;
		SetUniqueParameter(&gNPC[n]);
	}

	return TRUE;
}

BOOL Replacement_SetLifeObject(int x, int y, int val)
{
	int n = 0x100;
	while (n < NPC_MAX && gNPC[n].cond)
		++n;

	if (n == NPC_MAX)
		return FALSE;

	memset(&gNPC[n], 0, sizeof(NPCHAR));
	gNPC[n].cond |= 0x80;
	gNPC[n].direct = 0;
	gNPC[n].code_char = 87;
	gNPC[n].x = x;
	gNPC[n].y = y;
	gNPC[n].bits = gNpcTable[gNPC[n].code_char].bits;
	gNPC[n].exp = val;
	SetUniqueParameter(&gNPC[n]);
	return TRUE;
}

void Replacement_VanishNpChar(NPCHAR* npc)
{
	int x, y;

	x = npc->x;
	y = npc->y;
	memset(npc, 0, sizeof(NPCHAR));
	npc->count1 = 0;
	npc->x = x;
	npc->y = y;
	npc->cond |= 0x80;
	npc->direct = 0;
	npc->code_char = 3;
	npc->bits = gNpcTable[npc->code_char].bits;
	npc->exp = gNpcTable[npc->code_char].exp;
	SetUniqueParameter(npc);
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
			gNPC[n].bits |= gNpcTable[gNPC[n].code_char].bits;
			gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
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

			gpNpcFuncTbl[code_char](&gNPC[n]);
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
			gNPC[n].bits |= gNpcTable[gNPC[n].code_char].bits;
			gNPC[n].exp = gNpcTable[gNPC[n].code_char].exp;
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

			gpNpcFuncTbl[code_char](&gNPC[n]);
		}
	}
}

void Replacement_DeleteNpCharEvent(int code)
{
	int i;

	for (i = 0; i < NPC_MAX; ++i)
	{
		if ((gNPC[i].cond & 0x80) && gNPC[i].code_event == code)
		{
			gNPC[i].cond = 0;
			SetNPCFlag(gNPC[i].code_flag);
		}
	}
}

void Replacement_DeleteNpCharCode(int code, BOOL bSmoke)
{
	int n;

	for (n = 0; n < NPC_MAX; ++n)
	{
		if ((gNPC[n].cond & 0x80) && gNPC[n].code_char == code)
		{
			gNPC[n].cond = 0;
			SetNPCFlag(gNPC[n].code_flag);

			if (bSmoke)
			{
				PlaySoundObject(gNPC[n].destroy_voice, SOUND_MODE_PLAY);

				switch (gNPC[n].size)
				{
				case 1:
					SetDestroyNpChar(gNPC[n].x, gNPC[n].y, gNPC[n].view.back, 4);
					break;

				case 2:
					SetDestroyNpChar(gNPC[n].x, gNPC[n].y, gNPC[n].view.back, 8);
					break;

				case 3:
					SetDestroyNpChar(gNPC[n].x, gNPC[n].y, gNPC[n].view.back, 16);
					break;
				}
			}
		}
	}
}