#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "TileCollisionBoss.h"
#include "TileCollisionEntity.h"

#include "main.h"

#include "mod_loader.h"
#include "cave_story.h"

void Replacement_HitBossMap(void)
{
	int x, y;
	unsigned char atrb[16];
	int judg;
	int offx[16];
	int offy[16];
	int b, j;

	offx[0] = 0;
	offx[1] = 1;
	offx[2] = 0;
	offx[3] = 1;
	offx[4] = 2;
	offx[5] = 2;
	offx[6] = 2;
	offx[7] = 0;
	offx[8] = 1;
	offx[9] = -1;
	offx[10] = -1;
	offx[11] = -1;
	offx[12] = -1;
	offx[13] = 0;
	offx[14] = 1;
	offx[15] = 2;

	offy[0] = 0;
	offy[1] = 0;
	offy[2] = 1;
	offy[3] = 1;
	offy[4] = 0;
	offy[5] = 1;
	offy[6] = 2;
	offy[7] = 2;
	offy[8] = 2;
	offy[9] = -1;
	offy[10] = 0;
	offy[11] = 1;
	offy[12] = 2;
	offy[13] = -1;
	offy[14] = -1;
	offy[15] = -1;

	for (b = 0; b < BOSS_MAX; ++b)
	{
		if (!(gBoss[b].cond & 0x80))
			continue;

		if (gBoss[b].bits & NPC_IGNORE_SOLIDITY)
			continue;

		if (gBoss[b].size >= 3)
		{
			judg = 16;
			x = gBoss[b].x / 0x10 / 0x200;
			y = gBoss[b].y / 0x10 / 0x200;
		}
		else
		{
			judg = 4;
			x = gBoss[b].x / 0x10 / 0x200;
			y = gBoss[b].y / 0x10 / 0x200;
		}

		gBoss[b].flag = 0;
		for (j = 0; j < judg; ++j)
		{
			atrb[j] = GetAttribute(x + offx[j], y + offy[j]);

			switch (atrb[j])
			{
			case 0x44:
				if (gBoss[b].bits & NPC_IGNORE_TILE_44)
					break;
				// Fallthrough
			case 0x05:
			case 0x41:
			case 0x43:
				JadgeHitNpCharBlock(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x47:
				JadgeHitNpCharBlock(&gBoss[b], x + offx[j], y + offy[j]);
				break;
				// Jump Thru's
			case 0x48:
				JudgeHitNpCharJumpThru(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x49:
				JudgeHitNpCharJumpThruUpsideDown(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x4A:
				JudgeHitNpCharJumpThruSidewayRight(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x4B:
				JudgeHitNpCharJumpThruSidewayLeft(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			// Ice Block
			case 0x4C:
				JadgeHitNpCharBlock(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x50:
				JudgeHitNpCharTriangleA(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x51:
				JudgeHitNpCharTriangleB(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x52:
				JudgeHitNpCharTriangleC(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x53:
				JudgeHitNpCharTriangleD(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x54:
				JudgeHitNpCharTriangleE(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x55:
				JudgeHitNpCharTriangleF(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x56:
				JudgeHitNpCharTriangleG(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x57:
				JudgeHitNpCharTriangleH(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x58:
				JudgeHitNpCharTriangleE(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x59:
				JudgeHitNpCharTriangleF(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x5A:
				JudgeHitNpCharTriangleG(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x5B:
				JudgeHitNpCharTriangleH(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x02:
			case 0x60:
			case 0x61:
			case 0x64:
				JadgeHitNpCharBlock(&gBoss[b], x + offx[j], y + offy[j]);
				JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x70:
				JudgeHitNpCharTriangleA(&gBoss[b], x + offx[j], y + offy[j]);
				JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x71:
				JudgeHitNpCharTriangleB(&gBoss[b], x + offx[j], y + offy[j]);
				JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x72:
				JudgeHitNpCharTriangleC(&gBoss[b], x + offx[j], y + offy[j]);
				JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x73:
				JudgeHitNpCharTriangleD(&gBoss[b], x + offx[j], y + offy[j]);
				JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x74:
				JudgeHitNpCharTriangleE(&gBoss[b], x + offx[j], y + offy[j]);
				JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x75:
				JudgeHitNpCharTriangleF(&gBoss[b], x + offx[j], y + offy[j]);
				JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x76:
				JudgeHitNpCharTriangleG(&gBoss[b], x + offx[j], y + offy[j]);
				JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			case 0x77:
				JudgeHitNpCharTriangleH(&gBoss[b], x + offx[j], y + offy[j]);
				JudgeHitNpCharWater(&gBoss[b], x + offx[j], y + offy[j]);
				break;
			}
		}
	}
}