#pragma once

#include "BKG.h"
#include "cave_story.h"
#include "Collectables.h"
#include "TextScriptVAR.h"

// Seems like Pixel named all of his structs in ALL CAPS
struct RESPAWN_PT
{
	int stage_no;
	int x;
	int y;
	int direct;
	short max_life;
	int equip;
	unsigned char flags[1000];
	int select_arms;
	int select_item;
	ARMS arms[8];
	ITEM items[32];
	char bkgTxT[bkgTxTSize];
	COLLECTABLES collectables[CollectableCount]; // collectables struct
	int playerMoney; // money
	// .. and now all of the customizable collectable stuff
	bool enable_collectables_a;
	bool enable_collectables_b;
	bool enable_collectables_c;
	bool enable_collectables_d;
	bool enable_collectables_e;
	// .. and now collectable offsets/positions
	int collectables_a_x_pos;
	int collectables_b_x_pos;
	int collectables_c_x_pos;
	int collectables_d_x_pos;
	int collectables_e_x_pos;
	int collectables_a_y_pos;
	int collectables_b_y_pos;
	int collectables_c_y_pos;
	int collectables_d_y_pos;
	int collectables_e_y_pos;
	int collectables_a_x_offset;
	int collectables_b_x_offset;
	int collectables_c_x_offset;
	int collectables_d_x_offset;
	int collectables_e_x_offset;
	// .. and now player physics i think (<PHY)
	int phy_physics_max_dash;
	int phy_physics_max_move;
	int phy_physics_gravity1;
	int phy_physics_gravity2;
	int phy_physics_dash1;
	int phy_physics_dash2;
	int phy_physics_resist;
	int phy_physics_jump;
	int phy_physics_water_max_dash;
	int phy_physics_water_max_move;
	int phy_physics_water_gravity1;
	int phy_physics_water_gravity2;
	int phy_physics_water_dash1;
	int phy_physics_water_dash2;
	int phy_physics_water_resist;
	int phy_physics_water_jump;
	int phy_walljump_horizontal_speed;
	int phy_walljump_jump_height;
	int phy_walljump_sliding_speed;
	int phy_walljump_water_horizontal_speed;
	int phy_walljump_water_jump_height;
	int phy_extrajump_jump_height;
	int phy_extrajump_water_jump_height;
	int phy_running_speed;
	int phy_bounce_speed;
	// .. TSC Var Data
	int varData[TSC_VAR_MAX];
	// <MIM
	int mim;
};

extern RESPAWN_PT gRespawnPoint; // Again with the "g" prefix
extern BOOL bSetRespawn;
extern BOOL PlayerIsRespawning;

extern int setting_revive_hp;

void SetRespawnPoint(int x, int y);
BOOL RespawnPlayer(void);
void Stage_SetRespawn();
void RevivePlayer();