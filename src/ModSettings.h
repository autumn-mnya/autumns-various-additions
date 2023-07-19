#pragma once

#include <Windows.h>

#include "cave_story.h"

extern bool setting_enable_surfaces;
extern bool setting_enable_sound_effect_code;
extern bool setting_enable_entity;
extern bool setting_enable_mychar;
extern bool setting_enable_ui;
extern bool setting_enable_tilecollision;
extern bool setting_enable_text_script_code;
extern bool setting_enable_savedata_code;
extern bool setting_enable_teleporter_bugfix;
extern bool setting_enable_pause_screen;

extern bool setting_enable_swapping_pixtone;

extern bool setting_collab_enabled;
extern const char* setting_collab_name;

void InitMod_Settings();