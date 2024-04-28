#pragma once

#include <Windows.h>

#include "cave_story.h"

#define MaxMoney 9999
#define MaxCollectables 9999
#define CollectableCount 5

typedef struct COLLECTABLES
{
	int numA;
	int numB;
	int numC;
	int numD;
	int numE;
} COLLECTABLES;

extern COLLECTABLES gCollectables;

extern bool enable_collectables_a;
extern bool enable_collectables_b;
extern bool enable_collectables_c;
extern bool enable_collectables_d;
extern bool enable_collectables_e;

extern int collectables_a_x_pos;
extern int collectables_b_x_pos;
extern int collectables_c_x_pos;
extern int collectables_d_x_pos;
extern int collectables_e_x_pos;
extern int collectables_a_y_pos;
extern int collectables_b_y_pos;
extern int collectables_c_y_pos;
extern int collectables_d_y_pos;
extern int collectables_e_y_pos;
extern int collectables_a_x_offset;
extern int collectables_b_x_offset;
extern int collectables_c_x_offset;
extern int collectables_d_x_offset;
extern int collectables_e_x_offset;

extern int playerMoney;

void InitCollectables();
void InitMoney();
void AddMoney(int x);
int GetCollectable(int type);
void AddCollectables(int x, int y);
void RemoveCollectables(int x, int y);
void SetCollectables(int x, int y);
void EnableCollectable(int x);
void DisableCollectable(int x);
void SetCollectablePosition(int x, int y, bool isVertical);
void SetCollectableXOffset(int x, int y);