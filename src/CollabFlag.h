#pragma once

#include <Windows.h>

#include "cave_story.h"

extern bool setting_enable_collab_flags; // global setting
extern int enable_collab_flags; // TSC command (also save this to the profile please)
extern unsigned char gCollabFlag[1000];

void InitAllTypeFlags(void);
void InitCollabFlags(void);
void SetCollabFlag(long a);
void CutCollabFlag(long a);
BOOL GetCollabFlag(long a);

void InitMod_CollabFlagLoading();