#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "MyChar.h"

#include "main.h"
#include "cave_story.h"

int current_jumps = 0;
int onWall;

bool setting_walljumps_enabled = false;
bool setting_walljumps_flag_enabled = false;
bool setting_doublejump_enabled = false;
bool setting_doublejump_flag_enabled = false;
int setting_walljumps_flag = 6500;
int setting_doublejump_flag = 6501;
int setting_walljump_jump_height = 1280; // the jump force of a wall jump
int setting_walljump_sliding_speed = 554; // wall jump sliding speed
int setting_extrajump_jump_height = 1280; // the jump force of an extra jump

// physics settings
// if the run button is enabled, you must set your own physics

int setting_physics_water_max_dash = 406;
int setting_physics_water_max_move = 767;
int setting_physics_water_gravity1 = 40;
int setting_physics_water_gravity2 = 16;
int setting_physics_water_jump = 640;
int setting_physics_water_dash1 = 42;
int setting_physics_water_dash2 = 16;
int setting_physics_water_resist = 25;

int setting_physics_max_dash = 812;
int setting_physics_max_move = 1535;
int setting_physics_gravity1 = 80;
int setting_physics_gravity2 = 32;
int setting_physics_jump = 1280;
int setting_physics_dash1 = 85;
int setting_physics_dash2 = 32;
int setting_physics_resist = 51;

void ActMyChar_RunButton(BOOL bKey, Physics *physics)
{
	// Controller players can't do this.
	if (gKey & gKeyShift)
	{
		physics->max_dash = 0x32C * 1.5;
	}
	else
	{
		physics->max_dash = 0x32C;
	}
}

void setPlayerPhysics(BOOL bKey, Physics *physics)
{
	if (gMC->flag & 0x100)
	{
		physics->max_dash = 0x32C / 2;
		physics->max_move = 0x5FF / 2;
		physics->gravity1 = 0x50 / 2;
		physics->gravity2 = 0x20 / 2;
		physics->jump = 0x500 / 2;
		physics->dash1 = 0x200 / 6 / 2;
		physics->dash2 = 0x200 / 16 / 2;
		physics->resist = 0x200 / 10 / 2;
	}
	else
	{
		physics->max_dash = 0x32C;
		physics->max_move = 0x5FF;
		physics->gravity1 = 0x50;
		physics->gravity2 = 0x20;
		physics->jump = 0x500;
		physics->dash1 = 0x200 / 6;
		physics->dash2 = 0x200 / 16;
		physics->resist = 0x200 / 10;
	}

	ActMyChar_RunButton(bKey, physics);
}

void ActMyChar_OnWall(BOOL bKey)
{
	if (gMC->flag & 4 && gKeyRight) // In contact with right wall
		onWall = 1;
	else if (gMC->flag & 1 && gKeyLeft) // In contact with left wall
		onWall = -1;
	else
		onWall = 0;
}

void ActMyChar_WallJump(BOOL bKey)
{
	//Wall jumping
	
	// Only enable walljumping if you either:
	// A: Have walljumps on flag set + the flag is set
	// B: Have walljumps on flag *not* set
	if ((setting_walljumps_flag_enabled == true && GetNPCFlag(setting_walljumps_flag)) || (setting_walljumps_flag_enabled == false))
	{
		if (bKey)
		{
			if (onWall != 0)
			{
				if (gKeyTrg & gKeyJump)
				{
					gMC->xm = onWall * -1000;
					gMC->ym = -setting_walljump_jump_height / 1.15;
					PlaySoundObject(15, SOUND_MODE_PLAY);
					if (onWall == 1)
					{
						SetCaret(gMC->x + (8 * 0x200), gMC->y, 13, 0);
						SetCaret(gMC->x + (8 * 0x200), gMC->y, 13, 0);
					}
					else {
						SetCaret(gMC->x - (8 * 0x200), gMC->y, 13, 0);
						SetCaret(gMC->x - (8 * 0x200), gMC->y, 13, 0);
					}

				}

				// Slide down the wall if holding against it
				if (gMC->ym > setting_walljump_sliding_speed)
					gMC->ym = setting_walljump_sliding_speed;
			}
		}
	}
}

void ActMyChar_AirJumps(BOOL bKey)
{
	//Air jumps
	int i;
	if (bKey)
	{
		if (gKeyTrg & gKeyJump && !(gMC->flag & 8) && gMC->boost_cnt == 0 && onWall == 0)
		{
			if (--current_jumps >= 0)
			{
				gMC->ym = -setting_extrajump_jump_height;
				PlaySoundObject(15, SOUND_MODE_PLAY);
				for (i = 0; i < 4; ++i)
					SetNpChar(4, gMC->x + (Random(-12, 12) * 0x200), gMC->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);
			}
		}
	}

	// Remove jumps when grounded
	if (gMC->flag & 8)
	{
		if (setting_doublejump_enabled)
		{
			if ((setting_doublejump_flag_enabled == true && GetNPCFlag(setting_doublejump_flag) || setting_doublejump_flag_enabled == false))
				current_jumps = 1; // 1 permanent air jump
			else
				current_jumps = 0;
		}
		else
			current_jumps = 0;
	}
}

void ActMyChar_Normal_Custom(BOOL bKey)
{
	ActMyChar_OnWall(bKey); // Check if the player is on a wall

	if (setting_walljumps_enabled)
		ActMyChar_WallJump(bKey);

	ActMyChar_AirJumps(bKey);
}