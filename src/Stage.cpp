#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Stage.h"

#include "ModSettings.h"
#include "BKG.h"
#include "cave_story.h"
#include "File.h"
#include "main.h"
#include "Generic.h"
#include "Profile.h"
#include "Respawn.h"

const char* const gDefaultStageTableName = "stage.tbl";

bool setting_external_stage_tbl_support = false;

void LoadBKG_TxT_Background()
{
	if (!(bkgTxT_Global[0] == 0)) // if it doesnt == 0
		BKG_LoadBackground(bkgTxT_Global);
}

// This is the final function call in TransferStage -- Only used if stage.tbl support is disabled!
void Replacement_TransferStage_ResetFlash_Call()
{
	ResetFlash();

	// reset bkg backgrounds on stage transition
	if (isLoadingSave == false)
	{
		if (PlayerIsRespawning == FALSE)
			BKG_ResetBackgrounds();
		else
			LoadBKG_TxT_Background();
	}
	else
	{
		LoadBKG_TxT_Background();
	}
}

const STAGE_TABLE* cTMT = gTMT;

// If stage.tbl support is enabled..
BOOL LoadStageTable(const char* name)
{
	char path[MAX_PATH];
	char collabPath[MAX_PATH];
	char namePath[StageTblMaxPath];

	sprintf(namePath, "%s", name);

	// Reset stage table path and put our new path in there
	if (!(stageTblPath[0] == 0))
	{
		memset(stageTblPath, 0, sizeof(stageTblPath));
		strcpy(stageTblPath, namePath);
	}

	unsigned char* file_buffer;
	size_t file_size;

	// Try to load stage.tbl
	if (name != NULL)
	{
		if (setting_collab_enabled == true)
			sprintf(path, "%s\\%s\\%s%s", gDataPath, setting_collab_name, name, ".stage.tbl");
		else
			sprintf(path, "%s\\%s%s", gDataPath, name, ".stage.tbl");
	}
	else
		sprintf(path, "%s\\%s", gDataPath, gDefaultStageTableName);

	file_buffer = LoadFileToMemory(path, &file_size);

	if (file_buffer != NULL)
	{
		const unsigned long entry_count = file_size / 0xE5;

		STAGE_TABLE* pTMT = (STAGE_TABLE*)malloc(entry_count * sizeof(STAGE_TABLE));

		if (pTMT != NULL)
		{
			for (unsigned long i = 0; i < entry_count; ++i)
			{
				unsigned char* entry = file_buffer + i * 0xE5;

				memcpy(pTMT[i].parts, entry, 0x20);
				memcpy(pTMT[i].map, entry + 0x20, 0x20);
				pTMT[i].bkType = (entry[0x40 + 3] << 24) | (entry[0x40 + 2] << 16) | (entry[0x40 + 1] << 8) | entry[0x40];
				memcpy(pTMT[i].back, entry + 0x44, 0x20);
				memcpy(pTMT[i].npc, entry + 0x64, 0x20);
				memcpy(pTMT[i].boss, entry + 0x84, 0x20);
				pTMT[i].boss_no = entry[0xA4];
				memcpy(pTMT[i].name, entry + 0xC5, 0x20); // Ignore japanese name
			}

			cTMT = pTMT;
			free(file_buffer);
			return TRUE;
		}

		free(file_buffer);
	}

	printf("Failed to load stage.tbl\n");
	return FALSE;
}

// If stage.tbl support is enabled, then we can do some fun stuff here!
BOOL Replacement_TransferStage(int no, int w, int x, int y)
{
	char path[MAX_PATH];
	char path_dir[20];
	BOOL bError;

	// Move character
	SetMyCharPosition(x * 0x10 * 0x200, y * 0x10 * 0x200);

	bError = FALSE;

	// Get path
	strcpy(path_dir, "Stage");

	// Load tileset
	sprintf(path, "%s\\Prt%s", path_dir, cTMT[no].parts);
	if (!ReloadBitmap_File(path, SURFACE_ID_LEVEL_TILESET))
		bError = TRUE;

	sprintf(path, "%s\\%s.pxa", path_dir, cTMT[no].parts);
	if (!LoadAttributeData(path))
		bError = TRUE;

	// Load tilemap
	sprintf(path, "%s\\%s.pxm", path_dir, cTMT[no].map);
	if (!LoadMapData2(path))
		bError = TRUE;

	// Load NPCs
	sprintf(path, "%s\\%s.pxe", path_dir, cTMT[no].map);
	if (!LoadEvent(path))
		bError = TRUE;

	// Load script
	sprintf(path, "%s\\%s.tsc", path_dir, cTMT[no].map);
	if (!LoadTextScript_Stage(path))
		bError = TRUE;

	// Load background
	sprintf(path, "%s", cTMT[no].back);
	if (!InitBack(path, cTMT[no].bkType))
		bError = TRUE;

	// Get path
	strcpy(path_dir, "Npc");

	// Load NPC sprite sheets
	sprintf(path, "%s\\Npc%s", path_dir, cTMT[no].npc);
	if (!ReloadBitmap_File(path, SURFACE_ID_LEVEL_SPRITESET_1))
		bError = TRUE;

	sprintf(path, "%s\\Npc%s", path_dir, cTMT[no].boss);
	if (!ReloadBitmap_File(path, SURFACE_ID_LEVEL_SPRITESET_2))
		bError = TRUE;

	if (bError)
		return FALSE;

	// Load map name
	ReadyMapName(cTMT[no].name);

	StartTextScript(w);
	SetFrameMyChar();
	ClearBullet();
	InitCaret();
	ClearValueView();
	ResetQuake();
	InitBossChar(cTMT[no].boss_no);
	Replacement_TransferStage_ResetFlash_Call(); // So we don't just copy the same code again lol
	gStageNo = no;

	return TRUE;
}