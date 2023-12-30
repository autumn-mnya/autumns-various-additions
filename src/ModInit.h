#pragma once

#include "cave_story.h"
#include "SurfaceDefines.h"

extern char init_surfaceName_0_Title[MaxSurfaceName];
extern char init_surfaceName_5_Image[MaxSurfaceName];
extern char init_surfaceName_6_Fade[MaxSurfaceName];
extern char init_surfaceName_8_ItemImage[MaxSurfaceName];
extern char init_surfaceName_11_Arms[MaxSurfaceName];
extern char init_surfaceName_12_ArmsImage[MaxSurfaceName];
extern char init_surfaceName_14_StageImage[MaxSurfaceName];
extern char init_surfaceName_15_Loading[MaxSurfaceName];
extern char init_surfaceName_16_MyChar[MaxSurfaceName];
extern char init_surfaceName_17_Bullet[MaxSurfaceName];
extern char init_surfaceName_19_Caret[MaxSurfaceName];
extern char init_surfaceName_20_NpcSym[MaxSurfaceName];
extern char init_surfaceName_23_NpcRegu[MaxSurfaceName];
extern char init_surfaceName_24_AutumnUI[MaxSurfaceName];
extern char init_surfaceName_25_AutumnObjects[MaxSurfaceName];
extern char init_surfaceName_26_TextBox[MaxSurfaceName];
extern char init_surfaceName_27_Face[MaxSurfaceName];
extern char init_surfaceName_38_AutumnItems[MaxSurfaceName];
extern char init_surfaceName_39_AutumnCharacters[MaxSurfaceName];

extern char init_collab_name[CollabNameMaxPath];

void InitMod_PreLaunch_CollectablesEnabled();
void InitMod_PreLaunch_CollectablesPositioning();
void InitMod_PreLaunch_PhysicsSettings();
void InitMod_PreLaunch_BoosterFuel();
void InitMod_PreLaunch_GenericLoad();
void InitMod_PreLaunch_CollabName();
void InitCollectablesEnabled();
void InitCollectablesPositioning();
void InitMyCharPhysics();
void InitMyCharBoostFuel();
void InitGameSurfaces();
void InitCollabName();