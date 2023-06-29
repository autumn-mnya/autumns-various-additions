#pragma once

#include "cave_story.h"
#include "Entity.h"

extern NPC_TABLE* gEntityTable;
extern NPC_TABLE* gNpcTable;

BOOL Replacement_LoadEntityTable(const char* path);
void ReleaseEntityTable(void);
void Replacement_InitNpChar(void);
BOOL Replacement_LoadEvent(const char* path_event);
void Replacement_SetNpChar(int code_char, int x, int y, int xm, int ym, int dir, NPCHAR* npc, int start_index);
void Replacement_ActNpChar(void);
void Replacement_SetExpObjects(int x, int y, int exp);
BOOL Replacement_SetBulletObject(int x, int y, int val);
BOOL Replacement_SetLifeObject(int x, int y, int val);
void Replacement_VanishNpChar(NPCHAR* npc);
void Replacement_ChangeNpCharByEvent(int code_event, int code_char, int dir);
void Replacement_ChangeCheckableNpCharByEvent(int code_event, int code_char, int dir);
void Replacement_DeleteNpCharEvent(int code);
void Replacement_DeleteNpCharCode(int code, BOOL bSmoke);

typedef void (*NPCFUNCTION)(NPCHAR*);
extern const NPCFUNCTION gpNpcFuncTbl[];