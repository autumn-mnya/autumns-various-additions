#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Main.h"
#include "ModSettings.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ASMPatches.h"
#include "ASMLoader.h"
#include "Collectables.h"
#include "CollabFlag.h"
#include "Config.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "MycParam.h"
#include "PauseScreen.h"
#include "Respawn.h"
#include "Stage.h"
#include "SurfaceDefines.h"
#include "TextScript.h"
#include "TextScriptCollabLoad.h"
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
bool setting_enable_savedata_code = true;
bool setting_enable_teleporter_bugfix = true;
bool setting_enable_pause_screen = false;

bool setting_enable_reset_pixtone_on_reset = false;
bool setting_enable_reset_stagetbl_on_reset = false;
bool setting_enable_reset_npctbl_on_reset = false;

bool setting_collab_enabled = false;
char setting_collab_name[CollabNameMaxPath] = "null";

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
	setting_enable_asm_loader = ModLoader_GetSettingBool("Enable Custom Hex Patch Loader", true);
	setting_enable_savedata_code = ModLoader_GetSettingBool("Enable Custom Save Data Code", true);
	setting_enable_teleporter_bugfix = ModLoader_GetSettingBool("Enable Teleporter Transparency Fix", true);
	setting_disable_window_rect_saving = ModLoader_GetSettingBool("Disable window.rect Saving", true);
	setting_disable_errorlog_saving = ModLoader_GetSettingBool("Disable error.log Saving", false);
	setting_enable_default_config_options = ModLoader_GetSettingBool("Autumn's Default Config Options", true);
	setting_enable_pause_screen = ModLoader_GetSettingBool("Enable Custom Pause Screen", false);

	///////////////////
	// Surface Names //
	///////////////////

	const char* settings_surfaceName_0_Title = ModLoader_GetSettingString("Default Title Surface Filename", "Title");
	const char* settings_surfaceName_5_Image = ModLoader_GetSettingString("Default <IMG Surface Filename", "Image\\0");
	const char* settings_surfaceName_6_Fade = ModLoader_GetSettingString("Default Fade Surface Filename", "Fade");
	const char* settings_surfaceName_8_ItemImage = ModLoader_GetSettingString("Default ItemImage Surface Filename", "ItemImage");
	const char* settings_surfaceName_11_Arms = ModLoader_GetSettingString("Default Arms Surface Filename", "Arms");
	const char* settings_surfaceName_12_ArmsImage = ModLoader_GetSettingString("Default ArmsImage Surface Filename", "ArmsImage");
	const char* settings_surfaceName_14_StageImage = ModLoader_GetSettingString("Default StageImage Surface Filename", "StageImage");
	const char* settings_surfaceName_15_Loading = ModLoader_GetSettingString("Default Loading Surface Filename", "Loading");
	const char* settings_surfaceName_16_MyChar = ModLoader_GetSettingString("Default MyChar Surface Filename", "MyChar");
	const char* settings_surfaceName_17_Bullet = ModLoader_GetSettingString("Default Bullet Surface Filename", "Bullet");
	const char* settings_surfaceName_19_Caret = ModLoader_GetSettingString("Default Caret Surface Filename", "Caret");
	const char* settings_surfaceName_20_NpcSym = ModLoader_GetSettingString("Default NpcSym Surface Filename", "Npc\\NpcSym");
	const char* settings_surfaceName_23_NpcRegu = ModLoader_GetSettingString("Default NpcRegu Surface Filename", "Npc\\NpcRegu");
	const char* settings_surfaceName_24_AutumnUI = ModLoader_GetSettingString("Default Autumn UI Surface Filename", "AutumnUI");
	const char* settings_surfaceName_25_AutumnObjects = ModLoader_GetSettingString("Default Autumn Objects Surface Filename", "Npc\\NpcAutumnObj");
	const char* settings_surfaceName_26_TextBox = ModLoader_GetSettingString("Default TextBox Surface Filename", "TextBox");
	const char* settings_surfaceName_27_Face = ModLoader_GetSettingString("Default Face Surface Filename", "Face");
	const char* settings_surfaceName_38_AutumnItems = ModLoader_GetSettingString("Default Autumn Items Surface Filename", "Autumn");
	const char* settings_surfaceName_39_AutumnCharacters = ModLoader_GetSettingString("Default Autumn Characters Surface Filename", "Npc\\NpcAutumnChar");

	// This is funny.
	strcpy(surfaceName_0_Title, settings_surfaceName_0_Title);
	strcpy(surfaceName_5_Image, settings_surfaceName_5_Image);
	strcpy(surfaceName_6_Fade, settings_surfaceName_6_Fade);
	strcpy(surfaceName_8_ItemImage, settings_surfaceName_8_ItemImage);
	strcpy(surfaceName_11_Arms, settings_surfaceName_11_Arms);
	strcpy(surfaceName_12_ArmsImage, settings_surfaceName_12_ArmsImage);
	strcpy(surfaceName_14_StageImage, settings_surfaceName_14_StageImage);
	strcpy(surfaceName_15_Loading, settings_surfaceName_15_Loading);
	strcpy(surfaceName_16_MyChar, settings_surfaceName_16_MyChar);
	strcpy(surfaceName_17_Bullet, settings_surfaceName_17_Bullet);
	strcpy(surfaceName_19_Caret, settings_surfaceName_19_Caret);
	strcpy(surfaceName_20_NpcSym, settings_surfaceName_20_NpcSym);
	strcpy(surfaceName_23_NpcRegu, settings_surfaceName_23_NpcRegu);
	strcpy(surfaceName_24_AutumnUI, settings_surfaceName_24_AutumnUI);
	strcpy(surfaceName_25_AutumnObjects, settings_surfaceName_25_AutumnObjects);
	strcpy(surfaceName_26_TextBox, settings_surfaceName_26_TextBox);
	strcpy(surfaceName_27_Face, settings_surfaceName_27_Face);
	strcpy(surfaceName_38_AutumnItems, settings_surfaceName_38_AutumnItems);
	strcpy(surfaceName_39_AutumnCharacters, settings_surfaceName_39_AutumnCharacters);

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

	// Booster Fuel
	booster_08_fuel = ModLoader_GetSettingInt("Booster 0.8 Fuel", 50);
	booster_20_fuel = ModLoader_GetSettingInt("Booster 2.0 Fuel", 50);

	// Version 1.0.7 physics

	// Booster 2.0 Acceleration -- Up and Left should be negative
	setting_physics_boost20_accel_up = -ModLoader_GetSettingInt("Booster 2.0 Acceleration (Up)", 1535);
	setting_physics_boost20_accel_left = -ModLoader_GetSettingInt("Booster 2.0 Acceleration (Left)", 1535);
	setting_physics_boost20_accel_right = ModLoader_GetSettingInt("Booster 2.0 Acceleration (Right)", 1535);
	setting_physics_boost20_accel_down = ModLoader_GetSettingInt("Booster 2.0 Acceleration (Down)", 1535);
	setting_physics_boost20_accel_up_no_key = -ModLoader_GetSettingInt("Booster 2.0 Acceleration (Up, No Key)", 1535);

	setting_physics_boost08_accel_add = ModLoader_GetSettingInt("Booster 0.8 Acceleration", 32);
	setting_physics_boost08_accel_max_speed = -ModLoader_GetSettingInt("Booster 0.8 Acceleration (Max Speed)", 1024);

	setting_physics_max_speed_left = -ModLoader_GetSettingInt("Player Max Speed (Left)", 1535);
	setting_physics_max_speed_right = ModLoader_GetSettingInt("Player Max Speed (Right)", 1535);
	setting_physics_max_speed_up = -ModLoader_GetSettingInt("Player Max Speed (Up)", 1535);
	setting_physics_max_speed_down = ModLoader_GetSettingInt("Player Max Speed (Down)", 1535);

	setting_physics_water_max_speed_left = -ModLoader_GetSettingInt("Underwater Player Max Speed (Left)", 767);
	setting_physics_water_max_speed_right = ModLoader_GetSettingInt("Underwater Player Max Speed (Right)", 767);
	setting_physics_water_max_speed_up = -ModLoader_GetSettingInt("Underwater Player Max Speed (Up)", 767);
	setting_physics_water_max_speed_down = ModLoader_GetSettingInt("Underwater Player Max Speed (Down)", 767);

	// Life Drain / Regen when flag is set

	setting_enable_life_drain = ModLoader_GetSettingBool("Life Drain when Flag is set", false);
	setting_enable_life_regen = ModLoader_GetSettingBool("Life Regen when Flag is set", false);

	// Flags for Life Drain/Regen

	setting_life_drain_flag = ModLoader_GetSettingInt("Life Drain Flag", 6504);
	setting_life_regen_flag = ModLoader_GetSettingInt("Life Regen Flag", 6505);

	// Wait timer for Life Drain/Regen
	setting_life_drain_wait = ModLoader_GetSettingInt("Life Drain Wait", 120);
	setting_life_regen_wait = ModLoader_GetSettingInt("Life Regen Wait", 240);

	// Amount of Life to Drain/Regen
	setting_life_drain_amount = ModLoader_GetSettingInt("Life Drain Amount", 1);
	setting_life_regen_amount = ModLoader_GetSettingInt("Life Regen Amount", 1);

	////////////////////
	// Tile Collision //
	////////////////////

	// Bounce Tile
	setting_bounce_speed = ModLoader_GetSettingInt("Bouncy Block Speed", 1535);
	setting_bounce_sfx = ModLoader_GetSettingInt("Bouncy Block Sound Effect", 25);

	// Ice Tile
	setting_ice_particles = ModLoader_GetSettingBool("Ice Block Particle Effects", true);

	/////////////////
	// Text Script //
	/////////////////

	// Ability to disable this
	setting_enable_mim_mod = ModLoader_GetSettingBool("Enable <MIM Command", true);

	// Disables the encryption the game uses for TSC.
	setting_disable_tsc_encryption = ModLoader_GetSettingBool("Disable TSC Encryption", false);

	// bigger playersize hacks maybe?
	mim_player_size_width = ModLoader_GetSettingInt("<MIM Player Width", 16);
	mim_player_size_height = ModLoader_GetSettingInt("<MIM Player Height", 16);

	setting_show_img_on_top_flag = ModLoader_GetSettingInt("Show Image Above Textbox Flag", 6503);
	setting_enable_money_code = ModLoader_GetSettingBool("Enable Money Code", false);

	// Disable Money from Entities
	setting_money_disable_enemy_money_drops = ModLoader_GetSettingBool("Disable Money from Entities", false);

	// Disable EXP from Entities (when money is enabled)
	setting_money_disable_exp_drops = ModLoader_GetSettingBool("Disable EXP with Money", false);

	// If grabbing exp will also add money
	setting_exp_acts_as_money = ModLoader_GetSettingBool("EXP also acts as Money", false);

	// If the money grabbed should be (exp / 2)
	setting_money_division = ModLoader_GetSettingBool("Money is divided by 2", true);

	// Money Hud position
	setting_money_hud_x = ModLoader_GetSettingInt("Money Hud X", 8);
	setting_money_hud_y = ModLoader_GetSettingInt("Money Hud Y", 48);

	// How offset the numbers are compared to the money icon
	setting_money_hud_x_number_offset = ModLoader_GetSettingInt("Money Hud X Number Offset", 16);

	// Collectables
	enable_collectables_a = ModLoader_GetSettingBool("Enable Collectables UI A", false);
	enable_collectables_b = ModLoader_GetSettingBool("Enable Collectables UI B", false);
	enable_collectables_c = ModLoader_GetSettingBool("Enable Collectables UI C", false);
	enable_collectables_d = ModLoader_GetSettingBool("Enable Collectables UI D", false);
	enable_collectables_e = ModLoader_GetSettingBool("Enable Collectables UI E", false);

	collectables_a_x_pos = ModLoader_GetSettingInt("Collectables A (X Position)", 8);
	collectables_a_y_pos = ModLoader_GetSettingInt("Collectables A (Y Position)", 56);
	collectables_a_x_offset = ModLoader_GetSettingInt("Collectables A (X Number Offset)", 0);
	collectables_b_x_pos = ModLoader_GetSettingInt("Collectables B (X Position)", 8);
	collectables_b_y_pos = ModLoader_GetSettingInt("Collectables B (Y Position)", 64);
	collectables_b_x_offset = ModLoader_GetSettingInt("Collectables B (X Number Offset)", 0);
	collectables_c_x_pos = ModLoader_GetSettingInt("Collectables C (X Position)", 8);
	collectables_c_y_pos = ModLoader_GetSettingInt("Collectables C (Y Position)", 72);
	collectables_c_x_offset = ModLoader_GetSettingInt("Collectables C (X Number Offset)", 0);
	collectables_d_x_pos = ModLoader_GetSettingInt("Collectables D (X Position)", 8);
	collectables_d_y_pos = ModLoader_GetSettingInt("Collectables D (Y Position)", 80);
	collectables_d_x_offset = ModLoader_GetSettingInt("Collectables D (X Number Offset)", 0);
	collectables_e_x_pos = ModLoader_GetSettingInt("Collectables E (X Position)", 8);
	collectables_e_y_pos = ModLoader_GetSettingInt("Collectables E (Y Position)", 88);
	collectables_e_x_offset = ModLoader_GetSettingInt("Collectables E (X Number Offset)", 0);

	// Revive HP
	setting_revive_hp = ModLoader_GetSettingInt("<RVI (Revive) HP amount", 3);

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

	////////////////////
	// Major Settings //
	////////////////////

	setting_external_stage_tbl_support = ModLoader_GetSettingBool("Enable External Stage.tbl Files", false);
	setting_enable_collab_npc_table = ModLoader_GetSettingBool("Enable Custom NPC Table Loading", false);

	setting_enable_custom_armsitem_location = ModLoader_GetSettingBool("Enable Custom ArmsItem.tsc Locations", false); // <ASC - Armsitem SCript
	setting_enable_custom_head_location = ModLoader_GetSettingBool("Enable Custom Head.tsc Locations", false); // HSC - Head SCript

	defaultArmsItemTSCName = ModLoader_GetSettingString("Default ArmsItem.tsc name", "ArmsItem");
	defaultHeadTSCName = ModLoader_GetSettingString("Default Head.tsc name", "Head");

	setting_collab_enabled = ModLoader_GetSettingBool("Collab Mode", false);
	const char* settings_collab_name = ModLoader_GetSettingString("Collab Name", "null");
	strcpy(setting_collab_name, settings_collab_name);
	setting_enable_collab_flags = ModLoader_GetSettingBool("Collab Flags", false);
	setting_enable_reset_pixtone_on_reset = ModLoader_GetSettingBool("Enable PixTone Reloading on Reset", false);
	setting_enable_reset_stagetbl_on_reset = ModLoader_GetSettingBool("Enable stage.tbl Reloading on Reset", false);
	setting_enable_reset_npctbl_on_reset = ModLoader_GetSettingBool("Enable npc.tbl Reloading on Reset", false);
}