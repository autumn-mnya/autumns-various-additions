#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "TileCollisionMyChar.h"

#include "main.h"

#include "mod_loader.h"
#include "cave_story.h"

int setting_bounce_speed = 1535;
int setting_bounce_sfx = 25;


// Tiletype 0x47
int JudgeHitMyCharBouncyBlock(int x, int y)
{
	int hit = 0;

	// Left wall
	if (gMC.y - gMC.hit.top < (y * 0x10 + 4) * 0x200
		&& gMC.y + gMC.hit.bottom >(y * 0x10 - 4) * 0x200
		&& gMC.x - gMC.hit.back < (x * 0x10 + 8) * 0x200
		&& gMC.x - gMC.hit.back > x * 0x10 * 0x200)
	{
		// Clip
		gMC.x = ((x * 0x10 + 8) * 0x200) + gMC.hit.back;

		// Halt momentum
		if (gMC.xm < -0x180)
			gMC.xm = -0x180;
		if (!(gKey & gKeyLeft) && gMC.xm < 0)
			gMC.xm = 0;

		// Set that a left wall was hit
		hit |= 1;
	}

	// Right wall
	if (gMC.y - gMC.hit.top < (y * 0x10 + 4) * 0x200
		&& gMC.y + gMC.hit.bottom >(y * 0x10 - 4) * 0x200
		&& gMC.x + gMC.hit.back > (x * 0x10 - 8) * 0x200
		&& gMC.x + gMC.hit.back < x * 0x10 * 0x200)
	{
		// Clip
		gMC.x = ((x * 0x10 - 8) * 0x200) - gMC.hit.back;

		// Halt momentum
		if (gMC.xm > 0x180)
			gMC.xm = 0x180;
		if (!(gKey & gKeyRight) && gMC.xm > 0)
			gMC.xm = 0;

		// Set that a right wall was hit
		hit |= 4;
	}

	// Ceiling
	if (gMC.x - gMC.hit.back < (x * 0x10 + 5) * 0x200
		&& gMC.x + gMC.hit.back >(x * 0x10 - 5) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 + 8) * 0x200
		&& gMC.y - gMC.hit.top > y * 0x10 * 0x200)
	{
		// Clip
		gMC.y = ((y * 0x10 + 8) * 0x200) + gMC.hit.top;

		// Halt momentum
		if (!(gMC.cond & 2) && gMC.ym < -0x200)
			PutlittleStar();
		if (gMC.ym < 0)
			gMC.ym = 0;

		// Set that a ceiling was hit
		hit |= 2;
	}

	// Floor
	if (gMC.x - gMC.hit.back < (x * 0x10 + 5) * 0x200
		&& gMC.x + gMC.hit.back >((x * 0x10 - 5) * 0x200)
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 8) * 0x200
		&& gMC.y + gMC.hit.bottom < y * 0x10 * 0x200)
	{
		// Clip
		gMC.y = ((y * 0x10 - 8) * 0x200) - gMC.hit.bottom;

		// Halt momentum
		if (gMC.ym > 0x400)
			PlaySoundObject(setting_bounce_sfx, SOUND_MODE_PLAY); // If you hit the ground, play this sound

		// Set that a floor was hit
		hit |= 8;

		if (hit & 8)
		{
			gMC.ym = -setting_bounce_speed;
		}
	}

	return hit;
}

// Tiletype 0x48
int JudgeHitMyCharJumpThru(int x, int y)
{
	int hit = 0;

	if (gMC.ym < 0 || gMC.y - gMC.ym + gMC.hit.bottom >(y * 0x10 - 8) * 0x200)
		return hit;

	if (gMC.x - gMC.hit.back < (x * 0x10 + 5) * 0x200
		&& gMC.x + gMC.hit.back >((x * 0x10 - 5) * 0x200)
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 8) * 0x200
		&& gMC.y + gMC.hit.bottom < y * 0x10 * 0x200)
	{
		// Clip
		gMC.y = ((y * 0x10 - 8) * 0x200) - gMC.hit.bottom;

		// Halt momentum
		if (gMC.ym > 0x400)
			PlaySoundObject(23, SOUND_MODE_PLAY);
		if (gMC.ym > 0)
			gMC.ym = 0;

		// Set that a floor was hit
		hit |= 8;
	}

	return hit;
}

// Tiletype 0x49
int JudgeHitMyCharJumpThruUpsideDown(int x, int y)
{
	int hit = 0;

	if (gMC.ym > 0 || gMC.y + gMC.ym - gMC.hit.top < (y * 0x10 - 8) * 0x200)
		return hit;

	if (gMC.x - gMC.hit.back < (x * 0x10 + 5) * 0x200
		&& gMC.x + gMC.hit.back > (x * 0x10 - 5) * 0x200
		&& gMC.y - gMC.hit.top < (y * 0x10 + 8) * 0x200
		&& gMC.y - gMC.hit.top > y * 0x10 * 0x200)
	{
		// Clip
		gMC.y = ((y * 0x10 + 8) * 0x200) + gMC.hit.top;

		// Halt momentum
		if (!(gMC.cond & 2) && gMC.ym < -0x200)
			PutlittleStar();
		if (gMC.ym < 0)
			gMC.ym = 0;

		// Set that a ceiling was hit
		hit |= 2;
	}

	return hit;
}

// Tiletype 0x4A
int JudgeHitMyCharJumpThruSidewayRight(int x, int y)
{
	int hit = 0;

	if (gMC.xm >= 0 || (gMC.x - gMC.xm - gMC.hit.back < (x * 0x10 + 8) * 0x200))
		return hit;

	if (gMC.y - gMC.hit.top < (y * 0x10 + 4) * 0x200
		&& gMC.y + gMC.hit.bottom >(y * 0x10 - 4) * 0x200
		&& gMC.x - gMC.hit.back < (x * 0x10 + 8) * 0x200
		&& gMC.x - gMC.hit.back > x * 0x10 * 0x200)
	{
		// Clip
		gMC.x = ((x * 0x10 + 8) * 0x200) + gMC.hit.back;

		// Halt momentum
		if (gMC.xm < -0x180)
			gMC.xm = -0x180;
		if (!(gKey & gKeyLeft) && gMC.xm < 0)
			gMC.xm = 0;

		// Set that a left wall was hit
		hit |= 1;
	}

	return hit;
}

// Tiletype 0x4B
int JudgeHitMyCharJumpThruSidewayLeft(int x, int y)
{
	int hit = 0;

	if (gMC.xm <= 0 || gMC.x - gMC.xm + gMC.hit.back > (x * 0x10 - 8) * 0x200)
		return hit;

	// Right wall
	if (gMC.y - gMC.hit.top < (y * 0x10 + 4) * 0x200
		&& gMC.y + gMC.hit.bottom >(y * 0x10 - 4) * 0x200
		&& gMC.x + gMC.hit.back > (x * 0x10 - 8) * 0x200
		&& gMC.x + gMC.hit.back < x * 0x10 * 0x200)
	{
		// Clip
		gMC.x = ((x * 0x10 - 8) * 0x200) - gMC.hit.back;

		// Halt momentum
		if (gMC.xm > 0x180)
			gMC.xm = 0x180;
		if (!(gKey & gKeyRight) && gMC.xm > 0)
			gMC.xm = 0;

		// Set that a right wall was hit
		hit |= 4;
	}

	return hit;
}

int JudgeHitMyCharIce(int x, int y)
{
	int hit = 0;
	// Floor
	if (gMC.x - gMC.hit.back < (x * 0x10 + 5) * 0x200
		&& gMC.x + gMC.hit.back >((x * 0x10 - 5) * 0x200)
		&& gMC.y + gMC.hit.bottom > (y * 0x10 - 8) * 0x200
		&& gMC.y + gMC.hit.bottom < y * 0x10 * 0x200)
	{
		// Set that a floor was hit
		hit |= 0x200;
	}

	return hit;
}

void HitMyCharMapAVA()
{
	int x, y;
	int i;
	unsigned char atrb[4];

	x = gMC.x / 0x10 / 0x200;
	y = gMC.y / 0x10 / 0x200;

	int offx[4];
	int offy[4];

	offx[0] = 0;
	offx[1] = 1;
	offx[2] = 0;
	offx[3] = 1;

	offy[0] = 0;
	offy[1] = 0;
	offy[2] = 1;
	offy[3] = 1;

	for (i = 0; i < 4; ++i)
	{
		atrb[i] = GetAttribute(x + offx[i], y + offy[i]);

		switch (atrb[i])
		{
			// New Bouncy Block
		case 0x47:
			gMC.flag |= JudgeHitMyCharBouncyBlock(x + offx[i], y + offy[i]);
			break;

			// Jump Thru's
		case 0x48:
			gMC.flag |= JudgeHitMyCharJumpThru(x + offx[i], y + offy[i]);
			break;

		case 0x49:
			gMC.flag |= JudgeHitMyCharJumpThruUpsideDown(x + offx[i], y + offy[i]);
			break;

		case 0x4A:
			gMC.flag |= JudgeHitMyCharJumpThruSidewayRight(x + offx[i], y + offy[i]);
			break;

		case 0x4B:
			gMC.flag |= JudgeHitMyCharJumpThruSidewayLeft(x + offx[i], y + offy[i]);
			break;

		case 0x4C:
			gMC.flag |= JudgeHitMyCharIce(x + offx[i], y + offy[i]);
			gMC.flag |= JudgeHitMyCharBlock(x + offx[i], y + offy[i]);
			break;

			// Ice Slopes
		case 0x58:
			gMC.flag |= JudgeHitMyCharIce(x + offx[i], y + offy[i]);
			gMC.flag |= JudgeHitMyCharTriangleE(x + offx[i], y + offy[i]); // Slope Left ground
			break;

		case 0x59:
			gMC.flag |= JudgeHitMyCharIce(x + offx[i], y + offy[i]);
			gMC.flag |= JudgeHitMyCharTriangleF(x + offx[i], y + offy[i]); // Slope Left 2 ground
			break;

		case 0x5A:
			gMC.flag |= JudgeHitMyCharIce(x + offx[i], y + offy[i]);
			gMC.flag |= JudgeHitMyCharTriangleG(x + offx[i], y + offy[i]); // Slope Right ground
			break;

		case 0x5B:
			gMC.flag |= JudgeHitMyCharIce(x + offx[i], y + offy[i]);
			gMC.flag |= JudgeHitMyCharTriangleH(x + offx[i], y + offy[i]); // Slope Right 2 ground
			break;

			// Jump Thru's Water
		case 0x65:
			gMC.flag |= JudgeHitMyCharJumpThru(x + offx[i], y + offy[i]);
			gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
			break;

		case 0x66:
			gMC.flag |= JudgeHitMyCharJumpThruUpsideDown(x + offx[i], y + offy[i]);
			gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
			break;

		case 0x67:
			gMC.flag |= JudgeHitMyCharJumpThruSidewayRight(x + offx[i], y + offy[i]);
			gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
			break;

		case 0x68:
			gMC.flag |= JudgeHitMyCharJumpThruSidewayLeft(x + offx[i], y + offy[i]);
			gMC.flag |= JudgeHitMyCharWater(x + offx[i], y + offy[i]);
			break;
		}
	}
}