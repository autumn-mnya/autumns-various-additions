#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "SurfaceDefines.h"

#include "ModSettings.h"
#include "cave_story.h"
#include "Draw.h"
#include "File.h"
#include "main.h"

// This is used for the default surface names

char surfaceName_0_Title[MaxSurfaceName] = "Title";
char surfaceName_5_Image[MaxSurfaceName] = "Image\\0";
char surfaceName_6_Fade[MaxSurfaceName] = "Fade";
char surfaceName_8_ItemImage[MaxSurfaceName] = "ItemImage";
char surfaceName_11_Arms[MaxSurfaceName] = "Arms";
char surfaceName_12_ArmsImage[MaxSurfaceName] = "ArmsImage";
char surfaceName_14_StageImage[MaxSurfaceName] = "StageImage";
char surfaceName_15_Loading[MaxSurfaceName] = "Loading";
char surfaceName_16_MyChar[MaxSurfaceName] = "MyChar";
char surfaceName_17_Bullet[MaxSurfaceName] = "Bullet";
char surfaceName_19_Caret[MaxSurfaceName] = "Caret";
char surfaceName_20_NpcSym[MaxSurfaceName] = "Npc\\NpcSym";
char surfaceName_23_NpcRegu[MaxSurfaceName] = "Npc\\NpcRegu";
char surfaceName_24_AutumnUI[MaxSurfaceName] = "AutumnUI";
char surfaceName_25_AutumnObjects[MaxSurfaceName] = "Npc\\NpcAutumnObj";
char surfaceName_26_TextBox[MaxSurfaceName] = "TextBox";
char surfaceName_27_Face[MaxSurfaceName] = "Face";
char surfaceName_38_AutumnItems[MaxSurfaceName] = "Autumn";
char surfaceName_39_AutumnCharacters[MaxSurfaceName] = "Npc\\NpcAutumnChar";