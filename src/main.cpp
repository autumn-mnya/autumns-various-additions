#include <windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>


#include "mod_loader.h"
#include "cave_story.h"

static const char* const lpWindowName = "An example dll!";

// Changes the window title to the string above.
void ExampleFunction(HWND hWnd)
{
	char window_name[0x100];

	sprintf(window_name, "%s", lpWindowName);
	SetWindowTextA(hWnd, window_name);
}

void InitMod(void)
{
	ModLoader_WriteJump((void*)0x412320, (void*)ExampleFunction);
}
