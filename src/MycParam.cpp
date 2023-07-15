#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "MycParam.h"

#include "main.h"
#include "cave_story.h"
#include "Collectables.h"
#include "Entity.h"
#include "ModSettings.h"
#include "TextScript.h"

bool setting_enable_life_drain = false;
bool setting_enable_life_regen = false;

int setting_life_drain_flag = 6504;
int setting_life_regen_flag = 6505;

int setting_life_drain_wait = 60;
int setting_life_regen_wait = 120;

int setting_life_drain_amount = 1;
int setting_life_regen_amount = 1;

void RemoveMaxLifeMyChar(int val)
{
	gMC->max_life -= (short)val;
	if (gMC->max_life > 232)
		gMC->max_life = 232;
	gMC->life -= (short)val;
	gMC->lifeBr = gMC->life;
}

void PutMyMoney()
{
	RECT rcDollarSign = { 8, 0, 16, 8 };
	PutBitmap3(&grcGame, setting_money_hud_x + 8, setting_money_hud_y, &rcDollarSign, SURFACE_ID_AUTUMN_HUD);
	PutNumber4(setting_money_hud_x + setting_money_hud_x_number_offset, setting_money_hud_y, playerMoney, FALSE);
}

void PutMyCollectables()
{
	RECT rcCollectable[CollectableCount] = {
		{16, 0, 24, 8},
		{16, 8, 24, 16},
		{16, 16, 24, 24},
		{16, 24, 24, 32},
		{16, 32, 24, 40},
	};

	if (enable_collectables_a == true)
	{
		PutBitmap3(&grcGame, collectables_a_x_pos + 8, collectables_a_y_pos, &rcCollectable[0], SURFACE_ID_AUTUMN_HUD);
		PutNumber4(collectables_a_x_pos + collectables_a_x_offset, collectables_a_y_pos, gCollectables.numA, FALSE);
	}
	if (enable_collectables_b == true)
	{
		PutBitmap3(&grcGame, collectables_b_x_pos + 8, collectables_b_y_pos, &rcCollectable[1], SURFACE_ID_AUTUMN_HUD);
		PutNumber4(collectables_b_x_pos + collectables_b_x_offset, collectables_b_y_pos, gCollectables.numB, FALSE);
	}
	if (enable_collectables_c == true)
	{
		PutBitmap3(&grcGame, collectables_c_x_pos + 8, collectables_c_y_pos, &rcCollectable[2], SURFACE_ID_AUTUMN_HUD);
		PutNumber4(collectables_c_x_pos + collectables_c_x_offset, collectables_c_y_pos, gCollectables.numC, FALSE);
	}
	if (enable_collectables_d == true)
	{
		PutBitmap3(&grcGame, collectables_d_x_pos + 8, collectables_d_y_pos, &rcCollectable[3], SURFACE_ID_AUTUMN_HUD);
		PutNumber4(collectables_d_x_pos + collectables_d_x_offset, collectables_d_y_pos, gCollectables.numD, FALSE);
	}
	if (enable_collectables_e == true)
	{
		PutBitmap3(&grcGame, collectables_e_x_pos + 8, collectables_e_y_pos, &rcCollectable[4], SURFACE_ID_AUTUMN_HUD);
		PutNumber4(collectables_e_x_pos + collectables_e_x_offset, collectables_e_y_pos, gCollectables.numE, FALSE);
	}

}

int waitLifeDrain = 0;
int waitLifeRegen = 0;

void PlayerLifeDrain()
{
	if (waitLifeDrain > setting_life_drain_wait)
	{
		if (gMC->life != 1)
			gMC->life -= setting_life_drain_amount;
		else
			PlayerDeath();

		waitLifeDrain = 0;
	}

	++waitLifeDrain;
}

void PlayerLifeRegen()
{
	if (waitLifeRegen > setting_life_regen_wait)
	{
		if (gMC->life != gMC->max_life)
			gMC->life += setting_life_regen_amount;

		waitLifeRegen = 0;
	}

	++waitLifeRegen;
}

// 0x410856
void Replacement_PutActiveArmsList_Call()
{
	PutActiveArmsList();

	if (setting_enable_money_code)
		PutMyMoney();

	PutMyCollectables();

	// Enable life drain/regen and have them run only while the HUD is visible
	if (setting_enable_life_drain == true && GetNPCFlag(setting_life_drain_flag))
		PlayerLifeDrain();

	if (setting_enable_life_regen == true && GetNPCFlag(setting_life_regen_flag))
		PlayerLifeRegen();
}