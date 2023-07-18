#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "GenericLoad.h"

#include "main.h"
#include "mod_loader.h"
#include "cave_story.h"
#include "SurfaceDefines.h"
#include "TextScript.h"

// okay so this is probably the dumbest decision but idk what else to do, we're going to replace every single MakeSurface_File call and figure out the name and stuff lol

// 0x4113BB
void Replacement_LoadGenericData_MyChar_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_16_MyChar, surf);
}

// 0x4113D5
void Replacement_LoadGenericData_Title_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_0_Title, surf);
}

// 0x4113EF
void Replacement_LoadGenericData_ArmsImage_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_12_ArmsImage, surf);
}

// 0x411409
void Replacement_LoadGenericData_Arms_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_11_Arms, surf);
}

// 0x411423
void Replacement_LoadGenericData_ItemImage_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_8_ItemImage, surf);
}

// 0x41143D
void Replacement_LoadGenericData_StageImageSurface_Call(const char* name, SurfaceID surf)
{
	// Custom
	MakeSurface_File(surfaceName_5_Image, SURFACE_ID_TSC_IMG);
	MakeSurface_File(surfaceName_24_AutumnUI, SURFACE_ID_AUTUMN_HUD);
	MakeSurface_File(surfaceName_25_AutumnObjects, SURFACE_ID_AUTUMN_OBJECTS);
	MakeSurface_File(surfaceName_38_AutumnItems, SURFACE_ID_AUTUMN_ITEMS);
	MakeSurface_File(surfaceName_39_AutumnCharacters, SURFACE_ID_AUTUMN_CHARACTERS);
	MakeSurface_File(surfaceName_14_StageImage, surf);
}

// 0x411457
void Replacement_LoadGenericData_NpcSym_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_20_NpcSym, surf);
}

// 0x411471
void Replacement_LoadGenericData_NpcRegu_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_23_NpcRegu, surf);
}

// 0x41148B
void Replacement_LoadGenericData_TextBox_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_26_TextBox, surf);
}

// 0x4114A5
void Replacement_LoadGenericData_Caret_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_19_Caret, surf);
}

// 0x4114BF
void Replacement_LoadGenericData_Bullet_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_17_Bullet, surf);
}

// 0x4114D9
void Replacement_LoadGenericData_Face_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_27_Face, surf);
}

// 0x4114F3
void Replacement_LoadGenericData_Fade_Call(const char* name, SurfaceID surf)
{
	MakeSurface_File(surfaceName_6_Fade, surf);
}

void Replacement_LoadGenericData_Calls()
{
	ModLoader_WriteCall((void*)0x4113BB, (void*)Replacement_LoadGenericData_MyChar_Call);
	ModLoader_WriteCall((void*)0x4113D5, (void*)Replacement_LoadGenericData_Title_Call);
	ModLoader_WriteCall((void*)0x4113EF, (void*)Replacement_LoadGenericData_ArmsImage_Call);
	ModLoader_WriteCall((void*)0x411409, (void*)Replacement_LoadGenericData_Arms_Call);
	ModLoader_WriteCall((void*)0x411423, (void*)Replacement_LoadGenericData_ItemImage_Call);
	ModLoader_WriteCall((void*)0x41143D, (void*)Replacement_LoadGenericData_StageImageSurface_Call);
	ModLoader_WriteCall((void*)0x411457, (void*)Replacement_LoadGenericData_NpcSym_Call);
	ModLoader_WriteCall((void*)0x411471, (void*)Replacement_LoadGenericData_NpcRegu_Call);
	ModLoader_WriteCall((void*)0x41148B, (void*)Replacement_LoadGenericData_TextBox_Call);
	ModLoader_WriteCall((void*)0x4114A5, (void*)Replacement_LoadGenericData_Caret_Call);
	ModLoader_WriteCall((void*)0x4114BF, (void*)Replacement_LoadGenericData_Bullet_Call);
	ModLoader_WriteCall((void*)0x4114D9, (void*)Replacement_LoadGenericData_Face_Call);
	ModLoader_WriteCall((void*)0x4114F3, (void*)Replacement_LoadGenericData_Fade_Call);
}