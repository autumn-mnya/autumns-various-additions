#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Entity.h"

#include "main.h"
#include "cave_story.h"
#include "EntityLoad.h"

#define CustomNpcValues(N) gCustomNPC[(N - gNPC)]

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

	if (CustomNpcValues(npc).CustomValue01 != 0)
		boostfuelamount = CustomNpcValues(npc).CustomValue01;

	if (CustomNpcValues(npc).CustomValue01 != 0)
		cooldown = CustomNpcValues(npc).CustomValue02;

	switch (npc->act_no)
	{
		case 0:
			npc->act_wait = 0;
			npc->ani_no = 0;

			if (npc->y - npc->hit.top < (gMC->y + gMC->hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC->y - gMC->hit.top) &&
				npc->x - npc->hit.back < (gMC->x + gMC->hit.front) &&
				npc->x + npc->hit.front >(gMC->x - gMC->hit.back))
			{
				PlaySoundObject(22, SOUND_MODE_PLAY);
				if (npc->direct != 2)
					gMC->boost_cnt = boostfuelamount;
				else
					gMC->boost_cnt = 0;

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
}

// Lock-On Trigger
void ActEntity362(NPCHAR* npc)
{
	int lockon_trigger_speed = 0x5FF;
	int lockon_trigger_tl_x = 0;
	int lockon_trigger_tl_y = 0;
	int lockon_trigger_br_x = 16;
	int lockon_trigger_br_y = 16;

	if (CustomNpcValues(npc).CustomValue01 != 0)
		lockon_trigger_speed = CustomNpcValues(npc).CustomValue01;

	// Top Left X rect
	if (CustomNpcValues(npc).CustomValue02 != 0)
		lockon_trigger_tl_x = CustomNpcValues(npc).CustomValue02;

	// Top Left Y rect
	if (CustomNpcValues(npc).CustomValue03 != 0)
		lockon_trigger_tl_y = CustomNpcValues(npc).CustomValue03;

	// Bottom Right X rect

	if (CustomNpcValues(npc).CustomValue04 != 0)
		lockon_trigger_br_x = CustomNpcValues(npc).CustomValue04;

	// Bottom Right Y rect

	if (CustomNpcValues(npc).CustomValue05 != 0)
		lockon_trigger_br_y = CustomNpcValues(npc).CustomValue05;

	// Surface override
	if (CustomNpcValues(npc).CustomValue06 != 0)
		npc->surf = SurfaceID(CustomNpcValues(npc).CustomValue06);

	RECT rect = { lockon_trigger_tl_x, lockon_trigger_tl_y, lockon_trigger_br_x, lockon_trigger_br_y };

	npc->bits |= NPC_EVENT_WHEN_TOUCHED;

	// Trigger that moves in both Horizontal and Vertical directions
	if (npc->x < gMC->x)
		npc->x += lockon_trigger_speed;
	else
		npc->x -= lockon_trigger_speed;


	if (npc->y < gMC->y)
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
				if (npc->x < gMC->x)
				{
					npc->x += 0x5FF;
				}
				else
				{
					npc->x -= 0x5FF;
				}

				// If the player is on one side, run one event. If on the other, run another event.
				if (gMC->y > npc->y)
					npc->code_event = npc->count1 + 10; // Add 10 to the event #
				else
					npc->code_event = npc->count1;
			}
			else // Vertical Moving Trigger
			{
				if (npc->y < gMC->y)
					npc->y += 0x5FF;
				else
					npc->y -= 0x5FF;

				// If the player is on one side, run one event. If on the other, run another event.
				if (gMC->x < npc->x)
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
			if (npc->direct == 0)
			{
				npc->x = gMC->x;
				npc->y = gMC->y;
				npc->act_no = 1;
			}
			else
			{
				npc->x = gMC->x;
				npc->y = gMC->y;
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

	if (npc->y < gMC->y)
		npc->y += 0x5FF;
	else
		npc->y -= 0x5FF;

	// If the player touches the Entities collision, bounce them backwards automatically.
	if (npc->y - npc->hit.top < (gMC->y + gMC->hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC->y - gMC->hit.top) &&
		npc->x - npc->hit.back < (gMC->x + gMC->hit.front) &&
		npc->x + npc->hit.front >(gMC->x - gMC->hit.back))
	{
		if (gMC->x < npc->x)
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
		if (npc->x < gMC->x)
			npc->x += 0x5FF;
		else
			npc->x -= 0x5FF;
	}
	else
	{
		if (npc->y < gMC->y)
			npc->y += 0x5FF;
		else
			npc->y -= 0x5FF;
	}

	// If the player touches the Entities collision, kill them immediately.
	if (npc->y - npc->hit.top < (gMC->y + gMC->hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC->y - gMC->hit.top) &&
		npc->x - npc->hit.back < (gMC->x + gMC->hit.front) &&
		npc->x + npc->hit.front >(gMC->x - gMC->hit.back))
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
		{32, 64, 48, 80},
		{48, 64, 64, 80},
	};

	RECT rectRight[2] =
	{
		{32, 48, 48, 64},
		{48, 48, 64, 64},
	};

	int amount = 50;
	int cooldown = 50;

	if (CustomNpcValues(npc).CustomValue01 != 0)
		amount = CustomNpcValues(npc).CustomValue01;

	if (CustomNpcValues(npc).CustomValue02 != 0)
		cooldown = CustomNpcValues(npc).CustomValue02;


	switch (npc->act_no)
	{
		case 0:
			npc->act_wait = 0;
			npc->ani_no = 0;

			if (npc->y - npc->hit.top < (gMC->y + gMC->hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC->y - gMC->hit.top) &&
				npc->x - npc->hit.back < (gMC->x + gMC->hit.front) &&
				npc->x + npc->hit.front >(gMC->x - gMC->hit.back))
			{
				PlaySoundObject(22, SOUND_MODE_PLAY);
				gMC->boost_cnt = amount;
				// Store the players Y value to make it appear normal
				npc->count1 = gMC->y;

				// Set direction of the Player to the same as the boost direction
				if (npc->direct == 2)
					gMC->direct = 2;
				else
					gMC->direct = 0;

				npc->act_no = 1;
			}
			break;
		case 1:
			npc->ani_no = 1;

			if (gMC->boost_cnt != 0)
			{
				switch (npc->direct)
				{
					default:
						gMC->boost_sw = 1;
						gMC->ym = 0;
						gMC->y = npc->count1;
						gMC->xm = -0x5FF;
						break;

					case 2:
						gMC->boost_sw = 1;
						gMC->ym = 0;
						gMC->y = npc->count1;
						gMC->xm = 0x5FF;
						break;

				}

				--gMC->boost_cnt;
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
		if (gMC->boost_sw == 1)
		{
			// Go up when going into a wall
			if (gMC->flag & 5)
				gMC->ym = -0x100;

			// Move in direction facing
			if (gMC->direct == 0)
				gMC->xm -= 0x20;
			if (gMC->direct == 2)
				gMC->xm += 0x20;

			// Boost particles (and sound)
			if (gMC->boost_cnt % 3 == 1)
			{
				if (gMC->direct == 0)
					SetCaret(gMC->x + (2 * 0x200), gMC->y + (2 * 0x200), CARET_EXHAUST, DIR_RIGHT);
				if (gMC->direct == 2)
					SetCaret(gMC->x - (2 * 0x200), gMC->y + (2 * 0x200), CARET_EXHAUST, DIR_LEFT);

				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC->boost_sw == 2)
		{
			// Move upwards
			gMC->ym -= 0x20;

			// Boost particles (and sound)
			if (gMC->boost_cnt % 3 == 1)
			{
				SetCaret(gMC->x, gMC->y + (6 * 0x200), CARET_EXHAUST, DIR_DOWN);
				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC->boost_sw == 3 && gMC->boost_cnt % 3 == 1)
		{
			// Boost particles (and sound)
			SetCaret(gMC->x, gMC->y - (6 * 0x200), CARET_EXHAUST, DIR_UP);
			PlaySoundObject(113, SOUND_MODE_PLAY);
		}
	}

	if (npc->direct != 2)
		npc->rect = rectLeft[npc->ani_no];
	else
		npc->rect = rectRight[npc->ani_no];
}

// Forced Boost (Vertical)
void ActEntity368(NPCHAR* npc)
{
	RECT rectUp[2] = {
		{0, 48, 16, 64},
		{16, 48, 32, 64},
	};

	RECT rectDown[2] =
	{
		{0, 64, 16, 80},
		{16, 64, 32, 80},
	};

	int amount = 50;
	int cooldown = 50;

	if (CustomNpcValues(npc).CustomValue01 != 0)
		amount = CustomNpcValues(npc).CustomValue01;

	if (CustomNpcValues(npc).CustomValue02 != 0)
		cooldown = CustomNpcValues(npc).CustomValue02;

	switch (npc->act_no)
	{
	case 0:
		npc->act_wait = 0;
		npc->ani_no = 0;

		if (npc->y - npc->hit.top < (gMC->y + gMC->hit.bottom) &&
			npc->y + npc->hit.bottom >(gMC->y - gMC->hit.top) &&
			npc->x - npc->hit.back < (gMC->x + gMC->hit.front) &&
			npc->x + npc->hit.front >(gMC->x - gMC->hit.back))
		{
			PlaySoundObject(22, SOUND_MODE_PLAY);
			gMC->boost_cnt = amount;

			npc->act_no = 1;
		}
		break;
	case 1:
		npc->ani_no = 1;

		if (gMC->boost_cnt != 0)
		{
			switch (npc->direct)
			{
				default:
					gMC->boost_sw = 2;
					gMC->xm = 0;
					gMC->ym = -0x5FF;
					break;

				case 2:
					gMC->boost_sw = 3;
					gMC->xm = 0;
					gMC->ym = 0x5FF;
					break;
			}

			--gMC->boost_cnt;
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
		if (gMC->boost_sw == 1)
		{
			// Go up when going into a wall
			if (gMC->flag & 5)
				gMC->ym = -0x100;

			// Move in direction facing
			if (gMC->direct == 0)
				gMC->xm -= 0x20;
			if (gMC->direct == 2)
				gMC->xm += 0x20;

			// Boost particles (and sound)
			if (gMC->boost_cnt % 3 == 1)
			{
				if (gMC->direct == 0)
					SetCaret(gMC->x + (2 * 0x200), gMC->y + (2 * 0x200), CARET_EXHAUST, DIR_RIGHT);
				if (gMC->direct == 2)
					SetCaret(gMC->x - (2 * 0x200), gMC->y + (2 * 0x200), CARET_EXHAUST, DIR_LEFT);

				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC->boost_sw == 2)
		{
			// Move upwards
			gMC->ym -= 0x20;

			// Boost particles (and sound)
			if (gMC->boost_cnt % 3 == 1)
			{
				SetCaret(gMC->x, gMC->y + (6 * 0x200), CARET_EXHAUST, DIR_DOWN);
				PlaySoundObject(113, SOUND_MODE_PLAY);
			}
		}
		else if (gMC->boost_sw == 3 && gMC->boost_cnt % 3 == 1)
		{
			// Boost particles (and sound)
			SetCaret(gMC->x, gMC->y - (6 * 0x200), CARET_EXHAUST, DIR_UP);
			PlaySoundObject(113, SOUND_MODE_PLAY);
		}
	}

	if (npc->direct != 2)
		npc->rect = rectUp[npc->ani_no];
	else
		npc->rect = rectDown[npc->ani_no];
}

// Flag Trigger
void ActEntity369(NPCHAR* npc)
{
	RECT rect = { 0, 0, 16, 16 };

	if (npc->direct == 0)
	{
		if (npc->x < gMC->x)
			npc->x += 0x5FF;
		else
			npc->x -= 0x5FF;
	}
	else
	{
		if (npc->y < gMC->y)
			npc->y += 0x5FF;
		else
			npc->y -= 0x5FF;
	}

	if (npc->y - npc->hit.top < (gMC->y + gMC->hit.bottom) &&
		npc->y + npc->hit.bottom >(gMC->y - gMC->hit.top) &&
		npc->x - npc->hit.back < (gMC->x + gMC->hit.front) &&
		npc->x + npc->hit.front >(gMC->x - gMC->hit.back))
	{
		if (!(npc->bits & NPC_CUSTOM_FLAG))
		{
			if (CustomNpcValues(npc).CustomValue01 != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValue01);

			if (CustomNpcValues(npc).CustomValue02 != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValue02);

			if (CustomNpcValues(npc).CustomValue03 != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValue03);

			if (CustomNpcValues(npc).CustomValue04 != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValue04);

			if (CustomNpcValues(npc).CustomValue05 != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValue05);

			if (CustomNpcValues(npc).CustomValue06 != 0)
				CutNPCFlag(CustomNpcValues(npc).CustomValue06);
		}
		else
		{
			if (CustomNpcValues(npc).CustomValue01 != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValue01);

			if (CustomNpcValues(npc).CustomValue02 != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValue02);

			if (CustomNpcValues(npc).CustomValue03 != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValue03);

			if (CustomNpcValues(npc).CustomValue04 != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValue04);

			if (CustomNpcValues(npc).CustomValue05 != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValue05);

			if (CustomNpcValues(npc).CustomValue06 != 0)
				SetNPCFlag(CustomNpcValues(npc).CustomValue06);
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
			if (npc->act_wait >= 8 && npc->x - (96 * 0x200) < gMC->x && npc->x + (96 * 0x200) > gMC->x && npc->y - (32 * 0x200) < gMC->y && npc->y + (96 * 0x200) > gMC->y)
			{
				if (npc->x > gMC->x)
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

			if (npc->act_wait >= 8 && npc->x - (48 * 0x200) < gMC->x && npc->x + (48 * 0x200) > gMC->x && npc->y - (32 * 0x200) < gMC->y && npc->y + (160 * 0x200) > gMC->y)
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

				if (npc->x > gMC->x)
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
			if (npc->x < gMC->x)
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
				deg = GetArktan(npc->x - gMC->x, npc->y - gMC->y);
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

	if (CustomNpcValues(npc).CustomValue01 != 0)
		x_radius = CustomNpcValues(npc).CustomValue01;

	if (CustomNpcValues(npc).CustomValue02 != 0)
		y_radius = CustomNpcValues(npc).CustomValue02;

	// Left, Right, Up, Down. 16 is 1 tile, so doing math we can have a radius for the Camera to target to.
	if (npc->x - ((16 * x_radius) * 0x200)
		< gMC->x && npc->x + ((16 * x_radius) * 0x200)
		> gMC->x && npc->y - ((16 * y_radius) * 0x200)
		< gMC->y && npc->y + ((16 * y_radius) * 0x200)
		> gMC->y)
	{
		gFrame->tgt_x = &npc->x;
		gFrame->tgt_y = &npc->y;
	}
	else if (npc->x - ((16 * (x_radius + 1)) * 0x200)
		< gMC->x && npc->x + ((16 * (x_radius + 1)) * 0x200)
		> gMC->x && npc->y - ((16 * (y_radius + 1)) * 0x200)
		< gMC->y && npc->y + ((16 * (y_radius + 1)) * 0x200)
		> gMC->y)
	{
		gFrame->tgt_x = &gMC->tgt_x;
		gFrame->tgt_y = &gMC->tgt_y;
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

	if (CustomNpcValues(npc).CustomValue01 != 0)
		tra_map = CustomNpcValues(npc).CustomValue01;

	if (CustomNpcValues(npc).CustomValue02 != 0)
		tra_event = CustomNpcValues(npc).CustomValue02;

	if (CustomNpcValues(npc).CustomValue03 != 0)
		tra_x = CustomNpcValues(npc).CustomValue03;

	if (CustomNpcValues(npc).CustomValue04 != 0)
		tra_y = CustomNpcValues(npc).CustomValue04;

	if (CustomNpcValues(npc).CustomValue05 != 0)
		fade_dir = CustomNpcValues(npc).CustomValue05;

	if (npc->direct == 0)
	{
		if (npc->x < gMC->x)
			npc->x += 0x5FF;
		else
			npc->x -= 0x5FF;
	}
	else
	{
		if (npc->y < gMC->y)
			npc->y += 0x5FF;
		else
			npc->y -= 0x5FF;
	}

	switch (npc->act_no)
	{
		case 0:
			if (npc->y - npc->hit.top < (gMC->y + gMC->hit.bottom) &&
				npc->y + npc->hit.bottom >(gMC->y - gMC->hit.top) &&
				npc->x - npc->hit.back < (gMC->x + gMC->hit.front) &&
				npc->x + npc->hit.front >(gMC->x - gMC->hit.back))
			{
				g_GameFlags &= ~2;
				g_GameFlags |= 1;
				gMC->xm = 0;
				if (fade_dir <= 4)
					StartFadeOut(fade_dir);
				npc->act_no = 1;
			}
			break;

		case 1:
			if (gFade->mode == 0)
				TransferStage(tra_map, tra_event, tra_x, tra_y);
			break;
	}

	npc->rect = rect;
}

// Custom EXP Capsule
void ActEntity373(NPCHAR* npc)
{
	int exp_amount = 10;

	if (CustomNpcValues(npc).CustomValue01 != 0)
		exp_amount = CustomNpcValues(npc).CustomValue01;

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

	if (CustomNpcValues(npc).CustomValue01 != 0)
		speed = CustomNpcValues(npc).CustomValue01;

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

			if (gMC->x > npc->x - (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC->x < npc->x + (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC->y > npc->y - (((WINDOW_HEIGHT / 2) + 120) * 0x200) && gMC->y < npc->y + (((WINDOW_HEIGHT / 2) + 120) * 0x200))
			{
				if (Random(0, 5) == 1)
					SetNpChar(199, npc->x, npc->y + (Random(-8, 8) * 0x200), 0, 0, 0, NULL, 0x100);
			}

			if (gMC->y < npc->y + (8 * 0x200) && gMC->y > npc->y - (8 * 0x200) && gMC->x < npc->x && gMC->x > npc->x - (96 * 0x200))
			{
				gMC->xm -= speed;
				gMC->cond |= 0x20;
			}

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

	if (CustomNpcValues(npc).CustomValue01 != 0)
		speed = CustomNpcValues(npc).CustomValue01;

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

			if (gMC->x > npc->x - (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC->x < npc->x + (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC->y > npc->y - (((WINDOW_HEIGHT / 2) + 120) * 0x200) && gMC->y < npc->y + (((WINDOW_HEIGHT / 2) + 120) * 0x200))
			{
				if (Random(0, 5) == 1)
					SetNpChar(199, npc->x + (Random(-8, 8) * 0x200), npc->y, 0, 0, 1, NULL, 0x100);
			}

			if (gMC->x < npc->x + (8 * 0x200) && gMC->x > npc->x - (8 * 0x200) && gMC->y < npc->y && gMC->y > npc->y - (96 * 0x200))
				gMC->ym -= speed;

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

	if (CustomNpcValues(npc).CustomValue01 != 0)
		speed = CustomNpcValues(npc).CustomValue01;

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

			if (gMC->x > npc->x - (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC->x < npc->x + (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC->y > npc->y - (((WINDOW_HEIGHT / 2) + 120) * 0x200) && gMC->y < npc->y + (((WINDOW_HEIGHT / 2) + 120) * 0x200))
			{
				if (Random(0, 5) == 1)
					SetNpChar(199, npc->x, npc->y + (Random(-8, 8) * 0x200), 0, 0, 2, NULL, 0x100);
			}

			if (gMC->y < npc->y + (8 * 0x200) && gMC->y > npc->y - (8 * 0x200) && gMC->x < npc->x + (96 * 0x200) && gMC->x > npc->x)
			{
				gMC->xm += speed;
				gMC->cond |= 0x20;
			}

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

	if (CustomNpcValues(npc).CustomValue01 != 0)
		speed = CustomNpcValues(npc).CustomValue01;

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

			if (gMC->x > npc->x - (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC->x < npc->x + (((WINDOW_WIDTH / 2) + 160) * 0x200) && gMC->y > npc->y - (((WINDOW_HEIGHT / 2) + 120) * 0x200) && gMC->y < npc->y + (((WINDOW_HEIGHT / 2) + 120) * 0x200))
			{
				if (Random(0, 5) == 1)
					SetNpChar(199, npc->x + (Random(-8, 8) * 0x200), npc->y, 0, 0, 3, NULL, 0x100);
			}

			if (gMC->x < npc->x + (8 * 0x200) && gMC->x > npc->x - (8 * 0x200) && gMC->y < npc->y + (96 * 0x200) && gMC->y > npc->y)
				gMC->ym += speed;

			break;
	}

	npc->rect = rc[npc->ani_no];
}

// Custom Spikes
void ActEntity378(NPCHAR* npc)
{
	RECT rects[4] = {
		{256, 200, 272, 216},
		{272, 200, 288, 216},
		{288, 200, 304, 216},
		{304, 200, 320, 216},
	};

	npc->rect = rects[CustomNpcValues(npc).CustomValue01];
}