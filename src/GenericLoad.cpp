#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "GenericLoad.h"

#include "main.h"
#include "ModSettings.h"
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

void ReloadAllCustomGenericData()
{
	ReleaseSurface(SURFACE_ID_TITLE);
	MakeSurface_File(surfaceName_0_Title, SURFACE_ID_TITLE);

	ReleaseSurface(SURFACE_ID_TSC_IMG);
	MakeSurface_File(surfaceName_5_Image, SURFACE_ID_TSC_IMG);

	ReleaseSurface(SURFACE_ID_FADE);
	MakeSurface_File(surfaceName_6_Fade, SURFACE_ID_FADE);

	ReleaseSurface(SURFACE_ID_ITEM_IMAGE);
	MakeSurface_File(surfaceName_8_ItemImage, SURFACE_ID_ITEM_IMAGE);

	ReleaseSurface(SURFACE_ID_ARMS);
	MakeSurface_File(surfaceName_11_Arms, SURFACE_ID_ARMS);

	ReleaseSurface(SURFACE_ID_ARMS_IMAGE);
	MakeSurface_File(surfaceName_12_ArmsImage, SURFACE_ID_ARMS_IMAGE);

	ReleaseSurface(SURFACE_ID_STAGE_ITEM);
	MakeSurface_File(surfaceName_14_StageImage, SURFACE_ID_STAGE_ITEM);

	ReleaseSurface(SURFACE_ID_MY_CHAR);
	MakeSurface_File(surfaceName_16_MyChar, SURFACE_ID_MY_CHAR);

	ReleaseSurface(SURFACE_ID_BULLET);
	MakeSurface_File(surfaceName_17_Bullet, SURFACE_ID_BULLET);

	ReleaseSurface(SURFACE_ID_BULLET);
	MakeSurface_File(surfaceName_17_Bullet, SURFACE_ID_BULLET);

	ReleaseSurface(SURFACE_ID_CARET);
	MakeSurface_File(surfaceName_19_Caret, SURFACE_ID_CARET);

	ReleaseSurface(SURFACE_ID_NPC_SYM);
	MakeSurface_File(surfaceName_20_NpcSym, SURFACE_ID_NPC_SYM);

	ReleaseSurface(SURFACE_ID_NPC_REGU);
	MakeSurface_File(surfaceName_23_NpcRegu, SURFACE_ID_NPC_REGU);

	ReleaseSurface(SURFACE_ID_AUTUMN_HUD);
	MakeSurface_File(surfaceName_24_AutumnUI, SURFACE_ID_AUTUMN_HUD);

	ReleaseSurface(SURFACE_ID_AUTUMN_OBJECTS);
	MakeSurface_File(surfaceName_25_AutumnObjects, SURFACE_ID_AUTUMN_OBJECTS);

	ReleaseSurface(SURFACE_ID_TEXT_BOX);
	MakeSurface_File(surfaceName_26_TextBox, SURFACE_ID_TEXT_BOX);

	ReleaseSurface(SURFACE_ID_FACE);
	MakeSurface_File(surfaceName_27_Face, SURFACE_ID_FACE);

	ReleaseSurface(SURFACE_ID_AUTUMN_ITEMS);
	MakeSurface_File(surfaceName_38_AutumnItems, SURFACE_ID_AUTUMN_ITEMS);

	ReleaseSurface(SURFACE_ID_AUTUMN_CHARACTERS);
	MakeSurface_File(surfaceName_39_AutumnCharacters, SURFACE_ID_AUTUMN_CHARACTERS);
}

void LoadCustomGenericData()
{
	// Here we compare the current surface name string to the string that was used when starting the game, and if they're different, we reload the surfaces.
	char CollabPath[MAX_PATH];
	memset(CollabPath, 0, sizeof(CollabPath));

	// Title
	if (!(strcmp(surfaceName_0_Title, init_surfaceName_0_Title) == 0))
	{
		ReleaseSurface(SURFACE_ID_TITLE);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_0_Title);
			MakeSurface_File(CollabPath, SURFACE_ID_TITLE);
		}
		else
			MakeSurface_File(surfaceName_0_Title, SURFACE_ID_TITLE);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// <IMG
	if (!(strcmp(surfaceName_5_Image, init_surfaceName_5_Image) == 0))
	{
		ReleaseSurface(SURFACE_ID_TSC_IMG);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_5_Image);
			MakeSurface_File(CollabPath, SURFACE_ID_TSC_IMG);
		}
		else
			MakeSurface_File(surfaceName_5_Image, SURFACE_ID_TSC_IMG);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// Fade
	if (!(strcmp(surfaceName_6_Fade, init_surfaceName_6_Fade) == 0))
	{
		ReleaseSurface(SURFACE_ID_FADE);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_6_Fade);
			MakeSurface_File(CollabPath, SURFACE_ID_FADE);
		}
		else
			MakeSurface_File(surfaceName_6_Fade, SURFACE_ID_FADE);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// ItemImage
	if (!(strcmp(surfaceName_8_ItemImage, init_surfaceName_8_ItemImage) == 0))
	{
		ReleaseSurface(SURFACE_ID_ITEM_IMAGE);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_8_ItemImage);
			MakeSurface_File(CollabPath, SURFACE_ID_ITEM_IMAGE);
		}
		else
			MakeSurface_File(surfaceName_8_ItemImage, SURFACE_ID_ITEM_IMAGE);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// Arms
	if (!(strcmp(surfaceName_11_Arms, init_surfaceName_11_Arms) == 0))
	{
		ReleaseSurface(SURFACE_ID_ARMS);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_11_Arms);
			MakeSurface_File(CollabPath, SURFACE_ID_ARMS);
		}
		else
			MakeSurface_File(surfaceName_11_Arms, SURFACE_ID_ARMS);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// ArmsImage
	if (!(strcmp(surfaceName_12_ArmsImage, init_surfaceName_12_ArmsImage) == 0))
	{
		ReleaseSurface(SURFACE_ID_ARMS_IMAGE);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_12_ArmsImage);
			MakeSurface_File(CollabPath, SURFACE_ID_ARMS_IMAGE);
		}
		else
			MakeSurface_File(surfaceName_12_ArmsImage, SURFACE_ID_ARMS_IMAGE);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// StageImage
	if (!(strcmp(surfaceName_14_StageImage, init_surfaceName_14_StageImage) == 0))
	{
		ReleaseSurface(SURFACE_ID_STAGE_ITEM);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_14_StageImage);
			MakeSurface_File(CollabPath, SURFACE_ID_STAGE_ITEM);
		}
		else
			MakeSurface_File(surfaceName_14_StageImage, SURFACE_ID_STAGE_ITEM);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// Loading is skipped (for now? i dunno lol)

	// MyChar
	if (!(strcmp(surfaceName_16_MyChar, init_surfaceName_16_MyChar) == 0))
	{
		ReleaseSurface(SURFACE_ID_MY_CHAR);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_16_MyChar);
			MakeSurface_File(CollabPath, SURFACE_ID_MY_CHAR);
		}
		else
			MakeSurface_File(surfaceName_16_MyChar, SURFACE_ID_MY_CHAR);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// Bullet
	if (!(strcmp(surfaceName_17_Bullet, init_surfaceName_17_Bullet) == 0))
	{
		ReleaseSurface(SURFACE_ID_BULLET);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_17_Bullet);
			MakeSurface_File(CollabPath, SURFACE_ID_BULLET);
		}
		else
			MakeSurface_File(surfaceName_17_Bullet, SURFACE_ID_BULLET);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// Caret
	if (!(strcmp(surfaceName_19_Caret, init_surfaceName_19_Caret) == 0))
	{
		ReleaseSurface(SURFACE_ID_CARET);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_19_Caret);
			MakeSurface_File(CollabPath, SURFACE_ID_CARET);
		}
		else
			MakeSurface_File(surfaceName_19_Caret, SURFACE_ID_CARET);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// NpcSym
	if (!(strcmp(surfaceName_20_NpcSym, init_surfaceName_20_NpcSym) == 0))
	{
		ReleaseSurface(SURFACE_ID_NPC_SYM);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_20_NpcSym);
			MakeSurface_File(CollabPath, SURFACE_ID_NPC_SYM);
		}
		else
			MakeSurface_File(surfaceName_20_NpcSym, SURFACE_ID_NPC_SYM);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// NpcRegu
	if (!(strcmp(surfaceName_23_NpcRegu, init_surfaceName_23_NpcRegu) == 0))
	{
		ReleaseSurface(SURFACE_ID_NPC_REGU);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_23_NpcRegu);
			MakeSurface_File(CollabPath, SURFACE_ID_NPC_REGU);
		}
		else
			MakeSurface_File(surfaceName_23_NpcRegu, SURFACE_ID_NPC_REGU);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// AutumnUI
	if (!(strcmp(surfaceName_24_AutumnUI, init_surfaceName_24_AutumnUI) == 0))
	{
		ReleaseSurface(SURFACE_ID_AUTUMN_HUD);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_24_AutumnUI);
			MakeSurface_File(CollabPath, SURFACE_ID_AUTUMN_HUD);
		}
		else
			MakeSurface_File(surfaceName_24_AutumnUI, SURFACE_ID_AUTUMN_HUD);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// NpcAutumnObj
	if (!(strcmp(surfaceName_25_AutumnObjects, init_surfaceName_25_AutumnObjects) == 0))
	{
		ReleaseSurface(SURFACE_ID_AUTUMN_OBJECTS);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_25_AutumnObjects);
			MakeSurface_File(CollabPath, SURFACE_ID_AUTUMN_OBJECTS);
		}
		else
			MakeSurface_File(surfaceName_25_AutumnObjects, SURFACE_ID_AUTUMN_OBJECTS);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// TextBox
	if (!(strcmp(surfaceName_26_TextBox, init_surfaceName_26_TextBox) == 0))
	{
		ReleaseSurface(SURFACE_ID_TEXT_BOX);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_26_TextBox);
			MakeSurface_File(CollabPath, SURFACE_ID_TEXT_BOX);
		}
		else
			MakeSurface_File(surfaceName_26_TextBox, SURFACE_ID_TEXT_BOX);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// Face
	if (!(strcmp(surfaceName_27_Face, init_surfaceName_27_Face) == 0))
	{
		ReleaseSurface(SURFACE_ID_FACE);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_27_Face);
			MakeSurface_File(CollabPath, SURFACE_ID_FACE);
		}
		else
			MakeSurface_File(surfaceName_27_Face, SURFACE_ID_FACE);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// Autumn
	if (!(strcmp(surfaceName_38_AutumnItems, init_surfaceName_38_AutumnItems) == 0))
	{
		ReleaseSurface(SURFACE_ID_AUTUMN_ITEMS);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_38_AutumnItems);
			MakeSurface_File(CollabPath, SURFACE_ID_AUTUMN_ITEMS);
		}
		else
			MakeSurface_File(surfaceName_38_AutumnItems, SURFACE_ID_AUTUMN_ITEMS);
	}

	memset(CollabPath, 0, sizeof(CollabPath));

	// NpcAutumnChar
	if (!(strcmp(surfaceName_39_AutumnCharacters, init_surfaceName_39_AutumnCharacters) == 0))
	{
		ReleaseSurface(SURFACE_ID_AUTUMN_CHARACTERS);
		if (setting_collab_enabled == true)
		{
			sprintf(CollabPath, "%s\\%s", setting_collab_name, surfaceName_39_AutumnCharacters);
			MakeSurface_File(CollabPath, SURFACE_ID_AUTUMN_CHARACTERS);
		}
		else
			MakeSurface_File(surfaceName_39_AutumnCharacters, SURFACE_ID_AUTUMN_CHARACTERS);
	}
}

void ResetCustomGenericData()
{
	// Here we compare the current surface name string to the string that was used when starting the game, and if they're different, we reload the surfaces.

	// Title
	if (!(strcmp(surfaceName_0_Title, init_surfaceName_0_Title) == 0))
	{
		ReleaseSurface(SURFACE_ID_TITLE);
		MakeSurface_File(init_surfaceName_0_Title, SURFACE_ID_TITLE);
	}

	// <IMG
	if (!(strcmp(surfaceName_5_Image, init_surfaceName_5_Image) == 0))
	{
		ReleaseSurface(SURFACE_ID_TSC_IMG);
		MakeSurface_File(init_surfaceName_5_Image, SURFACE_ID_TSC_IMG);
	}

	// Fade
	if (!(strcmp(surfaceName_6_Fade, init_surfaceName_6_Fade) == 0))
	{
		ReleaseSurface(SURFACE_ID_FADE);
		MakeSurface_File(init_surfaceName_6_Fade, SURFACE_ID_FADE);
	}

	// ItemImage
	if (!(strcmp(surfaceName_8_ItemImage, init_surfaceName_8_ItemImage) == 0))
	{
		ReleaseSurface(SURFACE_ID_ITEM_IMAGE);
		MakeSurface_File(init_surfaceName_8_ItemImage, SURFACE_ID_ITEM_IMAGE);
	}

	// Arms
	if (!(strcmp(surfaceName_11_Arms, init_surfaceName_11_Arms) == 0))
	{
		ReleaseSurface(SURFACE_ID_ARMS);
		MakeSurface_File(init_surfaceName_11_Arms, SURFACE_ID_ARMS);
	}

	// ArmsImage
	if (!(strcmp(surfaceName_12_ArmsImage, init_surfaceName_12_ArmsImage) == 0))
	{
		ReleaseSurface(SURFACE_ID_ARMS_IMAGE);
		MakeSurface_File(init_surfaceName_12_ArmsImage, SURFACE_ID_ARMS_IMAGE);
	}

	// StageImage
	if (!(strcmp(surfaceName_14_StageImage, init_surfaceName_14_StageImage) == 0))
	{
		ReleaseSurface(SURFACE_ID_STAGE_ITEM);
		MakeSurface_File(init_surfaceName_14_StageImage, SURFACE_ID_STAGE_ITEM);
	}

	// Loading is skipped (for now? i dunno lol)

	// MyChar
	if (!(strcmp(surfaceName_16_MyChar, init_surfaceName_16_MyChar) == 0))
	{
		ReleaseSurface(SURFACE_ID_MY_CHAR);
		MakeSurface_File(init_surfaceName_16_MyChar, SURFACE_ID_MY_CHAR);
	}

	// Bullet
	if (!(strcmp(surfaceName_17_Bullet, init_surfaceName_17_Bullet) == 0))
	{
		ReleaseSurface(SURFACE_ID_BULLET);
		MakeSurface_File(init_surfaceName_17_Bullet, SURFACE_ID_BULLET);
	}

	// Caret
	if (!(strcmp(surfaceName_19_Caret, init_surfaceName_19_Caret) == 0))
	{
		ReleaseSurface(SURFACE_ID_CARET);
		MakeSurface_File(init_surfaceName_19_Caret, SURFACE_ID_CARET);
	}

	// NpcSym
	if (!(strcmp(surfaceName_20_NpcSym, init_surfaceName_20_NpcSym) == 0))
	{
		ReleaseSurface(SURFACE_ID_NPC_SYM);
		MakeSurface_File(init_surfaceName_20_NpcSym, SURFACE_ID_NPC_SYM);
	}

	// NpcRegu
	if (!(strcmp(surfaceName_23_NpcRegu, init_surfaceName_23_NpcRegu) == 0))
	{
		ReleaseSurface(SURFACE_ID_NPC_REGU);
		MakeSurface_File(init_surfaceName_23_NpcRegu, SURFACE_ID_NPC_REGU);
	}

	// AutumnUI
	if (!(strcmp(surfaceName_24_AutumnUI, init_surfaceName_24_AutumnUI) == 0))
	{
		ReleaseSurface(SURFACE_ID_AUTUMN_HUD);
		MakeSurface_File(init_surfaceName_24_AutumnUI, SURFACE_ID_AUTUMN_HUD);
	}

	// NpcAutumnObj
	if (!(strcmp(surfaceName_25_AutumnObjects, init_surfaceName_25_AutumnObjects) == 0))
	{
		ReleaseSurface(SURFACE_ID_AUTUMN_OBJECTS);
		MakeSurface_File(init_surfaceName_25_AutumnObjects, SURFACE_ID_AUTUMN_OBJECTS);
	}

	// TextBox
	if (!(strcmp(surfaceName_26_TextBox, init_surfaceName_26_TextBox) == 0))
	{
		ReleaseSurface(SURFACE_ID_TEXT_BOX);
		MakeSurface_File(init_surfaceName_26_TextBox, SURFACE_ID_TEXT_BOX);
	}

	// Face
	if (!(strcmp(surfaceName_27_Face, init_surfaceName_27_Face) == 0))
	{
		ReleaseSurface(SURFACE_ID_FACE);
		MakeSurface_File(init_surfaceName_27_Face, SURFACE_ID_FACE);
	}

	// Autumn
	if (!(strcmp(surfaceName_38_AutumnItems, init_surfaceName_38_AutumnItems) == 0))
	{
		ReleaseSurface(SURFACE_ID_AUTUMN_ITEMS);
		MakeSurface_File(init_surfaceName_38_AutumnItems, SURFACE_ID_AUTUMN_ITEMS);
	}

	// NpcAutumnChar
	if (!(strcmp(surfaceName_39_AutumnCharacters, init_surfaceName_39_AutumnCharacters) == 0))
	{
		ReleaseSurface(SURFACE_ID_AUTUMN_CHARACTERS);
		MakeSurface_File(init_surfaceName_39_AutumnCharacters, SURFACE_ID_AUTUMN_CHARACTERS);
	}
}