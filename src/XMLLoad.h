#pragma once

#include <Windows.h>

#include "cave_story.h"

// Define a structure to hold loaded XML data
struct LoadedXMLData
{
    bool isLoaded;
};

extern LoadedXMLData loadedData;
void LoadAndExecuteFunctions(const char* fileName);