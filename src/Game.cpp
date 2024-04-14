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
#include "TextScriptCollabLoad.h"

#include "DialogueChoice.h"
#include "XMLLoad.h"

#include "fmod/fmodAudio.h"

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

void AutumnsVariousAdditionsModeTitleInit()
{
	ResetCustomGenericData(); // Reset the sprites if they were changed from the defaults in settings.ini
	Reset_CustomScriptNames(); // Reset custom script names on the title
	if (setting_enable_reset_pixtone_on_reset)
		Reset_PixToneFolder(); // reset pixtone folder
	ResetTSC_Image();
}

void AutumnsVariousAdditionsModeActionInit() // Use this to load generic data
{
	// XML data
	loadedData.isLoaded = false; // reset this to false on new game
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

void AutumnsVariousAdditionsStageTableInit()
{
	LoadStageTable(NULL);
}

// FMOD

const char* setting_titlescreen_cavestory_music = gNull1Name;
const char* setting_titlescreen_hell_music = gNull1Name;
const char* setting_titlescreen_toroko_music = gNull1Name;
const char* setting_titlescreen_white_music = gNull1Name;
const char* setting_titlescreen_safety_music = gNull1Name;

void Replacement_ModeOpening_ChangeMusic_Silence_Call(MusicID no)
{
	ChangeMusic(no);
	StopFmodAllAudio();
}

void Replacement_ModeTitle_ChangeMusic_RunningHell_Call(MusicID no)
{
	ChangeMusic(no);
	PlayFModAudio(setting_titlescreen_hell_music);
}

void Replacement_ModeTitle_ChangeMusic_TorokosTheme_Call(MusicID no)
{
	ChangeMusic(no);
	PlayFModAudio(setting_titlescreen_toroko_music);
}

void Replacement_ModeTitle_ChangeMusic_White_Call(MusicID no)
{
	ChangeMusic(no);
	PlayFModAudio(setting_titlescreen_white_music);
}

void Replacement_ModeTitle_ChangeMusic_Safety_Call(MusicID no)
{
	ChangeMusic(no);
	PlayFModAudio(setting_titlescreen_safety_music);
}

void Replacement_ModeTitle_ChangeMusic_CaveStory_Call(MusicID no)
{
	ChangeMusic(no);
	PlayFModAudio(setting_titlescreen_cavestory_music);
}

void Replacement_ModeTitle_ChangeMusic_Silence_Call(MusicID no)
{
	// fmod audio stops getting updated outside of the loop, so we update it once the music is supposed to be stopped
	ChangeMusic(no);
	StopFmodAllAudio();
	FmodStudioObj->update();
}