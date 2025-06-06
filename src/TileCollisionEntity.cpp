#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "TileCollisionEntity.h"

#include "main.h"

#include "mod_loader.h"
#include "cave_story.h"

// Tiletype 0x48
void JudgeHitNpCharJumpThru(NPCHAR* npc, int x, int y)
{
	int hit = 0;

	if (npc->ym < 0 || npc->y - npc->ym + npc->hit.bottom > (y * 0x10 - 8) * 0x200)
		hit |= 0;

	if (npc->x - npc->hit.back < (x * 0x10 + 5) * 0x200
		&& npc->x + npc->hit.back >(x * 0x10 - 5) * 0x200
		&& npc->y + npc->hit.bottom > (y * 0x10 - 8) * 0x200
		&& npc->y + npc->hit.bottom < y * 0x10 * 0x200)
	{
		npc->y = ((y * 0x10 - 8) * 0x200) - npc->hit.bottom;
		npc->ym = 0;
		hit |= 8;
	}

	npc->flag |= hit;
}

// Tiletype 0x49
void JudgeHitNpCharJumpThruUpsideDown(NPCHAR* npc, int x, int y)
{
	int hit = 0;

	if (npc->ym > 0 || npc->y + npc->ym - npc->hit.top < (y * 0x10 - 8) * 0x200)
		hit |= 0;

	if (npc->x - npc->hit.back < (x * 0x10 + 5) * 0x200
		&& npc->x + npc->hit.back >(x * 0x10 - 5) * 0x200
		&& npc->y - npc->hit.top < (y * 0x10 + 8) * 0x200
		&& npc->y - npc->hit.top > y * 0x10 * 0x200)
	{
		npc->y = ((y * 0x10 + 8) * 0x200) + npc->hit.top;
		npc->ym = 0;
		hit |= 2;
	}

	npc->flag |= hit;
}

// Tiletype 0x4A
void JudgeHitNpCharJumpThruSidewayRight(NPCHAR* npc, int x, int y)
{
	int hit = 0;

	if (npc->xm >= 0 || (npc->x - npc->xm - npc->hit.back < (x * 0x10 + 8) * 0x200))
		hit |= 0;

	if (npc->y - npc->hit.top < (y * 0x10 + 4) * 0x200
		&& npc->y + npc->hit.bottom >(y * 0x10 - 4) * 0x200
		&& npc->x - npc->hit.back < (x * 0x10 + 8) * 0x200
		&& npc->x - npc->hit.back > x * 0x10 * 0x200)
	{
		// Clip
		npc->x = ((x * 0x10 + 8) * 0x200) + npc->hit.back;

		// Halt momentum
		if (npc->xm < 0)
			npc->xm = 0;

		// Set that a left wall was hit
		hit |= 1;
	}

	npc->flag |= hit;
}

// Tiletype 0x4B
void JudgeHitNpCharJumpThruSidewayLeft(NPCHAR* npc, int x, int y)
{
	int hit = 0;

	if (npc->xm <= 0 || npc->x - npc->xm + npc->hit.back >(x * 0x10 - 8) * 0x200)
		hit |= 0;

	// Right wall
	if (npc->y - npc->hit.top < (y * 0x10 + 4) * 0x200
		&& npc->y + npc->hit.bottom >(y * 0x10 - 4) * 0x200
		&& npc->x + npc->hit.back > (x * 0x10 - 8) * 0x200
		&& npc->x + npc->hit.back < x * 0x10 * 0x200)
	{
		// Clip
		npc->x = ((x * 0x10 - 8) * 0x200) - npc->hit.back;

		if (npc->xm > 0)
			npc->xm = 0;

		// Set that a right wall was hit
		hit |= 4;
	}

	npc->flag |= hit;
}

void HitNpCharMapAVA()
{
	int x, y;
	int judg;

	int offx[9];
	int offy[9];

	int i, j;

	offx[0] = 0;
	offx[1] = 1;
	offx[2] = 0;
	offx[3] = 1;
	offx[4] = 2;
	offx[5] = 2;
	offx[6] = 2;
	offx[7] = 0;
	offx[8] = 1;

	offy[0] = 0;
	offy[1] = 0;
	offy[2] = 1;
	offy[3] = 1;
	offy[4] = 0;
	offy[5] = 1;
	offy[6] = 2;
	offy[7] = 2;
	offy[8] = 2;

	for (i = 0; i < NPC_MAX; ++i)
	{
		if (!(gNPC[i].cond & 0x80))
			continue;

		if (gNPC[i].bits & NPC_IGNORE_SOLIDITY)
			continue;

		if (gNPC[i].size >= 3)
		{
			judg = 9;
			x = (gNPC[i].x - 0x1000) / 0x10 / 0x200;
			y = (gNPC[i].y - 0x1000) / 0x10 / 0x200;
		}
		else
		{
			judg = 4;
			x = gNPC[i].x / 0x10 / 0x200;
			y = gNPC[i].y / 0x10 / 0x200;
		}

		for (j = 0; j < judg; ++j)
		{
			switch (GetAttribute(x + offx[j], y + offy[j]))
			{
				// No NPC block
			case 0x44:
				if (gNPC[i].bits & NPC_IGNORE_TILE_44)
					break;
				// Fallthrough

			// Bounce Block
			case 0x47:
				JadgeHitNpCharBlock(&gNPC[i], x + offx[j], y + offy[j]);
				break;

				// Jump Thru's
			case 0x48:
				JudgeHitNpCharJumpThru(&gNPC[i], x + offx[j], y + offy[j]);
				break;

			case 0x49:
				JudgeHitNpCharJumpThruUpsideDown(&gNPC[i], x + offx[j], y + offy[j]);
				break;

			case 0x4A:
				JudgeHitNpCharJumpThruSidewayRight(&gNPC[i], x + offx[j], y + offy[j]);
				break;

			case 0x4B:
				JudgeHitNpCharJumpThruSidewayLeft(&gNPC[i], x + offx[j], y + offy[j]);
				break;

			// Ice Block
			case 0x4C:
				JadgeHitNpCharBlock(&gNPC[i], x + offx[j], y + offy[j]);
				break;

			// Ice Slopes
			case 0x58:
				JudgeHitNpCharTriangleE(&gNPC[i], x + offx[j], y + offy[j]);
				break;

			case 0x59:
				JudgeHitNpCharTriangleF(&gNPC[i], x + offx[j], y + offy[j]);
				break;

			case 0x5A:
				JudgeHitNpCharTriangleG(&gNPC[i], x + offx[j], y + offy[j]);
				break;

			case 0x5B:
				JudgeHitNpCharTriangleH(&gNPC[i], x + offx[j], y + offy[j]);
				break;
			}
		}
	}
}