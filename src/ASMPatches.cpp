#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "ASMPatches.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "Main.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "MyChar.h"

// Patch for running LoadCustomEvent
void InitCustomEntityPatch()
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

// Patch for running new MyChar code
void InitCustomMyCharPatch()
{
	unsigned char optimizedSpdLimit[] = {
		0xFF, 0x75, 0x08,             // push dword ptr [ebp+8] ; bKey
		0x90, 0x90, 0x90, 0x90, 0x90, // space for call to new code
		0x59,                         // pop ecx
		0xA1, 0x3C, 0xE6, 0x49, 0x00, // mov eax, dword ptr [49E63C] ; gMC.flag
		0xF6, 0xC4, 0x01,             // test ah, 1
		0x74, 0x61                    // je short 4160D7
	};
	WriteProcessMemory(GetCurrentProcess(), (void*)0x416063, optimizedSpdLimit, sizeof optimizedSpdLimit, NULL);
	ModLoader_WriteCall((void*)0x416066, ActMyChar_Normal_Custom); // Insert your function here
}

// Patch for changing physics (used only when the setting is enabled)
void InitMyCharPhysicsPatch()
{
	unsigned char physicsPatch[] = {
		0x8D, 0x45, 0xD8,
		0x50,
		0xFF, 0x75, 0x08,
		0xE8, 0x1D, 0xA9, 0xBE, 0xFF, // function CALL goes here (0x4156DE)
		0x83, 0xC4, 0x08,
		0xEB, 0x6F,
		0x90,
		0x90,
		0x90,
		0x90,
	};
	WriteProcessMemory(GetCurrentProcess(), (void*)0x4156D7, physicsPatch, sizeof physicsPatch, NULL);
	ModLoader_WriteCall((void*)0x4156DE, setPlayerPhysics);
}