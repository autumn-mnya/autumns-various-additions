#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Stage.h"

#include "BKG.h"
#include "cave_story.h"
#include "main.h"
#include "Generic.h"
#include "Profile.h"
#include "Respawn.h"

void LoadBKG_TxT_Background()
{
	if (!(bkgTxT_Global[0] == 0)) // if it doesnt == 0
		BKG_LoadBackground(bkgTxT_Global);
}

// This is the final function call in TransferStage
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