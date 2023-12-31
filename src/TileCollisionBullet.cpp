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

static void Vanish(BULLET* bul)
{
	if (bul->code_bullet != 37 && bul->code_bullet != 38 && bul->code_bullet != 39)
		PlaySoundObject(28, SOUND_MODE_PLAY);
	else
		SetCaret(bul->x, bul->y, CARET_PROJECTILE_DISSIPATION, DIR_UP);

	bul->cond = 0;
	SetCaret(bul->x, bul->y, CARET_PROJECTILE_DISSIPATION, DIR_RIGHT);
}

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

int Replacement_JudgeHitBulletBlock2(int x, int y, unsigned char* atrb, BULLET* bul)
{
	BOOL block[4];
	int workX, workY;
	int hit, i;

	hit = 0;

	if (bul->bbits & 0x40)
	{
		for (i = 0; i < 4; ++i)
		{
			if (*atrb == 0x41 || *atrb == 0x61 || *atrb == 0x47 || *atrb == 0x4C)
				block[i] = TRUE;
			else
				block[i] = FALSE;

			++atrb;
		}
	}
	else
	{
		for (i = 0; i < 4; ++i)
		{
			if (*atrb == 0x41 || *atrb == 0x43 || *atrb == 0x61 || *atrb == 0x47 || *atrb == 0x4C)
				block[i] = TRUE;
			else
				block[i] = FALSE;

			++atrb;
		}
	}

	workX = ((x * 16) + 8) * 0x200;
	workY = ((y * 16) + 8) * 0x200;

	// Left wall
	if (block[0] && block[2])
	{
		if (bul->x - bul->blockXL < workX)
			hit |= 1;
	}
	else if (block[0] && !block[2])
	{
		if (bul->x - bul->blockXL < workX && bul->y - bul->blockYL < workY - (3 * 0x200))
			hit |= 1;
	}
	else if (!block[0] && block[2])
	{
		if (bul->x - bul->blockXL < workX && bul->y + bul->blockYL > workY + (3 * 0x200))
			hit |= 1;
	}

	// Right wall
	if (block[1] && block[3])
	{
		if (bul->x + bul->blockXL > workX)
			hit |= 4;
	}
	else if (block[1] && !block[3])
	{
		if (bul->x + bul->blockXL > workX && bul->y - bul->blockYL < workY - (3 * 0x200))
			hit |= 4;
	}
	else if (!block[1] && block[3])
	{
		if (bul->x + bul->blockXL > workX && bul->y + bul->blockYL > workY + (3 * 0x200))
			hit |= 4;
	}

	// Ceiling
	if (block[0] && block[1])
	{
		if (bul->y - bul->blockYL < workY)
			hit |= 2;
	}
	else if (block[0] && !block[1])
	{
		if (bul->y - bul->blockYL < workY && bul->x - bul->blockXL < workX - (3 * 0x200))
			hit |= 2;
	}
	else if (!block[0] && block[1])
	{
		if (bul->y - bul->blockYL < workY && bul->x + bul->blockXL > workX + (3 * 0x200))
			hit |= 2;
	}

	// Ground
	if (block[2] && block[3])
	{
		if (bul->y + bul->blockYL > workY)
			hit |= 8;
	}
	else if (block[2] && !block[3])
	{
		if (bul->y + bul->blockYL > workY && bul->x - bul->blockXL < workX - (3 * 0x200))
			hit |= 8;
	}
	else if (!block[2] && block[3])
	{
		if (bul->y + bul->blockYL > workY && bul->x + bul->blockXL > workX + (3 * 0x200))
			hit |= 8;
	}

	// Clip
	if (bul->bbits & 8)
	{
		if (hit & 1)
			bul->x = workX + bul->blockXL;
		else if (hit & 4)
			bul->x = workX - bul->blockXL;
		else if (hit & 2)
			bul->y = workY + bul->blockYL;
		else if (hit & 8)
			bul->y = workY - bul->blockYL;
	}
	else
	{
		if (hit & 0xF)
			Vanish(bul);
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
				case 0x61:
				case 0x64:
					gBul[i].flag |= JudgeHitBulletBlock(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x47:
				case 0x48:
				case 0x49:
				case 0x4A:
				case 0x4B:
				case 0x4C: // This is an ice tile !! it should have block collision
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

				// Includes Ice Slopes
				case 0x54:
				case 0x58:
				case 0x74:
					gBul[i].flag |= JudgeHitBulletTriangleE(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x55:
				case 0x59:
				case 0x75:
					gBul[i].flag |= JudgeHitBulletTriangleF(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x56:
				case 0x5A:
				case 0x76:
					gBul[i].flag |= JudgeHitBulletTriangleG(x + offx[j], y + offy[j], &gBul[i]);
					break;

				case 0x57:
				case 0x5B:
				case 0x77:
					gBul[i].flag |= JudgeHitBulletTriangleH(x + offx[j], y + offy[j], &gBul[i]);
					break;
				}
			}

			gBul[i].flag |= JudgeHitBulletBlock2(x, y, atrb, &gBul[i]);
		}
	}
}