#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Draw.h"

#include "Collectables.h"

#include "main.h"
#include "mod_loader.h"
#include "cave_story.h"
#include "Profile.h"
#include "TextScript.h"

int playerCollectables = 0;
int playerMoney = 0;

COLLECTABLES gCollectables;

bool enable_collectables_a = false;
bool enable_collectables_b = false;
bool enable_collectables_c = false;
bool enable_collectables_d = false;
bool enable_collectables_e = false;


int collectables_a_x_pos = 8;
int collectables_b_x_pos = 8;
int collectables_c_x_pos = 8;
int collectables_d_x_pos = 8;
int collectables_e_x_pos = 8;
int collectables_a_y_pos = 56;
int collectables_b_y_pos = 64;
int collectables_c_y_pos = 72;
int collectables_d_y_pos = 80;
int collectables_e_y_pos = 88;
int collectables_a_x_offset = 0;
int collectables_b_x_offset = 0;
int collectables_c_x_offset = 0;
int collectables_d_x_offset = 0;
int collectables_e_x_offset = 0;

void InitCollectables()
{
	playerCollectables = 0;
	playerMoney = 0;
}

void AddMoney(int x)
{
	if (playerMoney <= MaxMoney)
		playerMoney += x;
}

void AddCollectables(int x, int y)
{
	switch (x)
	{
		default:
		case 0:
			gCollectables.numA += y;
			break;

		case 1:
			gCollectables.numB += y;
			break;

		case 2:
			gCollectables.numC += y;
			break;

		case 3:
			gCollectables.numD += y;
			break;

		case 4:
			gCollectables.numE += y;
			break;
	}
}

void RemoveCollectables(int x, int y)
{
	switch (x)
	{
		default:
		case 0:
			gCollectables.numA -= y;
			break;

		case 1:
			gCollectables.numB -= y;
			break;

		case 2:
			gCollectables.numC -= y;
			break;

		case 3:
			gCollectables.numD -= y;
			break;

		case 4:
			gCollectables.numE -= y;
			break;
	}
}

void SetCollectables(int x, int y)
{
	switch (x)
	{
		default:
		case 0:
			gCollectables.numA = y;
			break;

		case 1:
			gCollectables.numB = y;
			break;

		case 2:
			gCollectables.numC = y;
			break;

		case 3:
			gCollectables.numD = y;
			break;

		case 4:
			gCollectables.numE = y;
			break;
	}
}

void EnableCollectable(int x)
{
	switch (x)
	{
		default:
		case 0:
			enable_collectables_a = true;
			break;

		case 1:
			enable_collectables_b = true;
			break;

		case 2:
			enable_collectables_c = true;
			break;

		case 3:
			enable_collectables_d = true;
			break;

		case 4:
			enable_collectables_e = true;
			break;
	}
}

void DisableCollectable(int x)
{
	switch (x)
	{
		default:
		case 0:
			enable_collectables_a = false;
			break;

		case 1:
			enable_collectables_b = false;
			break;

		case 2:
			enable_collectables_c = false;
			break;

		case 3:
			enable_collectables_d = false;
			break;

		case 4:
			enable_collectables_e = false;
			break;
	}
}