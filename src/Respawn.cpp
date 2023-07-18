#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Main.h"
#include "Respawn.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ASMPatches.h"
#include "ASMLoader.h"
#include "BKG.h"
#include "Collectables.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "MycParam.h"
#include "PauseScreen.h"
#include "TextScript.h"
#include "TileCollisionBoss.h"
#include "TileCollisionBullet.h"
#include "TileCollisionEntity.h"
#include "TileCollisionMyChar.h"
#include "TextScriptVAR.h"

RESPAWN_PT gRespawnPoint;
BOOL bSetRespawn = FALSE;
BOOL PlayerIsRespawning = FALSE;

void SetRespawnPoint(int x, int y)
{
	gRespawnPoint.stage_no = gStageNo;
	gRespawnPoint.x = x;
	gRespawnPoint.y = y;
	gRespawnPoint.direct = gMC->direct;
	gRespawnPoint.max_life = gMC->max_life;
	gRespawnPoint.equip = gMC->equip;
	Freeware_memcpy(gRespawnPoint.flags, gFlagNPC, sizeof(gFlagNPC));
	Freeware_memcpy(gRespawnPoint.arms, gArmsData, sizeof(gRespawnPoint.arms));
	Freeware_memcpy(gRespawnPoint.items, gItemData, sizeof(gRespawnPoint.items));
	// more custom stuff
	Freeware_memcpy(gRespawnPoint.bkgTxT, bkgTxT_Global, sizeof(gRespawnPoint.bkgTxT)); // <BKG txt file?
	Freeware_memcpy(gRespawnPoint.collectables, &gCollectables, sizeof(gRespawnPoint.collectables));
	gRespawnPoint.playerMoney = playerMoney; // money
	gRespawnPoint.enable_collectables_a = enable_collectables_a; // collectables enabled
	gRespawnPoint.enable_collectables_b = enable_collectables_b;
	gRespawnPoint.enable_collectables_c = enable_collectables_c;
	gRespawnPoint.enable_collectables_d = enable_collectables_d;
	gRespawnPoint.enable_collectables_e = enable_collectables_e;
	// collectables positioning
	// X positions
	gRespawnPoint.collectables_a_x_pos = collectables_a_x_pos;
	gRespawnPoint.collectables_b_x_pos = collectables_b_x_pos;
	gRespawnPoint.collectables_c_x_pos = collectables_c_x_pos;
	gRespawnPoint.collectables_d_x_pos = collectables_d_x_pos;
	gRespawnPoint.collectables_e_x_pos = collectables_e_x_pos;
	// Y positions
	gRespawnPoint.collectables_a_y_pos = collectables_a_y_pos;
	gRespawnPoint.collectables_b_y_pos = collectables_b_y_pos;
	gRespawnPoint.collectables_c_y_pos = collectables_c_y_pos;
	gRespawnPoint.collectables_d_y_pos = collectables_d_y_pos;
	gRespawnPoint.collectables_e_y_pos = collectables_e_y_pos;
	// X offsets
	gRespawnPoint.collectables_a_x_offset = collectables_a_x_offset;
	gRespawnPoint.collectables_b_x_offset = collectables_a_x_offset;
	gRespawnPoint.collectables_c_x_offset = collectables_a_x_offset;
	gRespawnPoint.collectables_d_x_offset = collectables_a_x_offset;
	gRespawnPoint.collectables_e_x_offset = collectables_a_x_offset;
	// <PHY
	gRespawnPoint.phy_physics_max_dash = setting_physics_max_dash;
	gRespawnPoint.phy_physics_max_move = setting_physics_max_move;
	gRespawnPoint.phy_physics_gravity1 = setting_physics_gravity1;
	gRespawnPoint.phy_physics_gravity2 = setting_physics_gravity2;
	gRespawnPoint.phy_physics_dash1 = setting_physics_dash1;
	gRespawnPoint.phy_physics_dash2 = setting_physics_dash2;
	gRespawnPoint.phy_physics_resist = setting_physics_resist;
	gRespawnPoint.phy_physics_jump = setting_physics_jump;
	gRespawnPoint.phy_physics_water_max_dash = setting_physics_water_max_dash;
	gRespawnPoint.phy_physics_water_max_move = setting_physics_water_max_move;
	gRespawnPoint.phy_physics_water_gravity1 = setting_physics_water_gravity1;
	gRespawnPoint.phy_physics_water_gravity2 = setting_physics_water_gravity2;
	gRespawnPoint.phy_physics_water_dash1 = setting_physics_water_dash1;
	gRespawnPoint.phy_physics_water_dash2 = setting_physics_water_dash2;
	gRespawnPoint.phy_physics_water_resist = setting_physics_water_resist;
	gRespawnPoint.phy_physics_water_jump = setting_physics_water_jump;
	gRespawnPoint.phy_walljump_horizontal_speed = setting_walljump_horizontal_speed;
	gRespawnPoint.phy_walljump_jump_height = setting_walljump_jump_height;
	gRespawnPoint.phy_walljump_sliding_speed = setting_walljump_sliding_speed;
	gRespawnPoint.phy_walljump_water_horizontal_speed = setting_walljump_water_horizontal_speed;
	gRespawnPoint.phy_walljump_water_jump_height = setting_walljump_water_jump_height;
	gRespawnPoint.phy_extrajump_jump_height = setting_extrajump_jump_height;
	gRespawnPoint.phy_extrajump_water_jump_height = setting_extrajump_water_jump_height;
	gRespawnPoint.phy_running_speed = setting_running_speed;
	gRespawnPoint.phy_bounce_speed = setting_bounce_speed;
	// TSC VarData
	Freeware_memcpy(gRespawnPoint.varData, varData, sizeof(gRespawnPoint.varData));
	// <MIM
	gRespawnPoint.mim = mim_num;
}

BOOL RespawnPlayer(void)
{
	PlayerIsRespawning = TRUE;

	// Modeling this after LoadProfile
	Freeware_memcpy(gFlagNPC, gRespawnPoint.flags, sizeof(gFlagNPC));
	Freeware_memcpy(bkgTxT_Global, gRespawnPoint.bkgTxT, sizeof(bkgTxT_Global));

	InitMyChar();
	if (!TransferStage(gRespawnPoint.stage_no, 0, gRespawnPoint.x, gRespawnPoint.y))
		return FALSE;

	gMC->equip = gRespawnPoint.equip;
	gMC->direct = gRespawnPoint.direct;
	gMC->max_life = gRespawnPoint.max_life;
	gMC->life = gMC->max_life;
	gMC->cond = 0x80;
	gMC->air = 1000;
	gMC->lifeBr = gMC->life;
	gMC->x = gRespawnPoint.x * 0x10 * 0x200;
	gMC->y = gRespawnPoint.y * 0x10 * 0x200;
	
	gSelectedArms = gRespawnPoint.select_arms;
	gSelectedItem = gRespawnPoint.select_item;

	Freeware_memcpy(gArmsData, gRespawnPoint.arms, sizeof(gArmsData));
	Freeware_memcpy(gItemData, gRespawnPoint.items, sizeof(gItemData));

	// Set custom data
	// <BKG txt file
	
	// Collectables Values
	Freeware_memcpy(&gCollectables, gRespawnPoint.collectables, sizeof(gCollectables));
	// Money
	playerMoney = gRespawnPoint.playerMoney;
	// Enable Collectables
	enable_collectables_a = gRespawnPoint.enable_collectables_a;
	enable_collectables_b = gRespawnPoint.enable_collectables_b;
	enable_collectables_c = gRespawnPoint.enable_collectables_c;
	enable_collectables_d = gRespawnPoint.enable_collectables_d;
	enable_collectables_e = gRespawnPoint.enable_collectables_e;
	// Collectables Positioning
	collectables_a_x_pos = gRespawnPoint.collectables_a_x_pos;
	collectables_b_x_pos = gRespawnPoint.collectables_b_x_pos;
	collectables_c_x_pos = gRespawnPoint.collectables_c_x_pos;
	collectables_d_x_pos = gRespawnPoint.collectables_d_x_pos;
	collectables_e_x_pos = gRespawnPoint.collectables_e_x_pos;
	collectables_a_y_pos = gRespawnPoint.collectables_a_y_pos;
	collectables_b_y_pos = gRespawnPoint.collectables_b_y_pos;
	collectables_c_y_pos = gRespawnPoint.collectables_c_y_pos;
	collectables_d_y_pos = gRespawnPoint.collectables_d_y_pos;
	collectables_e_y_pos = gRespawnPoint.collectables_e_y_pos;
	collectables_a_x_offset = gRespawnPoint.collectables_a_x_offset;
	collectables_b_x_offset = gRespawnPoint.collectables_b_x_offset;
	collectables_c_x_offset = gRespawnPoint.collectables_c_x_offset;
	collectables_d_x_offset = gRespawnPoint.collectables_d_x_offset;
	collectables_e_x_offset = gRespawnPoint.collectables_e_x_offset;
	// <PHY
	setting_physics_max_dash = gRespawnPoint.phy_physics_max_dash;
	setting_physics_max_move = gRespawnPoint.phy_physics_max_move;
	setting_physics_gravity1 = gRespawnPoint.phy_physics_gravity1;
	setting_physics_gravity2 = gRespawnPoint.phy_physics_gravity2;
	setting_physics_dash1 = gRespawnPoint.phy_physics_dash1;
	setting_physics_dash2 = gRespawnPoint.phy_physics_dash2;
	setting_physics_resist = gRespawnPoint.phy_physics_resist;
	setting_physics_jump = gRespawnPoint.phy_physics_jump;
	setting_physics_water_max_dash = gRespawnPoint.phy_physics_water_max_dash;
	setting_physics_water_max_move = gRespawnPoint.phy_physics_water_max_move;
	setting_physics_water_gravity1 = gRespawnPoint.phy_physics_water_gravity1;
	setting_physics_water_gravity2 = gRespawnPoint.phy_physics_water_gravity2;
	setting_physics_water_dash1 = gRespawnPoint.phy_physics_water_dash1;
	setting_physics_water_dash2 = gRespawnPoint.phy_physics_water_dash2;
	setting_physics_water_resist = gRespawnPoint.phy_physics_water_resist;
	setting_physics_water_jump = gRespawnPoint.phy_physics_water_jump;
	setting_walljump_horizontal_speed = gRespawnPoint.phy_walljump_horizontal_speed;
	setting_walljump_jump_height = gRespawnPoint.phy_walljump_jump_height;
	setting_walljump_sliding_speed = gRespawnPoint.phy_walljump_sliding_speed;
	setting_walljump_water_horizontal_speed = gRespawnPoint.phy_walljump_water_horizontal_speed;
	setting_walljump_water_jump_height = gRespawnPoint.phy_walljump_water_jump_height;
	setting_extrajump_jump_height = gRespawnPoint.phy_extrajump_jump_height;
	setting_extrajump_water_jump_height = gRespawnPoint.phy_extrajump_water_jump_height;
	setting_running_speed = gRespawnPoint.phy_running_speed;
	setting_bounce_speed = gRespawnPoint.phy_bounce_speed;
	// <MIM
	mim_num = gRespawnPoint.mim;

	// Reset stuff
	ClearFade();
	SetFrameMyChar();
	SetFrameTargetMyChar(16);
	InitBossLife();
	CutNoise();
	InitStar();
	ClearValueView();
	if (PlayerIsRespawning == TRUE)
		PlayerIsRespawning = FALSE;
	gCurlyShoot_wait = 0;

	return TRUE;
}

void Stage_SetRespawn()
{
	bSetRespawn = FALSE;
	SetRespawnPoint((gMC->x + 0x1000) / 0x2000, (gMC->y + 0x1000) / 0x2000);
}