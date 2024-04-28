#include <windows.h>
#include <iostream>
#include "mod_loader.h"
#include "cave_story.h"
#include "API_Pause.h"

HMODULE pauseDLL = nullptr;  // Global variable

void LoadPauseMenuDll()
{
    pauseDLL = LoadLibrary("pause.dll");
    if (pauseDLL == nullptr) {
        std::cerr << "Failed to load pause.dll\n";
        // You might want to handle the error appropriately, e.g., throw an exception or return early.
    }
}

typedef Option** (*GetOptionMainFunc)();
typedef Option** (*GetOptionMenuFunc)();
typedef size_t* (*GetNumEntriesAddedMainFunc)();
typedef size_t* (*GetNumEntriesAddedMenuFunc)();
typedef void (*add_pause_entryFunc)(Option**,const char*,int(*)(OptionsMenu*,size_t,CallbackAction),void*,const char*,long,BOOL,size_t*);
typedef int (*EnterOptionsMenuFunc)(OptionsMenu*,size_t);

Option** GetOptionsMain()
{
    GetOptionMainFunc func = reinterpret_cast<GetOptionMainFunc>(
        GetProcAddress(pauseDLL, "GetOptionsMain"));

    if (func == nullptr) {
        std::cerr << "Failed to get the function pointer for GetOptionsMain\n";
        return nullptr;
    }

    Option** main = func();
    return main;
}

Option** GetOptionsMenu()
{
    GetOptionMenuFunc func = reinterpret_cast<GetOptionMenuFunc>(
        GetProcAddress(pauseDLL, "GetOptionsMenu"));

    if (func == nullptr) {
        std::cerr << "Failed to get the function pointer for GetOptionsMenu\n";
        return nullptr;
    }

    Option** main = func();
    return main;
}

size_t* GetNumEntriesAddedMain()
{
    GetNumEntriesAddedMainFunc func = reinterpret_cast<GetNumEntriesAddedMainFunc>(
        GetProcAddress(pauseDLL, "GetNumEntriesAddedMain"));

    if (func == nullptr) {
        std::cerr << "Failed to get the function pointer for GetNumEntriesAddedMain\n";
        return NULL;
    }

    size_t* val = func();
    return val;
}

size_t* GetNumEntriesAddedMenu()
{
    GetNumEntriesAddedMenuFunc func = reinterpret_cast<GetNumEntriesAddedMenuFunc>(
        GetProcAddress(pauseDLL, "GetNumEntriesAddedMenu"));

    if (func == nullptr) {
        std::cerr << "Failed to get the function pointer for GetNumEntriesAddedMenu\n";
        return NULL;
    }

    size_t* val = func();
    return val;
}

void add_pause_entry(Option** options_ptr, const char* name, int (*callback)(OptionsMenu* parent_menu, size_t this_option, CallbackAction action), void* user_data, const char* value_string, long value, BOOL disabled, size_t* num_entries_added)
{
    add_pause_entryFunc func = reinterpret_cast<add_pause_entryFunc>(
        GetProcAddress(pauseDLL, "add_pause_entry"));

    if (func == nullptr) {
        std::cerr << "Failed to get the function pointer for add_pause_entry\n";
        return;
    }

    func(options_ptr, name, callback, user_data, value_string, value, disabled, num_entries_added);
}

int EnterOptionsMenu(OptionsMenu* options_menu, size_t selected_option)
{
    EnterOptionsMenuFunc func = reinterpret_cast<EnterOptionsMenuFunc>(
        GetProcAddress(pauseDLL, "EnterOptionsMenu"));

    if (func == nullptr) {
        std::cerr << "Failed to get the function pointer for EnterOptionsMenu\n";
        return NULL;
    }

    int val = func(options_menu, selected_option);
    return val;
}