#pragma once

#include "cave_story.h"
#include "Entity.h"

struct CNPCHAR
{
	int CustomValue01;
	int CustomValue02;
	int CustomValue03;
	int CustomValue04;
	int CustomValue05;
	int CustomValue06;
};

extern CNPCHAR gCustomNPC[NPC_MAX];

BOOL LoadCustomEvent(const char* path_event, int npc_count);
void Replacement_ActNpChar(void);
void Replacement_ChangeNpCharByEvent(int code_event, int code_char, int dir);
void Replacement_ChangeCheckableNpCharByEvent(int code_event, int code_char, int dir);