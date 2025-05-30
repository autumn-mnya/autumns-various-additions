#pragma once

#include <Windows.h>

#include "cave_story.h"

#define CustomNpcValues(N) gCustomNPC[(N - gNPC)]

#define CustomNpcValueA(N) CustomNpcValues(N).CustomValueA
#define CustomNpcValueB(N) CustomNpcValues(N).CustomValueB
#define CustomNpcValueC(N) CustomNpcValues(N).CustomValueC
#define CustomNpcValueD(N) CustomNpcValues(N).CustomValueD
#define CustomNpcValueE(N) CustomNpcValues(N).CustomValueE
#define CustomNpcValueF(N) CustomNpcValues(N).CustomValueF

extern bool entity_IsWallboosting;
extern bool entity_IsIceWalled;
extern bool playerIsInFan;

enum NPCNames
{
	NPC_NULL = 0,
	NPC_EXP = 1,
	NPC_ENEMY_BEHEMOTH = 2,
	NPC_DAMAGE_TEXT_HOLDER = 3,
	NPC_SMOKE = 4,
	// To be continued
	NPC_ENEMY_FROG = 104,
	NPC_SPEECH_BALLOON_HEY_LOW = 105,
	NPC_SPEECH_BALLOON_HEY_HIGH = 106,
	NPC_MALCO_UNDAMAGED = 107,
	NPC_PROJECTILE_BALFROG_SPITBALL = 108,
	NPC_MALCO_DAMAGED = 109,
	NPC_ENEMY_PUCHI = 110,
	// To be continued
	NPC_KINGS_SWORD = 145
};

void ActEntity361(NPCHAR* npc);
void ActEntity362(NPCHAR* npc);
void ActEntity363(NPCHAR* npc);
void ActEntity364(NPCHAR* npc);
void ActEntity365(NPCHAR* npc);
void ActEntity366(NPCHAR* npc);
void ActEntity367(NPCHAR* npc);
void ActEntity368(NPCHAR* npc);
void ActEntity369(NPCHAR* npc);
void ActEntity370(NPCHAR* npc);
void ActEntity371(NPCHAR* npc);
void ActEntity372(NPCHAR* npc);
void ActEntity373(NPCHAR* npc);
void ActEntity374(NPCHAR* npc);
void ActEntity375(NPCHAR* npc);
void ActEntity376(NPCHAR* npc);
void ActEntity377(NPCHAR* npc);
void ActEntity378(NPCHAR* npc);
void ActEntity379(NPCHAR* npc);
void ActEntity380(NPCHAR* npc);
void ActEntity381(NPCHAR* npc);
void ActEntity382(NPCHAR* npc);
void ActEntity383(NPCHAR* npc);
void ActEntity384(NPCHAR* npc);
void ActEntity385(NPCHAR* npc);
void ActEntity386(NPCHAR* npc);
void ActEntity387(NPCHAR* npc);
void ActEntity388(NPCHAR* npc);
void ActEntity389(NPCHAR* npc);
void ActEntity390(NPCHAR* npc);
void ActEntity391(NPCHAR* npc);
void ActEntity392(NPCHAR* npc);
void ActEntity393(NPCHAR* npc);
void ActEntity394(NPCHAR* npc);
void ActEntity395(NPCHAR* npc);
void ActEntity396(NPCHAR* npc);
void ActEntity397(NPCHAR* npc);
void ActEntity398(NPCHAR* npc);
void ActEntity399(NPCHAR* npc);
void ActEntity400(NPCHAR* npc);
void ActEntity401(NPCHAR* npc);
void ActEntity402(NPCHAR* npc);
void ActEntity403(NPCHAR* npc);
void ActEntity404(NPCHAR* npc);
void ActEntity405(NPCHAR* npc);
void ActEntity406(NPCHAR* npc);
void ActEntity407(NPCHAR* npc);
void ActEntity408(NPCHAR* npc);
void ActEntity409(NPCHAR* npc);
void ActEntity410(NPCHAR* npc);
void ActEntity411(NPCHAR* npc);
void ActEntity412(NPCHAR* npc);
void ActEntity413(NPCHAR* npc);
void ActEntity414(NPCHAR* npc);
void ActEntity415(NPCHAR* npc);
void ActEntity416(NPCHAR* npc);
void ActEntity417(NPCHAR* npc);
void ActEntity418(NPCHAR* npc);
void ActEntity419(NPCHAR* npc);
void ActEntity420(NPCHAR* npc);
void ActEntity421(NPCHAR* npc);
void ActEntity422(NPCHAR* npc);
void ActEntity423(NPCHAR* npc);
void ActEntity424(NPCHAR* npc);
void ActEntity425(NPCHAR* npc);
void ActEntity426(NPCHAR* npc);
void ActEntity427(NPCHAR* npc);
void ActEntity428(NPCHAR* npc);
void ActEntity429(NPCHAR* npc);
void ActEntity430(NPCHAR* npc);
void ActEntity431(NPCHAR* npc);
void ActEntity432(NPCHAR* npc);
void ActEntity433(NPCHAR* npc);
void ActEntity434(NPCHAR* npc);
void ActEntity435(NPCHAR* npc);
void ActEntity436(NPCHAR* npc);
void ActEntity437(NPCHAR* npc);
void ActEntity438(NPCHAR* npc);
void ActEntity439(NPCHAR* npc);
void ActEntity440(NPCHAR* npc);
void ActEntity441(NPCHAR* npc);
void ActEntity442(NPCHAR* npc);
void ActEntity443(NPCHAR* npc);
void ActEntity444(NPCHAR* npc);
void ActEntity445(NPCHAR* npc);
void ActEntity446(NPCHAR* npc);
void ActEntity447(NPCHAR* npc);
void ActEntity448(NPCHAR* npc);
void ActEntity449(NPCHAR* npc);
void ActEntity450(NPCHAR* npc);
void ActEntity451(NPCHAR* npc);
void ActEntity452(NPCHAR* npc);
void ActEntity453(NPCHAR* npc);
void ActEntity454(NPCHAR* npc);
void ActEntity455(NPCHAR* npc);
void ActEntity456(NPCHAR* npc);
void ActEntity457(NPCHAR* npc);
void ActEntity458(NPCHAR* npc);
void ActEntity459(NPCHAR* npc);
void ActEntity460(NPCHAR* npc);
void ActEntity461(NPCHAR* npc);
void ActEntity462(NPCHAR* npc);
void ActEntity463(NPCHAR* npc);