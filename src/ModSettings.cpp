#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Main.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ASMPatches.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "PauseScreen.h"
#include "TextScript.h"
#include "TileCollisionBoss.h"
#include "TileCollisionBullet.h"
#include "TileCollisionEntity.h"
#include "TileCollisionMyChar.h"

// Options
bool setting_enable_surfaces = true;
bool setting_enable_sound_effect_code = false;
bool setting_enable_entity = true;
bool setting_enable_mychar = true;
bool setting_enable_ui = true;
bool setting_enable_tilecollision = true;
bool setting_enable_text_script_code = true;
bool setting_enable_pause_screen = false;

// For settings related things, this function will be used.
void InitMod_Settings()
{
	////////////
	// Global //
	////////////

	setting_enable_surfaces = ModLoader_GetSettingBool("Enable Custom Surface Code", true);
	setting_enable_sound_effect_code = ModLoader_GetSettingBool("Enable Custom Sound Effect Code", true);
	setting_enable_entity = ModLoader_GetSettingBool("Enable Custom Entity Code", true);
	setting_enable_mychar = ModLoader_GetSettingBool("Enable Custom MyChar Code", true);
	setting_enable_ui = ModLoader_GetSettingBool("Enable Custom UI Code", true);
	setting_enable_tilecollision = ModLoader_GetSettingBool("Enable Custom Tileset Code", true);
	setting_enable_text_script_code = ModLoader_GetSettingBool("Enable Custom TSC Code", true);
	setting_enable_pause_screen = ModLoader_GetSettingBool("Enable Custom Pause Screen", false);

	///////////////
	// ASM Hacks //
	///////////////

	////////////
	// MyChar //
	////////////

	// Walljumps
	setting_walljumps_enabled = ModLoader_GetSettingBool("Walljumps Enabled", false);
	setting_walljumps_flag_enabled = ModLoader_GetSettingBool("Walljumps Enable on Flag", false);
	setting_walljumps_flag = ModLoader_GetSettingInt("Walljumps Flag", 6500);
	setting_walljump_horizontal_speed = ModLoader_GetSettingInt("Walljumps Horizontal Speed", 1000);
	setting_walljump_water_horizontal_speed = ModLoader_GetSettingInt("Underwater Walljumps Horizontal Speed", 300);
	setting_walljump_jump_height = ModLoader_GetSettingInt("Walljumps Jump Momentum", 1113);
	setting_walljump_water_jump_height = ModLoader_GetSettingInt("Underwater Walljumps Jump Momentum", 556);
	setting_walljump_sliding_speed = ModLoader_GetSettingInt("Wallsliding Speed", 554);

	// Extra Jumps
	setting_extrajump_jump_height = ModLoader_GetSettingInt("Extra Jump Momentum", 1280);
	setting_extrajump_water_jump_height = ModLoader_GetSettingInt("Underwater Extra Jump Momentum", 640);

	// Extra Jump UI
	setting_jump_arrow_x_offset = ModLoader_GetSettingInt("Jump Arrow UI X Offset", 4);
	setting_jump_arrow_y_offset = ModLoader_GetSettingInt("Jump Arrow UI Y Offset", 20);

	// Extra Jump Limit
	setting_max_jump_arrow_display = ModLoader_GetSettingInt("Jump Arrow UI Limit", 5);

	// Extra Jump UI (Negative/Positive)
	setting_jump_arrow_x_offset_negative = ModLoader_GetSettingBool("Jump Arrow X Offset is negative", true);
	setting_jump_arrow_y_offset_negative = ModLoader_GetSettingBool("Jump Arrow Y Offset is negative", true);

	// Double Jump
	setting_doublejump_enabled = ModLoader_GetSettingBool("Double Jump Enabled", false);
	setting_doublejump_flag_enabled = ModLoader_GetSettingBool("Double Jump Enable on Flag", false);
	setting_doublejump_flag = ModLoader_GetSettingInt("Double Jump Flag", 6501);

	// Physics + Run button
	setting_physics_ini_enabled = ModLoader_GetSettingBool("Physics in Settings.ini", false);
	setting_run_button_enabled = ModLoader_GetSettingBool("Enable Run Button", false);
	setting_run_button_flag_enabled = ModLoader_GetSettingBool("Enable Run Button on Flag", false);
	setting_run_button_flag = ModLoader_GetSettingInt("Run Button Flag", 6502);

	setting_physics_max_dash = ModLoader_GetSettingInt("Max Walking Speed", 812);
	setting_physics_max_move = ModLoader_GetSettingInt("Max Top Movement Speed", 1535);
	setting_physics_gravity1 = ModLoader_GetSettingInt("Gravity", 80);
	setting_physics_gravity2 = ModLoader_GetSettingInt("Rising Gravity", 32);
	setting_physics_jump = ModLoader_GetSettingInt("Jump Momentum", 1280);
	setting_physics_dash1 = ModLoader_GetSettingInt("Walking Acceleration", 85);
	setting_physics_dash2 = ModLoader_GetSettingInt("Air Control", 32);
	setting_physics_resist = ModLoader_GetSettingInt("Friction", 51);

	setting_physics_water_max_dash = ModLoader_GetSettingInt("Underwater Max Walking Speed", 406);
	setting_physics_water_max_move = ModLoader_GetSettingInt("Underwater Top Movement Speed", 767);
	setting_physics_water_gravity1 = ModLoader_GetSettingInt("Underwater Gravity", 40);
	setting_physics_water_gravity2 = ModLoader_GetSettingInt("Underwater Rising Gravity", 16);
	setting_physics_water_jump = ModLoader_GetSettingInt("Underwater Jump Momentum", 640);
	setting_physics_water_dash1 = ModLoader_GetSettingInt("Underwater Walking Acceleration", 42);
	setting_physics_water_dash2 = ModLoader_GetSettingInt("Underwater Air Control", 16);
	setting_physics_water_resist = ModLoader_GetSettingInt("Underwater Friction", 25);

	setting_running_speed = ModLoader_GetSettingInt("Max Running Speed", 1218);

	////////////////////
	// Tile Collision //
	////////////////////

	// Bounce Tile
	setting_bounce_speed = ModLoader_GetSettingInt("Bouncy Block Speed", 1535);
	setting_bounce_sfx = ModLoader_GetSettingInt("Bouncy Block Sound Effect", 25);

	// Ice Tile
	setting_ice_particles = ModLoader_GetSettingBool("Ice Block Particle Effects", true);

	/////////////////////
	// Pause Menu Text //
	/////////////////////

	setting_pausetext_paused = ModLoader_GetSettingString("Paused Text", "PAUSED");
	setting_pausetext_resume = ModLoader_GetSettingString("Resume Text", "Resume");
	setting_pausetext_reset = ModLoader_GetSettingString("Reset Text", "Reset");
	setting_pausetext_options = ModLoader_GetSettingString("Options Text", "Options");
	setting_pausetext_quit = ModLoader_GetSettingString("Quit Text", "Quit");
	setting_pausetext_restart_required = ModLoader_GetSettingString("Restart Required Text", "RESTART REQUIRED");
	setting_pausetext_menu_options = ModLoader_GetSettingString("Options Menu Text", "OPTIONS");

	setting_pausetext_resolution = ModLoader_GetSettingString("Resolution Text", "Resolution");
	setting_pausetext_gamepad_enabled = ModLoader_GetSettingString("Gamepad Text", "Gamepad Enabled");
	setting_pausetext_windowsize_a = ModLoader_GetSettingString("Fullscreen Text", "Fullscreen");
	setting_pausetext_windowsize_b = ModLoader_GetSettingString("Windowed 1 Text", "Windowed 320x240");
	setting_pausetext_windowsize_c = ModLoader_GetSettingString("Windowed 2 Text", "Windowed 640x480");
	setting_pausetext_windowsize_d = ModLoader_GetSettingString("Fullscreen 24bit Text", "Fullscreen 24-Bit");
	setting_pausetext_windowsize_e = ModLoader_GetSettingString("Fullscreen 32bit Text", "Fullscreen 32-Bit");
	setting_pausetext_disabled = ModLoader_GetSettingString("Disabled Text", "Disabled");
	setting_pausetext_enabled = ModLoader_GetSettingString("Enabled Text", "Enabled");
	setting_pausetext_yes = ModLoader_GetSettingString("Yes Text", "Yes");
	setting_pausetext_no = ModLoader_GetSettingString("No Text", "No");
	setting_pausetext_are_you_sure = ModLoader_GetSettingString("Confirm Text", "Are you sure?");
	setting_pausetext_unsaved_progress = ModLoader_GetSettingString("Progress Text", "Unsaved progress will be lost!");
}