#pragma once

#include <Windows.h>

#include "cave_story.h"

extern int coordinate_focus_x;
extern int coordinate_focus_y;

void SetFrameTargetNpCharWithMyCharIndex(int event, int wait);
void SetFrameTargetMyCharOffset(int wait, int mode, int offset_x, int offset_y);
void SetFrameTargetCoordinate(int wait, int x, int y);