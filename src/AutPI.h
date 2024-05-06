// AutPI.h

#include <Windows.h>
#include "cave_story.h"
#include <vector>
#include "lua/Lua.h"

extern "C"
{
#include <lua.h>
}

extern HMODULE autpiDLL;  // Global variable

// Boss API
void AutPI_AddBoss(BOSSFUNCTION func, char* author, char* name);

// Caret API
void AutPI_AddCaret(CARETFUNCTION func, char* author, char* name);

// Game()
typedef void (*OpeningBelowFadeElementHandler)();
typedef void (*OpeningAboveFadeElementHandler)();
// GetTrg()
typedef void (*GetTrgElementHandler)();
// ModeOpening()
typedef void (*PreModeElementHandler)();
typedef void (*ReleaseElementHandler)();
typedef void (*OpeningBelowTextBoxElementHandler)();
typedef void (*OpeningAboveTextBoxElementHandler)();
typedef void (*OpeningEarlyActionElementHandler)();
typedef void (*OpeningActionElementHandler)();
typedef void (*OpeningInitElementHandler)();
typedef void (*OpeningBelowPutCaretElementHandler)();
typedef void (*OpeningAbovePutCaretElementHandler)();
typedef void (*MOBelowPutFPSElementHandler)();
typedef void (*MOAbovePutFPSElementHandler)();
// ModeTitle()
typedef void (*TitleInitElementHandler)();
typedef void (*TitleActionElementHandler)();
typedef void (*TitleBelowCounterElementHandler)();
typedef void (*MTBelowPutFPSElementHandler)();
typedef void (*MTAbovePutFPSElementHandler)();
// ModeAction()
typedef void (*PlayerHudElementHandler)();
typedef void (*CreditsHudElementHandler)();
typedef void (*BelowFadeElementHandler)();
typedef void (*AboveFadeElementHandler)();
typedef void (*BelowTextBoxElementHandler)();
typedef void (*AboveTextBoxElementHandler)();
typedef void (*BelowPlayerElementHandler)();
typedef void (*AbovePlayerElementHandler)();
typedef void (*EarlyActionElementHandler)();
typedef void (*ActionElementHandler)();
typedef void (*CreditsActionElementHandler)();
typedef void (*InitElementHandler)();
typedef void (*BelowPutCaretElementHandler)();
typedef void (*AbovePutCaretElementHandler)();
typedef void (*MABelowPutFPSElementHandler)();
typedef void (*MAAbovePutFPSElementHandler)();
// Profile
typedef void (*SaveProfilePreCloseElementHandler)();
typedef void (*SaveProfilePostCloseElementHandler)();
typedef void (*LoadProfilePreCloseElementHandler)();
typedef void (*LoadProfilePostCloseElementHandler)();
typedef void (*InitializeGameInitElementHandler)();
// PutFPS
typedef void (*PutFPSElementHandler)();
// TextScript
typedef void (*TextScriptSVPElementHandler)();
// TransferStage()
typedef void (*TransferStageInitElementHandler)();
// Lua
typedef void (*LuaPreGlobalModCSElementHandler)();
typedef void (*LuaMetadataElementHandler)();
typedef void (*LuaFuncElementHandler)();

void LoadAutPiDll();

// NpcTbl API
void AutPI_AddEntity(NPCFUNCTION func, char* author, char* name);

// Game() API
void RegisterPreModeElement(PreModeElementHandler handler);
void RegisterReleaseElement(ReleaseElementHandler handler);
// GetTrg() API
void RegisterGetTrgElement(GetTrgElementHandler handler);
// ModeOpening() API
void RegisterOpeningBelowFadeElement(OpeningBelowFadeElementHandler handler);
void RegisterOpeningAboveFadeElement(OpeningAboveFadeElementHandler handler);
void RegisterOpeningBelowTextBoxElement(OpeningBelowTextBoxElementHandler handler);
void RegisterOpeningAboveTextBoxElement(OpeningAboveTextBoxElementHandler handler);
void RegisterOpeningEarlyActionElement(OpeningEarlyActionElementHandler handler);
void RegisterOpeningActionElement(OpeningActionElementHandler handler);
void RegisterOpeningInitElement(OpeningInitElementHandler handler);
void RegisterOpeningBelowPutCaretElement(OpeningBelowPutCaretElementHandler handler);
void RegisterOpeningAbovePutCaretElement(OpeningAbovePutCaretElementHandler handler);
void RegisterModeOpeningBelowPutFPSElement(MOBelowPutFPSElementHandler handler);
void RegisterModeOpeningAbovePutFPSElement(MOAbovePutFPSElementHandler handler);
// ModeTitle() API
void RegisterTitleInitElement(TitleInitElementHandler handler);
void RegisterTitleActionElement(TitleActionElementHandler handler);
void RegisterTitleBelowCounterElement(TitleBelowCounterElementHandler handler);
void RegisterModeTitleBelowPutFPSElement(MTBelowPutFPSElementHandler handler);
void RegisterModeTitleAbovePutFPSElement(MTAbovePutFPSElementHandler handler);
// ModeAction() API
void RegisterPlayerHudElement(PlayerHudElementHandler handler);
void RegisterCreditsHudElement(CreditsHudElementHandler handler);
void RegisterBelowFadeElement(BelowFadeElementHandler handler);
void RegisterAboveFadeElement(AboveFadeElementHandler handler);
void RegisterBelowTextBoxElement(BelowTextBoxElementHandler handler);
void RegisterAboveTextBoxElement(AboveTextBoxElementHandler handler);
void RegisterBelowPlayerElement(BelowPlayerElementHandler handler);
void RegisterAbovePlayerElement(AbovePlayerElementHandler handler);
void RegisterEarlyActionElement(EarlyActionElementHandler handler);
void RegisterActionElement(ActionElementHandler handler);
void RegisterCreditsActionElement(CreditsActionElementHandler handler);
void RegisterInitElement(InitElementHandler handler);
void RegisterBelowPutCaretElement(BelowPutCaretElementHandler handler);
void RegisterAbovePutCaretElement(AbovePutCaretElementHandler handler);
void RegisterModeActionBelowPutFPSElement(MABelowPutFPSElementHandler handler);
void RegisterModeActionAbovePutFPSElement(MAAbovePutFPSElementHandler handler);
// Profile API
void RegisterSaveProfilePreCloseElement(SaveProfilePreCloseElementHandler handler);
void RegisterSaveProfilePostCloseElement(SaveProfilePostCloseElementHandler handler);
void RegisterLoadProfilePreCloseElement(LoadProfilePreCloseElementHandler handler);
void RegisterLoadProfilePostCloseElement(LoadProfilePostCloseElementHandler handler);
void RegisterInitializeGameInitElement(InitializeGameInitElementHandler handler);
// PutFPS API
void RegisterPutFPSElement(PutFPSElementHandler handler);
// TextScript API
void RegisterSVPElement(TextScriptSVPElementHandler handler);
// TransferStage() API
void RegisterTransferStageInitElement(TransferStageInitElementHandler handler);
// Lua API
lua_State* GetLuaL();
BOOL ReadStructBasic(lua_State* L, const char* name, STRUCT_TABLE* table, void* data, int length);
BOOL Write2StructBasic(lua_State* L, const char* name, STRUCT_TABLE* table, void* data, int length);
void PushFunctionTable(lua_State* L, const char* name, const FUNCTION_TABLE* table, int length, BOOL pop);
void PushFunctionTableModName(lua_State* L, const char* modname, const char* name, const FUNCTION_TABLE* table, int length, BOOL pop);
void PushSimpleMetatables(lua_State* L, const METATABLE_TABLE* table, int length);
void RegisterLuaPreGlobalModCSElement(LuaPreGlobalModCSElementHandler handler);
void RegisterLuaMetadataElement(LuaMetadataElementHandler handler);
void RegisterLuaFuncElement(LuaFuncElementHandler handler);