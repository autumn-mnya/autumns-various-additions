#pragma once

#include <Windows.h>

#include "cave_story.h"

extern bool setting_external_stage_tbl_support;

void AutumnsVariousAdditionsTransferStageInit();
BOOL LoadStageTable(const char* name);
BOOL Replacement_TransferStage(int no, int w, int x, int y);