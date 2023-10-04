#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "TextScriptVAR.h"
#include "TextScript.h"

#include "main.h"
#include "mod_loader.h"
#include "BKG.h"
#include "cave_story.h"
#include "Collectables.h"
#include "MyChar.h"
#include "TileCollisionMyChar.h"

int varData[TSC_VAR_MAX]; // 100 total right now, i think thats good enough??

int GetVariable(int id)
{
    switch (id)
    {
        case 501:
            return gMC.x;

        case 502:
            return gMC.y;

        case 503:
            return gMC.xm;

        case 504:
            return gMC.ym;

        case 505:
            return gMC.life;

        case 506:
            return gMC.max_life;

        case 507:
            return gMC.air;

        case 508:
            return gMC.cond;

        case 509:
            return gMC.equip;

        case 510:
            return gMC.unit;

        case 511:
            return gMC.boost_cnt;

        case 512:
            return gMC.direct;

        case 513:
            return gSelectedArms;

        case 514:
            return gSelectedItem;

        case 515:
            return gStageNo;

        case 516:
            return gMusicNo;

        case 517:
            return gCollectables.numA;

        case 518:
            return gCollectables.numB;

        case 519:
            return gCollectables.numC;

        case 520:
            return gCollectables.numD;

        case 521:
            return gCollectables.numE;

        case 522:
            return playerMoney;

        case 523:
            return setting_physics_max_dash;

        case 524:
            return setting_physics_max_move;

        case 525:
            return setting_physics_gravity1;

        case 526:
            return setting_physics_gravity2;

        case 527:
            return setting_physics_dash1;

        case 528:
            return setting_physics_dash2;

        case 529:
            return setting_physics_resist;

        case 530:
            return setting_physics_jump;

        case 531:
            return setting_physics_water_max_dash;

        case 532:
            return setting_physics_water_max_move;

        case 533:
            return setting_physics_water_gravity1;

        case 534:
            return setting_physics_water_gravity2;

        case 535:
            return setting_physics_water_dash1;

        case 536:
            return setting_physics_water_dash2;

        case 537:
            return setting_physics_water_resist;

        case 538:
            return setting_physics_water_jump;

        case 539:
            return setting_walljump_horizontal_speed;

        case 540:
            return setting_walljump_jump_height;

        case 541:
            return setting_walljump_sliding_speed;

        case 542:
            return setting_walljump_water_horizontal_speed;

        case 543:
            return setting_walljump_water_jump_height;

        case 544:
            return setting_extrajump_jump_height;

        case 545:
            return setting_extrajump_water_jump_height;

        case 546:
            return setting_running_speed;

        case 547:
            return setting_bounce_speed;

        case 548:
            return current_jumps;

        case 549:
            return numBks;

        case 550:
            return lock_direction;

        case 551:
            return gMap.width;

        case 552:
            return gMap.length;

        default:
            if (id <= TSC_VAR_MAX)
                return varData[id];
            else
                return 0; // return 0 if its not in the varData
    }
}

void SetVariable(int id, int value)
{
    switch (id)
    {
        case 501:
            gMC.x = value;
            break;

        case 502:
            gMC.y = value;
            break;

        case 503:
            gMC.xm = value;
            break;

        case 504:
            gMC.ym = value;
            break;

        case 505:
            gMC.life = value;
            break;

        case 506:
            gMC.max_life = value;
            break;

        case 507:
            gMC.air = value;
            break;

        case 508:
            gMC.cond = value;
            break;

        case 509:
            gMC.equip = value;
            break;

        case 510:
            gMC.unit = value;
            break;

        case 511:
            gMC.boost_cnt = value;
            break;

        case 512:
            gMC.direct = value;
            break;

        case 513:
            gSelectedArms = value;
            break;

        case 514:
            gSelectedItem = value;
            break;

        case 515:
            gStageNo = value;
            break;

        case 516:
            gMusicNo = value;
            break;

        case 517:
            gCollectables.numA = value;
            break;

        case 518:
            gCollectables.numB = value;
            break;

        case 519:
            gCollectables.numC = value;
            break;

        case 520:
            gCollectables.numD = value;
            break;

        case 521:
            gCollectables.numE = value;
            break;

        case 522:
            playerMoney = value;
            break;

        case 523:
            setting_physics_max_dash = value;
            break;

        case 524:
            setting_physics_max_move = value;
            break;

        case 525:
            setting_physics_gravity1 = value;
            break;

        case 526:
            setting_physics_gravity2 = value;
            break;

        case 527:
            setting_physics_dash1 = value;
            break;

        case 528:
            setting_physics_dash2 = value;
            break;

        case 529:
            setting_physics_resist = value;
            break;

        case 530:
            setting_physics_jump = value;
            break;

        case 531:
            setting_physics_water_max_dash = value;
            break;

        case 532:
            setting_physics_water_max_move = value;
            break;

        case 533:
            setting_physics_water_gravity1 = value;
            break;

        case 534:
            setting_physics_water_gravity2 = value;
            break;

        case 535:
            setting_physics_water_dash1 = value;
            break;

        case 536:
            setting_physics_water_dash2 = value;
            break;

        case 537:
            setting_physics_water_resist = value;
            break;

        case 538:
            setting_physics_water_jump = value;
            break;

        case 539:
            setting_walljump_horizontal_speed = value;
            break;

        case 540:
            setting_walljump_jump_height = value;
            break;

        case 541:
            setting_walljump_sliding_speed = value;
            break;

        case 542:
            setting_walljump_water_horizontal_speed = value;
            break;

        case 543:
            setting_walljump_water_jump_height = value;
            break;

        case 544:
            setting_extrajump_jump_height = value;
            break;

        case 545:
            setting_extrajump_water_jump_height = value;
            break;

        case 546:
            setting_running_speed = value;
            break;

        case 547:
            setting_bounce_speed = value;
            break;

        case 548:
            current_jumps = value;
            break;

        case 549:
            numBks = value;
            break;

        case 550:
            lock_direction = value;
            break;

        case 551:
            gMap.width = value;
            break;

        case 552:
            gMap.length = value;
            break;

        default:
            if (id <= TSC_VAR_MAX)
                varData[id] = value;
    }
}

void InitTSCVariables()
{
    memset(varData, 0, sizeof(varData));
}

void InitVarData()
{
    for (int i = 0; i < TSC_VAR_MAX; ++i)
    {
        varData[i] = 0;
    }
}