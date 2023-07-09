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

static int CustomTextScriptCommands(MLHookCPURegisters* regs, void* ud)
{
	(void)ud;

	char* where = TextScriptBuffer + gTS->p_read;
	if (where[0] != '<')
		return 0;
	if (strncmp(where + 1, "DIE", 3) == 0) // example command that kills the player
	{
		PlayerDeath();
		gTS->p_read += 4;
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