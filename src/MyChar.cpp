#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "MyChar.h"

#include "main.h"
#include "ModSettings.h"
#include "cave_story.h"
#include "Collectables.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "Profile.h"
#include "Stage.h"
#include "TextScript.h"
#include "TextScriptVAR.h"
#include "TileCollisionMyChar.h"

int current_jumps = 0;
int onWall;

bool setting_walljumps_enabled = false;
bool setting_walljumps_flag_enabled = false;
bool setting_doublejump_enabled = false;
bool setting_doublejump_flag_enabled = false;
int setting_walljumps_flag = 6500;
int setting_doublejump_flag = 6501;
int setting_walljump_horizontal_speed = 1000;
int setting_walljump_water_horizontal_speed = 300;
int setting_walljump_jump_height = 1113; // the jump force of a wall jump
int setting_walljump_water_jump_height = 556; // the jump force of a wall jump while in water
int setting_walljump_sliding_speed = 554; // wall jump sliding speed
int setting_extrajump_jump_height = 1280; // the jump force of an extra jump
int setting_extrajump_water_jump_height = 640; // the jump force of an extra jump while in water

// physics settings
// if the run button is enabled, you must set your own physics

bool setting_physics_ini_enabled = false;
bool setting_run_button_enabled = false;
bool setting_run_button_flag_enabled = false;
int setting_run_button_flag = 6502;

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

// Speed for running with the run button
int setting_running_speed = 1218;

// Ice particles
bool setting_ice_particles = true;

// Jump Arrow Display Offsets
int setting_jump_arrow_x_offset = 4;
int setting_jump_arrow_y_offset = 20;

// Jump Arrow is negative
bool setting_jump_arrow_x_offset_negative = true;
bool setting_jump_arrow_y_offset_negative = true;

// Jump Arrow max display (Won't show anymore)
int setting_max_jump_arrow_display = 5;

int custom_camera_offset_x = 0;
int custom_camera_offset_y = 0;
int custom_index_x = 0;
int custom_index_y = 0;
int custom_tgt_x = 0;
int custom_tgt_y = 0;

bool is_direction_locked = false;
int lock_direction = 0;

int mim_num = 0;

int mim_player_size_width = 16;
int mim_player_size_height = 16;

void Replacement_InitMyChar_memset_Call(void* dst, int val, size_t size)
{
	Freeware_memset(dst, val, size);
	InitMyCharPhysics();
	InitCollectables();
	InitCollectablesPositioning();
	InitCollectablesEnabled();
	InitMoney();
	InitTSCVariables();
	InitMyCharBoostFuel();

	if (setting_external_stage_tbl_support == true)
	{
		if (stageTblPath[0] == 0)
			LoadStageTable(NULL); // load default stage.tbl when running InitMyChar
	}

	if (setting_enable_collab_npc_table == true)
	{
		if (npcTblPath[0] == 0)
			LoadCustomNpcTable(NULL); // load default npc.tbl when running InitMyChar
	}

	mim_num = 0;
}

// Doesn't use built-in MyChar data
void InitMyChar_Original(void)
{
	Freeware_memset(&gMC, 0, sizeof(MYCHAR));
	gMC->cond = 0x80;
	gMC->direct = 2;

	gMC->view.back = 8 * 0x200;
	gMC->view.top = 8 * 0x200;
	gMC->view.front = 8 * 0x200;
	gMC->view.bottom = 8 * 0x200;

	gMC->hit.back = 5 * 0x200;
	gMC->hit.top = 8 * 0x200;
	gMC->hit.front = 5 * 0x200;
	gMC->hit.bottom = 8 * 0x200;

	gMC->life = 3;
	gMC->max_life = 3;
	gMC->unit = 0;
}

void InitMyCollabData()
{
	InitMyChar_Original();
	InitCollectablesPositioning();
	InitCollectablesEnabled();
	InitTSCVariables();
	InitMyCharBoostFuel();
}

void SetPlayerPhysics(int x, int y)
{
	switch (x)
	{
		default:
		case 0:
			setting_physics_max_dash = y;
			break;

		case 1:
			setting_physics_max_move = y;
			break;

		case 2:
			setting_physics_gravity1 = y;
			break;

		case 3:
			setting_physics_gravity2 = y;
			break;

		case 4:
			setting_physics_dash1 = y;
			break;

		case 5:
			setting_physics_dash2 = y;
			break;

		case 6:
			setting_physics_resist = y;
			break;

		case 7:
			setting_physics_jump = y;
			break;

		case 8:
			setting_physics_water_max_dash = y;
			break;

		case 9:
			setting_physics_water_max_move = y;
			break;

		case 10:
			setting_physics_water_gravity1 = y;
			break;

		case 11:
			setting_physics_water_gravity2 = y;
			break;

		case 12:
			setting_physics_water_dash1 = y;
			break;

		case 13:
			setting_physics_water_dash2 = y;
			break;

		case 14:
			setting_physics_water_resist = y;
			break;

		case 15:
			setting_physics_water_jump = y;
			break;

		// Custom DLL <PHY changes

		// Wall jumps
		case 16:
			setting_walljump_horizontal_speed = y;
			break;

		case 17:
			setting_walljump_jump_height = y;
			break;

		case 18:
			setting_walljump_sliding_speed = y;
			break;

		case 19:
			setting_walljump_water_horizontal_speed = y;
			break;

		case 20:
			setting_walljump_water_jump_height = y;
			break;

		// Extra jumps
		case 21:
			setting_extrajump_jump_height = y;
			break;

		case 22:
			setting_extrajump_water_jump_height = y;
			break;

		// Run button speed
		case 23:
			setting_running_speed = y;
			break;

		// bounce block speed
		case 24:
			setting_bounce_speed = y;
			break;
	}
}

void PutPlayersJumps(int fx, int fy)
{
	RECT rcJumpArrow = { 0, 0, 8, 8 };

	// Jank way of allowing negative/positive values in settings.ini
	int jump_arrow_negative_x = -1;
	int jump_arrow_negative_y = -1;

	if (setting_jump_arrow_x_offset_negative == true)
		jump_arrow_negative_x = -1;
	else
		jump_arrow_negative_x = 1;

	// This is so fucked
	if (setting_jump_arrow_y_offset_negative == true)
		jump_arrow_negative_y = -1;
	else
		jump_arrow_negative_y = 1;

	// Rect Changing
	if (current_jumps >= 2)
	{
		// Only do change rect if you arent going above the max limit
		if (!(current_jumps >= (setting_max_jump_arrow_display + 1)))
		{
			rcJumpArrow.top += 8 * (current_jumps - 1);
			rcJumpArrow.bottom += 8 * (current_jumps - 1);
		}

		// Display final jump for the max display setting
		if (current_jumps > setting_max_jump_arrow_display)
		{
			rcJumpArrow.top += 8 * (setting_max_jump_arrow_display - 1);
			rcJumpArrow.bottom += 8 * (setting_max_jump_arrow_display - 1);
		}
	}

	if (current_jumps >= 1)
		PutBitmap3(&grcGame, (gMC->x / 0x200) + (setting_jump_arrow_x_offset * jump_arrow_negative_x) - (fx / 0x200), (gMC->y / 0x200) + (setting_jump_arrow_y_offset * jump_arrow_negative_y) - (fy / 0x200), &rcJumpArrow, SURFACE_ID_AUTUMN_HUD);
}

// 0x415535
void Replacement_PutMyChar_PutChar_Call(const RECT* r, int a, int b, const RECT* r2, SurfaceID s)
{
	RECT rect_player = gMC->rect;

	rect_player.top += (mim_player_size_height * 2) * mim_num;
	rect_player.bottom += (mim_player_size_height * 2) * mim_num;

	PutBitmap3(r, a, b, &rect_player, s);
}

void Replacement_PutMyChar_Call(int fx, int fy)
{
	PutMyChar(fx, fy);

	PutPlayersJumps(fx, fy);
}

void Replacement_SetMyCharPosition_InitStar_Call()
{
	// Possibly reset the offset and locked direction here?
	custom_tgt_x = gMC->x;
	custom_tgt_y = gMC->y;
	InitStar();
}

void CameraHorizontalHelper(BOOL bKey)
{
	if (gMC->direct == 0)
	{
		custom_index_x -= 0x200;
		if (custom_index_x < -0x8000)
			custom_index_x = -0x8000;
	}
	else
	{
		custom_index_x += 0x200;
		if (custom_index_x > 0x8000)
			custom_index_x = 0x8000;
	}
}

void CameraVerticalHelper(BOOL bKey)
{
	if (gKey & gKeyUp && bKey)
	{
		custom_index_y -= 0x200;
		if (custom_index_y < -0x8000)
			custom_index_y = -0x8000;
	}
	else if (gKey & gKeyDown && bKey)
	{
		custom_index_y += 0x200;
		if (custom_index_y > 0x8000)
			custom_index_y = 0x8000;
	}
}

void ActMyChar_CustomCamOffset(BOOL bKey)
{
	// Camera
	if (is_direction_locked == false)
	{
		if (gMC->direct == 0)
		{
			custom_index_x -= 0x200;
			if (custom_index_x < -0x8000)
				custom_index_x = -0x8000;
		}
		else
		{
			custom_index_x += 0x200;
			if (custom_index_x > 0x8000)
				custom_index_x = 0x8000;
		}
		if (gKey & gKeyUp && bKey)
		{
			custom_index_y -= 0x200;
			if (custom_index_y < -0x8000)
				custom_index_y = -0x8000;
		}
		else if (gKey & gKeyDown && bKey)
		{
			custom_index_y += 0x200;
			if (custom_index_y > 0x8000)
				custom_index_y = 0x8000;
		}
		else
		{
			if (custom_index_y > 0x200)
				custom_index_y -= 0x200;
			if (custom_index_y < -0x200)
				custom_index_y += 0x200;
		}
	}
	else if (is_direction_locked == true)
	{
		switch (lock_direction)
		{
			default:
				custom_index_x = -0x8000;
				CameraVerticalHelper(bKey);
				break;

			case 1:
				custom_index_y = -0x8000;
				CameraHorizontalHelper(bKey);
				break;

			case 2:
				custom_index_x = 0x8000;
				CameraVerticalHelper(bKey);
				break;

			case 3:
				custom_index_y = 0x8000;
				CameraHorizontalHelper(bKey);
				break;

			case 4:
				custom_index_x = 0;
				custom_index_y = 0;
				break;
		}
	}

	custom_tgt_x = gMC->x + (custom_index_x + (custom_camera_offset_x * 0x200));
	custom_tgt_y = gMC->y + (custom_index_y + (custom_camera_offset_y * 0x200));
}

void ActMyChar_RunButton(BOOL bKey, Physics *physics)
{
	// Controller players can't do this.
	if ((setting_run_button_flag_enabled == true && GetNPCFlag(setting_run_button_flag)) || (setting_run_button_flag_enabled == false))
	{
		if (gMC->flag & 8) // only while grounded
		{
			if ((gKey & gKeyShift) && !(gMC->flag & 0x100)) // if running + not in water
			{
				physics->max_dash = setting_running_speed;
			}
		}
	}
}

// Apologies Autumn

// Don't name these the same as normal functions as they apparently mess up using them normally
int custom_sign(double x) {
	if (x > 0) return 1;
	if (x < 0) return -1;
	return 0;
}

int custom_abs(int x) 
{
	if (x >= 0)
		return 1;
	else
		return 1;
}

void setPlayerPhysics(BOOL bKey, Physics *physics)
{
	if (gMC->flag & 0x100) // While in water
	{
		physics->max_dash = setting_physics_water_max_dash;
		physics->max_move = setting_physics_water_max_move;
		physics->gravity1 = setting_physics_water_gravity1;
		physics->gravity2 = setting_physics_water_gravity2;
		physics->jump = setting_physics_water_jump;
		physics->dash1 = setting_physics_water_dash1;
		physics->dash2 = setting_physics_water_dash2;
		physics->resist = setting_physics_water_resist;
	}
	else
	{
		physics->max_dash = setting_physics_max_dash;
		physics->max_move = setting_physics_max_move;
		physics->gravity1 = setting_physics_gravity1;
		physics->gravity2 = setting_physics_gravity2;
		physics->jump = setting_physics_jump;
		physics->dash1 = setting_physics_dash1;
		physics->dash2 = setting_physics_dash2;
		physics->resist = setting_physics_resist;
	}

	// Run button when the setting is set to true
	if (setting_run_button_enabled)
		ActMyChar_RunButton(bKey, physics);

	// Ice
	int kLeft = (bKey && gKey & gKeyLeft) ? 1 : 0;
	int kRight = (bKey && gKey & gKeyRight) ? 1 : 0;

	int h_input = (kRight - kLeft);
	bool resisting = (h_input != 0 && custom_sign(gMC->xm) != h_input && custom_sign(gMC->xm) != 0);

	if (gMC->flag & 0x200)
	{
		if (h_input && setting_ice_particles)
			SetCaret(gMC->x, gMC->y + (4 * 0x200), 13, 0);

		if (resisting)
			physics->dash1 = setting_physics_water_dash1 / 2;
		else
			physics->dash1 = setting_physics_water_dash1;
		physics->resist = 12;
	}
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

void SpawnWalljumpCarets(int type)
{
	switch (type)
	{
		default:
			if (onWall == 1)
			{
				SetCaret(gMC->x + (8 * 0x200), gMC->y, 13, 0);
				SetCaret(gMC->x + (8 * 0x200), gMC->y, 13, 0);
			}
			else {
				SetCaret(gMC->x - (8 * 0x200), gMC->y, 13, 0);
				SetCaret(gMC->x - (8 * 0x200), gMC->y, 13, 0);
			}
			break;

		case 1:
			if (onWall == 1)
				SetCaret(gMC->x + (8 * 0x200), gMC->y, 13, 0);
			else
				SetCaret(gMC->x - (8 * 0x200), gMC->y, 13, 0);
			break;
	}
	
}

void ActMyChar_WallJump(BOOL bKey)
{
	//Wall jumping
	
	// Only enable walljumping if you either:
	// A: Have walljumps on flag set + the flag is set
	// B: Have walljumps on flag *not* set
	int walljump_speed = 0;
	int walljump_height = 0;

	if (gMC->flag & 0x100)
	{
		walljump_speed = setting_walljump_water_horizontal_speed;
		walljump_height = setting_walljump_water_jump_height;
	}
	else
	{
		walljump_speed = setting_walljump_horizontal_speed;
		walljump_height = setting_walljump_jump_height;
	}

	if ((setting_walljumps_flag_enabled == true && GetNPCFlag(setting_walljumps_flag)) || (setting_walljumps_flag_enabled == false))
	{
		if (bKey && entity_IsIceWalled == false) // If you have input + arent on an ice wall
		{
			if ((onWall != 0) && ((!(gMC->flag & 8)) || entity_IsWallboosting == true)) // If on a wall, and not grounded/wallboosting..
			{
				if (gKeyTrg & gKeyJump)
				{
					gMC->xm = onWall * -walljump_speed;
					gMC->ym = -walljump_height;
					PlaySoundObject(15, SOUND_MODE_PLAY);
					SpawnWalljumpCarets(0);
				}

				// Slide down the wall if holding against it
				if (entity_IsWallboosting == false)
				{
					if (gMC->ym > setting_walljump_sliding_speed)
						gMC->ym = setting_walljump_sliding_speed;
				}
				else // if you're wallboosting on a wallboost entity
				{
					SpawnWalljumpCarets(1);
					gMC->ym += -0x120; // not customizable atm
					PlaySoundObject(152, SOUND_MODE_PLAY);
				}
			}
		}

		if (onWall == 0 && entity_IsIceWalled == true)
			entity_IsIceWalled = false;

	}
}

void DoAirJump(int jump_height)
{
	int i = 0;

	if (--current_jumps >= 0) // Only jump if we have more than 0 air jumps
	{
		gMC->ym = -jump_height;
		PlaySoundObject(15, SOUND_MODE_PLAY);
		for (i = 0; i < 4; ++i)
			SetNpChar(4, gMC->x + (Random(-12, 12) * 0x200), gMC->y + (Random(-12, 12) * 0x200), Random(-341, 341), Random(-0x600, 0), 0, NULL, 0x100);
	}
}

void ActMyChar_AirJumps(BOOL bKey)
{
	//Air jumps
	int airjump_height = 0;

	if (gMC->flag & 0x100)
		airjump_height = setting_extrajump_water_jump_height;
	else
		airjump_height = setting_extrajump_jump_height;

	if (bKey)
	{
		if (gKeyTrg & gKeyJump && !(gMC->flag & 8) && gMC->boost_cnt == 0)
		{
			// This code is awful and should be rewritten at some point i think ..
			if (setting_walljumps_enabled == true) // If wall jumps are enabled
			{
				// If the walljump flag is set or if walljump flag is disabled
				if ((setting_walljumps_flag_enabled == true && GetNPCFlag(setting_walljumps_flag) && onWall == 0) || (setting_walljumps_flag_enabled == false) && onWall == 0)
					DoAirJump(airjump_height);
				else if ((setting_walljumps_flag_enabled == true && (!(GetNPCFlag(setting_walljumps_flag))))) // If walljumps flag is true but the flag isnt set, dont check for onWall
					DoAirJump(airjump_height);
			}
			else // If wall jumps are disabled, do the extra jumps with no extra checks.
				DoAirJump(airjump_height);
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

	ActMyChar_CustomCamOffset(bKey);
}