#pragma once

#include "cave_story.h"
#include "Entity.h"

extern bool setting_enable_custom_npc_table;

struct CNPCHAR
{
	int CustomValueA;
	int CustomValueB;
	int CustomValueC;
	int CustomValueD;
	int CustomValueE;
	int CustomValueF;
};

extern CNPCHAR gCustomNPC[NPC_MAX];

BOOL LoadCustomEvent(const char* path_event, int npc_count);
void Replacement_ActNpChar(void);
void Replacement_ChangeNpCharByEvent(int code_event, int code_char, int dir);
void Replacement_ChangeCheckableNpCharByEvent(int code_event, int code_char, int dir);
void Replacement_HitMyCharNpChar(void);
void Replacement_SetExpObjects(int x, int y, int exp);
// Custom
void BossExplosionAtNpc(int event);
// Custom npc.tbl functionality
void LoadCustomNpcTable(const char* name);