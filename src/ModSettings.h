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
extern bool setting_enable_teleporter_bugfix;

extern bool setting_enable_reset_pixtone_on_reset;
extern bool setting_enable_reset_npctbl_on_reset;

extern bool legacy_extra_jumps_ui;

extern bool replace_fan_code;

extern bool pause_menu_ava_enabled;

void InitMod_Settings();
void InitSettingsRevamp();