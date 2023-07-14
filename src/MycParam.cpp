#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "MycParam.h"

#include "main.h"
#include "cave_story.h"
#include "Entity.h"

void RemoveMaxLifeMyChar(int val)
{
	gMC->max_life -= (short)val;
	if (gMC->max_life > 232)
		gMC->max_life = 232;
	gMC->life -= (short)val;
	gMC->lifeBr = gMC->life;
}