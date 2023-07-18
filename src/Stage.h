#pragma once

#include <Windows.h>

#include "cave_story.h"

extern bool setting_external_stage_tbl_support;

void Replacement_TransferStage_ResetFlash_Call();
BOOL LoadStageTable(const char* name);
BOOL Replacement_TransferStage(int no, int w, int x, int y);