#pragma once

#include <Windows.h>

#include "cave_story.h"

#include "mod_loader_hooks.h"

#define ImgFolderSize 0x32
#define ImgNameSize 0x32

extern char TSC_IMG_Folder[ImgFolderSize];

void InitMod_TSC();