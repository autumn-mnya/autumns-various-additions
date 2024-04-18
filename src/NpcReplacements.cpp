#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "NpcReplacements.h"

#include "main.h"
#include "cave_story.h"
#include "Entity.h"
#include "MyChar.h"
#include "TextScript.h"

// The quote npcs are only replaced if using <MIM

// Quote (teleport out)
void Replacement_ActNpc111(NPCHAR* npc)
{
	RECT rcLeft[2] = {
		{0, 0, 0 + mim_player_size_width, 0 + mim_player_size_height},
		{16, 0, 16 + mim_player_size_width, 0 + mim_player_size_height},
	};

	RECT rcRight[2] = {
		{0, 16, 0 + mim_player_size_width, 16 + mim_player_size_height},
		{16, 16, 16 + mim_player_size_width, 16 + mim_player_size_height},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->y -= 16 * 0x200;
			break;

		case 1:
			if (++npc->act_wait > 20)
			{
				npc->act_wait = 0;
				npc->act_no = 2;
				npc->ani_no = 1;
				npc->ym = -0x2FF;
			}

			break;

		case 2:
			if (npc->ym > 0)
				npc->hit.bottom = 16 * 0x200;

			if (npc->flag & 8)
			{
				npc->act_no = 3;
				npc->act_wait = 0;
				npc->ani_no = 0;
			}

			break;

		case 3:
			if (++npc->act_wait > 40)
			{
				npc->act_no = 4;
				npc->act_wait = 64;
				PlaySoundObject(29, SOUND_MODE_PLAY);
			}

			break;

		case 4:
			--npc->act_wait;
			npc->ani_no = 0;

			if (npc->act_wait == 0)
				npc->cond = 0;

			break;
	}

	npc->ym += 0x40;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Use different sprites if <MIM is active
	npc->rect.top += 32 * mim_num;
	npc->rect.bottom += 32 * mim_num;

	// Use a different sprite if the player is wearing the Mimiga Mask
	if (gMC.equip & EQUIP_MIMIGA_MASK)
	{
		npc->rect.top += 32;
		npc->rect.bottom += 32;
	}

	if (npc->act_no == 4)
	{
		npc->rect.bottom = npc->rect.top + (npc->act_wait / 4);

		if (npc->act_wait / 2 % 2)
			++npc->rect.left;
	}
}

// Quote (teleport in)
void Replacement_ActNpc112(NPCHAR* npc)
{
	RECT rcLeft[2] = {
		{0, 0, 0 + mim_player_size_width, 0 + mim_player_size_height},
		{16, 0, 16 + mim_player_size_width, 0 + mim_player_size_height},
	};

	RECT rcRight[2] = {
		{0, 16, 0 + mim_player_size_width, 16 + mim_player_size_height},
		{16, 16, 16 + mim_player_size_width, 16 + mim_player_size_height},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->x += 16 * 0x200;
			npc->y += 8 * 0x200;
			PlaySoundObject(29, SOUND_MODE_PLAY);
			// Fallthrough
		case 1:
			if (++npc->act_wait == 64)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
			}

			break;

		case 2:
			if (++npc->act_wait > 20)
			{
				npc->act_no = 3;
				npc->ani_no = 1;
				npc->hit.bottom = 8 * 0x200;
			}

			break;

		case 3:
			if (npc->flag & 8)
			{
				npc->act_no = 4;
				npc->act_wait = 0;
				npc->ani_no = 0;
			}

			break;
	}

	npc->ym += 0x40;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Use different sprites if <MIM is active
	npc->rect.top += 32 * mim_num;
	npc->rect.bottom += 32 * mim_num;

	// Use a different sprite if the player is wearing the Mimiga Mask
	if (gMC.equip & EQUIP_MIMIGA_MASK)
	{
		npc->rect.top += 32;
		npc->rect.bottom += 32;
	}

	if (npc->act_no == 1)
	{
		npc->rect.bottom = npc->rect.top + (npc->act_wait / 4);

		if (npc->act_wait / 2 % 2)
			++npc->rect.left;
	}
}

// Quote
void Replacement_ActNpc150(NPCHAR* npc)
{
	int i;

	RECT rcLeft[9] = {
		{0, 0, 0 + mim_player_size_width, 0 + mim_player_size_height},
		{48, 0, 48 + mim_player_size_width, 0 + mim_player_size_height},
		{144, 0, 144 + mim_player_size_width, 0 + mim_player_size_height},
		{16, 0, 16 + mim_player_size_width, 0 + mim_player_size_height},
		{0, 0, 0 + mim_player_size_width, 0 + mim_player_size_height},
		{32, 0, 32 + mim_player_size_width, 0 + mim_player_size_height},
		{0, 0, 0 + mim_player_size_width, 0 + mim_player_size_height},
		{160, 0, 160 + mim_player_size_width, 0 + mim_player_size_height},
		{112, 0, 112 + mim_player_size_width, 0 + mim_player_size_height},
	};

	RECT rcRight[9] = {
		{0, 16, 0 + mim_player_size_width, 16 + mim_player_size_height},
		{48, 16, 48 + mim_player_size_width, 16 + mim_player_size_height},
		{144, 16, 144 + mim_player_size_width, 16 + mim_player_size_height},
		{16, 16, 16 + mim_player_size_width, 16 + mim_player_size_height},
		{0, 16, 0 + mim_player_size_width, 16 + mim_player_size_height},
		{32, 16, 32 + mim_player_size_width, 16 + mim_player_size_height},
		{0, 16, 0 + mim_player_size_width, 16 + mim_player_size_height},
		{160, 16, 160 + mim_player_size_width, 16 + mim_player_size_height},
		{112, 16, 112 + mim_player_size_width, 16 + mim_player_size_height},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;

			if (npc->direct > 10)
			{
				npc->x = gMC.x;
				npc->y = gMC.y;
				npc->direct -= 10;
			}
			break;

		case 2:
			npc->ani_no = 1;
			break;

		case 10:
			npc->act_no = 11;

			for (i = 0; i < 4; ++i)
				SetNpChar(4, npc->x, npc->y, Random(-0x155, 0x155), Random(-0x600, 0), 0, NULL, 0x100);

			PlaySoundObject(71, SOUND_MODE_PLAY);
			// Fallthrough
		case 11:
			npc->ani_no = 2;
			break;

		case 20:
			npc->act_no = 21;
			npc->act_wait = 64;
			PlaySoundObject(29, SOUND_MODE_PLAY);
			// Fallthrough
		case 21:
			if (--npc->act_wait == 0)
				npc->cond = 0;

			break;

		case 50:
			npc->act_no = 51;
			npc->ani_no = 3;
			npc->ani_wait = 0;
			// Fallthrough
		case 51:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 6)
				npc->ani_no = 3;

			if (npc->direct == 0)
				npc->x -= 1 * 0x200;
			else
				npc->x += 1 * 0x200;

			break;

		case 60:
			npc->act_no = 61;
			npc->ani_no = 7;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			// Fallthrough
		case 61:
			npc->tgt_y += 0x100;
			npc->x = npc->tgt_x + (Random(-1, 1) * 0x200);
			npc->y = npc->tgt_y + (Random(-1, 1) * 0x200);
			break;

		case 70:
			npc->act_no = 71;
			npc->act_wait = 0;
			npc->ani_no = 3;
			npc->ani_wait = 0;
			// Fallthrough
		case 71:
			if (npc->direct == 0)
				npc->x += 0x100;
			else
				npc->x -= 0x100;

			if (++npc->ani_wait > 8)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 6)
				npc->ani_no = 3;

			break;

		case 80:
			npc->ani_no = 8;
			break;

		case 99:
		case 100:
			npc->act_no = 101;
			npc->ani_no = 3;
			npc->ani_wait = 0;
			// Fallthrough
		case 101:
			npc->ym += 0x40;

			if (npc->ym > 0x5FF)
				npc->ym = 0x5FF;

			if (npc->flag & 8)
			{
				npc->ym = 0;
				npc->act_no = 102;
			}

			npc->y += npc->ym;
			break;

		case 102:
			if (++npc->ani_wait > 8)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 6)
				npc->ani_no = 3;

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Use different sprites if <MIM is active
	npc->rect.top += 32 * mim_num;
	npc->rect.bottom += 32 * mim_num;

	if (npc->act_no == 21)
	{
		npc->rect.bottom = npc->rect.top + (npc->act_wait / 4);

		if (npc->act_wait / 2 % 2)
			++npc->rect.left;
	}

	// Use a different sprite if the player is wearing the Mimiga Mask
	if (gMC.equip & EQUIP_MIMIGA_MASK)
	{
		npc->rect.top += 32;
		npc->rect.bottom += 32;
	}
}

// The fan npcs are only replaced if "Replace Fan NPC code" is set to true in fmod.ini.

// Fan (left)
void Replacement_ActNpc096(NPCHAR* npc)
{
	RECT rc[3] = {
		{272, 120, 288, 136},
		{288, 120, 304, 136},
		{304, 120, 320, 136},
	};

	switch (npc->act_no)
	{
	case 0:
		if (npc->direct == 2)
			npc->act_no = 2;
		else
			npc->ani_no = 1;

		// Fallthrough
	case 1:
		npc->ani_no = 0;
		break;

	case 2:
		if (++npc->ani_wait > 0)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 2)
			npc->ani_no = 0;

		if (gMC.x > npc->x - (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC.x < npc->x + (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC.y > npc->y - (((WINDOW_HEIGHT / 2) + 120) * 0x200) && gMC.y < npc->y + (((WINDOW_HEIGHT / 2) + 120) * 0x200))
		{
			if (Random(0, 5) == 1)
				SetNpChar(199, npc->x, npc->y + (Random(-8, 8) * 0x200), 0, 0, 0, NULL, 0x100);
		}

		if (gMC.y < npc->y + (8 * 0x200) && gMC.y > npc->y - (8 * 0x200) && gMC.x < npc->x && gMC.x > npc->x - (96 * 0x200))
		{
			gMC.xm -= 0x88;
			playerIsInFan = true;
			gMC.cond |= 0x20;
		}
		else
			playerIsInFan = false;

		break;
	}

	npc->rect = rc[npc->ani_no];
}

// Fan (up)
void Replacement_ActNpc097(NPCHAR* npc)
{
	RECT rc[3] = {
		{272, 136, 288, 152},
		{288, 136, 304, 152},
		{304, 136, 320, 152},
	};

	switch (npc->act_no)
	{
	case 0:
		if (npc->direct == 2)
			npc->act_no = 2;
		else
			npc->ani_no = 1;

		// Fallthrough
	case 1:
		npc->ani_no = 0;
		break;

	case 2:
		if (++npc->ani_wait > 0)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 2)
			npc->ani_no = 0;

		if (gMC.x > npc->x - (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC.x < npc->x + (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC.y > npc->y - (((WINDOW_HEIGHT / 2) + 120) * 0x200) && gMC.y < npc->y + (((WINDOW_HEIGHT / 2) + 120) * 0x200))
		{
			if (Random(0, 5) == 1)
				SetNpChar(199, npc->x + (Random(-8, 8) * 0x200), npc->y, 0, 0, 1, NULL, 0x100);
		}

		if (gMC.x < npc->x + (8 * 0x200) && gMC.x > npc->x - (8 * 0x200) && gMC.y < npc->y && gMC.y > npc->y - (96 * 0x200))
		{
			gMC.ym -= 0x88;
			playerIsInFan = true;
		}
		else
			playerIsInFan = false;

		break;
	}

	npc->rect = rc[npc->ani_no];
}

// Fan (right)
void Replacement_ActNpc098(NPCHAR* npc)
{
	RECT rc[3] = {
		{272, 152, 288, 168},
		{288, 152, 304, 168},
		{304, 152, 320, 168},
	};

	switch (npc->act_no)
	{
	case 0:
		if (npc->direct == 2)
			npc->act_no = 2;
		else
			npc->ani_no = 1;

		// Fallthrough
	case 1:
		npc->ani_no = 0;
		break;

	case 2:
		if (++npc->ani_wait > 0)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 2)
			npc->ani_no = 0;

		if (gMC.x > npc->x - (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC.x < npc->x + (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC.y > npc->y - (((WINDOW_HEIGHT / 2) + 120) * 0x200) && gMC.y < npc->y + (((WINDOW_HEIGHT / 2) + 120) * 0x200))
		{
			if (Random(0, 5) == 1)
				SetNpChar(199, npc->x, npc->y + (Random(-8, 8) * 0x200), 0, 0, 2, NULL, 0x100);
		}

		if (gMC.y < npc->y + (8 * 0x200) && gMC.y > npc->y - (8 * 0x200) && gMC.x < npc->x + (96 * 0x200) && gMC.x > npc->x)
		{
			gMC.xm += 0x88;
			playerIsInFan = true;
			gMC.cond |= 0x20;
		}
		else
			playerIsInFan = false;

		break;
	}

	npc->rect = rc[npc->ani_no];
}

// Fan (down)
void Replacement_ActNpc099(NPCHAR* npc)
{
	RECT rc[3] = {
		{272, 168, 288, 184},
		{288, 168, 304, 184},
		{304, 168, 320, 184},
	};

	switch (npc->act_no)
	{
	case 0:
		if (npc->direct == 2)
			npc->act_no = 2;
		else
			npc->ani_no = 1;

		// Fallthrough
	case 1:
		npc->ani_no = 0;
		break;

	case 2:
		if (++npc->ani_wait > 0)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 2)
			npc->ani_no = 0;

		if (gMC.x > npc->x - (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC.x < npc->x + (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC.y > npc->y - (((WINDOW_HEIGHT / 2) + 120) * 0x200) && gMC.y < npc->y + (((WINDOW_HEIGHT / 2) + 120) * 0x200))
		{
			if (Random(0, 5) == 1)
				SetNpChar(199, npc->x + (Random(-8, 8) * 0x200), npc->y, 0, 0, 3, NULL, 0x100);
		}

		if (gMC.x < npc->x + (8 * 0x200) && gMC.x > npc->x - (8 * 0x200) && gMC.y < npc->y + (96 * 0x200) && gMC.y > npc->y)
		{
			gMC.ym += 0x88;
			playerIsInFan = true;
		}
		else
			playerIsInFan = false;

		break;
	}

	npc->rect = rc[npc->ani_no];
}