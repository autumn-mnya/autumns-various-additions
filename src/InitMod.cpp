#include <Windows.h>
#include <shlwapi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Main.h"
#include "ModSettings.h"

#include "AutPI.h"
#include "InitMod.h"

#include "mod_loader.h"
#include "cave_story.h"
#include "ASMPatches.h"
#include "ASMLoader.h"
#include "BKG.h"
#include "Collectables.h"
#include "Config.h"
#include "Draw.h"
#include "Entity.h"
#include "EntityLoad.h"
#include "EntityTable.h"
#include "Game.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "LoadPixtone.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpcReplacements.h"
#include "PauseScreen.h"
#include "Profile.h"
#include "Stage.h"
#include "SurfaceDefines.h"
#include "TextScript.h"
#include "TileCollisionBoss.h"
#include "TileCollisionBullet.h"
#include "TileCollisionEntity.h"
#include "TileCollisionMyChar.h"

#include "ModInit.h"

// Inits anything relating to entities.
void InitMod_Entity()
{
	AddAVAEntities();
	InitCustomEntityPatch();

	// This only gets replaced if TSC is loaded
	if (setting_enable_text_script_code)
	{
		if (setting_enable_money_code == true)
		{
			ModLoader_WriteJump((void*)0x419030, (void*)Replacement_HitMyCharNpChar);

			if (setting_money_disable_enemy_money_drops == false)
				ModLoader_WriteJump((void*)0x46F2B0, (void*)Replacement_SetExpObjects);
		}

		// Replace NPCs when using <MIM
		if (setting_enable_mim_mod)
		{
			ModLoader_WriteJump((void*)0x43CDE0, (void*)Replacement_ActNpc111);
			ModLoader_WriteJump((void*)0x43D0A0, (void*)Replacement_ActNpc112);
			ModLoader_WriteJump((void*)0x445660, (void*)Replacement_ActNpc150);
		}
	}

	// This sets "isLoadingSave" to false after we're done loading, and after entities are loaded
	RegisterActionElement(AutumnsVariousAdditionsModeActionAct);
}

// Loads the new surface files (We can't go above 40, but we can use the unused ones. Except 3 and 4 as they are used in the netplay dll!)
void InitMod_Sprites()
{
	Replacement_LoadGenericData_Calls();
	InitMod_PreLaunch_GenericLoad();
	RegisterInitElement(AutumnsVariousAdditionsModeActionInit);
}

void InitMod_SFX()
{
	ModLoader_WriteCall((void*)0x411DEF, (void*)Replacement_LoadGenericData_Pixtone_Sprintf);
}

// Init MyChar replacement
void InitMod_MyChar()
{
	RegisterInitElement(Replacement_SetMyCharPosition_InitStar_Call);
	ModLoader_WriteCall((void*)0x414B5F, (void*)Replacement_InitMyChar_memset_Call); // for initmychar related things, we replace the memset call and do our own thing
	InitMyCharPhysicsPatch();
	InitCustomMyCharPatch();
	if (setting_enable_text_script_code && setting_enable_mim_mod) // Only if TSC + <MIM is enabled
		ModLoader_WriteCall((void*)0x415535, (void*)Replacement_PutMyChar_PutChar_Call);

	InitMod_PreLaunch_PhysicsSettings(); // Init Settings
}

void InitMod_TileCollision()
{
	ModLoader_WriteJump((void*)0x402B30, (void*)Replacement_JudgeHitBulletBlock2);
	ModLoader_WriteJump((void*)0x403740, (void*)Replacement_HitBulletMap);
	ModLoader_WriteJump((void*)0x417E40, (void*)Replacement_HitMyCharMap);
	ModLoader_WriteJump((void*)0x471160, (void*)Replacement_HitNpCharMap);
	ModLoader_WriteJump((void*)0x473080, (void*)Replacement_HitBossMap);
}

void InitMod_PauseScreen()
{
	ModLoader_WriteJump((void*)0x40DD70, (void*)Call_Pause);
}

void InitMod_SaveData()
{
	ModLoader_WriteCall((void*)0x41D213, (void*)Replacement_SaveProfile_LastMemcpy_Call); // autpi
	ModLoader_WriteCall((void*)0x41D22D, (void*)Replacement_SaveProfile_fwrite_Call); // autpi
	ModLoader_WriteCall((void*)0x41D353, (void*)Replacement_LoadProfile_fclose_Call); // autpi
	isLoadingSave = false;
}

void InitMod_GameUI()
{
	if (legacy_extra_jumps_ui == true)
		RegisterAbovePlayerElement(AutumnsVariousAdditionsExtraJumpUI);
	else
		RegisterPlayerHudElement(AutumnsVariousAdditionsExtraJumpUI);

	RegisterPlayerHudElement(AutumnsVariousAdditionsPlayerHud);
}

// Init TSC <IMG related calls
void InitMod_TSCImage()
{
	RegisterOpeningInitElement(AutumnsVariousAdditionsModeOpeningInit);
	RegisterTitleInitElement(AutumnsVariousAdditionsModeTitleInit);
	ModLoader_WriteCall((void*)0x41D508, (void*)Replacement_LoadProfile_ClearFade_Call);
	ModLoader_WriteCall((void*)0x41D576, (void*)Replacement_InitializeGame_ClearArmsData_Call); // AUTPI
	RegisterOpeningBelowTextBoxElement(AutumnsVariousAdditions_ModeOpening_TextBoxBelow);
	RegisterOpeningAboveTextBoxElement(AutumnsVariousAdditionsTextBoxAbove);
	RegisterBelowTextBoxElement(AutumnsVariousAdditions_ModeAction_TextBoxBelow);
	RegisterAboveTextBoxElement(AutumnsVariousAdditionsTextBoxAbove);
}

// Init TSC <BKG related calls
// AutPI is not used for this function.
void InitMod_TSCBkg()
{
	ModLoader_WriteCall((void*)0x402339, (void*)Replacement_InitBack_ReloadBitmap_File_Call); // Release Surface + MakeSurface_File instead of reloading bitmap
	ModLoader_WriteCall((void*)0x40F881, (void*)Replacement_ModeOpening_PutStageBack_Call);
	ModLoader_WriteCall((void*)0x40F8D1, (void*)Replacement_ModeOpening_PutFront_Call);
	ModLoader_WriteCall((void*)0x410643, (void*)Replacement_ModeAction_PutStageBack_Call);
	ModLoader_WriteCall((void*)0x4106C3, (void*)Replacement_ModeAction_PutFront_Call);
	if (setting_external_stage_tbl_support == false) // Only replace this if stage.tbl support is disabled
		RegisterTransferStageInitElement(AutumnsVariousAdditionsTransferStageInit);
	memset(bkgTxT_Global, 0, sizeof(bkgTxT_Global));

	// Get path of the Bkg folder
	strcpy(gBkgPath, gDataPath);
	strcat(gBkgPath, "\\bkg");
}

void InitMod_ExternalStageTable()
{
	RegisterPreModeElement(AutumnsVariousAdditionsStageTableInit);
	ModLoader_WriteJump((void*)0x420BE0, (void*)Replacement_TransferStage); // TransferStage replaced for stage.tbl changing
}

void InitMod_TeleporterMenuFix()
{
	ModLoader_WriteCall((void*)0x41DB16, (void*)PutBitmap4);
}

void InitMod_DisableWindowRect()
{
	ModLoader_WriteCall((void*)0x40F710, (void*)Replacement_SaveWindowRect);
}

void InitMod_DisableErrorLog()
{
	ModLoader_WriteJump((void*)0x410DE0, (void*)Replacement_ErrorLog);
}

void InitMod_AutumnConfigDefaults()
{
	ModLoader_WriteCall((void*)0x4124E0, (void*)Replacement_DefaultConfigData);
}