#include <Windows.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "BKG.h"

#include "cave_story.h"
#include "main.h"
#include "Generic.h"

SUBKG bkList[0x400] = {};
int numBks = 0;

char backgroundName[MAX_PATH];
int backgroundType = 0;

void BKG_ResetBackgrounds()
{
	numBks = 0;

	memset(bkList, 0, sizeof(bkList));
}

void BKG_SetBackground(int i, int b1, int b2, int b3, int b4, int r1, int r2, int d1, int d2, int t, double m1, double m2, int f, int s, double o1, double o2)
{
	bkList[i].isActive = true;
	bkList[i].bmX = b1;
	bkList[i].bmY = b2;
	bkList[i].bmW = b3;
	bkList[i].bmH = b4;
	bkList[i].repX = r1;
	bkList[i].repY = r2;
	bkList[i].xDist = d1;
	bkList[i].yDist = d2;
	bkList[i].type = t;
	bkList[i].xm = m1;
	bkList[i].ym = m2;
	bkList[i].xCount = 0;
	bkList[i].yCount = 0;
	bkList[i].spriteNum = f;
	bkList[i].animFrame = 0;
	bkList[i].animSpeed = s;
	bkList[i].counter = 0;
	bkList[i].x = o1;// - r1 * b3 / 2;
	bkList[i].y = o2;// -r2 * b4 / 2;
	if (bkList[i].type & 1024)
	{
		bkList[i].x = Random(0, bkList[i].x);
		bkList[i].y = Random(0, bkList[i].y);
		bkList[i].animSpeed = Random(bkList[i].animSpeed, bkList[i].animSpeed * 2);
		bkList[i].counter = Random(0, bkList[i].animSpeed);
		bkList[i].animFrame = Random(0, bkList[i].spriteNum);
	}
	bkList[i].bkgXval = 0;
	bkList[i].bkgYval = 0;
}

void BKG_LoadBackground(char* file)
{
	FILE* fptr;
	char path[MAX_PATH];
	char pathtxt[50];

	// Get path
	sprintf(path, "%s\\%s", gBkgPath, "");
	sprintf(pathtxt, "%s.txt", file);
	strcat(path, pathtxt);

	// Open file
	fptr = fopen(path, "rb");


	BKG_ResetBackgrounds();

	int bkXOffset, bkYOffset, bkWidth, bkHeight, repeatX, repeatY, repeatGapX, repeatGapY, t, f, s;
	double m1, m2, o1, o2;
	char line[256];
	int bkCount = 0;
	char backFi[256];
	fgets(line, sizeof(line), fptr);
	sscanf(line, "%s", backFi);
	InitBack(backFi, 0);

	// set backgroundName and backgroundType for save file
	memset(backgroundName, 0, sizeof(backgroundName));
	strcpy(backgroundName, backFi);
	backgroundType = 0;

	for (int i = 0; fgets(line, sizeof(line), fptr); i = i)
	{
		if (('0' <= line[0] && line[0] <= '9') || line[0] == '-')
		{
			switch (i)
			{
			case(0):
				if (sscanf(line, "%d", &bkXOffset))
					i++;
				break;
			case(1):
				if (sscanf(line, "%d", &bkYOffset))
					i++;
				break;
			case(2):
				if (sscanf(line, "%d", &bkWidth))
					i++;
				break;
			case(3):
				if (sscanf(line, "%d", &bkHeight))
					i++;
				break;
			case(4):
				if (sscanf(line, "%d", &repeatX))
					i++;
				break;
			case(5):
				if (sscanf(line, "%d", &repeatY))
					i++;
				break;
			case(6):
				if (sscanf(line, "%d", &repeatGapX))
					i++;
				break;
			case(7):
				if (sscanf(line, "%d", &repeatGapY))
					i++;
				break;
			case(8):
				if (sscanf(line, "%d", &t))
					i++;
				break;
			case(9):
				if (sscanf(line, "%lf", &m1))
					i++;
				break;
			case(10):
				if (sscanf(line, "%lf", &m2))
					i++;
				break;
			case(11):
				if (sscanf(line, "%d", &f))
					i++;
				break;
			case(12):
				if (sscanf(line, "%d", &s))
					i++;
				break;
			case(13):
				if (sscanf(line, "%lf", &o1))
					i++;
				break;
			case(14):
				if (sscanf(line, "%lf", &o2))
				{
					i = 0;
					BKG_SetBackground(bkCount, bkXOffset, bkYOffset, bkWidth, bkHeight, repeatX, repeatY, repeatGapX, repeatGapY, t, m1, m2, f, s, o1, o2);
					bkCount++;
				}
				break;
			}
		}
	}

	fclose(fptr);
	numBks = bkCount;
}

void BKG_RenderBackgrounds(int CS_camera_x_pos, int CS_camera_y_pos, bool forg)
{
	for (int i = 0; i < numBks; i++)
	{
		if (bkList[i].isActive) {
			int x1 = bkList[i].bmX + (bkList[i].bmW * bkList[i].animFrame);
			int y1 = bkList[i].bmY;
			int x2 = x1 + bkList[i].bmW;
			int y2 = y1 + bkList[i].bmH;

			RECT tRec = { x1, y1, x2, y2 };

			if (bkList[i].spriteNum > 1) {
				if (++bkList[i].counter > bkList[i].animSpeed)
				{
					if (++bkList[i].animFrame >= bkList[i].spriteNum)
					{
						bkList[i].animFrame = 0;
					}
					bkList[i].counter = 0;
				}
			}

			int repX = bkList[i].repX;
			int repY = bkList[i].repY;

			if (repX == 1) {
				bkList[i].xDist = 0;
			}

			int bitm = bkList[i].type;

			if (bitm & 4)
			{
				bkList[i].bkgXval -= bkList[i].xm;
				if (bkList[i].bkgXval + bkList[i].x + (bkList[i].bmW + bkList[i].xDist) * bkList[i].repX < 0)
				{
					bkList[i].bkgXval = bkList[i].bkgXval + (bkList[i].bmW + bkList[i].xDist) + WINDOW_WIDTH;
					if (bitm & 128) {
						bkList[i].bkgYval += Random(0 - bkList[i].animSpeed, bkList[i].animSpeed);
					}
				}
				if (bkList[i].bkgXval + bkList[i].x > WINDOW_WIDTH)
				{
					bkList[i].bkgXval = bkList[i].bkgXval - WINDOW_WIDTH;
					if (bitm & 128) {
						bkList[i].bkgYval += Random(0 - bkList[i].animSpeed, bkList[i].animSpeed);
					}
				}
			}
			if (bitm & 8)
			{
				bkList[i].bkgYval -= bkList[i].ym;
				if (bkList[i].bkgYval + bkList[i].y < -(bkList[i].bmH + bkList[i].yDist) * bkList[i].repY)
				{
					bkList[i].bkgYval = bkList[i].bkgYval + (bkList[i].bmH + bkList[i].yDist) * bkList[i].repY;
					if (bitm & 64) {
						bkList[i].bkgXval += Random(0 - bkList[i].animSpeed, bkList[i].animSpeed);
					}
				}
				if (bkList[i].bkgYval + bkList[i].y > (bkList[i].bmH + bkList[i].yDist) * bkList[i].repY)
				{
					bkList[i].bkgYval = bkList[i].bkgYval - (bkList[i].bmH + bkList[i].yDist) * bkList[i].repY;
					if (bitm & 64) {
						bkList[i].bkgXval += Random(0 - bkList[i].animSpeed, bkList[i].animSpeed);
					}
				}
			}

			double yOff = bkList[i].bkgYval;

			if (bitm & 16)
			{
				yOff += (gWaterY - CS_camera_y_pos) * 0x200;
			}

			if (bkList[i].y != 0) {
				yOff += (bkList[i].y);  //for map snapping
			}

			if (bitm & 2)
			{
				yOff -= CS_camera_y_pos / 0x200 * bkList[i].ym;
			}
			if (bitm & 512) {
				yOff -= CS_camera_y_pos / 0x200;
			}

			yOff = (int)yOff;

			for (int y = 0; y < repY && yOff < gMap.length * 16; y++)
			{
				double xOff = bkList[i].bkgXval;

				if (bkList[i].x != 0) {
					xOff += (bkList[i].x);
				}

				if (bitm & 1)
				{
					xOff -= CS_camera_x_pos / 0x200 * bkList[i].xm;
				}
				if (bitm & 256) {
					xOff -= CS_camera_x_pos / 0x200;
				}

				xOff = (int)xOff;

				for (int x = 0; x < repX && xOff < gMap.width * 16; x++)
				{
					int xDo = xOff, yDo = yOff;
					if ((bitm & 32 && forg) || (!(bitm & 32) && !forg)) {
						if (i == 0)
						{
							PutBitmap4(&grcGame, xDo, yDo, &tRec, SURFACE_ID_LEVEL_BACKGROUND);
						}
						else
						{
							PutBitmap3(&grcGame, xDo, yDo, &tRec, SURFACE_ID_LEVEL_BACKGROUND);
						}
					}

					xOff += bkList[i].bmW + bkList[i].xDist;
				}

				yOff += bkList[i].bmH + bkList[i].yDist;
			}
		}
	}
}

void Replacement_ModeOpening_PutBack_Call(int frame_x, int frame_y)
{
	PutBack(frame_x, frame_y);
	BKG_RenderBackgrounds(frame_x, frame_y, false);
}

void Replacement_ModeOpening_PutFront_Call(int frame_x, int frame_y)
{
	BKG_RenderBackgrounds(frame_x, frame_y, true);
	PutFront(frame_x, frame_y);
}

void Replacement_ModeAction_PutBack_Call(int frame_x, int frame_y)
{
	PutBack(frame_x, frame_y);
	BKG_RenderBackgrounds(frame_x, frame_y, false);
}

void Replacement_ModeAction_PutFront_Call(int frame_x, int frame_y)
{
	BKG_RenderBackgrounds(frame_x, frame_y, true);
	PutFront(frame_x, frame_y);
}

void Replacement_TransferStage_InitBack_Call(const char* n, int t)
{
	memset(backgroundName, 0, sizeof(backgroundName));
	strcpy(backgroundName, n);
	backgroundType = t;

	InitBack(n, t);
}

// reset bkg backgrounds on stage transition
void Replacement_TransferStage_ResetFlash_Call()
{
	ResetFlash();
	BKG_ResetBackgrounds();
}