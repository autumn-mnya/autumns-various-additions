#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "ExternalTables.h"

#include "cave_story.h"
#include "main.h"
#include "ModSettings.h"

bool setting_enable_write_tables = false;

const char* const gArmsTblName = "arms_level.tbl";
const char* const gBulletTblName = "bullet.tbl";

BOOL CreateArmsTable(const char* name)
{
	FILE* fp;
	char path[MAX_PATH];

	// Get path
	if (name != NULL)
		sprintf(path, "%s\\%s", gModulePath, name);
	else
		sprintf(path, "%s\\%s", gModulePath, gArmsTblName);

	fp = fopen(path, "wb");
	if (fp == NULL)
		return FALSE;

	fwrite(&gArmsLevelTable, sizeof(ARMS_LEVEL), 14, fp);

	fclose(fp);

	return TRUE;
}

void InitMod_CreateTables()
{
	CreateArmsTable(NULL);
}