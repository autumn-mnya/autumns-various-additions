#include "Entity.h"
#include "cave_story.h"

// Save Point Example
void ActEntity361(NPCHAR* npc)
{
	RECT rect[8] = {
		{96, 16, 112, 32},
		{112, 16, 128, 32},
		{128, 16, 144, 32},
		{144, 16, 160, 32},
		{160, 16, 176, 32},
		{176, 16, 192, 32},
		{192, 16, 208, 32},
		{208, 16, 224, 32},
	};

	int i;

	switch (npc->act_no)
	{
	case 0:
		npc->bits |= NPC_INTERACTABLE;
		npc->act_no = 1;

		if (npc->direct == 2)
		{
			npc->bits &= ~NPC_INTERACTABLE;
			npc->ym = -0x200;

			for (i = 0; i < 4; ++i)
				SetNpChar(4, npc->x + (Random(-12, 12) * 0x200), npc->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);
		}

		// Fallthrough
	case 1:
		if (npc->flag & 8)
			npc->bits |= NPC_INTERACTABLE;

		break;
	}

	if (++npc->ani_wait > 2)
	{
		npc->ani_wait = 0;
		++npc->ani_no;
	}

	if (npc->ani_no > 7)
		npc->ani_no = 0;

	npc->ym += 0x40;
	if (npc->ym > 0x5FF)
		npc->ym = 0x5FF;

	npc->y += npc->ym;

	npc->rect = rect[npc->ani_no];
}