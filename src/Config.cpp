#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "main.h"
#include "Config.h"

#include "mod_loader.h"
#include "cave_story.h"

BOOL SaveConfigData(ConfigData* conf)
{
	// Get path
	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gModulePath, gConfigFileName);

	// Open file
	FILE* fp = fopen(path, "wb");
	if (fp == NULL)
		return FALSE;

	// Read data
	fwrite(conf, sizeof(ConfigData), 1, fp); // Not the original name

	// Close file
	fclose(fp);

	return TRUE;
}