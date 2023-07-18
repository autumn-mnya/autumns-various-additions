#pragma once

#include <Windows.h>

#include "cave_story.h"

extern bool setting_disable_window_rect_saving;

BOOL IsKeyDataFile(const char* name);
BOOL IsKeyBKGFile(const char* name);
BOOL Replacement_SaveWindowRect(HWND hWnd, const char* filename);