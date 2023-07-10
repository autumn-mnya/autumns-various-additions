#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "TextScript.h"

#include "main.h"

#include "mod_loader.h"
#include "mod_loader_hooks.h"
#include "cave_story.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "Frame.h"

static int CustomTextScriptCommands(MLHookCPURegisters* regs, void* ud)
{
	(void)ud;
	int w, x, y, z;

	char* where = TextScriptBuffer + gTS->p_read;
	if (where[0] != '<')
		return 0;
	if (strncmp(where + 1, "TRM", 3) == 0)
	{
		z = GetTextScriptNo(gTS->p_read + 4);
		w = GetTextScriptNo(gTS->p_read + 9);
		x = GetTextScriptNo(gTS->p_read + 14);
		y = GetTextScriptNo(gTS->p_read + 19);

		int xm = gMC->xm;
		int ym = gMC->ym;

		if (!TransferStage(z, w, x, y))
		{
			return enum_ESCRETURN_exit;
		}

		// Restore player velocity
		gMC->xm = xm;
		gMC->ym = ym;
	}
	else if (strncmp(where + 1, "FN2", 3) == 0) // Focus on Npc 2
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		SetFrameTargetNpCharWithMyCharIndex(x, y);
		gTS->p_read += 13;
	}
	else if (strncmp(where + 1, "FM2", 3) == 0) // Focus on Me 2
	{
		x = GetTextScriptNo(gTS->p_read + 4);
		y = GetTextScriptNo(gTS->p_read + 9);
		SetFrameTargetMyCharOffset(x, y);
		gTS->p_read += 13;
	}
	else
		return 0;
	
	regs->eip = CSJ_tsc_done;
	return 1;
}

void InitMod_TSC()
{
	ModLoader_AddStackableHook(CSH_tsc_start, CustomTextScriptCommands, (void*)0);
}