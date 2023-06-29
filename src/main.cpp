#include "Windows.h"
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "main.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "Entity.h"
#include "EntityLoad.h"

// Paths
char gModulePath[MAX_PATH];
char gDataPath[MAX_PATH];

static void Replacement_SetUniqueParameter(NPCHAR* npc)
{
	int code = npc->code_char;
	npc->surf = (SurfaceID)gNpcTable[code].surf;
	npc->hit_voice = gNpcTable[code].hit_voice;
	npc->destroy_voice = gNpcTable[code].destroy_voice;
	npc->damage = gNpcTable[code].damage;
	npc->size = gNpcTable[code].size;
	npc->life = gNpcTable[code].life;
	npc->hit.front = gNpcTable[code].hit.front * 0x200;
	npc->hit.back = gNpcTable[code].hit.back * 0x200;
	npc->hit.top = gNpcTable[code].hit.top * 0x200;
	npc->hit.bottom = gNpcTable[code].hit.bottom * 0x200;
	npc->view.front = gNpcTable[code].view.front * 0x200;
	npc->view.back = gNpcTable[code].view.back * 0x200;
	npc->view.top = gNpcTable[code].view.top * 0x200;
	npc->view.bottom = gNpcTable[code].view.bottom * 0x200;
}

void InitMod_Entity()
{
	ModLoader_WriteJump((void*)0x472400, (void*)Replacement_LoadEntityTable); // LoadNpcTable except im going to add more npcs
	ModLoader_WriteJump((void*)0x472710, (void*)ReleaseEntityTable); // Read Above
	ModLoader_WriteJump((void*)0x46EE50, (void*)Replacement_SetUniqueParameter);
	ModLoader_WriteJump((void*)0x46EB30, (void*)Replacement_InitNpChar);
	ModLoader_WriteJump((void*)0x46EB50, (void*)Replacement_LoadEvent);
	ModLoader_WriteJump((void*)0x46EFD0, (void*)Replacement_SetNpChar);
	ModLoader_WriteJump((void*)0x46FA00, (void*)Replacement_ActNpChar);
	ModLoader_WriteJump((void*)0x46F2B0, (void*)Replacement_SetExpObjects);
	ModLoader_WriteJump((void*)0x46F430, (void*)Replacement_SetBulletObject);
	ModLoader_WriteJump((void*)0x46F630, (void*)Replacement_SetLifeObject);
	ModLoader_WriteJump((void*)0x46F760, (void*)Replacement_VanishNpChar);
	ModLoader_WriteJump((void*)0x46FAB0, (void*)Replacement_ChangeNpCharByEvent);
	ModLoader_WriteJump((void*)0x46FD10, (void*)Replacement_ChangeCheckableNpCharByEvent);
	ModLoader_WriteJump((void*)0x470250, (void*)Replacement_DeleteNpCharEvent);
	ModLoader_WriteJump((void*)0x4702D0, (void*)Replacement_DeleteNpCharCode);
}

void InitMod(void)
{
	// Get executable's path
	GetModuleFileNameA(NULL, gModulePath, MAX_PATH);
	PathRemoveFileSpecA(gModulePath);

	// Get path of the data folder
	strcpy(gDataPath, gModulePath);
	strcat(gDataPath, "\\data");

	// InitMod_Entity();
}
