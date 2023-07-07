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
#include "MyChar.h"

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