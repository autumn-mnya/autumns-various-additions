#pragma once

#define BKGCount 0x400
#define bkgTxTSize 0x30

typedef struct SUBKG
{
	bool isActive;		//Sub Background is in use
	int bmX;			//Bitmap X offset
	int bmY;			//Bitmap Y offset
	int bmW;			//Bitmap width
	int bmH;			//Bitmap height
	int repX;			//Number of times repeated width-wise
	int repY;			//Number of times repeated height-wise
	int xDist;			//Horizontal distance between each repeated section
	int yDist;			//Vertical distance between each repeated section
	int type;			//Scroll type
	double xm;			//Horizontal scroll speed
	double ym;			//Vertical scroll speed
	int xCount;			//X scroll counter
	int yCount;			//Y scroll counter
	int spriteNum;		//Number of frames
	int animFrame;		//Current Frame
	int animSpeed;		//Animation speed
	int counter;		//Animation counter
	double x;			//X offset
	double y;			//Y offset
	double bkgXval = 0;
	double bkgYval = 0;
}SUBKG;

extern SUBKG bkList[BKGCount];
extern int numBks;
extern char bkgTxT_Global[bkgTxTSize];

void Replacement_InitBack_ReloadBitmap_File_Call(const char* fName);
void BKG_ResetBackgrounds();
void BKG_SetBackground(int i, int b1, int b2, int b3, int b4, int r1, int r2, int d1, int d2, int t, double m1, double m2, int f, int s, double o1, double o2);
void BKG_SetParameter(int w, int x, int y);
void BKG_LoadBackground(char* file);
void BKG_RenderBackgrounds(int CS_camera_x_pos, int CS_camera_y_pos, bool forg);
void PutBKGBack();
void PutBKGFront();