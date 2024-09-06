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
#include "Config.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "Game.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "MycParam.h"
#include "Respawn.h"
#include "Stage.h"
#include "SurfaceDefines.h"
#include "TextScript.h"
#include "TextScriptCustomLoad.h"
#include "TileCollisionBoss.h"
#include "TileCollisionBullet.h"
#include "TileCollisionEntity.h"
#include "TileCollisionMyChar.h"

#include "library/INIReader.h"

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

bool setting_enable_reset_pixtone_on_reset = false;
bool setting_enable_reset_npctbl_on_reset = false;

bool legacy_extra_jumps_ui = false;

// Version 1.1.3
bool pause_menu_ava_enabled = false;

// Used to be used before 1.1.2 --> the settings system has seen a massive overhaul.
void InitMod_Settings()
{
}

// Helper function to safely copy a string into a dynamically allocated char* buffer
char* copyString(const std::string& str) {
	char* buffer = new char[str.length() + 1]; // Allocate memory for the string (+1 for null terminator)
	strcpy(buffer, str.c_str()); // Copy string data into the buffer
	return buffer;
}

INIReader ini_getName(char* name)
{
	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gAVAConfigPath, name);
	INIReader reader(path);

	return reader;
}

void Init_INI_main()
{
	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gAVAConfigPath, "main.ini");
	INIReader main(path);

	setting_enable_surfaces = main.GetBoolean("Main", "Enable Custom Surface Code", true);
	setting_enable_sound_effect_code = main.GetBoolean("Main", "Enable Custom Sound Effect Code", true);
	setting_enable_entity = main.GetBoolean("Main", "Enable Custom Entity Code", true);
	setting_enable_mychar = main.GetBoolean("Main", "Enable Custom MyChar Code", true);
	setting_enable_ui = main.GetBoolean("Main", "Enable Custom UI Code", true);
	setting_enable_tilecollision = main.GetBoolean("Main", "Enable Custom Tileset Code", true);
	setting_enable_text_script_code = main.GetBoolean("Main", "Enable Custom TSC Code", true);
	setting_enable_asm_loader = main.GetBoolean("Main", "Enable Custom Hex Patch Loader", true);
	setting_enable_savedata_code = main.GetBoolean("Main", "Enable Custom Save Data Code", true);
	setting_enable_custom_npc_table = main.GetBoolean("NpcTable", "Enable Custom NPC Table Loading", false);
	setting_enable_reset_npctbl_on_reset = main.GetBoolean("NpcTable", "Enable npc.tbl Reloading on Reset", false);
	pause_menu_ava_enabled = main.GetBoolean("Extra", "Enable AVA Pause Additions", false);
}

void Init_INI_graphics()
{
	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gAVAConfigPath, "graphics.ini");
	INIReader graphics(path);

	// [Surface Names]
	char* settings_surfaceName_0_Title = copyString(graphics.GetString("Surface Names", "Default Title Surface Filename", "Title"));
	const char* settings_surfaceName_5_Image = copyString(graphics.GetString("Surface Names", "Default <IMG Surface Filename", "Image\\0"));
	const char* settings_surfaceName_6_Fade = copyString(graphics.GetString("Surface Names", "Default Fade Surface Filename", "Fade"));
	const char* settings_surfaceName_8_ItemImage = copyString(graphics.GetString("Surface Names", "Default ItemImage Surface Filename", "ItemImage"));
	const char* settings_surfaceName_11_Arms = copyString(graphics.GetString("Surface Names", "Default Arms Surface Filename", "Arms"));
	const char* settings_surfaceName_12_ArmsImage = copyString(graphics.GetString("Surface Names", "Default ArmsImage Surface Filename", "ArmsImage"));
	const char* settings_surfaceName_14_StageImage = copyString(graphics.GetString("Surface Names", "Default StageImage Surface Filename", "StageImage"));
	const char* settings_surfaceName_15_Loading = copyString(graphics.GetString("Surface Names", "Default Loading Surface Filename", "Loading"));
	const char* settings_surfaceName_16_MyChar = copyString(graphics.GetString("Surface Names", "Default MyChar Surface Filename", "MyChar"));
	const char* settings_surfaceName_17_Bullet = copyString(graphics.GetString("Surface Names", "Default Bullet Surface Filename", "Bullet"));
	const char* settings_surfaceName_19_Caret = copyString(graphics.GetString("Surface Names", "Default Caret Surface Filename", "Caret"));
	const char* settings_surfaceName_20_NpcSym = copyString(graphics.GetString("Surface Names", "Default NpcSym Surface Filename", "Npc\\NpcSym"));
	const char* settings_surfaceName_23_NpcRegu = copyString(graphics.GetString("Surface Names", "Default NpcRegu Surface Filename", "Npc\\NpcRegu"));
	const char* settings_surfaceName_24_AutumnUI = copyString(graphics.GetString("Surface Names", "Default Autumn UI Surface Filename", "AutumnUI"));
	const char* settings_surfaceName_25_AutumnObjects = copyString(graphics.GetString("Surface Names", "Default Autumn Objects Surface Filename", "Npc\\NpcAutumnObj"));
	const char* settings_surfaceName_26_TextBox = copyString(graphics.GetString("Surface Names", "Default TextBox Surface Filename", "TextBox"));
	const char* settings_surfaceName_27_Face = copyString(graphics.GetString("Surface Names", "Default Face Surface Filename", "Face"));
	const char* settings_surfaceName_38_AutumnItems = copyString(graphics.GetString("Surface Names", "Default Autumn Items Surface Filename", "Autumn"));
	const char* settings_surfaceName_39_AutumnCharacters = copyString(graphics.GetString("Surface Names", "Default Autumn Characters Surface Filename", "Npc\\NpcAutumnChar"));

	// Have to do some ugly stuff here until I can get better at programming, really. Sorry!
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

	// [UI]

	legacy_extra_jumps_ui = graphics.GetBoolean("UI", "Legacy Extra Jump UI Display", false);
	setting_jump_arrow_x_offset = graphics.GetInteger("UI", "Jump Arrow UI X Offset", -4);
	setting_jump_arrow_y_offset = graphics.GetInteger("UI", "Jump Arrow UI Y Offset", -20);

	setting_max_jump_arrow_display = graphics.GetInteger("UI", "Jump Arrow UI Limit", 5);

	setting_money_hud_x = graphics.GetInteger("UI", "Money Hud X", 8);
	setting_money_hud_y = graphics.GetInteger("UI", "Money Hud Y", 48);

	setting_money_hud_x_number_offset = graphics.GetInteger("UI", "Money Hud X Number Offset", 16);

	enable_collectables_a = graphics.GetBoolean("UI", "Enable Collectables UI A", false);
	enable_collectables_b = graphics.GetBoolean("UI", "Enable Collectables UI B", false);
	enable_collectables_c = graphics.GetBoolean("UI", "Enable Collectables UI C", false);
	enable_collectables_d = graphics.GetBoolean("UI", "Enable Collectables UI D", false);
	enable_collectables_e = graphics.GetBoolean("UI", "Enable Collectables UI E", false);

	collectables_a_x_pos = graphics.GetInteger("UI", "Collectables A (X Position)", 8);
	collectables_a_y_pos = graphics.GetInteger("UI", "Collectables A (Y Position)", 56);
	collectables_a_x_offset = graphics.GetInteger("UI", "Collectables A (X Number Offset)", 0);

	collectables_b_x_pos = graphics.GetInteger("UI", "Collectables B (X Position)", 8);
	collectables_b_y_pos = graphics.GetInteger("UI", "Collectables B (Y Position)", 64);
	collectables_b_x_offset = graphics.GetInteger("UI", "Collectables B (X Number Offset)", 0);

	collectables_c_x_pos = graphics.GetInteger("UI", "Collectables C (X Position)", 8);
	collectables_c_y_pos = graphics.GetInteger("UI", "Collectables C (Y Position)", 72);
	collectables_c_x_offset = graphics.GetInteger("UI", "Collectables C (X Number Offset)", 0);

	collectables_d_x_pos = graphics.GetInteger("UI", "Collectables D (X Position)", 8);
	collectables_d_y_pos = graphics.GetInteger("UI", "Collectables D (Y Position)", 80);
	collectables_d_x_offset = graphics.GetInteger("UI", "Collectables D (X Number Offset)", 0);

	collectables_e_x_pos = graphics.GetInteger("UI", "Collectables E (X Position)", 8);
	collectables_e_y_pos = graphics.GetInteger("UI", "Collectables E (Y Position)", 88);
	collectables_e_x_offset = graphics.GetInteger("UI", "Collectables E (X Number Offset)", 0);

	setting_ice_particles = graphics.GetBoolean("Visuals", "Ice Block Particle Effects", true);
}

void Init_INI_mychar()
{
	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gAVAConfigPath, "mychar.ini");
	INIReader mychar(path);

	jump_sound_effect = mychar.GetInteger("Main", "AVA Jump Sound Effect", 15);

	max_coyote = mychar.GetInteger("QOL", "Coyote Frames", 0);
	max_buffer = mychar.GetInteger("QOL", "Buffer Frames", 0);


	setting_walljumps_enabled = mychar.GetBoolean("Walljumps", "Walljumps Enabled", false);
	walljumps_can_be_buffered = mychar.GetBoolean("Walljumps", "Walljumps Can Be Buffered", false);
	setting_walljumps_flag_enabled = mychar.GetBoolean("Walljumps", "Walljumps Enable on Flag", false);
	setting_walljumps_flag = mychar.GetInteger("Walljumps", "Walljumps Flag", 6500);
	setting_walljump_horizontal_speed = mychar.GetInteger("Walljumps", "Walljumps Horizontal Speed", 1000);
	setting_walljump_water_horizontal_speed = mychar.GetInteger("Walljumps", "Underwater Walljumps Horizontal Speed", 300);
	setting_walljump_jump_height = mychar.GetInteger("Walljumps", "Walljumps Jump Momentum", 1113);
	setting_walljump_water_jump_height = mychar.GetInteger("Walljumps", "Underwater Walljumps Jump Momentum", 556);
	setting_walljump_sliding_speed = mychar.GetInteger("Walljumps", "Wallsliding Speed", 554);

	setting_extrajump_jump_height = mychar.GetInteger("Extra Jumps", "Extra Jump Momentum", 1280);
	setting_extrajump_water_jump_height = mychar.GetInteger("Extra Jumps", "Underwater Extra Jump Momentum", 640);

	setting_doublejump_enabled = mychar.GetBoolean("Double Jump", "Double Jump Enabled", false);
	setting_doublejump_flag_enabled = mychar.GetBoolean("Double Jump", "Double Jump Enable on Flag", false);
	setting_doublejump_flag = mychar.GetInteger("Double Jump", "Double Jump Flag", 6501);
	setting_double_jump_wall_jump_refresh = mychar.GetBoolean("Double Jump", "Walljumps refresh Double Jump", false);

	setting_run_button_enabled = mychar.GetBoolean("Sprinting", "Enable Run Button", false);
	setting_run_button_flag_enabled = mychar.GetBoolean("Sprinting", "Enable Run Button on Flag", false);
	setting_run_button_flag = mychar.GetInteger("Sprinting", "Run Button Flag", 6502);
	setting_running_speed = mychar.GetInteger("Sprinting", "Max Running Speed", 1218);

	setting_physics_max_dash = mychar.GetInteger("Physics", "Max Walking Speed", 812);
	setting_physics_max_move = mychar.GetInteger("Physics", "Max Top Movement Speed", 1535);
	setting_physics_gravity1 = mychar.GetInteger("Physics", "Gravity", 80);
	setting_physics_gravity2 = mychar.GetInteger("Physics", "Rising Gravity", 32);
	setting_physics_jump = mychar.GetInteger("Physics", "Jump Momentum", 1280);
	setting_physics_dash1 = mychar.GetInteger("Physics", "Walking Acceleration", 85);
	setting_physics_dash2 = mychar.GetInteger("Physics", "Air Control", 32);
	setting_physics_resist = mychar.GetInteger("Physics", "Friction", 51);

	setting_physics_water_max_dash = mychar.GetInteger("Physics", "Underwater Max Walking Speed", 406);
	setting_physics_water_max_move = mychar.GetInteger("Physics", "Underwater Top Movement Speed", 767);
	setting_physics_water_gravity1 = mychar.GetInteger("Physics", "Underwater Gravity", 40);
	setting_physics_water_gravity2 = mychar.GetInteger("Physics", "Underwater Rising Gravity", 16);
	setting_physics_water_jump = mychar.GetInteger("Physics", "Underwater Jump Momentum", 640);
	setting_physics_water_dash1 = mychar.GetInteger("Physics", "Underwater Walking Acceleration", 42);
	setting_physics_water_dash2 = mychar.GetInteger("Physics", "Underwater Air Control", 16);
	setting_physics_water_resist = mychar.GetInteger("Physics", "Underwater Friction", 25);

	booster_08_fuel = mychar.GetInteger("Physics", "Booster 0.8 Fuel", 50);
	booster_20_fuel = mychar.GetInteger("Physics", "Booster 2.0 Fuel", 50);

	setting_physics_boost20_accel_up = mychar.GetInteger("Physics", "Booster 2.0 Acceleration (Up)", -1535);
	setting_physics_boost20_accel_left = mychar.GetInteger("Physics", "Booster 2.0 Acceleration (Left)", -1535);
	setting_physics_boost20_accel_right = mychar.GetInteger("Physics", "Booster 2.0 Acceleration (Right)", 1535);
	setting_physics_boost20_accel_down = mychar.GetInteger("Physics", "Booster 2.0 Acceleration (Down)", 1535);
	setting_physics_boost20_accel_up_no_key = mychar.GetInteger("Physics", "Booster 2.0 Acceleration (Up, No Key)", -1535);

	setting_physics_boost08_accel_add = mychar.GetInteger("Physics", "Booster 0.8 Acceleration", 32);
	setting_physics_boost08_accel_max_speed = mychar.GetInteger("Physics", "Booster 0.8 Acceleration (Max Speed)", -1024);

	setting_physics_max_speed_left = mychar.GetInteger("Physics", "Player Max Speed (Left)", -1535);
	setting_physics_max_speed_right = mychar.GetInteger("Physics", "Player Max Speed (Right)", 1535);
	setting_physics_max_speed_up = mychar.GetInteger("Physics", "Player Max Speed (Up)", -1535);
	setting_physics_max_speed_down = mychar.GetInteger("Physics", "Player Max Speed (Down)", 1535);

	setting_physics_water_max_speed_left = mychar.GetInteger("Physics", "Underwater Player Max Speed (Left)", -767);
	setting_physics_water_max_speed_right = mychar.GetInteger("Physics", "Underwater Player Max Speed (Right)", 767);
	setting_physics_water_max_speed_up = mychar.GetInteger("Physics", "Underwater Player Max Speed (Up)", -767);
	setting_physics_water_max_speed_down = mychar.GetInteger("Physics", "Underwater Player Max Speed (Down)", 767);

	setting_enable_life_drain = mychar.GetBoolean("Life Drain/Regen", "Life Drain when Flag is set", false);
	setting_enable_life_regen = mychar.GetBoolean("Life Drain/Regen", "Life Regen when Flag is set", false);

	setting_life_drain_flag = mychar.GetInteger("Life Drain/Regen", "Life Drain Flag", 6504);
	setting_life_regen_flag = mychar.GetInteger("Life Drain/Regen", "Life Regen Flag", 6505);

	setting_life_drain_wait = mychar.GetInteger("Life Drain/Regen", "Life Drain Wait", 120);
	setting_life_regen_wait = mychar.GetInteger("Life Drain/Regen", "Life Regen Wait", 240);

	setting_life_drain_amount = mychar.GetInteger("Life Drain/Regen", "Life Drain Amount", 1);
	setting_life_regen_amount = mychar.GetInteger("Life Drain/Regen", "Life Regen Amount", 1);

	setting_bounce_speed = mychar.GetInteger("Collision", "Bouncy Block Speed", 1535);
	setting_bounce_sfx = mychar.GetInteger("Collision", "Bouncy Block Sound Effect", 25);
}

void Init_INI_qol()
{
	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gAVAConfigPath, "qol.ini");
	INIReader qol(path);

	setting_enable_teleporter_bugfix = qol.GetBoolean("QOL", "Enable Teleporter Transparency Fix", true);
	setting_disable_window_rect_saving = qol.GetBoolean("QOL", "Disable window.rect Saving", true);
	setting_disable_errorlog_saving = qol.GetBoolean("QOL", "Disable error.log Saving", false);
	setting_enable_default_config_options = qol.GetBoolean("QOL", "Autumn's Default Config Options", true);
}

void Init_INI_tsc()
{
	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gAVAConfigPath, "tsc.ini");
	INIReader tsc(path);

	setting_enable_mim_mod = tsc.GetBoolean("Main", "Enable <MIM Command", true);
	setting_disable_tsc_encryption = tsc.GetBoolean("Main", "Disable TSC Encryption", false);
	setting_show_img_on_top_flag = tsc.GetInteger("Main", "Show Image Above Textbox Flag", 6503);
	setting_enable_custom_armsitem_location = tsc.GetBoolean("Main", "Enable Custom ArmsItem.tsc Locations", false);
	setting_enable_custom_head_location = tsc.GetBoolean("Main", "Enable Custom Head.tsc Locations", false);
	defaultArmsItemTSCName = copyString(tsc.GetString("Main", "Default ArmsItem.tsc name", "ArmsItem"));
	defaultHeadTSCName = copyString(tsc.GetString("Main", "Default Head.tsc name", "Head"));

	mim_player_size_width = tsc.GetInteger("Commands", "<MIM Player Width", 16);
	mim_player_size_height = tsc.GetInteger("Commands", "<MIM Player Height", 16);
	setting_revive_hp = tsc.GetInteger("Commands", "<RVI (Revive) HP amount", 3);

	setting_enable_money_code = tsc.GetBoolean("Money", "Enable Money Code", false);
	setting_money_disable_enemy_money_drops = tsc.GetBoolean("Money", "Disable Money from Entities", false);
	setting_money_disable_exp_drops = tsc.GetBoolean("Money", "Disable EXP with Money", false);
	setting_exp_acts_as_money = tsc.GetBoolean("Money", "EXP also acts as Money", false);
	setting_money_division = tsc.GetBoolean("Money", "Money is divided by 2", true);
}

void InitSettingsRevamp()
{
	Init_INI_main();
	printf("Loaded main.ini\n");
	Init_INI_graphics();
	printf("Loaded graphics.ini\n");
	Init_INI_mychar();
	printf("Loaded mychar.ini\n");
	Init_INI_qol();
	printf("Loaded qol.ini\n");
	Init_INI_tsc();
	printf("Loaded tsc.ini\n");
}