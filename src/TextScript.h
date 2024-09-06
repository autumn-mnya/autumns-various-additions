#pragma once

#include <Windows.h>

#include "cave_story.h"

#include "mod_loader_hooks.h"

#include <string>
#include <deque>
#include <random>
#include <tuple>

#define ImgFolderSize 48
#define ImgNameSize 0x32

#define NpcTblMaxPath 120

/// Event queue

// Note: This was coded by periwinkle in 2020/2021 or so, not autumn!!

class EventQueue
{
	struct MapChange
	{
		int map_no;
		int starting_event;
		int x;
		int y;
	};
	std::deque<MapChange> events;
	// Use our own randomness because Cave Story's RNG uses a fixed seed,
	// so you get the same random numbers every time you start the game.
	// (Is this even an issue? It might not be if there is an entity in the hub world
	//  that calls the RNG very frequently, e.g. every frame.)
	std::mt19937 mt;
public:
	EventQueue();

	void push(int event_no);
	void push(int map_no, int event_no, int x, int y);
	std::tuple<int, int, int, int> pop();
	void shuffle();
	void clear();
};
extern EventQueue gEventQueue; // Why does everything start with a "g", anyways?
// Oh well, may as well do the same just for "consistency".

// Booleans
extern bool setting_enable_money_code;
extern bool setting_money_disable_enemy_money_drops;
extern bool setting_money_disable_exp_drops;
extern bool setting_money_division;
extern bool setting_exp_acts_as_money;
extern bool setting_enable_mim_mod;
extern bool setting_disable_tsc_encryption;
// Money Hud
extern int setting_money_hud_x;
extern int setting_money_hud_y;
extern int setting_money_hud_x_number_offset;
// Booster Fuel
extern int booster_08_fuel;
extern int booster_20_fuel;

// FMOD
extern bool setting_enable_fmod;
extern const char* gNull1Name;
extern const char* gNull2Name;
extern const char* gNull3Name;
extern const char* gNull4Name;
extern const char* gNull5Name;
extern const char* gNull6Name;
extern const char* gNull7Name;
extern const char* gNull8Name;
extern char eventName[MAX_PATH];
extern char eventName2[MAX_PATH];
extern char eventName3[MAX_PATH];
extern char eventName4[MAX_PATH];
extern char eventName5[MAX_PATH];
extern char eventName6[MAX_PATH];
extern char eventName7[MAX_PATH];
extern char eventName8[MAX_PATH];

extern char TSC_IMG_Folder[ImgFolderSize];

// Custom npc tbl path
extern char npcTblPath[NpcTblMaxPath];

void ResetTablePaths();
void Mod_WriteBoosterFuel();
void InitMod_TSC();