#pragma once

#include <Windows.h>

#include "cave_story.h"

extern bool setting_disable_window_rect_saving;
extern bool setting_disable_errorlog_saving;

BOOL IsKeyDataFile(const char* name);
BOOL Replacement_SaveWindowRect(HWND hWnd, const char* filename);
BOOL Replacement_ErrorLog(const char* string, int value);