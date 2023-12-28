#include "DialogueChoice.h"

#include <string.h>
#include <string>
#include "cave_story.h"

#include <Windows.h>

#include "Draw.h"
#include "Main.h"
#include "Game.h"
#include "MyChar.h"
#include "MycParam.h"

#define ChoiceFrameCount 6

CHOICE gChoiceData[CHOICE_MAX];

int gSelectedChoice;

void ClearDialogueChoiceData(void)
{
	memset(gChoiceData, 0, sizeof(gChoiceData));
}

BOOL AddDialogueChoiceData(int event, const char* name)
{
	for (int i = 1; i < CHOICE_MAX; ++i)
	{
		if (gChoiceData[i].index == 0)
		{
			gChoiceData[i].event = event;
			strcpy(gChoiceData[i].name, name);

			gChoiceData[i].index = i; // get id?
			return TRUE;
		}
	}

	return FALSE;
}

void MoveDialogueChoiceCursor(void)
{
	int option_num;
	option_num = 0;
	while (gChoiceData[option_num + 1].index != 0)
		++option_num;

	if (gKeyTrg & gKeyUp)
		gSelectedChoice -= 1;

	if (gKeyTrg & gKeyDown)
		gSelectedChoice += 1;

	gSelectedChoice = (gSelectedChoice + option_num) % option_num;

	if (gKeyTrg & (gKeyUp | gKeyDown))
		PlaySoundObject(1, SOUND_MODE_PLAY);
}

static RECT rc = { 0, 0, 160, 12 };

void PutDialogueChoiceObject(void)
{
	RECT rcView = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	int option_num;
	option_num = 0;
	while (gChoiceData[option_num + 1].index != 0)
		++option_num;

	RECT rcBox[3] = {
		{ 0, 0, 244, 8 },	 // Top
		{ 0, 8, 244, 16 },	 // Body
		{ 0, 16, 244, 24 },  // Bottom
	};

	int offset;
	if (gTS.wait < 2)
		offset = ((WINDOW_HEIGHT / 2) - 96) + (2 - gTS.wait) * 4;
	else
		offset = ((WINDOW_HEIGHT / 2) - 96);

	// Regular cave story box
	PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 122, offset + 8, &rcBox[0], SURFACE_ID_TEXT_BOX);
	int a;
	for (a = 1; a < option_num + 1; a++)
	{
		PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 122, offset + (a * 16), &rcBox[1], SURFACE_ID_TEXT_BOX);
		PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 122, offset + 8 + (a * 16), &rcBox[1], SURFACE_ID_TEXT_BOX);
	}
	PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 122, offset + (a * 16), &rcBox[2], SURFACE_ID_TEXT_BOX);

	for (int i = 1; i < CHOICE_MAX; i++)
		PutText(120, offset + 4 + (16 * i), gChoiceData[i].name, RGB(0xFF, 0xFF, 0xFF));

	if (gTS.wait != ChoiceFrameCount)
		return;

	RECT rcCur = { 112, 88, 128, 104 };
	PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 110, offset + 16 + (16 * gSelectedChoice), &rcCur, SURFACE_ID_TEXT_BOX);
}

void PutDialogueChoices()
{
	if (gTS.mode == 8)
	{
		if (gTS.wait < ChoiceFrameCount)
			++gTS.wait;
		else
		{
			PutDialogueChoiceObject();

			MoveDialogueChoiceCursor();
			if (gKeyTrg & gKeyOk)
			{
				PlaySoundObject(18, SOUND_MODE_PLAY);
				if (gChoiceData[gSelectedChoice + 1].event != 0)
					JumpTextScript(gChoiceData[gSelectedChoice + 1].event);
				else
				{
					gTS.mode = 1;
					gTS.wait_beam = 0;
				}
				ClearDialogueChoiceData();
			}
		}
	}
}