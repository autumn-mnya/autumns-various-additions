#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "LoadPixtone.h"

#include "cave_story.h"
#include "main.h"

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

void SetFrameTargetMyCharOffset(int wait, int offset_x, int offset_y)
{
	gFrame->tgt_x = &gMC->tgt_x + offset_x;
	gFrame->tgt_y = &gMC->tgt_y + offset_y;
	gFrame->wait = wait;
}