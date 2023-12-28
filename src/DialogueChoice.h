#pragma once

#include <Windows.h>

#include "cave_story.h"

#define CHOICE_MAX 8 // Max

typedef struct CHOICE
{
	char name[50];
	int index;
	int event;

} CHOICE;

extern CHOICE gChoiceData[CHOICE_MAX];

extern int gSelectedChoice;

void ClearDialogueChoiceData(void);
BOOL AddDialogueChoiceData(int event, const char* name);
void MoveDialogueChoiceCursor(void);
void PutDialogueChoiceObject(void);
void PutDialogueChoices();