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

bool setting_enable_default_config_options = true;

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

void Replacement_DefaultConfigData(ConfigData* conf)
{
	// Clear old configuration data
	memset(conf, 0, sizeof(ConfigData));

	// Default to windowed mode instead of fullscreen
	conf->display_mode = 2;

	// Disable gamepad by default if there isnt a config
	conf->bJoystick = FALSE;

	// Reset joystick settings (as these can't simply be set to 0)
	conf->joystick_button[0] = 2;
	conf->joystick_button[1] = 1;
	conf->joystick_button[2] = 5;
	conf->joystick_button[3] = 6;
	conf->joystick_button[4] = 3;
	conf->joystick_button[5] = 4;
	conf->joystick_button[6] = 6;
	conf->joystick_button[7] = 3;
}