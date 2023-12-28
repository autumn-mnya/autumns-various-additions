#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Game.h"

#include "main.h"
#include "mod_loader.h"
#include "ModSettings.h"
#include "cave_story.h"
#include "Draw.h"
#include "EntityLoad.h"
#include "GenericLoad.h"
#include "LoadPixtone.h"
#include "Profile.h"
#include "Respawn.h"
#include "Stage.h"
#include "TextScript.h"
#include "TextScriptCollabLoad.h"

#include "DialogueChoice.h"
#include "XMLLoad.h"

// Set Respawn
// 0x40F770
void Replacement_ModeOpening_SetFrameTargetMyChar_Call(int x)
{
	if (bSetRespawn)
		Stage_SetRespawn();

	SetFrameTargetMyChar(x);
}

// Put <IMG on screen

void Replacement_ModeOpening_PutTextScript_Call()
{
	// Show <IMG behind textbox when flag isnt set
	if (!(GetNPCFlag(setting_show_img_on_top_flag)))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);

	PutTextScript();
	PutDialogueChoices();

	// Show <IMG infront of textbox when flag is set
	if (GetNPCFlag(setting_show_img_on_top_flag))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);
}

// <IMG resetting calls among others
void Replacement_ModeOpening_SetFadeMask_Call()
{
	SetFadeMask();
	ResetCustomGenericData(); // Reset the sprites if they were changed from the defaults in settings.ini
	Reset_CustomScriptNames(); // Reset custom script names in the opening
	if (setting_enable_reset_pixtone_on_reset)
		Reset_PixToneFolder(); // reset pixtone folder
	if (setting_external_stage_tbl_support && setting_enable_reset_stagetbl_on_reset)
		LoadStageTable(NULL); // reset stage table on modeopening
	if (setting_enable_collab_npc_table && setting_enable_reset_npctbl_on_reset)
		LoadCustomNpcTable(NULL); // reset npc table on modeopening
	ResetTSC_Image();
	BKG_ResetBackgrounds();
}

void Replacement_ModeTitle_InitStar_Call()
{
	InitStar();
	ResetCustomGenericData(); // Reset the sprites if they were changed from the defaults in settings.ini
	Reset_CustomScriptNames(); // Reset custom script names on the title
	if (setting_enable_reset_pixtone_on_reset)
		Reset_PixToneFolder(); // reset pixtone folder
	ResetTSC_Image();
}

// 0x41043B
void Replacement_ModeAction_InitMyChar_Call() // Use this to load generic data
{
	// XML data
	loadedData.isLoaded = false; // reset this to false on new game
	InitGameSurfaces(); // init
	LoadCustomGenericData(); // load
	InitMyChar();
}

// 0x410564
void Replacement_ModeAction_ActValueView_Call()
{
	// turn off isLoadingSave if it is active
	if (isLoadingSave == true)
		isLoadingSave = false;

	ActValueView();
}

void Replacement_ModeAction_PutTextScript_Call()
{
	// Show <IMG behind textbox when flag isnt set
	if (!(GetNPCFlag(setting_show_img_on_top_flag)))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);

	if (bSetRespawn)
		Stage_SetRespawn();

	PutTextScript();
	PutDialogueChoices();

	// Show <IMG infront of textbox when flag is set
	if (GetNPCFlag(setting_show_img_on_top_flag))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);
}

// 0x40F67C
void Replacement_Game_InitTextScript2_Call()
{
	LoadStageTable(NULL);
	InitTextScript2();
}