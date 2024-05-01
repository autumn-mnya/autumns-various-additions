#pragma once

#include <Windows.h>
#include "cave_story.h"
#include <vector>

extern HMODULE pauseDLL;

typedef void (*SaveConfigElementHandler)();

enum
{
	CALLBACK_CONTINUE = -1,
	CALLBACK_PREVIOUS_MENU = -2,
	CALLBACK_RESET = -3,
	CALLBACK_EXIT = -4,
};

typedef enum CallbackAction
{
	ACTION_INIT,
	ACTION_DEINIT,
	ACTION_UPDATE,
	ACTION_OK,
	ACTION_LEFT,
	ACTION_RIGHT
} CallbackAction;

typedef struct Option
{
	const char* name;
	int (*callback)(struct OptionsMenu* parent_menu, size_t this_option, CallbackAction action);
	void* user_data;
	const char* value_string;
	long value;
	BOOL disabled;
	long attribute_size;
	long attribute_index;
} Option;

typedef struct OptionsMenu
{
	const char* title;
	const char* subtitle;
	struct Option* options;
	size_t total_options;
	int x_offset;
	BOOL submenu;
} OptionsMenu;

void LoadPauseMenuDll();
Option** GetOptionsMain();
Option** GetOptionsMenu();
size_t* GetNumEntriesAddedMain();
size_t* GetNumEntriesAddedMenu();
void add_pause_entry(Option** options_ptr, const char* name, int (*callback)(OptionsMenu* parent_menu, size_t this_option, CallbackAction action), void* user_data, const char* value_string, long value, BOOL disabled, size_t* num_entries_added);
int EnterOptionsMenu(OptionsMenu* options_menu, size_t selected_option);
void RegisterSaveConfigElement(SaveConfigElementHandler handler);