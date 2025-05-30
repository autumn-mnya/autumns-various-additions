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

#include "AutPI.h"

// Contains the NPC function table.

#define ADD_NPC_FUNCTION(func) AutPI_AddEntity(func, #func)

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
	ActEntity381,
	ActEntity382,
	ActEntity383,
	ActEntity384,
	ActEntity385,
	ActEntity386,
	ActEntity387,
	ActEntity388,
	ActEntity389,
	ActEntity390,
	ActEntity391,
	ActEntity392,
	ActEntity393,
	ActEntity394,
	ActEntity395,
	ActEntity396,
	ActEntity397,
	ActEntity398,
	ActEntity399,
	ActEntity400,
	ActEntity401,
	ActEntity402,
	ActEntity403,
	ActEntity404,
	ActEntity405,
	ActEntity406,
	ActEntity407,
	ActEntity408,
	ActEntity409,
	ActEntity410,
	ActEntity411,
	ActEntity412,
	ActEntity413,
	ActEntity414,
	ActEntity415,
	ActEntity416,
	ActEntity417,
	ActEntity418,
	ActEntity419,
	ActEntity420,
	ActEntity421,
	ActEntity422,
	ActEntity423,
	ActEntity424,
	ActEntity425,
	ActEntity426,
	ActEntity427,
	ActEntity428,
	ActEntity429,
	ActEntity430,
	ActEntity431,
	ActEntity432,
	ActEntity433,
	ActEntity434,
	ActEntity435,
	ActEntity436,
	ActEntity437,
	ActEntity438,
	ActEntity439,
	ActEntity440,
	ActEntity441,
	ActEntity442,
	ActEntity443,
	ActEntity444,
	ActEntity445,
	ActEntity446,
	ActEntity447,
	ActEntity448,
	ActEntity449,
	ActEntity450,
	ActEntity451,
	ActEntity452,
	ActEntity453,
	ActEntity454,
	ActEntity455,
	ActEntity456,
	ActEntity457,
	ActEntity458,
	ActEntity459,
	ActEntity460,
	ActEntity461,
	ActEntity462,
	ActEntity463,
};

// Npc function table (1.1.2)

// Define the function AddAVAEntities
void AddAVAEntities()
{
	// Iterate through each ActEntity function in the gpEntityFuncTbl array
	for (size_t i = 0; i < sizeof(gpEntityFuncTbl) / sizeof(gpEntityFuncTbl[0]); ++i) {
		// Call AutPI_AddEntity with the current ActEntity function
		AutPI_AddEntity(gpEntityFuncTbl[i], "autumn", "ava");
	}
}