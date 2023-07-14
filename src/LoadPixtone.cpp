#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "LoadPixtone.h"

#include "cave_story.h"
#include "main.h"

static const struct
{
	int slot;
	const char* path;
} gCustomPtpTable[] = {
	{161, "PixTone/161.pxt"},
	{162, "PixTone/162.pxt"},
	{163, "PixTone/163.pxt"},
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
	int pt_size;

	pt_size = 0;

	for (unsigned int i = 0; i < sizeof(gCustomPtpTable) / sizeof(gCustomPtpTable[0]); ++i)
	{
		char path[MAX_PATH];
		sprintf(path, "%s/%s", gDataPath, gCustomPtpTable[i].path);

		PIXTONEPARAMETER pixtone_parameters[4];

		if (LoadPixToneFile(path, pixtone_parameters))
		{
			int ptp_num = 0;
			while (pixtone_parameters[ptp_num].use && ptp_num < 4)
				++ptp_num;

			pt_size += MakePixToneObject(pixtone_parameters, ptp_num, gCustomPtpTable[i].slot);
		}
	}
}

void Replacement_LoadGenericData_Pixtone_Sprintf(char* b, const char* f, int pt)
{
	Freeware_sprintf(b, f, pt);
	LoadCustomPixtoneData();
}