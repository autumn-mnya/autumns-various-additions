#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Generic.h"

#include "cave_story.h"
#include "main.h"

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