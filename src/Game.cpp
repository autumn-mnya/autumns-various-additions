#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Game.h"

#include "main.h"
#include "cave_story.h"
#include "Draw.h"
#include "Respawn.h"

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

	// Show <IMG infront of textbox when flag is set
	if (GetNPCFlag(setting_show_img_on_top_flag))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);
}

void Replacement_ModeAction_PutTextScript_Call()
{
	// Show <IMG behind textbox when flag isnt set
	if (!(GetNPCFlag(setting_show_img_on_top_flag)))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);

	if (bSetRespawn)
		Stage_SetRespawn();

	PutTextScript();

	// Show <IMG infront of textbox when flag is set
	if (GetNPCFlag(setting_show_img_on_top_flag))
		PutBitmap3(&grcGame, 0, 0, &rcTSCImage, SURFACE_ID_TSC_IMG);
}