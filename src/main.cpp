#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "main.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"

// Paths
char gModulePath[MAX_PATH];
char gDataPath[MAX_PATH];

// Options

void PlayerDeath()
{
	PlaySoundObject(17, SOUND_MODE_PLAY);
	gMC->cond = 0;
	SetDestroyNpChar(gMC->x, gMC->y, 10 * 0x200, 0x40);
	StartTextScript(40);
}

void InitMod_Settings()
{
}

// solution by periwinkle
void InitCustomEntity()
{
	unsigned char bytes[] = {
		0xFF, 0x75, 0xFC, // push dword ptr [ebp-4] ; count
		0xFF, 0x75, 0x08, // push dword ptr [ebp+8] ; path_event
		0xE8, 0x00, 0x00, 0x00, 0x00, // call [...]
		0x59, 0x59       // pop ecx (x2)
	};
	// Write call location
	*(unsigned int*)(&bytes[7]) = (unsigned int)LoadCustomEvent - 0x46EE42;
	// Patch bytes
	WriteProcessMemory(GetCurrentProcess(), (void*)0x46EE37, bytes, sizeof bytes, NULL);
}

void InitMod_Entity()
{
	InitCustomEntity();
	ModLoader_WriteJump((void*)0x46FA00, (void*)Replacement_ActNpChar);
	ModLoader_WriteJump((void*)0x46FAB0, (void*)Replacement_ChangeNpCharByEvent);
	ModLoader_WriteJump((void*)0x46FD10, (void*)Replacement_ChangeCheckableNpCharByEvent);
}

void InitMod_Sprites()
{
	ModLoader_WriteCall((void*)0x411546, (void*)Replacement_StageImageSurfaceCall);
}

void InitMod(void)
{
	// Get executable's path
	GetModuleFileNameA(NULL, gModulePath, MAX_PATH);
	PathRemoveFileSpecA(gModulePath);

	// Get path of the data folder
	strcpy(gDataPath, gModulePath);
	strcat(gDataPath, "\\data");

	// Settings set by User
	// InitMod_Settings();

	// Functions
	InitMod_Entity();
	InitMod_Sprites();
	// debug putmylife
	ModLoader_WriteJump((void*)0x41A1D0, (void*)Replacement_PutMyLife);
}
