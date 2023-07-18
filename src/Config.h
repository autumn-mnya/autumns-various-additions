#pragma once

#include <Windows.h>

#include "cave_story.h"

extern bool setting_enable_default_config_options;

BOOL SaveConfigData(ConfigData* conf);
void Replacement_DefaultConfigData(ConfigData* conf);