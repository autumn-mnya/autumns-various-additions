#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "CollabFlag.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ModSettings.h"
#include "main.h"
#include "Generic.h"
#include "Profile.h"

// Macros for setting, un-setting and getting flags
// Each flag is stored in a bit, so we can use the exact same macros we'd use for bits
#define SET_FLAG(x, i) ((x)[(i) / 8] |= 1 << ((i) % 8))
#define UNSET_FLAG(x, i) ((x)[(i) / 8] &= ~(1 << ((i) % 8)))
#define GET_FLAG(x, i) ((x)[(i) / 8] & (1 << ((i) % 8)))

bool setting_enable_collab_flags = false;
int enable_collab_flags = 0; // This is an int so that we don't have to deal with struct padding messyness
unsigned char gCollabFlag[1000];

// Flag initializers


void InitAllTypeFlags(void)
{
	memset(gFlagNPC, 0, sizeof(gFlagNPC));
	memset(gCollabFlag, 0, sizeof(gCollabFlag));
}

void InitCollabFlags(void)
{
	memset(gCollabFlag, 0, sizeof(gCollabFlag));
}

// Collab flags
// Remember to replace the calls throughout the game with this !!
void SetCollabFlag(long a)
{
	if (enable_collab_flags == 1)
		SET_FLAG(gCollabFlag, a);
	else
		SET_FLAG(gFlagNPC, a);
}

void CutCollabFlag(long a)
{
	if (enable_collab_flags == 1)
		UNSET_FLAG(gCollabFlag, a);
	else
		UNSET_FLAG(gFlagNPC, a);
}

BOOL GetCollabFlag(long a)
{
	if (enable_collab_flags == 1)
	{
		if (GET_FLAG(gCollabFlag, a))
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if (GET_FLAG(gFlagNPC, a))
			return TRUE;
		else
			return FALSE;
	}
}

void InitMod_CollabFlagLoading()
{
	ModLoader_WriteJump((void*)0x40E850, (void*)InitAllTypeFlags);

	if (setting_enable_collab_flags == true)
	{
		ModLoader_WriteJump((void*)0x40E890, (void*)SetCollabFlag);
		ModLoader_WriteJump((void*)0x40E8E0, (void*)CutCollabFlag);
		ModLoader_WriteJump((void*)0x40E930, (void*)GetCollabFlag);
	}
}