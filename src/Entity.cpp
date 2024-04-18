#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Entity.h"

#include "main.h"
#include "Draw.h"
#include "cave_story.h"
#include "EntityLoad.h"
#include "Frame.h"
#include "MyChar.h"
#include "MycParam.h"

#include "Profile.h"

// Currently contains every single custom entity's code!! Add new ones here basically, its the same process as CSE2 except you add the functions into the Entity.h file, and add them to
// the list in EntityTable.cpp.

// Another note: Certain values use " -> "'s instead of " . "'s. gMC .x becomes gMC.x, for example. Most things like gMC and gFrame are in "cave_story.h" but i may have forgotten one or two.
// If you need help using custom npc values, look at the Booster Refill npc as an example of the CustomNpcValues. It's pretty much a copy paste !!

bool entity_IsWallboosting = false;
bool entity_IsIceWalled = false;
bool playerIsInFan = false;

// Booster Fuel Refill
void ActEntity361(NPCHAR* npc)
{
	RECT rect[2] = {
		{32, 0, 48, 16},
		{48, 0, 64, 16},
	};

	RECT rectDir2[2] =
	{
		{64, 0, 80, 16},
		{80, 0, 96, 16},
	};

	int i;

	int boostfuelamount = 50;
	int cooldown = 150;
	int sprite_index = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		boostfuelamount = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		cooldown = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		sprite_index = CustomNpcValues(npc).CustomValueC;

	switch (npc->act_no)
	{
		case 0:
			npc->act_wait = 0;
			npc->ani_no = 0;

			if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
				npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
				npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
			{
				PlaySoundObject(22, SOUND_MODE_PLAY);
				if (npc->direct != 2)
					gMC.boost_cnt = boostfuelamount;
				else
					gMC.boost_cnt = 0;

				npc->act_no = 1;
			}
			break;
		case 1:
			npc->ani_no = 1;

			if (++npc->act_wait > cooldown)
			{
				npc->act_no = 0;
				PlaySoundObject(70, SOUND_MODE_PLAY);
			}
			break;
	}

	if (npc->direct != 2)
		npc->rect = rect[npc->ani_no];
	else
		npc->rect = rectDir2[npc->ani_no];

	npc->rect.top += (16 * 1) * sprite_index;
	npc->rect.bottom += (16 * 1) * sprite_index;
}

// Lock-On Trigger
void ActEntity362(NPCHAR* npc)
{
	int lockon_trigger_speed = 0x5FF;
	int lockon_trigger_tl_x = 0;
	int lockon_trigger_tl_y = 0;
	int lockon_trigger_br_x = 16;
	int lockon_trigger_br_y = 16;

	if (CustomNpcValues(npc).CustomValueA != 0)
		lockon_trigger_speed = CustomNpcValues(npc).CustomValueA;

	// Top Left X rect
	if (CustomNpcValues(npc).CustomValueB != 0)
		lockon_trigger_tl_x = CustomNpcValues(npc).CustomValueB;

	// Top Left Y rect
	if (CustomNpcValues(npc).CustomValueC != 0)
		lockon_trigger_tl_y = CustomNpcValues(npc).CustomValueC;

	// Bottom Right X rect

	if (CustomNpcValues(npc).CustomValueD != 0)
		lockon_trigger_br_x = CustomNpcValues(npc).CustomValueD;

	// Bottom Right Y rect

	if (CustomNpcValues(npc).CustomValueE != 0)
		lockon_trigger_br_y = CustomNpcValues(npc).CustomValueE;

	// Surface override
	if (CustomNpcValues(npc).CustomValueF != 0)
		npc->surf = SurfaceID(CustomNpcValues(npc).CustomValueF);

	RECT rect = { lockon_trigger_tl_x, lockon_trigger_tl_y, lockon_trigger_br_x, lockon_trigger_br_y };

	npc->bits |= NPC_EVENT_WHEN_TOUCHED;

	// Trigger that moves in both Horizontal and Vertical directions
	if (npc->x < gMC.x)
		npc->x += lockon_trigger_speed;
	else
		npc->x -= lockon_trigger_speed;


	if (npc->y < gMC.y)
		npc->y += lockon_trigger_speed;
	else
		npc->y -= lockon_trigger_speed;


	npc->rect = rect;
}

// 2-Way trigger
void ActEntity363(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	npc->bits |= NPC_EVENT_WHEN_TOUCHED;

	switch (npc->act_no)
	{
		case 0:
			// npc->count1 is the event # assigned to the NPC from the Editor. We store this for my code to work LOL
			npc->count1 = npc->code_event;
			npc->act_no = 1;
			break;

		case 1:
			if (npc->direct == 0) // Horizontal Moving Trigger
			{
				if (npc->x < gMC.x)
				{
					npc->x += 0x5FF;
				}
				else
				{
					npc->x -= 0x5FF;
				}

				// If the player is on one side, run one event. If on the other, run another event.
				if (gMC.y > npc->y)
					npc->code_event = npc->count1 + 10; // Add 10 to the event #
				else
					npc->code_event = npc->count1;
			}
			else // Vertical Moving Trigger
			{
				if (npc->y < gMC.y)
					npc->y += 0x5FF;
				else
					npc->y -= 0x5FF;

				// If the player is on one side, run one event. If on the other, run another event.
				if (gMC.x < npc->x)
					npc->code_event = npc->count1 + 10; // Add 10 to the event #
				else
					npc->code_event = npc->count1;
			}
			break;
	}


	npc->rect = rect;
}

// Player Matcher
void ActEntity364(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	switch (npc->act_no)
	{
		case 0:
			if (npc->direct == 2)
			{
				npc->x = gMC.x;
				npc->y = gMC.y;
				npc->act_no = 1;
			}
			else
			{
				npc->x = gMC.x;
				npc->y = gMC.y;
			}
			break;

		case 1:
			npc->cond = 0;
			break;
	}

	npc->rect = rect;
}

// Backstep Trigger
void ActEntity365(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	if (npc->y < gMC.y)
		npc->y += 0x5FF;
	else
		npc->y -= 0x5FF;

	// If the player touches the Entities collision, bounce them backwards automatically.
	if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
		npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
		npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
	{
		if (gMC.x < npc->x)
			BackStepMyChar(2);
		else
			BackStepMyChar(0);
	}

	npc->rect = rect;
}

// Kill Trigger
void ActEntity366(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	if (npc->direct == 0)
	{
		if (npc->x < gMC.x)
			npc->x += 0x5FF;
		else
			npc->x -= 0x5FF;
	}
	else
	{
		if (npc->y < gMC.y)
			npc->y += 0x5FF;
		else
			npc->y -= 0x5FF;
	}

	// If the player touches the Entities collision, kill them immediately.
	if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
		npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
		npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
	{
		PlayerDeath();
		npc->cond = 0;
	}

	npc->rect = rect;
}

// Forced Boost (Horizontal)
void ActEntity367(NPCHAR* npc)
{
	RECT rectLeft[2] = {
		{96, 0, 112, 16},
		{112, 0, 128, 16},
	};

	RECT rectRight[2] =
	{
		{128, 0, 144, 16},
		{144, 0, 160, 16},
	};

	int amount = 50;
	int cooldown = 50;
	int sprite_index = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		amount = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		cooldown = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		sprite_index = CustomNpcValues(npc).CustomValueC;


	switch (npc->act_no)
	{
		case 0:
			npc->act_wait = 0;
			npc->ani_no = 0;

			if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
				npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
				npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
			{
				PlaySoundObject(22, SOUND_MODE_PLAY);
				gMC.boost_cnt = amount;
				// Store the players Y value to make it appear normal
				npc->count1 = gMC.y;

				// Set direction of the Player to the same as the boost direction
				if (npc->direct == 2)
					gMC.direct = 2;
				else
					gMC.direct = 0;

				npc->act_no = 1;
			}
			break;
		case 1:
			npc->ani_no = 1;

			if (gMC.boost_cnt != 0)
			{
				switch (npc->direct)
				{
					default:
						gMC.boost_sw = 1;
						gMC.ym = 0;
						gMC.y = npc->count1;
						gMC.xm = -0x5FF;
						break;

					case 2:
						gMC.boost_sw = 1;
						gMC.ym = 0;
						gMC.y = npc->count1;
						gMC.xm = 0x5FF;
						break;

				}

				--gMC.boost_cnt;
			}


			if (++npc->act_wait > cooldown)
			{
				npc->count1 = 0;
				npc->act_no = 0;
				PlaySoundObject(70, SOUND_MODE_PLAY);
			}
			break;
	}

	if (npc->act_no != 0)
	{
		if (gMC.boost_sw == 1)
		{
			// Go up when going into a wall
			if (gMC.flag & 5)
				gMC.ym = -0x100;

			// Move in direction facing
			if (gMC.direct == 0)
				gMC.xm -= 0x20;
			if (gMC.direct == 2)
				gMC.xm += 0x20;

			// Boost particles (and sound)
			if (gMC.boost_cnt % 3 == 1)
			{
				if (gMC.direct == 0)
					SetCaret(gMC.x + (2 * 0x200), gMC.y + (2 * 0x200), CARET_EXHAUST, DIR_RIGHT);
				if (gMC.direct == 2)
					SetCaret(gMC.x - (2 * 0x200), gMC.y + (2 * 0x200), CARET_EXHAUST, DIR_LEFT);

				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC.boost_sw == 2)
		{
			// Move upwards
			gMC.ym -= 0x20;

			// Boost particles (and sound)
			if (gMC.boost_cnt % 3 == 1)
			{
				SetCaret(gMC.x, gMC.y + (6 * 0x200), CARET_EXHAUST, DIR_DOWN);
				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC.boost_sw == 3 && gMC.boost_cnt % 3 == 1)
		{
			// Boost particles (and sound)
			SetCaret(gMC.x, gMC.y - (6 * 0x200), CARET_EXHAUST, DIR_UP);
			PlaySoundObject(113, SOUND_MODE_PLAY);
		}
	}

	if (npc->direct != 2)
		npc->rect = rectLeft[npc->ani_no];
	else
		npc->rect = rectRight[npc->ani_no];

	npc->rect.top += (16 * 1) * sprite_index;
	npc->rect.bottom += (16 * 1) * sprite_index;
}

// Forced Boost (Vertical)
void ActEntity368(NPCHAR* npc)
{
	RECT rectUp[2] = {
		{160, 0, 176, 16},
		{176, 0, 192, 16},
	};

	RECT rectDown[2] =
	{
		{192, 0, 208, 16},
		{208, 0, 224, 16},
	};

	int amount = 50;
	int cooldown = 50;
	int sprite_index = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		amount = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		cooldown = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		sprite_index = CustomNpcValues(npc).CustomValueC;

	switch (npc->act_no)
	{
	case 0:
		npc->act_wait = 0;
		npc->ani_no = 0;

		if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
			npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
			npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
			npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
		{
			PlaySoundObject(22, SOUND_MODE_PLAY);
			gMC.boost_cnt = amount;

			npc->act_no = 1;
		}
		break;
	case 1:
		npc->ani_no = 1;

		if (gMC.boost_cnt != 0)
		{
			switch (npc->direct)
			{
				default:
					gMC.boost_sw = 2;
					gMC.xm = 0;
					gMC.ym = -0x5FF;
					break;

				case 2:
					gMC.boost_sw = 3;
					gMC.xm = 0;
					gMC.ym = 0x5FF;
					break;
			}

			--gMC.boost_cnt;
		}


		if (++npc->act_wait > cooldown)
		{
			npc->count1 = 0;
			npc->act_no = 0;
			PlaySoundObject(70, SOUND_MODE_PLAY);
		}
		break;
	}

	if (npc->act_no != 0)
	{
		if (gMC.boost_sw == 1)
		{
			// Go up when going into a wall
			if (gMC.flag & 5)
				gMC.ym = -0x100;

			// Move in direction facing
			if (gMC.direct == 0)
				gMC.xm -= 0x20;
			if (gMC.direct == 2)
				gMC.xm += 0x20;

			// Boost particles (and sound)
			if (gMC.boost_cnt % 3 == 1)
			{
				if (gMC.direct == 0)
					SetCaret(gMC.x + (2 * 0x200), gMC.y + (2 * 0x200), CARET_EXHAUST, DIR_RIGHT);
				if (gMC.direct == 2)
					SetCaret(gMC.x - (2 * 0x200), gMC.y + (2 * 0x200), CARET_EXHAUST, DIR_LEFT);

				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC.boost_sw == 2)
		{
			// Move upwards
			gMC.ym -= 0x20;

			// Boost particles (and sound)
			if (gMC.boost_cnt % 3 == 1)
			{
				SetCaret(gMC.x, gMC.y + (6 * 0x200), CARET_EXHAUST, DIR_DOWN);
				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC.boost_sw == 3 && gMC.boost_cnt % 3 == 1)
		{
			// Boost particles (and sound)
			SetCaret(gMC.x, gMC.y - (6 * 0x200), CARET_EXHAUST, DIR_UP);
			PlaySoundObject(113, SOUND_MODE_PLAY);
		}
	}

	if (npc->direct != 2)
		npc->rect = rectUp[npc->ani_no];
	else
		npc->rect = rectDown[npc->ani_no];

	npc->rect.top += (16 * 1) * sprite_index;
	npc->rect.bottom += (16 * 1) * sprite_index;
}

// Flag Trigger
void ActEntity369(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	if (npc->direct == 0)
	{
		if (npc->x < gMC.x)
			npc->x += 0x5FF;
		else
			npc->x -= 0x5FF;
	}
	else
	{
		if (npc->y < gMC.y)
			npc->y += 0x5FF;
		else
			npc->y -= 0x5FF;
	}

	if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
		npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
		npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
	{
		if (!(npc->bits & NPC_CUSTOM_FLAG))
		{
			if (CustomNpcValues(npc).CustomValueA != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValueA);

			if (CustomNpcValues(npc).CustomValueB != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValueB);

			if (CustomNpcValues(npc).CustomValueC != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValueC);

			if (CustomNpcValues(npc).CustomValueD != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValueD);

			if (CustomNpcValues(npc).CustomValueE != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValueE);

			if (CustomNpcValues(npc).CustomValueF != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValueF);
		}
		else
		{
			if (CustomNpcValues(npc).CustomValueA != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValueA);

			if (CustomNpcValues(npc).CustomValueB != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValueB);

			if (CustomNpcValues(npc).CustomValueC != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValueC);

			if (CustomNpcValues(npc).CustomValueD != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValueD);

			if (CustomNpcValues(npc).CustomValueE != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValueE);

			if (CustomNpcValues(npc).CustomValueF != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValueF);
		}
	}

	npc->rect = rect;
}

// Purple Critter (upside down)
void ActEntity370(NPCHAR* npc)
{
	int xm, ym;
	unsigned char deg;

	RECT rcLeft[6] = {
		{0, 192, 16, 208},
		{16, 192, 32, 208},
		{32, 192, 48, 208},
		{48, 192, 64, 208},
		{64, 192, 80, 208},
		{80, 192, 96, 208},
	};

	RECT rcRight[6] = {
		{0, 208, 16, 224},
		{16, 208, 32, 224},
		{32, 208, 48, 224},
		{48, 208, 64, 224},
		{64, 208, 80, 224},
		{80, 208, 96, 224},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->y -= 3 * 0x200;
			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (npc->act_wait >= 8 && npc->x - (96 * 0x200) < gMC.x && npc->x + (96 * 0x200) > gMC.x && npc->y - (32 * 0x200) < gMC.y && npc->y + (96 * 0x200) > gMC.y)
			{
				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;

				npc->ani_no = 1;
			}
			else
			{
				if (npc->act_wait < 8)
					++npc->act_wait;

				npc->ani_no = 0;
			}

			if (npc->shock)
			{
				npc->act_no = 2;
				npc->ani_no = 0;
				npc->act_wait = 0;
			}

			if (npc->act_wait >= 8 && npc->x - (48 * 0x200) < gMC.x && npc->x + (48 * 0x200) > gMC.x && npc->y - (32 * 0x200) < gMC.y && npc->y + (160 * 0x200) > gMC.y)
			{
				npc->act_no = 2;
				npc->ani_no = 0;
				npc->act_wait = 0;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 3;
				npc->ani_no = 2;
				npc->ym = 0x5FF;
				PlaySoundObject(30, SOUND_MODE_PLAY);

				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 3:
			if (npc->ym < -0x100)
			{
				npc->tgt_y = npc->y;
				npc->act_no = 4;
				npc->ani_no = 3;
				npc->act_wait = 0;
				npc->act_wait = 0;	// Duplicate line
			}

			break;

		case 4:
			if (npc->x < gMC.x)
				npc->direct = 2;
			else
				npc->direct = 0;

			++npc->act_wait;

			if (npc->flag & 13 || npc->act_wait > 60)
			{
				npc->damage = 3;
				npc->act_no = 5;
				npc->ani_no = 2;
				break;
			}

			if (npc->act_wait % 4 == 1)
				PlaySoundObject(109, SOUND_MODE_PLAY);

			if (npc->flag & COLL_CEILING)
				npc->ym = 0x200; // probably wrong

			if (npc->act_wait % 30 == 6)
			{
				deg = GetArktan(npc->x - gMC.x, npc->y - gMC.y);
				deg += (unsigned char)Random(-6, 6);
				ym = GetSin(deg) * 3;
				xm = GetCos(deg) * 3;

				SetNpChar(148, npc->x, npc->y, xm, ym, 0, NULL, 0x100);
				PlaySoundObject(39, SOUND_MODE_PLAY);
			}

			if (++npc->ani_wait > 0)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 3;

			break;

		case 5:
			if (npc->flag & COLL_CEILING)
			{
				npc->damage = 2;
				npc->xm = 0;
				npc->act_wait = 0;
				npc->ani_no = 0;
				npc->act_no = 1;
				PlaySoundObject(23, SOUND_MODE_PLAY);
			}

			break;
	}

	if (npc->act_no != 4)
	{
		npc->ym -= 0x20;
		if (npc->ym < -0x5FF)
			npc->ym = -0x5FF;
	}
	else
	{
		if (npc->y < npc->tgt_y)
			npc->ym += 0x10;
		else
			npc->ym -= 0x10;

		if (npc->ym < -0x200)
			npc->ym = -0x200;
		if (npc->ym > 0x200)
			npc->ym = 0x200;

		if (npc->xm < -0x200)
			npc->xm = -0x200;
		if (npc->xm > 0x200)
			npc->xm = 0x200;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];
}

// Camera Target
void ActEntity371(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	int x_radius = 5;
	int y_radius = 5;

	if (CustomNpcValues(npc).CustomValueA != 0)
		x_radius = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		y_radius = CustomNpcValues(npc).CustomValueB;

	// Left, Right, Up, Down. 16 is 1 tile, so doing math we can have a radius for the Camera to target to.
	switch (npc->act_no)
	{
		case 0:
			if (npc->x - ((16 * x_radius) * 0x200)
				< gMC.x && npc->x + ((16 * x_radius) * 0x200)
				> gMC.x && npc->y - ((16 * y_radius) * 0x200)
				< gMC.y && npc->y + ((16 * y_radius) * 0x200)
				> gMC.y)
			{
				gFrame.tgt_x = &npc->x;
				gFrame.tgt_y = &npc->y;
			}
			else if (npc->x - ((16 * (x_radius + 1)) * 0x200)
				< gMC.x && npc->x + ((16 * (x_radius + 1)) * 0x200)
				> gMC.x && npc->y - ((16 * (y_radius + 1)) * 0x200)
				< gMC.y && npc->y + ((16 * (y_radius + 1)) * 0x200)
				> gMC.y)
			{
				gFrame.tgt_x = &gMC.tgt_x;
				gFrame.tgt_y = &gMC.tgt_y;
			}
			break;

		case 1:
			gFrame.tgt_x = &gMC.tgt_x;
			gFrame.tgt_y = &gMC.tgt_y;
			npc->cond = 0;
			break;
	}

	npc->rect = rect;
}

// Transport Trigger
void ActEntity372(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	int tra_map = 1;
	int tra_event = 90;
	int tra_x = 10;
	int tra_y = 8;
	int fade_dir = 4;

	if (CustomNpcValues(npc).CustomValueA != 0)
		tra_map = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		tra_event = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		tra_x = CustomNpcValues(npc).CustomValueC;

	if (CustomNpcValues(npc).CustomValueD != 0)
		tra_y = CustomNpcValues(npc).CustomValueD;

	if (CustomNpcValues(npc).CustomValueE != 0)
		fade_dir = CustomNpcValues(npc).CustomValueE;

	if (npc->direct == 0)
	{
		if (npc->x < gMC.x)
			npc->x += 0x5FF;
		else
			npc->x -= 0x5FF;
	}
	else
	{
		if (npc->y < gMC.y)
			npc->y += 0x5FF;
		else
			npc->y -= 0x5FF;
	}

	switch (npc->act_no)
	{
		case 0:
			if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
				npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
				npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
			{
				g_GameFlags &= ~2;
				g_GameFlags |= 1;
				gMC.xm = 0;
				if (fade_dir <= 4)
					StartFadeOut(fade_dir);
				npc->act_no = 1;
			}
			break;

		case 1:
			if (gFade.mode == 0)
				TransferStage(tra_map, tra_event, tra_x, tra_y);
			break;
	}

	npc->rect = rect;
}

// Energy Capsule (Custom)
void ActEntity373(NPCHAR* npc)
{
	int exp_amount = 10;

	if (CustomNpcValues(npc).CustomValueA != 0)
		exp_amount = CustomNpcValues(npc).CustomValueA;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			break;
		}

		if (npc->life <= 100)
		{
			SetExpObjects(npc->x, npc->y, exp_amount);
			SetDestroyNpChar(npc->x, npc->y, npc->view.back, 8);
			PlaySoundObject(25, SOUND_MODE_PLAY);
			npc->cond = 0;
		}

	RECT rc[2] = {
		{0, 64, 16, 80},
		{16, 64, 32, 80},
	};

	npc->rect = rc[npc->ani_no];
}

// Customizable Fan (left)
void ActEntity374(NPCHAR* npc)
{
	RECT rc[3] = {
		{272, 120, 288, 136},
		{288, 120, 304, 136},
		{304, 120, 320, 136},
	};

	int speed = 136;

	if (CustomNpcValues(npc).CustomValueA != 0)
		speed = CustomNpcValues(npc).CustomValueA;

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
				gMC.xm -= speed;
				playerIsInFan = true;
				gMC.cond |= 0x20;
			}
			else
				playerIsInFan = false;

			break;
	}

	npc->rect = rc[npc->ani_no];
}

// Customizable Fan (up)
void ActEntity375(NPCHAR* npc)
{
	RECT rc[3] = {
		{272, 136, 288, 152},
		{288, 136, 304, 152},
		{304, 136, 320, 152},
	};

	int speed = 136;

	if (CustomNpcValues(npc).CustomValueA != 0)
		speed = CustomNpcValues(npc).CustomValueA;

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
				gMC.ym -= speed;
				playerIsInFan = true;
			}
			else
				playerIsInFan = false;

			break;
	}

	npc->rect = rc[npc->ani_no];
}

// Customizable Fan (right)
void ActEntity376(NPCHAR* npc)
{
	RECT rc[3] = {
		{272, 152, 288, 168},
		{288, 152, 304, 168},
		{304, 152, 320, 168},
	};

	int speed = 136;

	if (CustomNpcValues(npc).CustomValueA != 0)
		speed = CustomNpcValues(npc).CustomValueA;

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
				gMC.xm += speed;
				playerIsInFan = true;
				gMC.cond |= 0x20;
			}
			else
				playerIsInFan = false;

			break;
	}

	npc->rect = rc[npc->ani_no];
}

// Customizable Fan (down)
void ActEntity377(NPCHAR* npc)
{
	RECT rc[3] = {
		{272, 168, 288, 184},
		{288, 168, 304, 184},
		{304, 168, 320, 184},
	};

	int speed = 136;

	if (CustomNpcValues(npc).CustomValueA != 0)
		speed = CustomNpcValues(npc).CustomValueA;

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
				gMC.ym += speed;
				playerIsInFan = true;
			}
			else
				playerIsInFan = false;

			break;
	}

	npc->rect = rc[npc->ani_no];
}

// Spikes (Custom)
void ActEntity378(NPCHAR* npc)
{
	RECT rects[4] = {
		{256, 200, 272, 216},
		{272, 200, 288, 216},
		{288, 200, 304, 216},
		{304, 200, 320, 216},
	};

	npc->rect = rects[CustomNpcValues(npc).CustomValueA];
}

// Rolling (Custom)
void ActEntity379(NPCHAR* npc)
{
	RECT rc[3] = {
		{144, 136, 160, 152},
		{160, 136, 176, 152},
		{176, 136, 192, 152},
	};

	int acceleration = 64;
	int maxspeed = 1024;

	if (CustomNpcValues(npc).CustomValueA != 0)
		acceleration = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		maxspeed = CustomNpcValues(npc).CustomValueB;

	switch (npc->act_no)
	{
		case 0:
			ChangeMapParts(npc->x / 0x200 / 0x10, npc->y / 0x200 / 0x10, 0);

			if (npc->direct == 0)
				npc->act_no = 10;
			else
				npc->act_no = 30;

			break;

		case 10:
			npc->xm -= acceleration;
			npc->ym = 0;

			if (npc->flag & 1)
				npc->act_no = 20;

			break;

		case 20:
			npc->xm = 0;
			npc->ym -= acceleration;

			if (npc->flag & 2)
				npc->act_no = 30;

			break;

		case 30:
			npc->xm += acceleration;
			npc->ym = 0;

			if (npc->flag & 4)
				npc->act_no = 40;

			break;

		case 40:
			npc->xm = 0;
			npc->ym += acceleration;

			if (npc->flag & 8)
				npc->act_no = 10;

			break;
	}

	if (npc->xm < -maxspeed)
		npc->xm = -maxspeed;
	if (npc->xm > maxspeed)
		npc->xm = maxspeed;

	if (npc->ym < -maxspeed)
		npc->ym = -maxspeed;
	if (npc->ym > maxspeed)
		npc->ym = maxspeed;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	npc->rect = rc[npc->ani_no];
}

// Acid/Lava Drop Generator (Custom)
void ActEntity380(NPCHAR* npc)
{
	RECT rc[4] = {
		{0, 0, 0, 0},
		{104, 0, 112, 16},
		{112, 0, 120, 16},
		{120, 0, 128, 16},
	};

	int dropWait = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		dropWait = CustomNpcValues(npc).CustomValueA;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->act_wait = npc->code_event;
			// Fallthrough
		case 1:
			npc->ani_no = 0;

			if (npc->act_wait != 0)
			{
				--npc->act_wait;
				return;
			}

			npc->act_no = 10;
			npc->ani_wait = 0;
			break;

		case 10:
			if (++npc->ani_wait > 10)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
			{
				npc->ani_no = 0;
				npc->act_no = 1;
				npc->act_wait = dropWait;
				SetNpChar(244, npc->x, npc->y, 0, 0, 0, NULL, 0x100);
			}

			break;
	}

	if (npc->ani_wait / 2 % 2)
		npc->x = npc->tgt_x;
	else
		npc->x = npc->tgt_x + 0x200;

	npc->rect = rc[npc->ani_no];
}

// Customizable Character Textures //

// Sue (Custom)
void ActEntity381(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	int n;

	RECT rcLeft[13] = {
		{0, 0, 16, 16}, // Standing Still
		{16, 0, 32, 16}, // Standing Still (blinking)
		{32, 0, 48, 16}, // Walking A
		{0, 0, 16, 16}, // Standing Still
		{48, 0, 64, 16}, // Walking B
		{0, 0, 16, 16}, // Standing Still
		{64, 0, 80, 16}, // Turned Back sprite
		{80, 0, 96, 16}, // Damaged
		{96, 0, 112, 16}, // Downed
		{128, 0, 144, 16}, // Angry
		{0, 0, 16, 16}, // Standing Still
		{112, 0, 128, 16}, // Carried
		{160, 0, 176, 16}, // Looking Up
	};

	RECT rcRight[13] = {
		{0, 16, 16, 32}, // Standing Still
		{16, 16, 32, 32}, // Standing Still (blinking)
		{32, 16, 48, 32}, // Walking A
		{0, 16, 16, 32}, // Standing Still
		{48, 16, 64, 32}, // Walking B
		{0, 16, 16, 32}, // Standing Still
		{64, 16, 80, 32}, // Turned Back sprite
		{80, 16, 96, 32}, // Damaged
		{96, 16, 112, 32}, // Downed
		{128, 16, 144, 32}, // Angry
		{0, 16, 16, 32}, // Standing Still
		{112, 16, 128, 32}, // Carried
		{160, 16, 176, 32}, // Looking Up
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			break;

		case 5:
			npc->ani_no = 6;
			npc->xm = 0;
			break;

		case 6:
			PlaySoundObject(50, SOUND_MODE_PLAY);
			npc->act_wait = 0;
			npc->act_no = 7;
			npc->ani_no = 7;
			// Fallthrough
		case 7:
			if (++npc->act_wait > 10)
				npc->act_no = 0;

			break;

		case 8:
			PlaySoundObject(50, SOUND_MODE_PLAY);
			npc->act_wait = 0;
			npc->act_no = 9;
			npc->ani_no = 7;
			npc->ym = -0x200;

			if (npc->direct == 0)
				npc->xm = 0x400;
			else
				npc->xm = -0x400;

			// Fallthrough
		case 9:
			if (++npc->act_wait > 3 && npc->flag & 8)
			{
				npc->act_no = 10;

				if (npc->direct == 0)
					npc->direct = 2;
				else
					npc->direct = 0;
			}

			break;

		case 10:
			npc->xm = 0;
			npc->ani_no = 8;
			break;

		case 11:
			npc->act_no = 12;
			npc->act_wait = 0;
			npc->ani_no = 9;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 12:
			if (++npc->ani_wait > 8)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 10)
				npc->ani_no = 9;

			break;

		case 13:
			npc->ani_no = 11;
			npc->xm = 0;
			npc->ym = 0;
			npc->act_no = 14;

			for (n = 0; n < NPC_MAX; ++n)
				if (gNPC[n].code_event == 501)
					break;

			if (n == NPC_MAX)
			{
				npc->act_no = 0;
				break;
			}

			npc->pNpc = &gNPC[n];
			// Fallthrough
		case 14:
			if (npc->pNpc->direct == 0)
				npc->direct = 2;
			else
				npc->direct = 0;

			if (npc->pNpc->direct == 0)
				npc->x = npc->pNpc->x - (6 * 0x200);
			else
				npc->x = npc->pNpc->x + (6 * 0x200);

			npc->y = npc->pNpc->y + (4 * 0x200);

			if (npc->pNpc->ani_no == 2 || npc->pNpc->ani_no == 4)
				npc->y -= 1 * 0x200;

			break;

		case 15:
			npc->act_no = 16;
			SetNpChar(257, npc->x + (128 * 0x200), npc->y, 0, 0, 0, NULL, 0);
			SetNpChar(257, npc->x + (128 * 0x200), npc->y, 0, 0, 2, NULL, 0x80);
			npc->xm = 0;
			npc->ani_no = 0;
			// Fallthrough
		case 16:
			gSuperXpos = npc->x - (24 * 0x200);
			gSuperYpos = npc->y - (8 * 0x200);
			break;

		case 17:
			npc->xm = 0;
			npc->ani_no = 12;
			gSuperXpos = npc->x;
			gSuperYpos = npc->y - (8 * 0x200);
			break;

		case 20:
			npc->act_no = 21;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 21:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			if (npc->x < gMC.x - (8 * 0x200))
			{
				npc->direct = 2;
				npc->act_no = 0;
			}

			break;

		case 30:
			npc->act_no = 31;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 31:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			break;

		case 40:
			npc->act_no = 41;
			npc->ani_no = 9;
			npc->ym = -0x400;
			break;
	}

	if (npc->act_no != 14)
	{
		npc->ym += 0x40;

		if (npc->xm > 0x400)
			npc->xm = 0x400;
		if (npc->xm < -0x400)
			npc->xm = -0x400;

		if (npc->ym > 0x5FF)
			npc->ym = 0x5FF;

		npc->x += npc->xm;
		npc->y += npc->ym;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Sue (being teleported by Misery) (Custom)
void ActEntity382(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{112, 0, 128, 16}, // Teleport A (Same as Carried)
		{144, 0, 160, 16}, // Teleport B
	};

	RECT rcRight[2] = {
		{112, 16, 128, 32}, // Teleport A (Same as Carried)
		{144, 16, 160, 32}, // Teleport B
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->x += 6 * 0x200;
			npc->tgt_x = npc->x;
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
			npc->ani_no = 0;

			if (npc->flag & 8)
			{
				npc->act_no = 4;
				npc->act_wait = 0;
				npc->ani_no = 1;
				PlaySoundObject(23, SOUND_MODE_PLAY);
			}

			break;
	}

	if (npc->act_no > 1)
	{
		npc->ym += 0x20;
		if (npc->ym > 0x5FF)
			npc->ym = 0x5FF;

		npc->y += npc->ym;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 1)
	{
		npc->rect.bottom = npc->rect.top + npc->act_wait / 4;

		if (npc->act_wait / 2 % 2)
			npc->x = npc->tgt_x;
		else
			npc->x = npc->tgt_x + (1 * 0x200);
	}

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Sue at PC (Custom)
void ActEntity383(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[3] = {
		{176, 0, 192, 24},
		{192, 0, 208, 24},
		{208, 0, 224, 24},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->x -= 4 * 0x200;
			npc->y += 16 * 0x200;
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (Random(0, 80) == 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (Random(0, 120) == 10)
			{
				npc->act_no = 3;
				npc->act_wait = 0;
				npc->ani_no = 2;
			}

			break;

		case 2:
			if (++npc->act_wait > 40)
			{
				npc->act_no = 3;
				npc->act_wait = 0;
				npc->ani_no = 2;
			}

			break;

		case 3:
			if (++npc->act_wait > 80)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	npc->rect = rcLeft[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 2) * customrect;
	npc->rect.bottom += (24 * 2) * customrect;
}

// Kazuma (Custom)
void ActEntity384(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[6] = {
		{224, 0, 240, 24}, // Standing Still
		{240, 0, 256, 24}, // Walking A
		{224, 0, 240, 24}, // Standing Still
		{256, 0, 272, 24}, // Walking B
		{224, 0, 240, 24}, // Standing Still
		{272, 0, 288, 24}, // Turned Back
	};

	RECT rcRight[6] = {
		{224, 24, 240, 48}, // Standing Still
		{240, 24, 256, 48}, // Walking A
		{224, 24, 240, 48}, // Standing Still
		{256, 24, 272, 48}, // Walking B
		{224, 24, 240, 48}, // Standing Still
		{272, 24, 288, 48}, // Turned Back
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 1;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			if (npc->direct == 0)
				npc->x -= 1 * 0x200;
			else
				npc->x += 1 * 0x200;

			break;

		case 5:
			npc->ani_no = 5;
			break;
	}

	npc->ym += 0x20;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 2) * customrect;
	npc->rect.bottom += (24 * 2) * customrect;
}

// Kazuma at computer (Custom)
void ActEntity385(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[3] = {
		{288, 0, 304, 24},
		{304, 0, 320, 24},
		{320, 0, 336, 24},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->x -= 4 * 0x200;
			npc->y += 16 * 0x200;
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (Random(0, 80) == 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (Random(0, 120) == 10)
			{
				npc->act_no = 3;
				npc->act_wait = 0;
				npc->ani_no = 2;
			}

			break;

		case 2:
			if (++npc->act_wait > 40)
			{
				npc->act_no = 3;
				npc->act_wait = 0;
				npc->ani_no = 2;
			}

			break;

		case 3:
			if (++npc->act_wait > 80)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	npc->rect = rcLeft[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 2) * customrect;
	npc->rect.bottom += (24 * 2) * customrect;
}

// Toroko (Custom)
void ActEntity386(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[8] = {
		{336, 0, 352, 16}, // Standing Still
		{352, 0, 368, 16}, // Standing Still (blinking)
		{368, 0, 384, 16}, // Running A
		{352, 0, 368, 16}, // Standing Still (blinking)
		{384, 0, 400, 16}, // Running B
		{352, 0, 368, 16}, // Standing Still (blinking)
		{448, 0, 464, 16}, // Trip
		{464, 0, 480, 16}, // Downed
	};

	RECT rcRight[8] = {
		{336, 16, 352, 32}, // Standing Still
		{352, 16, 368, 32}, // Standing Still (blinking)
		{368, 16, 384, 32}, // Running A
		{352, 16, 368, 32}, // Standing Still (blinking)
		{384, 16, 400, 32}, // Running B
		{352, 16, 368, 32}, // Standing Still (blinking)
		{448, 16, 464, 32}, // Trip
		{464, 16, 480, 32}, // Downed
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (npc->x - (16 * 0x200) < gMC.x && npc->x + (16 * 0x200) > gMC.x && npc->y - (16 * 0x200) < gMC.y && npc->y + (16 * 0x200) > gMC.y)
			{
				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 1;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			if (npc->flag & 1)
			{
				npc->direct = 2;
				npc->xm = 0x200;
			}

			if (npc->flag & 4)
			{
				npc->direct = 0;
				npc->xm = -0x200;
			}

			if (npc->direct == 0)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			break;

		case 6:
			npc->act_no = 7;
			npc->act_wait = 0;
			npc->ani_no = 1;
			npc->ani_wait = 0;
			npc->ym = -0x400;
			// Fallthrough
		case 7:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			if (npc->direct == 0)
				npc->xm = -0x100;
			else
				npc->xm = 0x100;

			if (npc->act_wait++ != 0 && npc->flag & 8)
				npc->act_no = 3;

			break;

		case 8:
			npc->ani_no = 1;
			npc->act_wait = 0;
			npc->act_no = 9;
			npc->ym = -0x200;
			// Fallthrough
		case 9:
			if (npc->act_wait++ != 0 && npc->flag & 8)
				npc->act_no = 0;

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 6;
			npc->ym = -0x400;
			PlaySoundObject(50, SOUND_MODE_PLAY);

			if (npc->direct == 0)
				npc->xm = -0x100;
			else
				npc->xm = 0x100;

			break;

		case 11:
			if (npc->act_wait++ != 0 && npc->flag & 8)
			{
				npc->act_no = 12;
				npc->ani_no = 7;
				npc->bits |= NPC_INTERACTABLE;
			}

			break;

		case 12:
			npc->xm = 0;
			break;
	}

	npc->ym += 0x40;

	if (npc->xm > 0x400)
		npc->xm = 0x400;
	if (npc->xm < -0x400)
		npc->xm = -0x400;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Toroko with stick (Custom)
void ActEntity387(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[6] = {
		{400, 0, 416, 16}, // Stick A
		{416, 0, 432, 16}, // Stick B
		{400, 0, 416, 16}, // Stick A
		{432, 0, 448, 16}, // Stick C
		{448, 0, 464, 16}, // Trip
		{464, 0, 480, 16}, // Downed
	};

	RECT rcRight[6] = {
		{64, 80, 80, 96}, // Stick A
		{80, 80, 96, 96}, // Stick B
		{64, 80, 80, 96}, // Stick A
		{96, 80, 112, 96}, // Stick C
		{112, 80, 128, 96}, // Trip
		{128, 80, 144, 96}, // Downed
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->act_wait = 0;
			npc->ani_wait = 0;
			npc->ym = -0x400;
			// Fallthrough
		case 1:
			if (npc->ym > 0)
				npc->bits &= ~NPC_IGNORE_SOLIDITY;

			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 0;

			if (npc->direct == 0)
				npc->xm = -0x100;
			else
				npc->xm = 0x100;

			if (npc->act_wait++ != 0 && npc->flag & 8)
				npc->act_no = 2;

			break;

		case 2:
			npc->act_no = 3;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 3:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 0;

			if (++npc->act_wait > 50)
			{
				npc->act_wait = 40;
				npc->xm *= -1;

				if (npc->direct == 0)
					npc->direct = 2;
				else
					npc->direct = 0;
			}

			if (npc->act_wait > 35)
				npc->bits |= NPC_SHOOTABLE;

			if (npc->direct == 0)
				npc->xm -= 0x40;
			else
				npc->xm += 0x40;

			if (npc->shock)
			{
				npc->act_no = 4;
				npc->ani_no = 4;
				npc->ym = -0x400;
				npc->bits &= ~NPC_SHOOTABLE;
				npc->damage = 0;
			}

			break;

		case 4:
			if (npc->direct == 0)
				npc->xm = -0x100;
			else
				npc->xm = 0x100;

			if (npc->act_wait++ != 0 && npc->flag & 8)
			{
				npc->act_no = 5;
				npc->bits |= NPC_INTERACTABLE;
			}

			break;

		case 5:
			npc->xm = 0;
			npc->ani_no = 5;
			break;
	}

	npc->ym += 0x40;

	if (npc->xm > 0x400)
		npc->xm = 0x400;
	if (npc->xm < -0x400)
		npc->xm = -0x400;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Toroko in bubble (Custom)
void ActEntity388(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[4] = {
		{480, 0, 504, 24}, // Small Bubble
		{504, 0, 528, 24}, // Big Bubble
		{480, 24, 504, 48}, // Toroko in Bubble A
		{504, 24, 528, 48}, // Toroko in Bubble B
	};

	switch (npc->act_no)
	{
		case 0:
			int a;
			for (a = 0; a < NPC_MAX; ++a)
				if (gNPC[a].code_event == 1000)
					break;

			if (a == NPC_MAX)
				break;

			npc->tgt_x = gNPC[a].x;
			npc->tgt_y = gNPC[a].y;
			npc->count1 = a;

			unsigned char deg;
			deg = GetArktan(npc->x - npc->tgt_x, npc->y - npc->tgt_y);
			npc->xm = GetCos(deg) * 2;
			npc->ym = GetSin(deg) * 2;

			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (npc->x - (3 * 0x200) < npc->tgt_x && npc->x + (3 * 0x200) > npc->tgt_x && npc->y - (3 * 0x200) < npc->tgt_y && npc->y + (3 * 0x200) > npc->tgt_y)
			{
				npc->act_no = 2;
				npc->ani_no = 2;
				gNPC[npc->count1].cond = 0;
				PlaySoundObject(21, SOUND_MODE_PLAY);
			}

			break;

		case 2:
			npc->xm -= 0x20;
			npc->ym -= 0x20;

			if (npc->xm < -0x5FF)
				npc->xm = -0x5FF;
			if (npc->ym < -0x5FF)
				npc->ym = -0x5FF;

			if (npc->y < -8 * 0x200)
				npc->cond = 0;

			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 2) * customrect;
	npc->rect.bottom += (24 * 2) * customrect;
}

// King (Custom)
void ActEntity389(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	int i;

	RECT rcLeft[11] = {
		{528, 0, 544, 16}, //  Standing Still
		{544, 0, 560, 16}, //  Standing Still (blinking)
		{560, 0, 576, 16}, //  Hurt (Falling)
		{576, 0, 592, 16}, //  Downed
		{592, 0, 608, 16}, //  Walking A
		{528, 0, 544, 16}, //  Standing Still
		{608, 0, 624, 16}, //  Walking B
		{528, 0, 544, 16}, //  Standing Still
		{576, 0, 592, 16}, //  Downed
		{0, 0, 0, 0},       //  Dead
		{640, 0, 656, 16}, // Blade from NpcSym
	};

	RECT rcRight[11] = {
		{528, 16, 544, 32}, //  Standing Still
		{544, 16, 560, 32}, //  Standing Still (blinking)
		{560, 16, 576, 32}, //  Hurt (Falling)
		{576, 16, 592, 32}, //  Downed
		{592, 16, 608, 32}, //  Walking A
		{528, 16, 544, 32}, //  Standing Still
		{608, 16, 624, 32}, //  Walking B
		{528, 16, 544, 32}, //  Standing Still
		{576, 16, 592, 32}, //  Downed
		{0, 0, 0, 0},       //  Dead
		{640, 16, 656, 32}, // Blade from NpcSym
	};

	switch (npc->act_no)
	{
		case 0: // Stood
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2: // Blink
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 5: // Lying down
			npc->ani_no = 3;
			npc->xm = 0;
			break;

		case 6: // Being knocked-back
			npc->act_no = 7;
			npc->act_wait = 0;
			npc->ani_wait = 0;
			npc->ym = -0x400;
			// Fallthrough
		case 7:
			npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			// If touching ground, enter 'lying down' state (the `act_wait` check is probably
			// so he doesn't do it before he even leaves the ground in the first place)
			if (npc->act_wait++ != 0 && npc->flag & 8)
				npc->act_no = 5;

			break;

		case 8: // Walking
			npc->act_no = 9;
			npc->ani_no = 4;
			npc->ani_wait = 0;
			// Fallthrough
		case 9:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
				npc->ani_no = 4;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			break;

		case 10: // Running
			npc->act_no = 11;
			npc->ani_no = 4;
			npc->ani_wait = 0;
			// Fallthrough
		case 11:
			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 7)
				npc->ani_no = 4;

			if (npc->direct == 0)
				npc->xm = -0x400;
			else
				npc->xm = 0x400;

			break;

		case 20: // Spawn his sword, before entering his 'idle' state
			SetNpChar(NPC_KINGS_SWORD, 0, 0, 0, 0, 2, npc, 0x100);
			npc->ani_no = 0;
			npc->act_no = 0;
			break;

		case 30: // Flying through air after being attacked by Misery
			npc->act_no = 31;
			npc->act_wait = 0;
			npc->ani_wait = 0;
			npc->ym = 0;
			// Fallthrough
		case 31:
			npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x600;
			else
				npc->xm = 0x600;

			if (npc->flag & 1)
			{
				npc->direct = 2;
				npc->act_no = 7;
				npc->act_wait = 0;
				npc->ani_wait = 0;
				npc->ym = -0x400;
				npc->xm = 0x200;
				PlaySoundObject(71, SOUND_MODE_PLAY);
				SetDestroyNpChar(npc->x, npc->y, 0x800, 4);
			}

			break;

		case 40: // Dying
			npc->act_no = 42;
			npc->act_wait = 0;
			npc->ani_no = 8;
			PlaySoundObject(29, SOUND_MODE_PLAY);
			// Fallthrough
		case 42:
			if (++npc->ani_no > 9)
				npc->ani_no = 8;

			if (++npc->act_wait > 100)
			{
				for (i = 0; i < 4; ++i)
					SetNpChar(NPC_SMOKE, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);

				npc->act_no = 50;
				npc->surf = SURFACE_ID_NPC_SYM;
				npc->ani_no = 10;
			}

			break;

		case 60: // Leap (used to attack Balrog in the Sand Zone storehouse)
			npc->ani_no = 6;
			npc->act_no = 61;
			npc->ym = -0x5FF;
			npc->xm = 0x400;
			npc->count2 = 1;
			break;

		case 61: // Leap - part 2
			npc->ym += 0x40;

			if (npc->flag & 8)
			{
				npc->act_no = 0;
				npc->count2 = 0;
				npc->xm = 0;
			}

			break;
	}

	// Apply gravity and speed-caps during most states
	if (npc->act_no < 30 || npc->act_no >= 40)
	{
		npc->ym += 0x40;

		if (npc->xm > 0x400)
			npc->xm = 0x400;
		if (npc->xm < -0x400)
			npc->xm = -0x400;

		if (npc->ym > 0x5FF)
			npc->ym = 0x5FF;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Professor Booster (Custom)
void ActEntity390(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[7] = {
		{656, 0, 672, 16}, // Standing Still
		{672, 0, 688, 16}, // Standing Still (blinking)
		{688, 0, 704, 16}, // Walking A
		{656, 0, 672, 16}, // Standing Still
		{704, 0, 720, 16}, // Walking B
		{656, 0, 672, 16}, // Standing Still
		{720, 0, 736, 16}, // Turned Back
	};

	RECT rcRight[7] = {
		{656, 16, 672, 32}, // Standing Still
		{672, 16, 688, 32}, // Standing Still (blinking)
		{688, 16, 704, 32}, // Walking A
		{656, 16, 672, 32}, // Standing Still
		{704, 16, 720, 32}, // Walking B
		{656, 16, 672, 32}, // Standing Still
		{720, 16, 736, 32}, // Turned Back
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->x -= 1 * 0x200;
			else
				npc->x += 1 * 0x200;

			break;

		case 5:
			npc->ani_no = 6;
			break;

		case 30:
			npc->act_no = 31;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->hit.bottom = 16 * 0x200;
			npc->x -= 16 * 0x200;
			npc->y += 8 * 0x200;
			PlaySoundObject(29, SOUND_MODE_PLAY);
			// Fallthrough
		case 31:
			if (++npc->act_wait == 64)
			{
				npc->act_no = 32;
				npc->act_wait = 0;
			}

			break;

		case 32:
			if (++npc->act_wait > 20)
			{
				npc->act_no = 33;
				npc->ani_no = 1;
				npc->hit.bottom = 8 * 0x200;
			}

			break;

		case 33:
			if (npc->flag & 8)
			{
				npc->act_no = 34;
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

	if (npc->act_no == 31)
	{
		npc->rect.bottom = npc->rect.top + (npc->act_wait / 4);

		if (npc->act_wait / 2 % 2)
			++npc->rect.left;
	}

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Jenka (Custom)
void ActEntity391(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{752, 0, 768, 16}, // Frame 1
		{768, 0, 784, 16}, // Frame 2
	};

	RECT rcRight[2] = {
		{752, 16, 768, 32}, // Frame 1
		{768, 16, 784, 32}, // Frame 2
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Jenka (Collapsed, Custom)
void ActEntity392(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[1] = {
		{784, 0, 800, 16}
	};

	RECT rcRight[1] = {
		{784, 16, 800, 32}
	};

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Misery (Floating, Custom)
void ActEntity393(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[8] = {
		{800, 0, 816, 16}, // Floating Still
		{816, 0, 832, 16}, // Floating Still (blinking)
		{832, 0, 848, 16}, // Standing Still
		{848, 0, 864, 16}, // Standing Still (blinking)
		{864, 0, 880, 16}, // Magic Usage 1 (normal)
		{880, 0, 896, 16}, // Magic Usage 2 (white)
		{896, 0, 912, 16}, // Magic Usage 3 (blue)
		{864, 0, 880, 16}, // Magic Usage 4 (normal)
	};

	RECT rcRight[8] = {
		{800, 16, 816, 32}, // Floating Still
		{816, 16, 832, 32}, // Floating Still (blinking)
		{832, 16, 848, 32}, // Standing Still
		{848, 16, 864, 32}, // Standing Still (blinking)
		{864, 16, 880, 32}, // Magic Usage 1 (normal)
		{880, 16, 896, 32}, // Magic Usage 2 (white)
		{896, 16, 912, 32}, // Magic Usage 3 (blue)
		{864, 16, 880, 32}, // Magic Usage 4 (normal)
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			npc->ani_no = 0;
			PlaySoundObject(29, SOUND_MODE_PLAY);
			// Fallthrough
		case 1:
			npc->x = npc->tgt_x + (Random(-1, 1) * 0x200);

			if (++npc->act_wait == 0x20)
				npc->act_no = 10;

			break;

		case 10:
			npc->act_no = 11;
			npc->act_wait = 0;
			npc->ani_no = 0;
			npc->ym = 0x200;
			// Fallthrough

		case 11:
			if (npc->tgt_y < npc->y)
				npc->ym -= 0x10;
			if (npc->tgt_y > npc->y)
				npc->ym += 0x10;

			if (npc->ym > 0x100)
				npc->ym = 0x100;
			if (npc->ym < -0x100)
				npc->ym = -0x100;

			break;

		case 13:
			npc->ani_no = 1;

			npc->ym += 0x40;
			if (npc->ym > 0x5FF)
				npc->ym = 0x5FF;

			if (npc->flag & 8)
			{
				PlaySoundObject(23, SOUND_MODE_PLAY);
				npc->ym = 0;
				npc->act_no = 14;
				npc->bits |= NPC_IGNORE_SOLIDITY;
				npc->ani_no = 2;
			}

			break;

		case 15:
			npc->act_no = 16;
			npc->act_wait = 0;
			npc->ani_no = 4;
			// Fallthrough
		case 16:
			if (++npc->act_wait == 30)
			{
				PlaySoundObject(21, SOUND_MODE_PLAY);
				SetNpChar(66, npc->x, npc->y - (16 * 0x200), 0, 0, 0, npc, 0);
			}

			if (npc->act_wait == 50)
				npc->act_no = 14;

			break;

		case 20:
			npc->act_no = 21;
			npc->ani_no = 0;
			npc->ym = 0;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 21:
			npc->ym -= 0x20;

			if (npc->y < -8 * 0x200)
				npc->cond = 0;

			break;

		case 25:
			npc->act_no = 26;
			npc->act_wait = 0;
			npc->ani_no = 5;
			npc->ani_wait = 0;
			// Fallthrough
		case 26:
			if (++npc->ani_no > 7)
				npc->ani_no = 5;

			if (++npc->act_wait == 30)
			{
				PlaySoundObject(101, SOUND_MODE_PLAY);
				SetFlash(0, 0, FLASH_MODE_FLASH);
				npc->act_no = 27;
				npc->ani_no = 7;
			}

			break;

		case 27:
			if (++npc->act_wait == 50)
				npc->act_no = 14;

			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->act_no == 11)
	{
		if (npc->ani_wait != 0)
		{
			--npc->ani_wait;
			npc->ani_no = 1;
		}
		else
		{
			if (Random(0, 100) == 1)
				npc->ani_wait = 30;

			npc->ani_no = 0;
		}
	}

	if (npc->act_no == 14)
	{
		if (npc->ani_wait != 0)
		{
			--npc->ani_wait;
			npc->ani_no = 3;
		}
		else
		{
			if (Random(0, 100) == 1)
				npc->ani_wait = 30;

			npc->ani_no = 2;
		}
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 1 && npc->ani_wait < 32)
		npc->rect.bottom = (++npc->ani_wait / 2) + npc->rect.bottom - 16;

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Misery (Standing, Custom)
void ActEntity394(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[9] = {
		{800, 0, 816, 16}, // Floating Still
		{816, 0, 832, 16}, // Floating Still (blinking)
		{832, 0, 848, 16}, // Standing Still
		{848, 0, 864, 16}, // Standing Still (blinking)
		{864, 0, 880, 16}, // Magic Usage 1 (normal)
		{880, 0, 896, 16}, // Magic Usage 2 (white)
		{896, 0, 912, 16}, // Magic Usage 3 (blue)
		{864, 0, 880, 16}, // Magic Usage 4 (normal)
		{912, 0, 928, 16}, // Downed
	};

	RECT rcRight[9] = {
		{800, 16, 816, 32}, // Floating Still
		{816, 16, 832, 32}, // Floating Still (blinking)
		{832, 16, 848, 32}, // Standing Still
		{848, 16, 864, 32}, // Standing Still (blinking)
		{864, 16, 880, 32}, // Magic Usage 1 (normal)
		{880, 16, 896, 32}, // Magic Usage 2 (white)
		{896, 16, 912, 32}, // Magic Usage 3 (blue)
		{864, 16, 880, 32}, // Magic Usage 4 (normal)
		{912, 16, 928, 32}, // Downed
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 2;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 3;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 2;
			}

			break;

		case 15:
			npc->act_no = 16;
			npc->act_wait = 0;
			npc->ani_no = 4;
			// Fallthrough
		case 16:
			if (++npc->act_wait == 30)
			{
				PlaySoundObject(21, SOUND_MODE_PLAY);
				SetNpChar(66, npc->x, npc->y - (16 * 0x200), 0, 0, 0, npc, 0);
			}

			if (npc->act_wait == 50)
				npc->act_no = 14;

			break;

		case 20:
			npc->act_no = 21;
			npc->ani_no = 0;
			npc->ym = 0;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 21:
			npc->ym -= 0x20;

			if (npc->y < -8 * 0x200)
				npc->cond = 0;

			break;

		case 25:
			npc->act_no = 26;
			npc->act_wait = 0;
			npc->ani_no = 5;
			npc->ani_wait = 0;
			// Fallthrough
		case 26:
			if (++npc->ani_no > 7)
				npc->ani_no = 5;

			if (++npc->act_wait == 30)
			{
				PlaySoundObject(101, SOUND_MODE_PLAY);
				SetFlash(0, 0, FLASH_MODE_FLASH);
				npc->act_no = 27;
				npc->ani_no = 7;
			}

			break;

		case 27:
			if (++npc->act_wait == 50)
			{
				npc->act_no = 0;
				npc->ani_no = 0;
			}

			break;

		case 30:
			npc->act_no = 31;
			npc->ani_no = 3;
			npc->ani_wait = 0;
			// Fallthrough
		case 31:
			if (++npc->ani_wait > 10)
			{
				npc->act_no = 32;
				npc->ani_no = 4;
				npc->ani_wait = 0;
			}

			break;

		case 32:
			if (++npc->ani_wait > 100)
			{
				npc->act_no = 1;
				npc->ani_no = 2;
			}

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 0;
			// Fallthrough
		case 41:
			npc->ani_no = 4;

			switch (++npc->act_wait)
			{
			case 30:
			case 40:
			case 50:
				SetNpChar(11, npc->x + (8 * 0x200), npc->y - (8 * 0x200), 0x600, Random(-0x200, 0), 0, NULL, 0x100);
				PlaySoundObject(33, SOUND_MODE_PLAY);
				break;
			}

			if (npc->act_wait > 50)
				npc->act_no = 0;

			break;

		case 50:
			npc->ani_no = 8;
			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->act_no == 11)
	{
		if (npc->ani_wait != 0)
		{
			--npc->ani_wait;
			npc->ani_no = 1;
		}
		else
		{
			if (Random(0, 100) == 1)
				npc->ani_wait = 30;

			npc->ani_no = 0;
		}
	}

	if (npc->act_no == 14)
	{
		if (npc->ani_wait != 0)
		{
			--npc->ani_wait;
			npc->ani_no = 3;
		}
		else
		{
			if (Random(0, 100) == 1)
				npc->ani_wait = 30;

			npc->ani_no = 2;
		}
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Curly (Custom)
void ActEntity395(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[10] = {
		{928, 0, 944, 16}, // Standing Still
		{944, 0, 960, 16}, // Walking A
		{928, 0, 944, 16}, // Standing Still
		{960, 0, 976, 16}, // Walking B
		{928, 0, 944, 16}, // Standing Still
		{1104, 0, 1120, 16}, // Loss
		{1040, 0, 1056, 16}, // Turned Back
		{1088, 0, 1104, 16}, // Damaged
		{1072, 0, 1088, 16}, // Downed
		{976, 0, 992, 16}, // Looking Up
	};

	RECT rcRight[10] = {
		{928, 16, 944, 32}, // Standing Still
		{944, 16, 960, 32}, // Walking A
		{928, 16, 944, 32}, // Standing Still
		{960, 16, 976, 32}, // Walking B
		{928, 16, 944, 32}, // Standing Still
		{1104, 16, 1120, 32}, // Loss
		{1040, 16, 1056, 32}, // Turned Back
		{1088, 16, 1104, 32}, // Damaged
		{1072, 16, 1088, 32}, // Downed
		{976, 16, 992, 32}, // Looking Up
	};

	switch (npc->act_no)
	{
		case 0:
			if (npc->direct == 4)
			{
				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			npc->xm = 0;
			npc->ym += 0x40;
			break;

		case 3:
			if (0)
			{
				// There used to be an if-statement here that didn't do anything, but the compiler optimised it out.
				// We only know this was here because empty if-statements affect the assembly's register usage.
				// Since there's no code, we have no idea what the original condition actually was.
			}

			if (0)
			{
				// Another empty if-statement
			}

			npc->act_no = 4;
			npc->ani_no = 1;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			npc->ym += 0x40;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			break;

		case 5:
			npc->act_no = 6;
			npc->ani_no = 5;
			SetDestroyNpChar(npc->x, npc->y, npc->view.back, 8);
			break;

		case 6:
			if (0)
			{
				// There used to be an if-statement here that didn't do anything, but the compiler optimised it out.
				// We only know this was here because empty if-statements affect the assembly's register usage.
				// Since there's no code, we have no idea what the original condition actually was.
			}

			npc->ani_no = 5;
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->ani_wait = 0;

			if (npc->x > gMC.x)
				npc->direct = 0;
			else
				npc->direct = 2;

			// Fallthrough
		case 11:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			if (npc->direct == 0)
				npc->x -= 1 * 0x200;
			else
				npc->x += 1 * 0x200;

			if (gMC.x < npc->x + (20 * 0x200) && gMC.x > npc->x - (20 * 0x200))
				npc->act_no = 0;

			break;

		case 20:
			npc->xm = 0;
			npc->ani_no = 6;
			break;

		case 21:
			npc->xm = 0;
			npc->ani_no = 9;
			break;

		case 30:
			npc->act_no = 31;
			npc->act_wait = 0;
			npc->ym = -0x400;
			// Fallthrough
		case 31:
			npc->ani_no = 7;

			if (npc->direct == 0)
				npc->xm = 0x200;
			else
				npc->xm = -0x200;

			npc->ym += 0x40;

			if (npc->act_wait++ != 0 && npc->flag & 8)
				npc->act_no = 32;

			break;

		case 32:
			npc->ym += 0x40;
			npc->ani_no = 8;
			npc->xm = 0;
			break;

		case 70:
			npc->act_no = 71;
			npc->act_wait = 0;
			npc->ani_no = 1;
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

			if (npc->ani_no > 4)
				npc->ani_no = 1;

			break;
	}

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Curly AI (Custom)
void ActEntity396(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	int xx, yy;

	RECT rcLeft[11] = {
		{928, 0, 944, 16}, // Standing Still
		{944, 0, 960, 16}, // Walking A
		{928, 0, 944, 16}, // Standing Still
		{960, 0, 976, 16}, // Walking B
		{928, 0, 944, 16}, // Standing Still
		{976, 0, 992, 16}, // Looking Up
		{992, 0, 1008, 16}, // Walking A (Looking Up)
		{976, 0, 992, 16}, // Looking Up
		{1008, 0, 1024, 16}, // Walking B (Looking Up)
		{976, 0, 992, 16}, // Looking Up
		{1072, 0, 1088, 16}, // Downed
	};

	RECT rcRight[11] = {
		{928, 16, 944, 32}, // Standing Still
		{944, 16, 960, 32}, // Walking A
		{928, 16, 944, 32}, // Standing Still
		{960, 16, 976, 32}, // Walking B
		{928, 16, 944, 32}, // Standing Still
		{976, 16, 992, 32}, // Looking Up
		{992, 16, 1008, 32}, // Walking A (Looking Up)
		{976, 16, 992, 32}, // Looking Up
		{1008, 16, 1024, 32}, // Walking B (Looking Up)
		{976, 16, 992, 32}, // Looking Up
		{1072, 16, 1088, 32}, // Downed
	};

	if (npc->y < gMC.y - (10 * 0x10 * 0x200))
	{
		if (npc->y < 16 * 0x10 * 0x200)
		{
			npc->tgt_x = 320 * 0x10 * 0x200;
			npc->tgt_y = npc->y;
		}
		else
		{
			npc->tgt_x = 0;
			npc->tgt_y = npc->y;
		}
	}
	else
	{
		if (gCurlyShoot_wait != 0)
		{
			npc->tgt_x = gCurlyShoot_x;
			npc->tgt_y = gCurlyShoot_y;
		}
		else
		{
			npc->tgt_x = gMC.x;
			npc->tgt_y = gMC.y;
		}
	}

	if (npc->xm < 0 && npc->flag & 1)
		npc->xm = 0;
	if (npc->xm > 0 && npc->flag & 4)
		npc->xm = 0;

	switch (npc->act_no)
	{
		case 20:
			npc->x = gMC.x;
			npc->y = gMC.y;
			npc->act_no = 100;
			npc->ani_no = 0;
			SetNpChar(183, 0, 0, 0, 0, 0, npc, 0x100);

			if (GetNPCFlag(563))
				SetNpChar(182, 0, 0, 0, 0, 0, npc, 0x100);
			else
				SetNpChar(181, 0, 0, 0, 0, 0, npc, 0x100);

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 0;
			npc->ani_no = 10;
			// Fallthrough
		case 41:
			if (++npc->act_wait == 750)
			{
				npc->bits &= ~NPC_INTERACTABLE;
				npc->ani_no = 0;
			}

			if (npc->act_wait > 1000)
			{
				npc->act_no = 100;
				npc->ani_no = 0;
				SetNpChar(183, 0, 0, 0, 0, 0, npc, 0x100);

				if (GetNPCFlag(563))
					SetNpChar(182, 0, 0, 0, 0, 0, npc, 0x100);
				else
					SetNpChar(181, 0, 0, 0, 0, 0, npc, 0x100);
			}

			break;

		case 100:
			npc->ani_no = 0;
			npc->xm = (npc->xm * 7) / 8;
			npc->count1 = 0;

			if (npc->x > npc->tgt_x + (16 * 0x200))
			{
				npc->act_no = 200;
				npc->ani_no = 1;
				npc->direct = 0;
				npc->act_wait = Random(20, 60);
			}
			else if (npc->x < npc->tgt_x - (16 * 0x200))
			{
				npc->act_no = 300;
				npc->ani_no = 1;
				npc->direct = 2;
				npc->act_wait = Random(20, 60);
			}

			break;

		case 200:
			npc->xm -= 0x20;
			npc->direct = 0;

			if (npc->flag & 1)
				++npc->count1;
			else
				npc->count1 = 0;

			break;

		case 210:
			npc->xm -= 0x20;
			npc->direct = 0;

			if (npc->flag & 8)
				npc->act_no = 100;

			break;

		case 300:
			npc->xm += 0x20;
			npc->direct = 2;

			if (npc->flag & 4)
				++npc->count1;
			else
				npc->count1 = 0;

			break;

		case 310:
			npc->xm += 0x20;
			npc->direct = 2;

			if (npc->flag & 8)
				npc->act_no = 100;

			break;
	}

	if (gCurlyShoot_wait != 0)
		--gCurlyShoot_wait;

	if (gCurlyShoot_wait == 70)
		npc->count2 = 10;

	if (gCurlyShoot_wait == 60 && npc->flag & 8 && Random(0, 2))
	{
		npc->count1 = 0;
		npc->ym = -0x600;
		npc->ani_no = 1;
		PlaySoundObject(15, SOUND_MODE_PLAY);

		if (npc->x > npc->tgt_x)
			npc->act_no = 210;
		else
			npc->act_no = 310;
	}

	xx = npc->x - npc->tgt_x;
	yy = npc->y - npc->tgt_y;

	if (xx < 0)
		xx *= -1;

	if (npc->act_no == 100)
	{
		if (xx + (2 * 0x200) < yy)
			npc->ani_no = 5;
		else
			npc->ani_no = 0;
	}

	if (npc->act_no == 210 || npc->act_no == 310)
	{
		if (xx + (2 * 0x200) < yy)
			npc->ani_no = 6;
		else
			npc->ani_no = 1;
	}

	if (npc->act_no == 200 || npc->act_no == 300)
	{
		++npc->ani_wait;

		if (xx + (2 * 0x200) < yy)
			npc->ani_no = 6 + (npc->ani_wait / 4 % 4);
		else
			npc->ani_no = 1 + (npc->ani_wait / 4 % 4);

		if (npc->act_wait)
		{
			--npc->act_wait;

			// I assume this is what was intended
			if (npc->flag & 8 && npc->count1 > 10)
			{
				npc->count1 = 0;
				npc->ym = -0x600;
				npc->act_no += 10;
				npc->ani_no = 1;
				PlaySoundObject(15, SOUND_MODE_PLAY);
			}
		}
		else
		{
			npc->act_no = 100;
			npc->ani_no = 0;
		}
	}

	if (npc->act_no >= 100 && npc->act_no < 500)
	{
		if (npc->x < gMC.x - (80 * 0x200) || npc->x > gMC.x + (80 * 0x200))
		{
			if (npc->flag & 5)
				npc->ym += 0x200 / 32;
			else
				npc->ym += 0x200 / 10;
		}
		else
		{
			npc->ym += 0x200 / 10;
		}
	}

	if (npc->xm > 0x300)
		npc->xm = 0x300;
	if (npc->xm < -0x300)
		npc->xm = -0x300;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->act_no >= 100 && !(npc->flag & 8))
	{
		switch (npc->ani_no)
		{
		case 1000:
			break;

		default:
			if (xx + (2 * 0x200) < yy)
				npc->ani_no = 6;
			else
				npc->ani_no = 1;

			break;
		}
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Curly (Carried, Custom)
void ActEntity397(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft = { 1120, 16, 1136, 32 };
	RECT rcRight = { 1136, 16, 1152, 32 };

	switch (npc->act_no)
	{
		case 0:
			npc->bits &= ~NPC_INTERACTABLE;
			npc->act_no = 1;
			// Fallthrough
		case 1:
			if (gMC.direct == 0)
				npc->direct = 0;
			else
				npc->direct = 2;

			npc->y = gMC.y - (4 * 0x200);

			if (npc->direct == 0)
			{
				npc->x = gMC.x + (3 * 0x200);
				npc->rect = rcLeft;
			}
			else
			{
				npc->x = gMC.x - (3 * 0x200);
				npc->rect = rcRight;
			}

			if (gMC.ani_no % 2)
				++npc->rect.top;

			break;

		case 10:
			npc->act_no = 11;
			npc->xm = 0x40;
			npc->ym = -0x20;

			npc->rect = rcLeft;
			break;

		case 11:
			if (npc->y < (64 * 0x200))
				npc->ym = 0x20;

			npc->x += npc->xm;
			npc->y += npc->ym;
			break;

		case 20:
			VanishNpChar(npc);
			SetDestroyNpCharUp(npc->x, npc->y, 0x2000, 0x40);
			break;
	}

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Curly (Carried + Shooting, Custom)
void ActEntity398(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[3] = {
		{944, 0, 960, 16}, // Looking Left
		{976, 0, 992, 16}, // Looking Up
		{1024, 0, 1040, 16}, // Locking Down
	};

	RECT rcRight[3] = {
		{944, 16, 960, 32}, // Looking Right
		{976, 16, 992, 32}, // Looking Up
		{1024, 16, 1040, 32}, // Locking Down
	};

	if (npc->act_no == 0)
	{
		npc->act_no = 1;
		npc->x = gMC.x;
		npc->y = gMC.y;
		SetNpChar(321, 0, 0, 0, 0, 0, npc, 0x100);
	}

	if (gMC.flag & 8)
	{
		if (gMC.up)
		{
			npc->tgt_x = gMC.x;
			npc->tgt_y = gMC.y - (10 * 0x200);
			npc->ani_no = 1;
		}
		else
		{
			npc->ani_no = 0;

			if (gMC.direct == 0)
			{
				npc->tgt_x = gMC.x + (7 * 0x200);
				npc->tgt_y = gMC.y - (3 * 0x200);
			}
			else
			{
				npc->tgt_x = gMC.x - (7 * 0x200);
				npc->tgt_y = gMC.y - (3 * 0x200);
			}
		}
	}
	else
	{
		if (gMC.up)
		{
			npc->tgt_x = gMC.x;
			npc->tgt_y = gMC.y + (8 * 0x200);
			npc->ani_no = 2;
		}
		else if (gMC.down)
		{
			npc->tgt_x = gMC.x;
			npc->tgt_y = gMC.y - (8 * 0x200);
			npc->ani_no = 1;
		}
		else
		{
			npc->ani_no = 0;

			if (gMC.direct == 0)
			{
				npc->tgt_x = gMC.x + (7 * 0x200);
				npc->tgt_y = gMC.y - (3 * 0x200);
			}
			else
			{
				npc->tgt_x = gMC.x - (7 * 0x200);
				npc->tgt_y = gMC.y - (3 * 0x200);
			}
		}
	}

	npc->x += (npc->tgt_x - npc->x) / 2;
	npc->y += (npc->tgt_y - npc->y) / 2;

	if (gMC.ani_no % 2)
		npc->y -= 1 * 0x200;

	if (gMC.direct == 0)
		npc->rect = rcRight[npc->ani_no];
	else
		npc->rect = rcLeft[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Santa (Custom)
void ActEntity399(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[7] = {
		{1152, 0, 1168, 16}, // Standing Still
		{1168, 0, 1184, 16}, // Standing Still (blinking)
		{1184, 0, 1200, 16}, // Walking A
		{1152, 0, 1168, 16}, // Standing Still
		{1200, 0, 1216, 16}, // Walking B
		{1152, 0, 1168, 16}, // Standing Still
		{1216, 0, 1232, 16}, // Turned Back
	};

	RECT rcRight[7] = {
		{1152, 16, 1168, 32}, // Standing Still
		{1168, 16, 1184, 32}, // Standing Still (blinking)
		{1184, 16, 1200, 32}, // Walking A
		{1152, 16, 1168, 32}, // Standing Still
		{1200, 16, 1216, 32}, // Walking B
		{1152, 16, 1168, 32}, // Standing Still
		{1216, 16, 1232, 32}, // Turned Back
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (npc->x - (32 * 0x200) < gMC.x && npc->x + (32 * 0x200) > gMC.x && npc->y - (32 * 0x200) < gMC.y && npc->y + (16 * 0x200) > gMC.y)
			{
				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}
			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				npc->ani_no++;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->x -= 1 * 0x200;
			else
				npc->x += 1 * 0x200;

			break;

		case 5:
			npc->ani_no = 6;
			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Santa (Caged, Custom)
void ActEntity400(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{1152, 0, 1168, 16}, // Standing Still
		{1168, 0, 1184, 16}, // Standing Still (blinking)
	};

	RECT rcRight[2] = {
		{1152, 16, 1168, 32}, // Standing Still
		{1168, 16, 1184, 32}, // Standing Still (blinking)
	};

	switch (npc->act_no)
	{
		case 0:
			npc->x += 1 * 0x200;
			npc->y -= 2 * 0x200;
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 160) == 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 12)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	if (gMC.x < npc->x)
		npc->direct = 0;
	else
		npc->direct = 2;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Chako (Custom)
void ActEntity401(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[7] = {
		{1232, 0, 1248, 16}, // Standing Still
		{1248, 0, 1264, 16}, // Standing Still (blinking)
		{1264, 0, 1280, 16}, // Walking A
		{1232, 0, 1248, 16}, // Standing Still
		{1280, 0, 1296, 16}, // Walking B
		{1232, 0, 1248, 16}, // Standing Still
		{1296, 0, 1312, 16}, // Sleeping
	};

	RECT rcRight[7] = {
		{1232, 16, 1248, 32}, // Standing Still
		{1248, 16, 1264, 32}, // Standing Still (blinking)
		{1264, 16, 1280, 32}, // Walking A
		{1232, 16, 1248, 32}, // Standing Still
		{1280, 16, 1296, 32}, // Walking B
		{1232, 16, 1248, 32}, // Standing Still
		{1296, 0, 1312, 16}, // Sleeping
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (npc->x - (32 * 0x200) < gMC.x && npc->x + (32 * 0x200) > gMC.x && npc->y - (32 * 0x200) < gMC.y && npc->y + (16 * 0x200) > gMC.y)
			{
				if (npc->x > gMC.x)
					npc->direct = 0;
				else
					npc->direct = 2;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 3:
			npc->act_no = 4;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 4:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->x -= 1 * 0x200;
			else
				npc->x += 1 * 0x200;

			break;

		case 10:
			npc->ani_no = 6;

			if (++npc->act_wait > 200)
			{
				npc->act_wait = 0;
				SetCaret(npc->x, npc->y, CARET_ZZZ, DIR_LEFT);
			}

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Chako (Caged, Custom)
void ActEntity402(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{1232, 0, 1248, 16}, // Standing Still
		{1248, 0, 1264, 16}, // Standing Still (blinking)
	};

	RECT rcRight[2] = {
		{1232, 16, 1248, 32}, // Standing Still
		{1248, 16, 1264, 32}, // Standing Still (blinking)
	};

	switch (npc->act_no)
	{
	case 0:
		npc->x -= 1 * 0x200;
		npc->y -= 2 * 0x200;
		npc->act_no = 1;
		npc->ani_no = 0;
		npc->ani_wait = 0;
		// Fallthrough
	case 1:
		if (Random(0, 160) == 1)
		{
			npc->act_no = 2;
			npc->act_wait = 0;
			npc->ani_no = 1;
		}

		break;

	case 2:
		if (++npc->act_wait > 12)
		{
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;
	}

	if (gMC.x < npc->x)
		npc->direct = 0;
	else
		npc->direct = 2;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Jack (Custom)
void ActEntity403(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[6] = {
		{1312, 0, 1328, 16}, // Standing Still
		{1328, 0, 1344, 16}, // Standing Still (blinking)
		{1344, 0, 1360, 16}, // Walking A
		{1312, 0, 1328, 16}, // Standing Still
		{1360, 0, 1376, 16}, // Walking B
		{1312, 0, 1328, 16}, // Standing Still
	};

	RECT rcRight[6] = {
		{1312, 16, 1328, 32}, // Standing Still
		{1328, 16, 1344, 32}, // Standing Still (blinking)
		{1344, 16, 1360, 32}, // Walking A
		{1312, 16, 1328, 32}, // Standing Still
		{1360, 16, 1376, 32}, // Walking B
		{1312, 16, 1328, 32}, // Standing Still
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 8:
			npc->act_no = 9;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 9:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			break;
	}

	npc->ym += 0x40;

	if (npc->xm > 0x400)
		npc->xm = 0x400;
	if (npc->xm < -0x400)
		npc->xm = -0x400;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Mahin (Custom)
void ActEntity404(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[3] = {
		{1376, 0, 1392, 16}, // Standing Still
		{1392, 0, 1408, 16}, // Standing Still (blinking)
		{1408, 0, 1424, 16}, // Turned Back
	};

	RECT rcRight[3] = {
		{1376, 16, 1392, 32}, // Standing Still
		{1392, 16, 1408, 32}, // Standing Still (blinking)
		{1408, 16, 1424, 32}, // Turned Back
	};

	switch (npc->act_no)
	{
	case 0:
		npc->act_no = 1;
		npc->ani_no = 2;
		npc->ani_wait = 0;
		break;

	case 2:
		npc->ani_no = 0;

		if (Random(0, 120) == 10)
		{
			npc->act_no = 3;
			npc->act_wait = 0;
			npc->ani_no = 1;
		}

		if (npc->x - (32 * 0x200) < gMC.x && npc->x + (32 * 0x200) > gMC.x && npc->y - (32 * 0x200) < gMC.y && npc->y + (16 * 0x200) > gMC.y)
		{
			if (npc->x > gMC.x)
				npc->direct = 0;
			else
				npc->direct = 2;
		}

		break;

	case 3:
		if (++npc->act_wait > 8)
		{
			npc->act_no = 2;
			npc->ani_no = 0;
		}

		break;
	}

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Blue robot (Sitting, Custom)
void ActEntity405(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect = { 1456, 0, 1472, 16 };

	npc->rect = rect;

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Blue robot (Standing, Custom)
void ActEntity406(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{1424, 0, 1440, 16}, // Standing Still
		{1440, 0, 1456, 16}, // Standing Still (blinking)
	};

	RECT rcRight[2] = {
		{1424, 16, 1440, 32}, // Standing Still
		{1440, 16, 1456, 32}, // Standing Still (blinking)
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough

		case 1:
			if (Random(0, 100) == 0)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 16)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Blue robot (Broken, Custom)
void ActEntity407(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rc = { 1472, 0, 1504, 16 };

	if (npc->act_no == 0)
	{
		npc->act_no = 1;
		npc->y += 4 * 0x200;
	}

	npc->rect = rc;

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Broken robot (Custom)
void ActEntity408(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[2] = {
		{1504, 0, 1520, 16},
		{1520, 0, 1536, 16},
	};

	int i;

	switch (npc->act_no)
	{
		case 0:
			npc->ani_no = 0;
			break;

		case 10:
			PlaySoundObject(72, SOUND_MODE_PLAY);

			for (i = 0; i < 8; ++i)
				SetNpChar(4, npc->x, npc->y + (Random(-8, 8) * 0x200), Random(-8, -2) * 0x200, Random(-3, 3) * 0x200, 0, NULL, 0x100);

			npc->cond = 0;
			break;

		case 20:
			if (++npc->ani_wait > 10)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			break;
	}

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Kanpachi (Custom)
void ActEntity409(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcRight[7] = {
		{1552, 0, 1568, 24}, // Standing Still
		{1568, 0, 1584, 24}, // Standing Still (blinking)
		{1584, 0, 1600, 24}, // Walking A
		{1552, 0, 1568, 24}, // Standing Still
		{1600, 0, 1616, 24}, // Walking B
		{1552, 0, 1568, 24}, // Standing Still
		{1536, 0, 1552, 24}, // Turned Back
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 60) == 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			// Fallthrough
		case 11:
			npc->xm = 0x200;

			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			++npc->act_wait;

			break;

		case 20:
			npc->xm = 0;
			npc->ani_no = 6;
			break;
	}

	npc->ym += 0x20;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 2) * customrect;
	npc->rect.bottom += (24 * 2) * customrect;
}

// Kanpachi (Fishing, Custom)
void ActEntity410(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{1632, 0, 1656, 24}, // Eyes Closed
		{1656, 0, 1680, 24}, // Eyes Open
	};

	switch (npc->act_no)
	{
	case 0:
		npc->act_no = 1;
		npc->ani_no = 0;
		npc->ani_wait = 0;
		// Fallthrough
	case 1:
		if (npc->x - (48 * 0x200) < gMC.x && npc->x + (48 * 0x200) > gMC.x && npc->y - (48 * 0x200) < gMC.y && npc->y + (16 * 0x200) > gMC.y)
			npc->ani_no = 1;
		else
			npc->ani_no = 0;

		break;
	}

	npc->rect = rcLeft[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 2) * customrect;
	npc->rect.bottom += (24 * 2) * customrect;
}

// Dr Gero (Custom)
void ActEntity411(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{1680, 0, 1696, 16}, // Standing Still
		{1696, 0, 1712, 16}, // Standing Still (blinking)
	};

	RECT rcRight[2] = {
		{1680, 16, 1696, 32}, // Standing Still
		{1696, 16, 1712, 32}, // Standing Still (blinking)
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Nurse Hasumi (Custom)
void ActEntity412(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{1712, 0, 1728, 16}, // Standing Still
		{1728, 0, 1744, 16}, // Standing Still (blinking)
	};

	RECT rcRight[2] = {
		{1712, 16, 1728, 32}, // Standing Still
		{1728, 16, 1744, 32}, // Standing Still (blinking)
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Malco (Custom)
void ActEntity413(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcPoweron[10] = {
		{1744, 0, 1760, 24}, // Powered Off
		{1760, 0, 1776, 24}, // Powered On (Sitting)
		{1776, 0, 1792, 24}, // Powered On (Standing)
		{1792, 0, 1808, 24}, // Freak Out A
		{1808, 0, 1824, 24}, // Freak Out B
		{1824, 0, 1840, 24}, // Broken
		{1776, 0, 1792, 24}, // Powered On (Standing)
		{1792, 0, 1808, 24}, // Freak Out A
		{1808, 0, 1824, 24}, // Freak Out B
		{1792, 0, 1808, 24}, // Freak Out A
	};

	int i;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;

			if (npc->direct == 2)
				npc->ani_no = 5;

			break;

		case 10:
			npc->act_no = 11;
			npc->act_wait = 0;
			npc->ani_wait = 0;

			for (i = 0; i < 4; ++i)
				SetNpChar(4, npc->x, npc->y, Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);

			// Fallthrough
		case 11:
			if (++npc->ani_wait > 1)
			{
				PlaySoundObject(43, SOUND_MODE_PLAY);
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			if (++npc->act_wait > 100)
				npc->act_no = 12;

			break;

		case 12:
			npc->act_no = 13;
			npc->act_wait = 0;
			npc->ani_no = 1;
			// Fallthrough
		case 13:
			if (++npc->act_wait > 50)
				npc->act_no = 14;

			break;

		case 14:
			npc->act_no = 15;
			npc->act_wait = 0;
			// Fallthrough
		case 15:
			if (npc->act_wait / 2 % 2)
			{
				npc->x += 1 * 0x200;
				PlaySoundObject(11, SOUND_MODE_PLAY);
			}
			else
			{
				npc->x -= 1 * 0x200;
			}

			if (++npc->act_wait > 50)
				npc->act_no = 16;

			break;

		case 16:
			npc->act_no = 17;
			npc->act_wait = 0;
			npc->ani_no = 2;
			PlaySoundObject(12, SOUND_MODE_PLAY);

			for (i = 0; i < 8; ++i)
				SetNpChar(4, npc->x, npc->y, Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);

			// Fallthrough
		case 17:
			if (++npc->act_wait > 150)
				npc->act_no = 18;

			break;

		case 18:
			npc->act_no = 19;
			npc->act_wait = 0;
			npc->ani_no = 3;
			npc->ani_wait = 0;
			// Fallthrough
		case 19:
			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
			{
				PlaySoundObject(11, SOUND_MODE_PLAY);
				npc->ani_no = 3;
			}

			if (++npc->act_wait > 100)
			{
				npc->act_no = 20;
				PlaySoundObject(12, SOUND_MODE_PLAY);

				for (i = 0; i < 4; ++i)
					SetNpChar(4, npc->x, npc->y, Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);
			}

			break;

		case 20:
			npc->ani_no = 4;
			break;

		case 21:
			npc->act_no = 22;
			npc->ani_no = 5;
			PlaySoundObject(51, SOUND_MODE_PLAY);
			break;

		case 100:
			npc->act_no = 101;
			npc->ani_no = 6;
			npc->ani_wait = 0;
			// Fallthrough
		case 101:
			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 9)
				npc->ani_no = 6;

			break;

		case 110:
			SetDestroyNpChar(npc->x, npc->y, 0x2000, 16);
			npc->cond = 0;
			break;
	}

	npc->rect = rcPoweron[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 2) * customrect;
	npc->rect.bottom += (24 * 2) * customrect;
}

// Malco (Broken, Custom)
void ActEntity414(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	int i;

	RECT rcLeft[2] = {
		{1840, 0, 1856, 24}, // Standing Still
		{1856, 0, 1872, 24}, // Standing Still (blinking)
	};

	RECT rcRight[2] = {
		{1840, 24, 1856, 48}, // Standing Still
		{1856, 24, 1872, 48}, // Standing Still (blinking)
	};

	switch (npc->act_no)
	{
	case 0:
		if (--npc->act_wait != 0)
			npc->act_no = 1;

		npc->ani_no = 0;
		npc->ani_wait = 0;
		// Fallthrough
	case 1:
		if (Random(0, 120) == 10)
		{
			npc->act_no = 2;
			npc->act_wait = 0;
			npc->ani_no = 1;
		}

		if (npc->x - (32 * 0x200) < gMC.x && npc->x + (32 * 0x200) > gMC.x && npc->y - (32 * 0x200) < gMC.y && npc->y + (16 * 0x200) > gMC.y)
		{
			if (npc->x > gMC.x)
				npc->direct = 0;
			else
				npc->direct = 2;
		}

		break;

	case 2:
		if (++npc->act_wait > 8)
		{
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;

	case 10:
		npc->act_no = 0;
		PlaySoundObject(12, SOUND_MODE_PLAY);

		for (i = 0; i < 8; ++i)
			SetNpChar(4, npc->x, npc->y, Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);

		break;
	}

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 2) * customrect;
	npc->rect.bottom += (24 * 2) * customrect;
}

// Cthulhu (Custom)
void ActEntity415(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{1872, 0, 1888, 24}, // Standing Still
		{1888, 0, 1904, 24}, // Standing Still (blinking)
	};

	RECT rcRight[2] = {
		{1872, 24, 1888, 48}, // Standing Still
		{1888, 24, 1904, 48}, // Standing Still (blinking)
	};

	switch (npc->act_no)
	{
	case 0:
		npc->act_no = 1;
		npc->ani_no = 0;
		npc->ani_wait = 0;
		// Fallthrough
	case 1:
		if (npc->x - (48 * 0x200) < gMC.x && npc->x + (48 * 0x200) > gMC.x && npc->y - (48 * 0x200) < gMC.y && npc->y + (16 * 0x200) > gMC.y)
			npc->ani_no = 1;
		else
			npc->ani_no = 0;

		break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 2) * customrect;
	npc->rect.bottom += (24 * 2) * customrect;
}

// Itoh (Custom)
void ActEntity416(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[8] = {
		{1904, 0, 1920, 16}, // Standing Still
		{1920, 0, 1936, 16}, // Standing Still (blinking)
		{1936, 0, 1952, 16}, // Jump Back
		{1952, 0, 1968, 16}, // Fear
		{1904, 16, 1920, 32}, // Standing Still [ RIGHT ]
		{1920, 16, 1936, 32}, // Walking A [ RIGHT ]
		{1904, 16, 1920, 32}, // Standing Still [ RIGHT ]
		{1936, 0, 1952, 16}, // Walking B [ RIGHT ]
	};

	switch (npc->act_no)
	{
	case 0:
		npc->act_no = 1;
		npc->ani_no = 0;
		npc->ani_wait = 0;
		npc->xm = 0;
		// Fallthrough
	case 1:
		if (Random(0, 120) == 10)
		{
			npc->act_no = 2;
			npc->act_wait = 0;
			npc->ani_no = 1;
		}

		break;

	case 2:
		if (++npc->act_wait > 8)
		{
			npc->act_no = 1;
			npc->ani_no = 0;
		}

		break;

	case 10:
		npc->ani_no = 2;
		npc->xm = 0;
		break;

	case 20:
		npc->act_no = 21;
		npc->ani_no = 2;
		npc->xm += 0x200;
		npc->ym -= 0x400;
		break;

	case 21:
		if (npc->flag & 8)
		{
			npc->ani_no = 3;
			npc->act_no = 30;
			npc->act_wait = 0;
			npc->xm = 0;
			npc->tgt_x = npc->x;
		}

		break;

	case 30:
		npc->ani_no = 3;

		if (++npc->act_wait / 2 % 2)
			npc->x = npc->tgt_x + (1 * 0x200);
		else
			npc->x = npc->tgt_x;

		break;

	case 40:
		npc->act_no = 41;
		npc->ym = -0x200;
		npc->ani_no = 2;
		// Fallthrough
	case 41:
		if (npc->flag & 8)
		{
			npc->act_no = 42;
			npc->ani_no = 4;
		}

		break;

	case 42:
		npc->xm = 0;
		npc->ani_no = 4;
		break;

	case 50:
		npc->act_no = 51;
		npc->act_wait = 0;
		// Fallthrough
	case 51:
		if (++npc->act_wait > 32)
			npc->act_no = 42;

		npc->xm = 0x200;

		if (++npc->ani_wait > 3)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 7)
			npc->ani_no = 4;

		break;
	}

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Demon Crown (Opening, Custom)
void ActEntity417(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rc = { 1968, 0, 1984, 16 };

	if (npc->act_no == 0)
	{
		npc->act_no = 1;
		npc->y += 6 * 0x200;
	}

	if (++npc->ani_wait % 8 == 1)
		SetCaret(npc->x + (Random(-8, 8) * 0x200), npc->y + (8 * 0x200), CARET_TINY_PARTICLES, DIR_UP);

	npc->rect = rc;

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Sky Dragon (Custom)
void ActEntity418(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcRight[4] = {
		{2000, 0, 2040, 40}, // Standing Still
		{2040, 0, 2080, 40}, // Standing Still (idle 2)
		{2080, 0, 2120, 40}, // Flying w/ Quote + Kazuma (frame 1)
		{2120, 0, 2160, 40}, // Flying w/ Quote + Kazuma (frame 2)
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->y -= 4 * 0x200;
			// Fallthrough
		case 1:
			if (++npc->ani_wait > 30)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
				npc->ani_no = 0;

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 2;
			npc->ani_wait = 0;
			npc->tgt_y = npc->y - (16 * 0x200);
			npc->tgt_x = npc->x - (6 * 0x200);
			npc->ym = 0;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 11:
			if (npc->x < npc->tgt_x)
				npc->xm += 8;
			else
				npc->xm -= 8;

			if (npc->y < npc->tgt_y)
				npc->ym += 8;
			else
				npc->ym -= 8;

			npc->x += npc->xm;
			npc->y += npc->ym;

			if (++npc->ani_wait > 5)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;

			break;

		case 20:
			npc->act_no = 21;
			npc->bits |= NPC_IGNORE_SOLIDITY;
			// Fallthrough
		case 21:
			if (npc->y < npc->tgt_y)
				npc->ym += 0x10;
			else
				npc->ym -= 0x10;

			npc->xm += 0x20;

			if (npc->xm > 0x600)
				npc->xm = 0x600;
			if (npc->xm < -0x600)
				npc->xm = -0x600;

			npc->x += npc->xm;
			npc->y += npc->ym;

			if (++npc->ani_wait > 2)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 3)
				npc->ani_no = 2;

			break;

		case 30:
			npc->act_no = 31;
			SetNpChar(297, 0, 0, 0, 0, 0, npc, 0x100);
			break;
	}

	npc->rect = rcRight[npc->ani_no];

	// Use different sprite if player is wearing the Mimiga Mask
	if (gMC.equip & EQUIP_MIMIGA_MASK)
	{
		if (npc->ani_no > 1)
		{
			npc->rect.top += 40;
			npc->rect.bottom += 40;
		}
	}

	// If the Unused / Custom NPC flag is checked, the dragon will add on rects per each <MIM !!
	if (npc->bits & NPC_CUSTOM_FLAG)
	{
		if (npc->ani_no > 1)
		{
			npc->rect.top += 32 * gCurrentMim;
			npc->rect.bottom += 32 * gCurrentMim;
		}
	}

	// Custom Rect
	npc->rect.top += (40 * 2) * customrect;
	npc->rect.bottom += (40 * 2) * customrect;
}

// Scooter (Custom)
void ActEntity419(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{2176, 0, 2208, 16}, // Default
		{2208, 0, 2240, 32}, // Kazuma + Booster
	};

	RECT rcRight[2] = {
		{2176, 16, 2208, 32}, // Default
		{2240, 0, 2272, 32}, // Kazuma + Booster
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->view.back = 16 * 0x200;
			npc->view.front = 16 * 0x200;
			npc->view.top = 8 * 0x200;
			npc->view.bottom = 8 * 0x200;
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->view.top = 16 * 0x200;
			npc->view.bottom = 16 * 0x200;
			npc->y -= 5 * 0x200;
			break;

		case 20:
			npc->act_no = 21;
			npc->act_wait = 1;
			npc->tgt_x = npc->x;
			npc->tgt_y = npc->y;
			// Fallthrough
		case 21:
			npc->x = npc->tgt_x + (Random(-1, 1) * 0x200);
			npc->y = npc->tgt_y + (Random(-1, 1) * 0x200);

			if (++npc->act_wait > 30)
				npc->act_no = 30;

			break;

		case 30:
			npc->act_no = 31;
			npc->act_wait = 1;
			npc->xm = -0x800;
			npc->x = npc->tgt_x;
			npc->y = npc->tgt_y;
			PlaySoundObject(44, SOUND_MODE_PLAY);
			// Fallthrough
		case 31:
			npc->xm += 0x20;
			npc->x += npc->xm;
			++npc->act_wait;
			npc->y = npc->tgt_y + (Random(-1, 1) * 0x200);

			if (npc->act_wait > 10)
				npc->direct = 2;

			if (npc->act_wait > 200)
				npc->act_no = 40;

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 2;
			npc->direct = 0;
			npc->y -= 48 * 0x200;
			npc->xm = -0x1000;
			// Fallthrough
		case 41:
			npc->x += npc->xm;
			npc->y += npc->ym;

			npc->act_wait += 2;

			if (npc->act_wait > 1200)
				npc->cond = 0;

			break;
	}

	if (npc->act_wait % 4 == 0 && npc->act_no >= 20)
	{
		PlaySoundObject(34, SOUND_MODE_PLAY);

		if (npc->direct == 0)
			SetCaret(npc->x + (10 * 0x200), npc->y + (10 * 0x200), CARET_EXHAUST, DIR_RIGHT);
		else
			SetCaret(npc->x - (10 * 0x200), npc->y + (10 * 0x200), CARET_EXHAUST, DIR_LEFT);
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Scooter (Broken, Custom)
void ActEntity420(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rc = { 2272, 0, 2336, 16 };

	switch (npc->act_no)
	{
	case 0:
		npc->act_no = 1;
		npc->y = npc->y;	// This line probably isn't accurate to the original source code, but it produces the same assembly
		npc->x += 24 * 0x200;
		break;
	}

	npc->rect = rc;

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Doctor (Custom)
void ActEntity421(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[3] = {
		{2352, 0, 2376, 32}, // Standing Still (idle 1)
		{2376, 0, 2400, 32}, // Standing Still (idle 2)
		{2400, 0, 2424, 32}, // Floating
	};

	RECT rcRight[3] = {
		{2352, 32, 2376, 64}, // Standing Still (idle 1)
		{2376, 32, 2400, 64}, // Standing Still (idle 2)
		{2400, 32, 2424, 64}, // Floating
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->xm = 0;
			npc->ym = 0;
			npc->y -= 8 * 0x200;
			// Fallthrough
		case 1:
			if (npc->flag & 8)
				npc->ani_no = 0;
			else
				npc->ani_no = 2;

			npc->ym += 0x40;
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 1;
			npc->ani_wait = 0;
			npc->count1 = 0;
			// Fallthrough
		case 11:
			if (++npc->ani_wait > 6)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
			{
				npc->ani_no = 0;
				++npc->count1;
			}

			if (npc->count1 > 8)
			{
				npc->ani_no = 0;
				npc->act_no = 1;
			}

			break;

		case 20:
			npc->act_no = 21;
			npc->act_wait = 0;
			npc->ani_no = 2;
			npc->tgt_y = npc->y - (32 * 0x200);
			// Fallthrough
		case 21:
			if (npc->y < npc->tgt_y)
				npc->ym += 0x20;
			else
				npc->ym -= 0x20;

			if (npc->ym > 0x200)
				npc->ym = 0x200;
			if (npc->ym < -0x200)
				npc->ym = -0x200;

			break;

		case 30:
			npc->act_no = 31;
			npc->xm = 0;
			npc->ym = 0;
			npc->act_wait = (npc->rect.bottom - npc->rect.top) * 2;
			PlaySoundObject(29, SOUND_MODE_PLAY);
			// Fallthrough
		case 31:
			--npc->act_wait;
			npc->ani_no = 0;

			if (npc->act_wait == 0)
				npc->cond = 0;

			break;

		case 40:
			npc->act_no = 41;
			npc->act_wait = 0;
			npc->xm = 0;
			npc->ym = 0;
			PlaySoundObject(29, SOUND_MODE_PLAY);
			// Fallthrough
		case 41:
			npc->ani_no = 2;

			if (++npc->act_wait < 64)
				break;

			npc->act_no = 20;
			break;
	}

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (npc->act_no == 31 || npc->act_no == 41)
	{
		npc->rect.bottom = npc->rect.top + (npc->act_wait / 2);

		if (npc->act_wait / 2 % 2)
			++npc->rect.left;
	}

	// Custom Rect
	npc->rect.top += (32 * 2) * customrect;
	npc->rect.bottom += (32 * 2) * customrect;
}

// Doctor (Facing away, Custom)
void ActEntity422(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[6] = {
		{2424, 0, 2448, 32}, // Turned Back A
		{2448, 0, 2472, 32}, // Turned Back B
		{2352, 0, 2376, 32}, // Standing Still A
		{2376, 0, 2400, 32}, // Standing Still B
		{2352, 32, 2376, 64}, // Standing Still (Hand Out A)
		{2376, 32, 2400, 64}, // Standing Still (Hand Out B)
	};

	switch (npc->act_no)
	{
		case 0:
			gSuperXpos = 0;
			npc->act_no = 1;
			npc->y -= 8 * 0x200;
			// Fallthrough
		case 1:
			npc->ani_no = 0;
			break;

		case 10:
			npc->act_no = 11;
			npc->ani_wait = 0;
			npc->ani_no = 0;
			npc->count1 = 0;
			// Fallthrough
		case 11:
			if (++npc->ani_wait > 5)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 1)
			{
				npc->ani_no = 0;
				++npc->count1;
			}

			if (npc->count1 > 5)
				npc->act_no = 1;

			break;

		case 20:
			npc->act_no = 21;
			// Fallthrough
		case 21:
			npc->ani_no = 2;
			break;

		case 40:
			npc->act_no = 41;
			SetNpChar(257, npc->x - (14 * 0x200), npc->y - (16 * 0x200), 0, 0, 0, NULL, 0x100);
			SetNpChar(257, npc->x - (14 * 0x200), npc->y - (16 * 0x200), 0, 0, 2, NULL, 0xAA);
			// Fallthrough
		case 41:
			npc->ani_no = 4;
			break;

		case 50:
			npc->act_no = 51;
			npc->ani_wait = 0;
			npc->ani_no = 4;
			npc->count1 = 0;
			// Fallthrough
		case 51:
			if (++npc->ani_wait > 5)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
			{
				npc->ani_no = 4;
				++npc->count1;
			}

			if (npc->count1 > 5)
				npc->act_no = 41;

			break;
	}

	npc->rect = rcLeft[npc->ani_no];

	// Custom Rect
	npc->rect.top += (32 * 2) * customrect;
	npc->rect.bottom += (32 * 2) * customrect;
}

// Doctor (Opening, Custom)
void ActEntity423(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rc[8] = {
		{2480, 0, 2496, 32}, // Standing Still A
		{2496, 0, 2512, 32}, // Standing Still B
		{2512, 0, 2528, 32}, // Walking A
		{2480, 0, 2496, 32}, // Standing Still A
		{2528, 0, 2544, 32}, // Walking B
		{2480, 0, 2496, 32}, // Standing Still A
		{2544, 0, 2560, 32}, // Turned Back A
		{2560, 0, 2576, 32}, // Turned Back B
	};

	switch (npc->act_no)
	{
	case 0:
		npc->act_no = 1;
		npc->y -= 8 * 0x200;
		// Fallthrough

	case 1:
		npc->ani_no = 0;
		break;

	case 10:
		npc->act_no = 11;
		npc->ani_no = 0;
		npc->ani_wait = 0;
		npc->count1 = 0;
		// Fallthrough

	case 11:
		if (++npc->ani_wait > 6)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 1)
		{
			npc->ani_no = 0;

			if (++npc->count1 > 7)
			{
				npc->ani_no = 0;
				npc->act_no = 1;
			}
		}

		break;

	case 20:
		npc->act_no = 21;
		npc->ani_no = 2;
		npc->ani_wait = 0;
		// Fallthrough

	case 21:
		if (++npc->ani_wait > 10)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 5)
			npc->ani_no = 2;

		npc->x += 0x100;

		break;

	case 30:
		npc->ani_no = 6;
		break;

	case 40:
		npc->act_no = 41;
		npc->ani_no = 6;
		npc->ani_wait = 0;
		npc->count1 = 0;
		// Fallthrough

	case 41:
		if (++npc->ani_wait > 6)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 7)
		{
			npc->ani_no = 6;

			if (++npc->count1 > 7)
			{
				npc->ani_no = 6;
				npc->act_no = 30;
			}
		}

		break;
	}

	npc->rect = rc[npc->ani_no];

	// Custom Rect
	npc->rect.top += (32 * 2) * customrect;
	npc->rect.bottom += (32 * 2) * customrect;
}

// Mimiga (Sleeping, Custom)
void ActEntity424(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rc = { 2576, 0, 2592, 16 };
	npc->rect = rc;

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Mimiga (Jailed, Custom)
void ActEntity425(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[6] = {
		{2592, 0, 2608, 16}, // Standing Still
		{2608, 0, 2624, 16}, // Standing Still (blinking)
		{2624, 0, 2640, 16}, // Walking A
		{2592, 0, 2608, 16}, // Standing Still
		{2640, 0, 2656, 16}, // Walking B
		{2592, 0, 2608, 16}, // Standing Still
	};

	RECT rcRight[6] = {
		{2592, 16, 2608, 32}, // Standing Still
		{2608, 16, 2624, 32}, // Standing Still (blinking)
		{2624, 16, 2640, 32}, // Walking A
		{2592, 16, 2608, 32}, // Standing Still
		{2640, 16, 2656, 32}, // Walking B
		{2592, 16, 2608, 32}, // Standing Still
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 60) == 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (Random(0, 60) == 1)
			{
				npc->act_no = 10;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 10:
			npc->act_no = 11;
			npc->act_wait = Random(0, 16);
			npc->ani_no = 2;
			npc->ani_wait = 0;

			if (Random(0, 9) % 2)
				npc->direct = 0;
			else
				npc->direct = 2;
			// Fallthrough
		case 11:
			if (npc->direct == 0 && npc->flag & 1)
				npc->direct = 2;
			else if (npc->direct == 2 && npc->flag & 4)
				npc->direct = 0;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (++npc->act_wait > 32)
				npc->act_no = 0;

			break;
	}

	npc->ym += 0x20;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Shovel Brigade (Custom)
void ActEntity426(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[2] = {
		{2656, 0, 2672, 16}, // Standing Still
		{2672, 0, 2688, 16}, // Standing Still (blinking)
	};

	RECT rcRight[2] = {
		{2656, 16, 2672, 32}, // Standing Still
		{2672, 16, 2688, 32}, // Standing Still (blinking)
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 120) == 10)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Shovel Brigade (Walking, Custom)
void ActEntity427(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[6] = {
		{2656, 0, 2672, 16}, // Standing Still
		{2672, 0, 2688, 16}, // Standing Still (blinking)
		{2688, 0, 2704, 16}, // Walking A
		{2656, 0, 2672, 16}, // Standing Still
		{2704, 0, 2720, 16}, // Walking B
		{2656, 0, 2672, 16}, // Standing Still
	};

	RECT rcRight[6] = {
		{2656, 16, 2672, 32}, // Standing Still
		{2672, 16, 2688, 32}, // Standing Still (blinking)
		{2688, 16, 2704, 32}, // Walking A
		{2656, 16, 2672, 32}, // Standing Still
		{2704, 16, 2720, 32}, // Walking B
		{2656, 16, 2672, 32}, // Standing Still
	};

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			npc->xm = 0;
			// Fallthrough
		case 1:
			if (Random(0, 60) == 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			if (Random(0, 60) == 1)
			{
				npc->act_no = 10;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 8)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 10:
			npc->act_no = 11;
			npc->act_wait = Random(0, 16);
			npc->ani_no = 2;
			npc->ani_wait = 0;

			if (Random(0, 9) % 2)
				npc->direct = 0;
			else
				npc->direct = 2;
			// Fallthrough
		case 11:
			if (npc->direct == 0 && npc->flag & 1)
				npc->direct = 2;
			else if (npc->direct == 2 && npc->flag & 4)
				npc->direct = 0;

			if (npc->direct == 0)
				npc->xm = -0x200;
			else
				npc->xm = 0x200;

			if (++npc->ani_wait > 4)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 5)
				npc->ani_no = 2;

			if (++npc->act_wait > 32)
				npc->act_no = 0;

			break;
	}

	npc->ym += 0x20;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->x += npc->xm;
	npc->y += npc->ym;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Shovel Brigade (Caged, Custom)
void ActEntity428(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[3] = {
		{2720, 0, 2736, 16}, // Standing Still
		{2736, 0, 2752, 16}, // Standing Still (blinking)
		{2752, 0, 2768, 16}, // Standing Still (blushing)
	};

	RECT rcRight[3] = {
		{2720, 16, 2736, 32}, // Standing Still
		{2736, 16, 2752, 32}, // Standing Still (blinking)
		{2752, 16, 2768, 32}, // Standing Still (blushing)
	};

	switch (npc->act_no)
	{
		case 0:
			npc->x += 0x200;
			npc->y -= 0x400;
			npc->act_no = 1;
			npc->ani_no = 0;
			npc->ani_wait = 0;
			// Fallthrough
		case 1:
			if (Random(0, 160) == 1)
			{
				npc->act_no = 2;
				npc->act_wait = 0;
				npc->ani_no = 1;
			}

			break;

		case 2:
			if (++npc->act_wait > 12)
			{
				npc->act_no = 1;
				npc->ani_no = 0;
			}

			break;

		case 10:
			npc->act_no = 11;
			npc->ani_no = 2;
			SetNpChar(87, npc->x, npc->y - (16 * 0x200), 0, 0, 0, NULL, 0x100);

			break;
	}

	if (gMC.x < npc->x)
		npc->direct = 0;
	else
		npc->direct = 2;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 2) * customrect;
	npc->rect.bottom += (16 * 2) * customrect;
}

// Customizable Object Textures //

// Forcefield (Custom)
void ActEntity429(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[4] = {
		{0, 0, 16, 16},
		{16, 0, 32, 16},
		{32, 0, 48, 16},
		{48, 0, 64, 16},
	};

	if (++npc->ani_wait > 0)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 3)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Santa's Key (Custom)
void ActEntity430(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[3] = {
		{64, 0, 80, 16},
		{80, 0, 96, 16},
		{96, 0, 112, 16},
	};

	int i;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;

			if (npc->direct == 2)
			{
				npc->ym = -0x200;

				for (i = 0; i < 4; ++i)
					SetNpChar(4, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);
			}

			break;
	}

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Chest (Closed, Custom)
void ActEntity431(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[3] = {
		{112, 0, 128, 16},
		{128, 0, 144, 16},
		{144, 0, 160, 16},
	};

	int i;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;
			npc->bits |= NPC_INTERACTABLE;

			if (npc->direct == 2)
			{
				npc->ym = -0x200;

				for (i = 0; i < 4; ++i)
					SetNpChar(4, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);
			}

			// Fallthrough
		case 1:
			npc->ani_no = 0;

			if (Random(0, 30) == 0)
				npc->act_no = 2;

			break;

		case 2:
			if (++npc->ani_wait > 1)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 2)
			{
				npc->ani_no = 0;
				npc->act_no = 1;
			}

			break;
	}

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	npc->rect = rcLeft[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Save Point (Custom)
void ActEntity432(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[8] = {
		{160, 0, 176, 16},
		{176, 0, 192, 16},
		{192, 0, 208, 16},
		{208, 0, 224, 16},
		{224, 0, 240, 16},
		{240, 0, 256, 16},
		{256, 0, 272, 16},
		{272, 0, 288, 16},
	};

	int i;

	switch (npc->act_no)
	{
		case 0:
			npc->bits |= NPC_INTERACTABLE;
			npc->act_no = 1;

			if (npc->direct == 2)
			{
				npc->bits &= ~NPC_INTERACTABLE;
				npc->ym = -0x200;

				for (i = 0; i < 4; ++i)
					SetNpChar(4, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);
			}

			// Fallthrough
		case 1:
			if (npc->flag & 8)
				npc->bits |= NPC_INTERACTABLE;

			break;
	}

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 7)
		npc->ani_no = 0;

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Health refill (Custom)
void ActEntity433(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[2] = {
		{288, 0, 304, 16},
		{304, 0, 320, 16},
	};

	int a;

	switch (npc->act_no)
	{
		case 0:
			npc->act_no = 1;

			if (npc->direct == 2)
			{
				npc->ym = -0x200;

				for (a = 0; a < 4; ++a)
					SetNpChar(4, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);
			}

			// Fallthrough
		case 1:
			a = Random(0, 30);

			if (a < 10)
				npc->act_no = 2;
			else if (a < 25)
				npc->act_no = 3;
			else
				npc->act_no = 4;

			npc->act_wait = Random(0x10, 0x40);
			npc->ani_wait = 0;
			break;

		case 2:
			npc->rect = rect[0];

			if (--npc->act_wait == 0)
				npc->act_no = 1;

			break;

		case 3:
			if (++npc->ani_wait % 2)
				npc->rect = rect[0];
			else
				npc->rect = rect[1];

			if (--npc->act_wait == 0)
				npc->act_no = 1;

			break;

		case 4:
			npc->rect = rect[1];

			if (--npc->act_wait == 0)
				npc->act_no = 1;

			break;
	}

	npc->ym += 0x40;

	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Door (Custom)
void ActEntity434(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	int i;

	RECT rect[2] = {
		{320, 0, 336, 24},
		{336, 0, 352, 24},
	};

	switch (npc->act_no)
	{
		case 0:
			if (npc->direct == 0)
				npc->rect = rect[0];
			else
				npc->rect = rect[1];

			break;

		case 1:
			for (i = 0; i < 4; ++i)
				SetNpChar(4, npc->x, npc->y, Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);

			npc->act_no = 0;
			npc->rect = rect[0];
			break;
	}

	// Custom Rect
	npc->rect.top += (24 * 1) * customrect;
	npc->rect.bottom += (24 * 1) * customrect;
}

// Chest (Open, Custom)
void ActEntity435(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	if (npc->act_no == 0)
	{
		npc->act_no = 1;

		if (npc->direct == 2)
			npc->y += 16 * 0x200;
	}

	RECT rect = { 352, 0, 368, 8 };

	npc->rect = rect;

	// Custom Rect
	npc->rect.top += (8 * 1) * customrect;
	npc->rect.bottom += (8 * 1) * customrect;
}

// Computer (Custom)
void ActEntity436(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft = { 384, 0, 416, 24 };

	RECT rcRight[3] = {
		{416, 0, 448, 24},
		{416, 0, 448, 24},
		{448, 0, 480, 24},
	};

	if (++npc->ani_wait > 3)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	if (npc->direct == 0)
		npc->rect = rcLeft;
	else
		npc->rect = rcRight[npc->ani_no];
}

// Teleporter (Custom)
void ActEntity437(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[2] = {
		{504, 0, 528, 32},
		{528, 0, 552, 32},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->ani_no = 0;
			break;

		case 1:
			if (++npc->ani_no > 1)
				npc->ani_no = 0;

			break;
	}

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (32 * 1) * customrect;
	npc->rect.bottom += (32 * 1) * customrect;
}

// Teleporter lights (Custom)
void ActEntity438(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[8] = {
		{552, 0, 576, 4},
		{552, 4, 576, 8},
		{552, 8, 576, 12},
		{552, 12, 576, 16},
		{552, 16, 576, 20},
		{552, 20, 576, 24},
		{552, 24, 576, 28},
		{552, 28, 576, 32},
	};

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 7)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (32 * 1) * customrect;
	npc->rect.bottom += (32 * 1) * customrect;
}

// Deathtrap (Custom)
void ActEntity439(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[1] = {
		{576, 0, 608, 24}
	};

	npc->rect = rcLeft[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 1) * customrect;
	npc->rect.bottom += (24 * 1) * customrect;
}

// Life Capsule (Custom)
void ActEntity440(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[2] = {
		{608, 0, 624, 16},
		{624, 0, 640, 16},
	};

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Signpost (Custom)
void ActEntity441(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[2] = {
		{640, 0, 656, 16},
		{656, 0, 672, 16},
	};

	if (++npc->ani_wait > 1)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 1)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Fireplace (Custom)
void ActEntity442(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[4] = {
		{672, 0, 688, 16},
		{688, 0, 704, 16},
		{704, 0, 720, 16},
		{720, 0, 736, 16},
	};

	switch (npc->act_no)
	{
	case 0:
		if (++npc->ani_wait > 3)
		{
			npc->ani_wait = 0;
			++npc->ani_no;
		}

		if (npc->ani_no > 3)
			npc->ani_no = 0;

		npc->rect = rect[npc->ani_no];
		break;

	case 10:
		npc->act_no = 11;
		SetDestroyNpChar(npc->x, npc->y, npc->view.back, 8);
		// Fallthrough
	case 11:
		npc->rect.left = 0;
		npc->rect.right = 0;
		break;
	}

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Save Sign (Custom)
void ActEntity443(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[2] = {
		{736, 0, 752, 16},
		{752, 0, 768, 16},
	};

	if (npc->direct == 0)
		npc->ani_no = 0;
	else
		npc->ani_no = 1;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Sparkle (Custom)
void ActEntity444(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[4] = {
		{768, 0, 784, 16},
		{784, 0, 800, 16},
		{800, 0, 816, 16},
		{816, 0, 832, 16},
	};

	if (++npc->ani_wait > 3)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 3)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Terminal (Custom)
void ActEntity445(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[3] = {
		{832, 0, 848, 24},
		{832, 0, 848, 24}, // Same as frame 1
		{848, 0, 864, 24},
	};

	RECT rcRight[3] = {
		{832, 0, 848, 24}, // Same as direction 0 sprite 1
		{864, 0, 880, 24},
		{880, 0, 896, 24},
	};

	switch (npc->act_no)
	{
		case 0:
			npc->ani_no = 0;

			if (npc->x - (8 * 0x200) < gMC.x && npc->x + (8 * 0x200) > gMC.x && npc->y - (16 * 0x200) < gMC.y && npc->y + (8 * 0x200) > gMC.y)
			{
				PlaySoundObject(43, SOUND_MODE_PLAY);
				npc->act_no = 1;
			}

			break;

		case 1:
			if (++npc->ani_no > 2)
				npc->ani_no = 1;

			break;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	// Custom Rect
	npc->rect.top += (24 * 1) * customrect;
	npc->rect.bottom += (24 * 1) * customrect;
}

// Malco computer screen (Custom)
void ActEntity446(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[3] = {
		{896, 0, 912, 16},
		{896, 0, 912, 16},
		{912, 0, 928, 16},
	};

	if (++npc->ani_wait > 3)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 2)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

// Malco computer wave (Custom)
void ActEntity447(NPCHAR* npc)
{
	int customrect = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		customrect = CustomNpcValues(npc).CustomValueA;

	RECT rect[4] = {
		{928, 0, 944, 16},
		{944, 0, 960, 16},
		{960, 0, 976, 16},
		{976, 0, 992, 16},
	};

	if (npc->act_no == 0)
	{
		npc->act_no = 1;
		npc->y += 8 * 0x200;
	}

	if (++npc->ani_wait > 0)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 3)
		npc->ani_no = 0;

	npc->rect = rect[npc->ani_no];

	// Custom Rect
	npc->rect.top += (16 * 1) * customrect;
	npc->rect.bottom += (16 * 1) * customrect;
}

//Jump Refill
void ActEntity448(NPCHAR* npc)
{
	RECT rect[2] = {
		{0, 0, 16, 16},
		{16, 0, 32, 16},
	};

	int jump_amount = 1;
	int cooldown = 150;
	int sprite_index = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		jump_amount = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		cooldown = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		sprite_index = CustomNpcValues(npc).CustomValueC;

	switch (npc->act_no)
	{
		case 0:
			npc->act_wait = 0;
			npc->ani_no = 0;

			if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
				npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
				npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
			{
				PlaySoundObject(22, SOUND_MODE_PLAY);
				current_jumps = jump_amount; // Set jumps to the jump amount -> 1 by default
				npc->act_no = 1;
			}
			break;
		case 1:
			npc->ani_no = 1;
			if (++npc->act_wait > cooldown) {
				npc->act_no = 0;
				PlaySoundObject(70, SOUND_MODE_PLAY);
			}
			break;
	}

	npc->rect = rect[npc->ani_no];

	npc->rect.top += (16 * 1) * sprite_index;
	npc->rect.bottom += (16 * 1) * sprite_index;
}

//Rewind Refill
void ActEntity449(NPCHAR* npc)
{
	RECT rect[2] = {
		{224, 0, 240, 16},
		{240, 0, 256, 16},
	};

	int cooldown_rewind = 150;
	int cooldown_respawn = 50;
	int sprite_index = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		cooldown_rewind = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		cooldown_respawn = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		sprite_index = CustomNpcValues(npc).CustomValueC;

	int i = 0;

	switch (npc->act_no)
	{
		case 0:
			npc->act_wait = cooldown_respawn;
			npc->act_no = 1;
			break;

		case 1:
			if (--npc->act_wait == 0)
				npc->act_no = 2;
			break;
		case 2:
			npc->act_wait = 0;
			npc->ani_no = 0;


			if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
				npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
				npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
			{
				PlaySoundObject(22, SOUND_MODE_PLAY);
				npc->act_no = 3;
			}
			break;
		case 3:
			npc->ani_no = 1;
			if (++npc->act_wait > cooldown_rewind)
			{
				// Restore the players X/Y values to this entity
				gMC.x = npc->x;
				gMC.y = npc->y;

				for (i = 0; i < 4; ++i)
					SetNpChar(4, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);

				npc->act_no = 0;
				PlaySoundObject(70, SOUND_MODE_PLAY);
			}
			break;
	}

	npc->rect = rect[npc->ani_no];

	npc->rect.top += (16 * 1) * sprite_index;
	npc->rect.bottom += (16 * 1) * sprite_index;
}

// Conveyor
void ActEntity450(NPCHAR* npc)
{
	// Default to 1 when the custom value is 0.
	int speed = 1;

	// If custom value does not equal 0, then use the custom value as the value instead of the default.
	if (CustomNpcValues(npc).CustomValueA != 0)
		speed = CustomNpcValues(npc).CustomValueA;

	switch (npc->act_no)
	{
		case 0:
			// todo make it detect if there's another conveyor next and if not change sprite
			if (npc->direct == 0)
				npc->act_no = 10;
			else
				npc->act_no = 20;
			break;
		case 10:
			if (gMC.x < npc->x + (8 * 0x200) && gMC.x > npc->x - (8 * 0x200) && gMC.y < npc->y && gMC.y > npc->y - (16 * 0x200) && gMC.flag & 8)
				gMC.x -= (0.5 * 0x200) * speed;
			break;
		case 20:
			if (gMC.x < npc->x + (8 * 0x200) && gMC.x > npc->x - (8 * 0x200) && gMC.y < npc->y && gMC.y > npc->y - (16 * 0x200) && gMC.flag & 8)
				gMC.x += (0.5 * 0x200) * speed;
			break;
	}


	// Visuals
	static float frame = 0; // initialize once
	float ani_speed = 0.1 * speed;
	frame += ani_speed;
	frame = fmod(frame, 8);
	npc->ani_no = floor(frame);

	RECT rcLeft = {
		256 + (npc->ani_no * 16),
		0,
		272 + (npc->ani_no * 16),
		16
	};

	RECT rcRight = {
		368 - (npc->ani_no * 16),
		0,
		384 - (npc->ani_no * 16),
		16
	};

	if (npc->direct == 0)
		npc->rect = rcLeft;
	else
		npc->rect = rcRight;
}

// Wall Booster
void ActEntity451(NPCHAR* npc)
{
	int wallbooster_sprite = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		wallbooster_sprite = CustomNpcValues(npc).CustomValueA;

	// Default size (1 tile)
	RECT rcLeft[8] = {
	{ 512, 0, 528, 16 },
	{ 528, 0, 544, 16 },
	{ 544, 0, 560, 16 },
	{ 560, 0, 576, 16 },
	{ 576, 0, 592, 16 },
	{ 592, 0, 608, 16 },
	{ 608, 0, 624, 16 },
	{ 624, 0, 640, 16 },
	};

	// Default size (1 tile)
	RECT rcRight[8] = {
	{ 384, 0, 400, 16 },
	{ 400, 0, 416, 16 },
	{ 416, 0, 432, 16 },
	{ 432, 0, 448, 16 },
	{ 448, 0, 464, 16 },
	{ 464, 0, 480, 16 },
	{ 480, 0, 496, 16 },
	{ 496, 0, 512, 16 },
	};

	// Do animation work
	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 7)
		npc->ani_no = 0;

	// Wallboosting is inside of the hitbox and on a wall
	if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
		npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
		npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
	{
		if (onWall != 0)
			entity_IsWallboosting = true;
		else
			entity_IsWallboosting = false;
	}

	if (onWall == 0)
		entity_IsWallboosting = false;

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (wallbooster_sprite < 4)
	{
		npc->rect.top += (16 * 1) * wallbooster_sprite;
		npc->rect.bottom += (16 * 1) * wallbooster_sprite;
	}
}

// Ice Wall
void ActEntity452(NPCHAR* npc)
{
	int icewall_sprite = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		icewall_sprite = CustomNpcValues(npc).CustomValueA;

	RECT rcLeft[5] = {
		{720, 0, 736, 16},
		{736, 0, 752, 16},
		{752, 0, 768, 16},
		{768, 0, 784, 16},
		{784, 0, 800, 16},
	};

	RECT rcRight[5] = {
		{640, 0, 656, 16},
		{656, 0, 672, 16},
		{672, 0, 688, 16},
		{688, 0, 704, 16},
		{704, 0, 720, 16},
	};

	// Randomize *when* the Ice Wall shining happens
	if ((Random(1, 30) == 7) && npc->count2 != 1)
		npc->count2 = 1;

	// Do animation work
	if (npc->count2 == 1)
	{
		if (++npc->count1 > Random(75, 115)) // Randomize how long it takes for the shine animation to play
		{
			if (++npc->ani_wait > 8)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no > 4)
			{
				npc->ani_no = 0;
				npc->count1 = 0;
			}
		}
	}



	// Icewalled when inside of the hitbox and on a wall
	if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
		npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
		npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
	{
			entity_IsIceWalled = true;
	}

	if (npc->direct == 0)
		npc->rect = rcLeft[npc->ani_no];
	else
		npc->rect = rcRight[npc->ani_no];

	if (icewall_sprite < 4)
	{
		npc->rect.top += (16 * 1) * icewall_sprite;
		npc->rect.bottom += (16 * 1) * icewall_sprite;
	}
}

// Spring
void ActEntity453(NPCHAR* npc)
{
	int start_momentum = 1535;
	int added_momentum = 1535;

	if (CustomNpcValues(npc).CustomValueA != 0)
		start_momentum = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		added_momentum = CustomNpcValues(npc).CustomValueB;

	RECT spring[10] = {
		{800, 0, 816, 16}, // 0 - 0
		{816, 0, 832, 16}, // 1 - 1
		{832, 0, 848, 16}, // 2 - 2
		{848, 0, 864, 16}, // 3 - 3
		{832, 0, 848, 16}, // 2 - 4
		{848, 0, 864, 16}, // 3 - 5
		{832, 0, 848, 16}, // 2 - 6
		{848, 0, 864, 16}, // 3 - 7
		{864, 0, 880, 16}, // 4 - 8
		{864, 0, 880, 16}, // 5 - 9
	};

	switch (npc->act_no)
	{
		case 0:
			if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
				npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
				npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
			{
				PlaySoundObject(8, SOUND_MODE_PLAY);
				gMC.ym += -0x5FF;
				npc->act_no = 1;
			}
			break;

		case 1:
			// Do animation work
			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no < 3)
				gMC.ym += -0x5FF;


			if (npc->ani_no > 8)
			{
				npc->ani_no = 0;
				npc->act_no = 0;
			}
			break;
	}

	npc->rect = spring[npc->ani_no];
}

// Spring (Left)
void ActEntity454(NPCHAR* npc)
{
	int start_x_momentum = 336;
	int start_y_momentum = 1024;
	int added_x_momentum = 144;
	int added_y_momentum = 32;

	if (CustomNpcValues(npc).CustomValueA != 0)
		start_x_momentum = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		start_y_momentum = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		added_x_momentum = CustomNpcValues(npc).CustomValueC;

	if (CustomNpcValues(npc).CustomValueD != 0)
		added_y_momentum = CustomNpcValues(npc).CustomValueD;

	RECT spring[10] = {
		{896, 0, 912, 16}, // 0 - 0
		{912, 0, 928, 16}, // 1 - 1
		{928, 0, 944, 16}, // 2 - 2
		{944, 0, 960, 16}, // 3 - 3
		{928, 0, 944, 16}, // 2 - 4
		{944, 0, 960, 16}, // 3 - 5
		{928, 0, 944, 16}, // 2 - 6
		{944, 0, 960, 16}, // 3 - 7
		{960, 0, 976, 16}, // 4 - 8
		{976, 0, 992, 16}, // 5 - 9
	};

	switch (npc->act_no)
	{
		case 0:
			if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
				npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
				npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
			{
				PlaySoundObject(8, SOUND_MODE_PLAY);
				gMC.xm = -start_x_momentum;
				gMC.ym = -start_y_momentum;
				npc->act_no = 1;
			}
			break;

		case 1:
			// Do animation work
			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no < 2)
				gMC.xm += -added_x_momentum;

			if (npc->ani_no < 3)
				gMC.ym += -added_y_momentum;


			if (npc->ani_no > 8)
			{
				npc->ani_no = 0;
				npc->act_no = 0;
			}
			break;
	}

	npc->rect = spring[npc->ani_no];
}

// Spring (Right)
void ActEntity455(NPCHAR* npc)
{
	int start_x_momentum = 336;
	int start_y_momentum = 1024;
	int added_x_momentum = 144;
	int added_y_momentum = 32;

	if (CustomNpcValues(npc).CustomValueA != 0)
		start_x_momentum = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		start_y_momentum = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		added_x_momentum = CustomNpcValues(npc).CustomValueC;

	if (CustomNpcValues(npc).CustomValueD != 0)
		added_y_momentum = CustomNpcValues(npc).CustomValueD;

	RECT spring[10] = {
		{992, 0, 1008, 16}, // 0 - 0
		{1008, 0, 1024, 16}, // 1 - 1
		{1024, 0, 1040, 16}, // 2 - 2
		{1040, 0, 1056, 16}, // 3 - 3
		{1024, 0, 1040, 16}, // 2 - 4
		{1040, 0, 1056, 16}, // 3 - 5
		{1024, 0, 1040, 16}, // 2 - 6
		{1040, 0, 1056, 16}, // 3 - 7
		{1056, 0, 1072, 16}, // 4 - 8
		{1072, 0, 1088, 16}, // 5 - 9
	};

	switch (npc->act_no)
	{
		case 0:
			if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
				npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
				npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
			{
				PlaySoundObject(8, SOUND_MODE_PLAY);
				gMC.xm = start_x_momentum;
				gMC.ym = -start_y_momentum;
				npc->act_no = 1;
			}
			break;

		case 1:
			// Do animation work
			if (++npc->ani_wait > 3)
			{
				npc->ani_wait = 0;
				++npc->ani_no;
			}

			if (npc->ani_no < 2)
				gMC.xm += added_x_momentum;

			if (npc->ani_no < 3)
				gMC.ym += -added_y_momentum;


			if (npc->ani_no > 8)
			{
				npc->ani_no = 0;
				npc->act_no = 0;
			}
			break;
	}

	npc->rect = spring[npc->ani_no];
}

// Player Matcher w/ Offset
void ActEntity456(NPCHAR* npc)
{
	int x_offset = 0;
	int y_offset = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		x_offset = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		y_offset = CustomNpcValues(npc).CustomValueB;

	RECT rect = { 0, 0, 16, 16 };

	switch (npc->act_no)
	{
		case 0:
			if (npc->direct == 2)
			{
				npc->x = gMC.x + (x_offset * 0x200 * 0x10);
				npc->y = gMC.y + (y_offset * 0x200 * 0x10);
				npc->act_no = 1;
			}
			else
			{
				npc->x = gMC.x + (x_offset * 0x200 * 0x10);
				npc->y = gMC.y + (y_offset * 0x200 * 0x10);
			}
			break;

		case 1:
			npc->cond = 0;
			break;
	}

	npc->rect = rect;
}

// Camera Offset Trigger
void ActEntity457(NPCHAR* npc)
{
	int camera_x_offset = 0;
	int camera_y_offset = 0;
	int wait = 16;
	int lock_cam_dir = 0;

	if (CustomNpcValues(npc).CustomValueA != 0)
		camera_x_offset = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		camera_y_offset = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		wait = CustomNpcValues(npc).CustomValueC;

	if (CustomNpcValues(npc).CustomValueD != 0)
		lock_cam_dir = CustomNpcValues(npc).CustomValueD;

	RECT rect = { 0, 0, 16, 16 };

	if (npc->direct == 0)
	{
		if (npc->x < gMC.x)
			npc->x += 0x5FF;
		else
			npc->x -= 0x5FF;
	}
	else
	{
		if (npc->y < gMC.y)
			npc->y += 0x5FF;
		else
			npc->y -= 0x5FF;
	}

	if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
		npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
		npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
	{
		// Do Camera Offset
		SetFrameTargetMyCharOffset(wait, 0, camera_x_offset, camera_y_offset);

		// Set locked camera direction if the flag is set
		if (npc->bits & NPC_CUSTOM_FLAG)
		{
			is_direction_locked = true;
			lock_direction = lock_cam_dir;
		}

		// Reset locked camera direction if the flag isnt set
		if (!(npc->bits & NPC_CUSTOM_FLAG))
		{
			is_direction_locked = false;
			lock_direction = 0;
		}
	}

	npc->rect = rect;
}

// Camera Target Trigger (Coordinate)
void ActEntity458(NPCHAR* npc)
{
	int camera_x_coordinate = 0;
	int camera_y_coordinate = 0;
	int wait = 16;

	if (CustomNpcValues(npc).CustomValueA != 0)
		camera_x_coordinate = CustomNpcValues(npc).CustomValueA;

	if (CustomNpcValues(npc).CustomValueB != 0)
		camera_y_coordinate = CustomNpcValues(npc).CustomValueB;

	if (CustomNpcValues(npc).CustomValueC != 0)
		wait = CustomNpcValues(npc).CustomValueC;

	RECT rect = { 0, 0, 16, 16 };

	if (npc->direct == 0)
	{
		if (npc->x < gMC.x)
			npc->x += 0x5FF;
		else
			npc->x -= 0x5FF;
	}
	else
	{
		if (npc->y < gMC.y)
			npc->y += 0x5FF;
		else
			npc->y -= 0x5FF;
	}

	if (npc->y - npc->hit.top < (gMC.y + gMC.hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC.y - gMC.hit.top) &&
		npc->x - npc->hit.back < (gMC.x + gMC.hit.front) &&
		npc->x + npc->hit.front >(gMC.x - gMC.hit.back))
	{
		// Do Camera Target
		SetFrameTargetCoordinate(wait, camera_x_coordinate * 0x200 * 0x10, camera_y_coordinate * 0x200 * 0x10);
	}

	npc->rect = rect;
}

// Money NPC
void ActEntity459(NPCHAR* npc)
{
	// In wind
	if (gBack.type == 6 || gBack.type == 6)
	{
		if (npc->act_no == 0)
		{
			// Set state
			npc->act_no = 1;

			// Set random speed
			npc->ym = Random(-0x80, 0x80);
			npc->xm = Random(0x7F, 0x100);
		}

		// Blow to the left
		npc->xm -= 8;

		// Destroy when off-screen
		if (npc->x < 80 * 0x200)
			npc->cond = 0;

		// Limit speed (except Pixel applied it to the X position)
		if (npc->x < -0x600)
			npc->x = -0x600;

		// Bounce off walls
		if (npc->flag & 1)
			npc->xm = 0x100;
		if (npc->flag & 2)
			npc->ym = 0x40;
		if (npc->flag & 8)
			npc->ym = -0x40;
	}
	// When not in wind
	else
	{
		if (npc->act_no == 0)
		{
			// Set state
			npc->act_no = 1;
			npc->ani_no = Random(0, 4);

			// Random speed
			npc->xm = Random(-0x200, 0x200);
			npc->ym = Random(-0x400, 0);

			// Random direction (reverse animation or not)
			if (Random(0, 1) != 0)
				npc->direct = 0;
			else
				npc->direct = 2;
		}

		// Gravity

		if (npc->flag & 0x100)
			npc->ym += 0x15;
		else
			npc->ym += 0x2A;

		// Bounce off walls
		if (npc->flag & 1 && npc->xm < 0)
			npc->xm *= -1;
		if (npc->flag & 4 && npc->xm > 0)
			npc->xm *= -1;

		// Bounce off ceiling
		if (npc->flag & 2 && npc->ym < 0)
			npc->ym *= -1;

		// Bounce off floor
		if (npc->flag & 8)
		{
			// PlaySoundObject(45, SOUND_MODE_PLAY);
			// npc->ym = -0x280;
			npc->xm = 2 * npc->xm / 3;
		}

		// Play bounce song (and try to clip out of floor if stuck)
		if (npc->flag & 0xD)
		{
			// PlaySoundObject(45, SOUND_MODE_PLAY);
			if (++npc->count2 > 2)
				npc->y -= 1 * 0x200;
		}
		else
		{
			npc->count2 = 0;
		}

		// Limit speed
		if (npc->xm < -0x5FF)
			npc->xm = -0x5FF;
		if (npc->xm > 0x5FF)
			npc->xm = 0x5FF;
		if (npc->ym < -0x5FF)
			npc->ym = -0x5FF;
		if (npc->ym > 0x5FF)
			npc->ym = 0x5FF;
	}

	// Move
	npc->y += npc->ym;
	npc->x += npc->xm;

	// Get framerects
	RECT rect[6] = {
		{ 1088, 0, 1104, 16},
		{1104, 0, 1120, 16},
		{1120, 0, 1136, 16},
		{1136, 0, 1152, 16},
		{1152, 0, 1168, 16},
		{1168, 0, 1184, 16},
	};

	RECT rcNo = { 0, 0, 0, 0 };

	// Animate
	++npc->ani_wait;

	if (npc->direct == 0)
	{
		if (npc->ani_wait > 2)
		{
			npc->ani_wait = 0;

			if (++npc->ani_no > 5)
				npc->ani_no = 0;
		}
	}
	else
	{
		if (npc->ani_wait > 2)
		{
			npc->ani_wait = 0;

			if (--npc->ani_no < 0)
				npc->ani_no = 5;
		}
	}

	npc->rect = rect[npc->ani_no];

	// Size
	if (npc->act_no != 0)
	{
		switch (npc->exp)
		{
			case 5:
				npc->rect.top += 16;
				npc->rect.bottom += 16;
				break;

			case 20:
				npc->rect.top += 32;
				npc->rect.bottom += 32;
				break;
		}

		npc->act_no = 1;
	}

	// Delete after 500 frames
	if (++npc->count1 > 500 && npc->ani_no == 5 && npc->ani_wait == 2)
		npc->cond = 0;

	// Blink after 400 frames
	if (npc->count1 > 400)
		if (npc->count1 / 2 % 2)
			npc->rect = rcNo;
}

// Load-Save-Event Controller
void ActEntity460(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	if (npc->act_no == 0)
	{
		npc->act_no = 1;

		if (npc->direct == 2)
			npc->y += 16 * 0x200;
	}

	if (isLoadingSave)
	{
		npc->cond = 0;
		StartTextScript(npc->code_event);
	}

	npc->rect = rect;
}

// Wind NPC (horizontal)
void ActEntity461(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	int speed = 136;

	if (npc->direct == 0)
		speed *= -1;

	gMC.xm += speed;

	npc->rect = rect;
}

// Wind NPC (vertical)
void ActEntity462(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	int speed = -128;

	if (npc->direct == 0)
		speed = 85;

	gMC.ym += speed;

	npc->rect = rect;
}

// Reverse Momentum
void ActEntity463(NPCHAR* npc)
{

}