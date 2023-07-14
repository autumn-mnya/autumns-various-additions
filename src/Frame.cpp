#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Frame.h"

#include "cave_story.h"
#include "main.h"
#include "MyChar.h"

int coordinate_focus_x = 0;
int coordinate_focus_y = 0;

// Focus on an NPC with the camera panning right/left + up/down
void SetFrameTargetNpCharWithMyCharIndex(int event, int wait)
{
	int i;
	for (i = 0; i < NPC_MAX; ++i)
		if (gNPC[i].code_event == event)
			break;

	if (i == NPC_MAX)
		return;

	gFrame->tgt_x = &gNPC[i].x + gMC->index_x;
	gFrame->tgt_y = &gNPC[i].y + gMC->index_y;
	gFrame->wait = wait;
}

// Focus on the player with an offset
void SetFrameTargetMyCharOffset(int wait, int mode, int offset_x, int offset_y)
{
	// Change to negative value depending on the mode
	switch (mode)
	{
		default:
			custom_camera_offset_x = offset_x;
			custom_camera_offset_y = offset_y;
			break;

		case 1:
			custom_camera_offset_x = offset_x * -1;
			break;

		case 2:
			custom_camera_offset_y = offset_y * -1;
			break;

		case 3:
			custom_camera_offset_x = offset_x * -1;
			custom_camera_offset_y = offset_y * -1;
			break;
	}


	gFrame->tgt_x = &custom_tgt_x;
	gFrame->tgt_y = &custom_tgt_y;
	gFrame->wait = wait;
}

// Focus on a coordinate
void SetFrameTargetCoordinate(int wait, int x, int y)
{
	coordinate_focus_x = x;
	coordinate_focus_y = y;

	gFrame->tgt_x = &coordinate_focus_x;
	gFrame->tgt_y = &coordinate_focus_y;
	gFrame->wait = wait;
}