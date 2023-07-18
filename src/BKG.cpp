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
#include "Profile.h"

SUBKG bkList[BKGCount] = {};
int numBks = 0;
char bkgTxT_Global[bkgTxTSize];

// 0x402339
void Replacement_InitBack_ReloadBitmap_File_Call(const char* fName)
{
	ReleaseSurface(0x1C);
	MakeSurface_File(fName, 0x1C);
}

void BKG_ResetBackgrounds()
{
	numBks = 0;

	memset(bkList, 0, sizeof(bkList));
	memset(bkgTxT_Global, 0, sizeof(bkgTxT_Global));
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
	if (bkList[i].type & 1024) // If type is 1024, background x/y position is random between 0 and x/y respectively, animation speed is random between (animation speed, and animation speed * 2), animation counter is random between 0 and animation speed, and starting animation frame as random between 0 and amount of frames.
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

// Used for <BKP
// Set parameter X for layer W to value Y
void BKG_SetParameter(int w, int x, int y)
{
	switch (x)
	{
		// Bitmap X offset
		default:
		case 0:
			bkList[w].bmX = y;
			break;

		// Bitmap Y offset
		case 1:
			bkList[w].bmY = y;
			break;

		// Bitmap width
		case 2:
			bkList[w].bmW = y;
			break;

		// Bitmap height
		case 3:
			bkList[w].bmH = y;
			break;

		// Number of times repeated width-wise
		case 4:
			bkList[w].repX = y;
			break;

		// Number of times repeated height-wise
		case 5:
			bkList[w].repY = y;
			break;

		// Horizontal distance between each repeated section
		case 6:
			bkList[w].xDist = y;
			break;

		// Vertical distance between each repeated section
		case 7:
			bkList[w].yDist = y;
			break;

		// Scroll type
		case 8:
			bkList[w].type = y;
			break;

		// Horizontal scroll Speed
		case 9:
			bkList[w].xm = y / 10;
			break;

		// Vertical scroll speed
			bkList[w].ym = y / 10;
		case 10:
			break;

		// Number of frames
		case 11:
			bkList[w].spriteNum = y;
			break;

		// Animation Speed
		case 12:
			bkList[w].animSpeed = y;
			break;

		// X offset
		case 13:
			bkList[w].x = y * 0x10;
			break;

		// Y offset
		case 14:
			bkList[w].y = y * 0x10;
			break;
	}
}

void BKG_LoadBackground(char* file)
{
	FILE* fptr;
	char path[MAX_PATH];
	char pathtxt[bkgTxTSize];

	// Get path
	sprintf(path, "%s\\%s", gBkgPath, "");
	sprintf(pathtxt, "%s.txt", file);
	strcat(path, pathtxt);

	// Open file
	fptr = fopen(path, "rb");

	// only reset background if the bkgTxT_Global path != 0
	if (bkgTxT_Global[0] == 0)
		BKG_ResetBackgrounds();

	// This is for the save file
	memcpy(bkgTxT_Global, file, sizeof(bkgTxT_Global));

	int bkXOffset, bkYOffset, bkWidth, bkHeight, repeatX, repeatY, repeatGapX, repeatGapY, t, f, s;
	double m1, m2, o1, o2;
	char line[256];
	int bkCount = 0;
	char backFi[256];
	fgets(line, sizeof(line), fptr);
	sscanf(line, "%s", backFi);
	InitBack(backFi, 0);

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
		if (bkList[i].isActive)
		{
			// Set RECT
			int x1 = bkList[i].bmX + (bkList[i].bmW * bkList[i].animFrame);
			int y1 = bkList[i].bmY;
			int x2 = x1 + bkList[i].bmW;
			int y2 = y1 + bkList[i].bmH;

			RECT tRec = { x1, y1, x2, y2 };

			if (bkList[i].spriteNum > 1) // Animated Background frames if sprite number is greater than 1
			{
				if (++bkList[i].counter > bkList[i].animSpeed)
				{
					if (++bkList[i].animFrame >= bkList[i].spriteNum)
					{
						bkList[i].animFrame = 0;
					}

					bkList[i].counter = 0;
				}
			}

			// Set repeat X/Y
			int repX = bkList[i].repX;
			int repY = bkList[i].repY;

			if (repX == 1) // If repeat X is 1, then the hozizontal distance between repeated backgrounds is set to 0.
			{
				bkList[i].xDist = 0;
			}

			// Create type bit
			int bitm = bkList[i].type;

			if (bitm & 4) // If we have type 4, then the background will scroll horizontally.
			{
				bkList[i].bkgXval -= bkList[i].xm;

				if (bkList[i].bkgXval + bkList[i].x + (bkList[i].bmW + bkList[i].xDist) * bkList[i].repX < 0)
				{
					bkList[i].bkgXval = bkList[i].bkgXval + (bkList[i].bmW + bkList[i].xDist) + WINDOW_WIDTH;

					if (bitm & 128) // If we have bit 128, then add random amount of speed to the Y direction, minimum being (0 subtracted by animation speed), maximum being animation speed.
					{
						bkList[i].bkgYval += Random(0 - bkList[i].animSpeed, bkList[i].animSpeed);
					}
				}

				if (bkList[i].bkgXval + bkList[i].x > WINDOW_WIDTH)
				{
					bkList[i].bkgXval = bkList[i].bkgXval - WINDOW_WIDTH;

					if (bitm & 128) // If we have bit 128, then add random amount of speed to the Y direction, minimum being (0 subtracted by animation speed), maximum being animation speed.
					{
						bkList[i].bkgYval += Random(0 - bkList[i].animSpeed, bkList[i].animSpeed);
					}
				}
			}

			if (bitm & 8) // If we have type 8, then the background will scroll vertically.
			{
				bkList[i].bkgYval -= bkList[i].ym;

				if (bkList[i].bkgYval + bkList[i].y < -(bkList[i].bmH + bkList[i].yDist) * bkList[i].repY)
				{
					bkList[i].bkgYval = bkList[i].bkgYval + (bkList[i].bmH + bkList[i].yDist) * bkList[i].repY;

					if (bitm & 64) // If we have bit 64, then add random amount of speed to the X direction, minimum being (0 subtracted by animation speed), maximum being animation speed.
					{
						bkList[i].bkgXval += Random(0 - bkList[i].animSpeed, bkList[i].animSpeed);
					}
				}

				if (bkList[i].bkgYval + bkList[i].y > (bkList[i].bmH + bkList[i].yDist) * bkList[i].repY)
				{
					bkList[i].bkgYval = bkList[i].bkgYval - (bkList[i].bmH + bkList[i].yDist) * bkList[i].repY;

					if (bitm & 64) // If we have bit 64, then add random amount of speed to the X direction, minimum being (0 subtracted by animation speed), maximum being animation speed.
					{
						bkList[i].bkgXval += Random(0 - bkList[i].animSpeed, bkList[i].animSpeed);
					}
				}
			}

			// Create yOff value
			double yOff = bkList[i].bkgYval;

			if (bitm & 16) // If we have bit 16, then the background aligns with gWaterY
			{
				yOff += (gWaterY - CS_camera_y_pos) * 0x200;
			}

			if (bkList[i].y != 0) // If the backgrounds Y does NOT equal 0, then move
			{
				yOff += (bkList[i].y);  //for map snapping
			}

			if (bitm & 2) // If we have bit 2, then move the background vertically with the players camera
			{
				yOff -= CS_camera_y_pos / 0x200 * bkList[i].ym;
			}

			if (bitm & 512) // If we have bit 512, lock the background to the X axis at all times
			{
				yOff -= CS_camera_y_pos / 0x200;
			}

			// Turn yOff into an int?
			yOff = (int)yOff;

			for (int y = 0; y < repY && yOff < gMap.length * 16; y++)
			{
				double xOff = bkList[i].bkgXval;

				if (bkList[i].x != 0)
				{
					xOff += (bkList[i].x);
				}

				if (bitm & 1) // If we have bit flag 1, move the background horizontally with the players camera
				{
					xOff -= CS_camera_x_pos / 0x200 * bkList[i].xm;
				}

				if (bitm & 256) // If we have bit 256, lock the background to the Y axis at all times
				{
					xOff -= CS_camera_x_pos / 0x200;
				}

				xOff = (int)xOff;

				for (int x = 0; x < repX && xOff < gMap.width * 16; x++)
				{
					int xDo = xOff, yDo = yOff;

					if ((bitm & 32 && forg) || (!(bitm & 32) && !forg))
					{
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