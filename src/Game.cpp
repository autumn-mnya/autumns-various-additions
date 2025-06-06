#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Game.h"

#include "main.h"
#include "ModInit.h"
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
#include "TextScriptCustomLoad.h"

#include "DialogueChoice.h"

#include "AutPI.h"

// Put <IMG on screen

void AutumnsVariousAdditions_ModeOpening_TextBoxBelow()
{
	// Show <IMG behind textbox when flag isnt set
	if (!(GetNPCFlag(setting_show_img_on_top_flag)))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);
}

void AutumnsVariousAdditions_ModeAction_TextBoxBelow()
{
	// Show <IMG behind textbox when flag isnt set
	if (!(GetNPCFlag(setting_show_img_on_top_flag)))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);

	if (bSetRespawn)
		Stage_SetRespawn();
}

void AutumnsVariousAdditionsTextBoxAbove()
{
	PutDialogueChoices();

	// Show <IMG infront of textbox when flag is set
	if (GetNPCFlag(setting_show_img_on_top_flag))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);
}

// <IMG resetting calls among others
void AutumnsVariousAdditionsModeOpeningInit()
{
	// SetFadeMask();
	ResetCustomGenericData(); // Reset the sprites if they were changed from the defaults in graphics.ini
	Reset_CustomScriptNames(); // Reset custom script names in the opening
	if (setting_enable_reset_pixtone_on_reset)
		Reset_PixToneFolder(); // reset pixtone folder
	if (setting_enable_custom_npc_table && setting_enable_reset_npctbl_on_reset)
		LoadCustomNpcTable(NULL); // reset npc table on modeopening
	ResetTSC_Image();
}

void AutumnsVariousAdditionsModeTitleInit()
{
	ResetCustomGenericData(); // Reset the sprites if they were changed from the defaults in graphics.ini
	Reset_CustomScriptNames(); // Reset custom script names on the title
	if (setting_enable_reset_pixtone_on_reset)
		Reset_PixToneFolder(); // reset pixtone folder
	ResetTSC_Image();
}

void AutumnsVariousAdditionsModeActionInit() // Use this to load generic data
{
	InitGameSurfaces(); // init surfaces
	LoadCustomGenericData(); // load surfaces
	ClearDialogueChoiceData();  // clear dialogue choices on reset
}

void AutumnsVariousAdditionsModeActionAct()
{
	// turn off isLoadingSave if it is active
	if (isLoadingSave == true)
		isLoadingSave = false;
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