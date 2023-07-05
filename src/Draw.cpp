#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Draw.h"

#include "main.h"

#include "mod_loader.h"
#include "cave_story.h"

// Replace MakeSurfaceFile with another call
void ReplaceSurfaceID(const char* n, SurfaceID no)
{
	MakeSurface_File(n, no);
}

void Replacement_StageImageSurfaceCall(int x, int y, int s, BOOL b)
{
	MakeSurface_File("Npc\\NpcAutumnObj", SURFACE_ID_AUTUMN_OBJECTS);
	MakeSurface_File("Autumn", SURFACE_ID_AUTUMN_ITEMS);
	MakeSurface_File("Npc\\NpcAutumnChar", SURFACE_ID_AUTUMN_CHARACTERS);
	MakeSurface_Generic(427, 240, s, b);
}

void Replacement_PutMyLife(BOOL flash)
{
	RECT rcCase = { 0, 40, 232, 48 };
	RECT rcLife = { 0, 24, 232, 32 };
	RECT rcBr = { 0, 32, 232, 40 };

	if (flash == TRUE && gMC->shock / 2 % 2)
		return;

	if (gMC->lifeBr < gMC->life)
		gMC->lifeBr = gMC->life;

	if (gMC->lifeBr > gMC->life)
	{
		if (++gMC->lifeBr_count > 30)
			--gMC->lifeBr;
	}
	else
	{
		gMC->lifeBr_count = 0;
	}

	// Draw bar
	rcCase.right = 64;
	rcLife.right = ((gMC->life * 40) / gMC->max_life) - 1;
	rcBr.right = ((gMC->lifeBr * 40) / gMC->max_life) - 1;

	PutBitmap3(&grcGame, 16, 40, &rcCase, SURFACE_ID_TEXT_BOX);
	PutBitmap3(&grcGame, 40, 40, &rcBr, SURFACE_ID_TEXT_BOX);
	PutBitmap3(&grcGame, 40, 40, &rcLife, SURFACE_ID_TEXT_BOX);
	PutNumber4(8, 40, gMC->lifeBr, FALSE);
	PutNumber4(8, 48, gMC->boost_cnt, FALSE);
}