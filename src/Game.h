#pragma once

#include <Windows.h>

#include "cave_story.h"

void AutumnsVariousAdditions_ModeOpening_TextBoxBelow();
void AutumnsVariousAdditions_ModeAction_TextBoxBelow();
void AutumnsVariousAdditionsTextBoxAbove();
void AutumnsVariousAdditionsModeOpeningInit();
void AutumnsVariousAdditionsModeTitleInit();
void AutumnsVariousAdditionsModeActionInit();
void AutumnsVariousAdditionsModeActionAct();
void Replacement_ModeAction_PutTextScript_Call();
void AutumnsVariousAdditionsStageTableInit();

//fmod

extern const char* setting_titlescreen_cavestory_music;
extern const char* setting_titlescreen_hell_music;
extern const char* setting_titlescreen_toroko_music;
extern const char* setting_titlescreen_white_music;
extern const char* setting_titlescreen_safety_music;

void Replacement_ModeOpening_ChangeMusic_Silence_Call(MusicID no);
void Replacement_ModeTitle_ChangeMusic_RunningHell_Call(MusicID no);
void Replacement_ModeTitle_ChangeMusic_TorokosTheme_Call(MusicID no);
void Replacement_ModeTitle_ChangeMusic_White_Call(MusicID no);
void Replacement_ModeTitle_ChangeMusic_Safety_Call(MusicID no);
void Replacement_ModeTitle_ChangeMusic_CaveStory_Call(MusicID no);
void Replacement_ModeTitle_ChangeMusic_Silence_Call(MusicID no);