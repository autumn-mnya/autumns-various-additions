#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Generic.h"

#include "cave_story.h"
#include "main.h"

bool setting_disable_window_rect_saving = true;
bool setting_disable_errorlog_saving = false;

BOOL IsKeyDataFile(const char* name)
{
	char path[MAX_PATH];
	FILE* fp;

	sprintf(path, "%s\\%s", gDataPath, name);

	fp = fopen(path, "rb");

	if (fp == NULL)
		return FALSE;

	fclose(fp);
	return TRUE;
}

BOOL IsKeyBKGFile(const char* name)
{
	char path[MAX_PATH];
	FILE* fp;

	sprintf(path, "%s\\%s", gBkgPath, name);

	fp = fopen(path, "rb");

	if (fp == NULL)
		return FALSE;

	fclose(fp);
	return TRUE;
}

// I honestly hate saving the window rect. Let's keep it disabled by default!
BOOL Replacement_SaveWindowRect(HWND hWnd, const char* filename)
{
	return TRUE;
}

// Disable error.log option
BOOL Replacement_ErrorLog(const char* string, int value)
{
	return TRUE;
}