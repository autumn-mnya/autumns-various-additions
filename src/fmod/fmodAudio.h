#pragma once

#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_common.h>

#include "cave_story.h"

extern FMOD_RESULT result;
extern FMOD::Studio::System* FmodStudioObj;
extern FMOD::Studio::Bank* FmodBankObj;
extern FMOD::Studio::EventDescription* FmodEventDescription;
extern FMOD::Studio::EventInstance* FmodMusicInstance;
extern FMOD::Studio::Bus* FmodEventBus;
extern FMOD::System* coreSystem;

void fmod_Init();
void fmod_LoadBanks();
void ReleaseFmod();
void PlayFModAudio(const char* audiofile);
void StopFmodAllAudio();
void FModClearEventNames();
void InjectMusicProgressParams();
void FModUITest();