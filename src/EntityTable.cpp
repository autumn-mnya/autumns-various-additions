#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "EntityTable.h"

#include "cave_story.h"
#include "main.h"
#include "Entity.h"
#include "EntityLoad.h"

// Npc function table
const NPCFUNCTION gpEntityFuncTbl[] = {
	ActEntity361,
	ActEntity362,
	ActEntity363,
	ActEntity364,
	ActEntity365,
	ActEntity366,
	ActEntity367,
	ActEntity368,
	ActEntity369,
	ActEntity370,
	ActEntity371,
	ActEntity372,
	ActEntity373,
	ActEntity374,
	ActEntity375,
	ActEntity376,
	ActEntity377,
	ActEntity378,
	ActEntity379,
	ActEntity380,
};