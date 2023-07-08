#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "TileCollisionBullet.h"

#include "main.h"

#include "mod_loader.h"
#include "cave_story.h"

int JudgeHitBulletJumpThru(int x, int y, BULLET* bul)
{
	int hit = 0;

	if (bul->ym < 0 || bul->y - bul->ym + bul->blockYL >(y * 0x10 - 8) * 0x200)
		return hit;

	if (bul->x - bul->blockXL < (x * 0x10 + 5) * 0x200
		&& bul->x + bul->blockXL >((x * 0x10 - 5) * 0x200)
		&& bul->y + bul->blockYL > (y * 0x10 - 8) * 0x200
		&& bul->y + bul->blockYL < y * 0x10 * 0x200)
	{
		// Clip
		bul->y = ((y * 0x10 - 8) * 0x200) - bul->blockYL;

		// Set that a floor was hit
		hit |= 0x200;
	}

	return hit;
}

void Replacement_HitBulletMap(void)
{
	int x, y;
	unsigned char atrb[4];

	int i, j;

	for (i = 0; i < BULLET_MAX; ++i)
	{
		int offx[4];
		int offy[4];

		if (!(gBul[i].cond & 0x80))
			continue;

		x = gBul[i].x / 0x10 / 0x200;
		y = gBul[i].y / 0x10 / 0x200;

		// Get surrounding tiles
		offx[0] = 0;
		offx[1] = 1;
		offx[2] = 0;
		offx[3] = 1;
		offy[0] = 0;
		offy[1] = 0;
		offy[2] = 1;
		offy[3] = 1;

		atrb[0] = GetAttribute(x, y);
		atrb[1] = GetAttribute(x + 1, y);
		atrb[2] = GetAttribute(x, y + 1);
		atrb[3] = GetAttribute(x + 1, y + 1);

		// Clear hit tiles
		gBul[i].flag = 0;

		if (gBul[i].bbits & 4)
		{
			// There probably used to be commented-out code here
		}
		else
		{
			for (j = 0; j < 4; ++j)
			{
				if (!(gBul[i].cond & 0x80))
					continue;

				switch (atrb[j])
				{
				case 0x41:
				case 0x43:
				case 0x44:
				case 0x47:
				case 0x48:
				case 0x49:
				case 0x4A:
				case 0x4B:
				case 0x61:
				case 0x64:
					gBul[i].flag |= JudgeHitBulletBlock(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x50:
				case 0x70:
					gBul[i].flag |= JudgeHitBulletTriangleA(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x51:
				case 0x71:
					gBul[i].flag |= JudgeHitBulletTriangleB(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x52:
				case 0x72:
					gBul[i].flag |= JudgeHitBulletTriangleC(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x53:
				case 0x73:
					gBul[i].flag |= JudgeHitBulletTriangleD(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x54:
				case 0x74:
					gBul[i].flag |= JudgeHitBulletTriangleE(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x55:
				case 0x75:
					gBul[i].flag |= JudgeHitBulletTriangleF(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x56:
				case 0x76:
					gBul[i].flag |= JudgeHitBulletTriangleG(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x57:
				case 0x77:
					gBul[i].flag |= JudgeHitBulletTriangleH(x + offx[j], y + offy[j], &gBul[i]);
					break;
				}
			}

			gBul[i].flag |= JudgeHitBulletBlock2(x, y, atrb, &gBul[i]);
		}
	}
}