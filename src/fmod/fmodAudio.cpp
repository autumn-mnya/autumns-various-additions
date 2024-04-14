#include <Windows.h>
// Filesystem
#include <iostream>
#include <filesystem>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_common.h>

#include <sstream>

#include "../main.h"
#include "../mod_loader.h"
#include "../cave_story.h"
#include "../TextScript.h"

namespace fs = std::filesystem;

const char* gBankName = "Master.bank";
const char* gStringsBankName = "Master.strings.bank";
const char* gNullPath = "";

// Fmod objects
FMOD_RESULT result;
FMOD::Studio::System* FmodStudioObj;
FMOD::Studio::Bank* FmodBankObj;
FMOD::Studio::EventDescription* FmodEventDescription;
FMOD::Studio::EventInstance* FmodMusicInstance; // Music Instance
FMOD::Studio::Bus* FmodEventBus; // Event Bus (Non Functional)
FMOD::System* coreSystem;

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error %d - %s", result, FMOD_ErrorString(result));
	}
}

void Common_Init(void** /*extraDriverData*/)
{
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

void fmod_Init()
{
	void* extraDriverData = NULL;
	Common_Init(&extraDriverData);

	FmodStudioObj = NULL;
	result = FmodStudioObj->create(&FmodStudioObj); // Create the Studio System object.

	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	coreSystem = NULL;
	FmodStudioObj->getCoreSystem(&coreSystem);
	coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);

	if (result == FMOD_OK)
		printf("FMOD success! (%d) %s\n", result, FMOD_ErrorString(result));

	// Initialize FMOD Studio, which will also initialize FMOD Core
	result = FmodStudioObj->initialize(1024, FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE, FMOD_INIT_NORMAL, extraDriverData);
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	if (result == FMOD_OK)
		printf("FMOD success! (%d) %s\n", result, FMOD_ErrorString(result));
}

// Load audio banks
void fmod_LoadBanks()
{
	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gAudioPath, gNullPath);

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file())
		{
			std::string filePath = entry.path().string();
			std::string extension = filePath.substr(filePath.find_last_of(".") + 1);

			// Check if the file extension is ".bank" before loading
			if (extension == "bank")
			{
				FmodStudioObj->loadBankFile(filePath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &FmodBankObj);
			}
		}
	}

	printf("FMOD Banks Loaded");
}

// Replease Fmod Audio
void ReleaseFmod()
{
	FmodBankObj->unload();
	FmodStudioObj->release();
}

// Play FMOD Audio
void PlayFModAudio(const char* audiofile)
{
    char folderPath[256];
    sprintf(folderPath, "%s\\%s", gAudioPath, gNullPath);

    bool loadedFromStringsBank = false;

    // Check if the "Master.strings.bank" file exists
    std::string stringsBankPath = std::string(gAudioPath) + "\\" + gNullPath + "\\" + "Master.strings.bank";
    if (std::filesystem::exists(stringsBankPath))
    {
        // Load the "Master.strings.bank" file
        FmodStudioObj->loadBankFile(stringsBankPath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &FmodBankObj);

        // Attempt to load the audio from the strings bank
        FMOD::Studio::EventDescription* eventDescription = nullptr;
        FMOD_RESULT result = FmodStudioObj->getEvent(audiofile, &eventDescription);
        if (result == FMOD_OK)
        {
            // Create audio instance
            FmodMusicInstance = nullptr;
            eventDescription->createInstance(&FmodMusicInstance);

            // Start audio
            FmodMusicInstance->start();

            // Release when finished
            FmodMusicInstance->release();

            loadedFromStringsBank = true;
        }

        // Unload the "Master.strings.bank" file
        FmodBankObj->unload();
    }

    if (!loadedFromStringsBank)
    {
        // Iterate over the remaining GUIDs text files
        for (const auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            if (entry.is_regular_file())
            {
                std::string filePath = entry.path().string();
                std::string extension = filePath.substr(filePath.find_last_of(".") + 1);

                // Check if the file extension is ".guids.txt" before opening
                if (extension == "txt")
                {
                    std::string filename = entry.path().filename().string();
                    std::string guidExtension = ".guids.txt";

                    // Check if the filename ends with ".guids.txt"
                    if (filename.size() >= guidExtension.size() &&
                        filename.compare(filename.size() - guidExtension.size(), guidExtension.size(), guidExtension) == 0)
                    {
                        FILE* file = fopen(filePath.c_str(), "r");
                        if (file)
                        {
                            char line[256];
                            FMOD_GUID guid;
                            while (fgets(line, sizeof(line), file))
                            {
                                char* space = strchr(line, ' ');
                                if (space)
                                {
                                    // Extract the GUID from the line
                                    char guidString[256];
                                    strncpy(guidString, line, space - line);
                                    guidString[space - line] = '\0';

                                    // Extract the event name from the line
                                    char eventName[256];
                                    strcpy(eventName, space + 1);

                                    if (strcmp(eventName, audiofile) == 0)
                                    {
                                        // Convert GUID string to FMOD_GUID
                                        FMOD::Studio::parseID(guidString, &guid);

                                        // Load audio
                                        FMOD::Studio::EventDescription* eventDescription = nullptr;
                                        FmodStudioObj->getEventByID(&guid, &eventDescription);

                                        // Create audio instance
                                        FmodMusicInstance = nullptr;
                                        eventDescription->createInstance(&FmodMusicInstance);

                                        // Start audio
                                        FmodMusicInstance->start();

                                        // Release when finished
                                        FmodMusicInstance->release();
                                        fclose(file);
                                        return;  // Found and played the audio, so exit the function
                                    }
                                }
                            }
                            fclose(file);
                        }
                    }
                }
            }
        }

        // Audio file not found in any GUIDs text file
        printf("Audio file '%s' not found.\n", audiofile);
    }
}

void StopFmodAllAudio()
{
	FmodMusicInstance->release();
    PlayFModAudio(gNull1Name);
    PlayFModAudio(gNull2Name);
    PlayFModAudio(gNull3Name);
    PlayFModAudio(gNull4Name);
    PlayFModAudio(gNull5Name);
    PlayFModAudio(gNull6Name);
    PlayFModAudio(gNull7Name);
    PlayFModAudio(gNull8Name);
}

void FModClearEventNames()
{
    memset(eventName, 0, sizeof(eventName));
    memset(eventName2, 0, sizeof(eventName2));
    memset(eventName3, 0, sizeof(eventName3));
    memset(eventName4, 0, sizeof(eventName4));
    memset(eventName5, 0, sizeof(eventName5));
    memset(eventName6, 0, sizeof(eventName6));
    memset(eventName7, 0, sizeof(eventName7));
    memset(eventName8, 0, sizeof(eventName8));
}