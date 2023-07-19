#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <iostream>
#include <filesystem>

#include "LoadPixtone.h"

#include "cave_story.h"
#include "main.h"
#include "ModSettings.h"

char default_pixtoneFolder[MaxPixTonePath] = "PixTone";
char global_pixtoneFolder[MaxPixTonePath] = "PixTone";

void Reset_PixToneFolder()
{
	strcpy(global_pixtoneFolder, default_pixtoneFolder);
	LoadUserCustomPixtoneData(NULL);
}

// Structure to hold the ID and file path of each sound effect
struct CustomPtpData
{
    int id;
    std::string path;
};

// Decompiled from PTone103.exe
static BOOL LoadPixToneFile(const char* filename, PIXTONEPARAMETER* pixtone_parameters)
{
	BOOL success = FALSE;

	FILE* fp = fopen(filename, "r");

	if (fp)
	{
		for (unsigned int i = 0; i < 4; ++i)
		{
			float freq;
			fscanf(fp, "use  :%d\n", &pixtone_parameters[i].use);
			fscanf(fp, "size :%d\n", &pixtone_parameters[i].size);
			fscanf(fp, "main_model   :%d\n", &pixtone_parameters[i].oMain.model);
			fscanf(fp, "main_freq    :%f\n", &freq);
			pixtone_parameters[i].oMain.num = freq;
			fscanf(fp, "main_top     :%d\n", &pixtone_parameters[i].oMain.top);
			fscanf(fp, "main_offset  :%d\n", &pixtone_parameters[i].oMain.offset);
			fscanf(fp, "pitch_model  :%d\n", &pixtone_parameters[i].oPitch.model);
			fscanf(fp, "pitch_freq   :%f\n", &freq);
			pixtone_parameters[i].oPitch.num = freq;
			fscanf(fp, "pitch_top    :%d\n", &pixtone_parameters[i].oPitch.top);
			fscanf(fp, "pitch_offset :%d\n", &pixtone_parameters[i].oPitch.offset);
			fscanf(fp, "volume_model :%d\n", &pixtone_parameters[i].oVolume.model);
			fscanf(fp, "volume_freq  :%f\n", &freq);
			pixtone_parameters[i].oVolume.num = freq;
			fscanf(fp, "volume_top   :%d\n", &pixtone_parameters[i].oVolume.top);
			fscanf(fp, "volume_offset:%d\n", &pixtone_parameters[i].oVolume.offset);
			fscanf(fp, "initialY:%d\n", &pixtone_parameters[i].initial);
			fscanf(fp, "ax      :%d\n", &pixtone_parameters[i].pointAx);
			fscanf(fp, "ay      :%d\n", &pixtone_parameters[i].pointAy);
			fscanf(fp, "bx      :%d\n", &pixtone_parameters[i].pointBx);
			fscanf(fp, "by      :%d\n", &pixtone_parameters[i].pointBy);
			fscanf(fp, "cx      :%d\n", &pixtone_parameters[i].pointCx);
			fscanf(fp, "cy      :%d\n\n", &pixtone_parameters[i].pointCy);
		}

		fclose(fp);

		success = TRUE;
	}

	return success;
}

void LoadCustomPixtoneData()
{
	char path[MAX_PATH];
	sprintf(path, "%s\\%s", gDataPath, "PixTone");

	std::vector<CustomPtpData> customPtpData;  // Vector to hold the ID and file path

	int pt_size = 0;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file())
		{
			// Extract the filename and extension
			std::string filePath = entry.path().string();
			std::string filename = entry.path().filename().string();

			// Determine the ID based on the filename
			int id = std::stoi(filename.substr(0, filename.find('.')));

			// Store the ID and file path in the vector
			customPtpData.push_back({ id, filePath });
		}
	}

	for (const auto& customData : customPtpData)
	{
		PIXTONEPARAMETER pixtone_parameters[4];

		if (LoadPixToneFile(customData.path.c_str(), pixtone_parameters))
		{
			int ptp_num = 0;
			while (pixtone_parameters[ptp_num].use && ptp_num < 4)
				++ptp_num;

			pt_size += MakePixToneObject(pixtone_parameters, ptp_num, customData.id);
		}
	}
}

void Replacement_LoadGenericData_Pixtone_Sprintf(char* b, const char* f, int pt)
{
	Freeware_sprintf(b, f, pt);
	LoadCustomPixtoneData();
}

void LoadUserCustomPixtoneData(const char* folder)
{
	char path[MAX_PATH];

	if (setting_collab_enabled == true)
	{
		if (folder != NULL)
			sprintf(path, "%s\\%s\\%s", gDataPath, setting_collab_name, folder);
		else
			sprintf(path, "%s\\%s", gDataPath, default_pixtoneFolder);
	}
	else
	{
		if (folder != NULL)
			sprintf(path, "%s\\%s", gDataPath, folder);
		else
			sprintf(path, "%s\\%s", gDataPath, default_pixtoneFolder);
	}

	std::vector<CustomPtpData> customPtpData;  // Vector to hold the ID and file path

	int pt_size = 0;

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file())
		{
			// Extract the filename and extension
			std::string filePath = entry.path().string();
			std::string filename = entry.path().filename().string();

			// Determine the ID based on the filename
			int id = std::stoi(filename.substr(0, filename.find('.')));

			// Store the ID and file path in the vector
			customPtpData.push_back({ id, filePath });
		}
	}

	for (const auto& customData : customPtpData)
	{
		PIXTONEPARAMETER pixtone_parameters[4];

		if (LoadPixToneFile(customData.path.c_str(), pixtone_parameters))
		{
			int ptp_num = 0;
			while (pixtone_parameters[ptp_num].use && ptp_num < 4)
				++ptp_num;

			pt_size += MakePixToneObject(pixtone_parameters, ptp_num, customData.id);
		}
	}
}