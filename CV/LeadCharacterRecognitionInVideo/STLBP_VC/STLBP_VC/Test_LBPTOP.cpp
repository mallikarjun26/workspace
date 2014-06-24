// Test_LBPTOP.cpp : has an example in main{} how to use LBP-TOP and VLBP
// The below codes are not optimized. It is straightforward for easy understanding.

// Copyright 2009 by Guoying Zhao & Matti Pietikainen


#include "StdAfx.h"
#include "math.h"

#include "cv.h"
#include "highgui.h"
#include "string.h"
//#include "afx.h"

#define	BOUND(x, lowerbound, upperbound)  { (x) = (x) > (lowerbound) ? (x) : (lowerbound); \
                                            (x) = (x) < (upperbound) ? (x) : (upperbound); };
typedef   unsigned char BYTE;
#define PI 3.1415926535897932
#define POW(nBit)   (1 << (nBit))
#define FREE(ptr) 	{if (NULL!=(ptr)) {delete[] ptr;  ptr=NULL;}}

// uniform pattern for LBP without roation invariance for neighboring points 8 (only for 8)
// If the number of neighboring points is 4, 16 or other numbers than 8, you need to change this lookup array accordingly.
int  UniformPattern59[256]={
	     1,   2,   3,   4,   5,   0,   6,   7,   8,   0,   0,   0,   9,   0,  10,  11,
		12,   0,   0,   0,   0,   0,   0,   0,  13,   0,   0,   0,  14,   0,  15,  16,
		17,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		18,   0,   0,   0,   0,   0,   0,   0,  19,   0,   0,   0,  20,   0,  21,  22,
		23,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		25,   0,   0,   0,   0,   0,   0,   0,  26,   0,   0,   0,  27,   0,  28,  29,
		30,  31,   0,  32,   0,   0,   0,  33,   0,   0,   0,   0,   0,   0,   0,  34,
		0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  35,
		0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
		0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  36,
		37,  38,   0,  39,   0,   0,   0,  40,   0,   0,   0,   0,   0,   0,   0,  41,
		0,    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  42,
		43,  44,   0,  45,   0,   0,   0,  46,   0,   0,   0,   0,   0,   0,   0,  47,
		48,  49,   0,  50,   0,   0,   0,  51,  52,  53,   0,  54,  55,  56,  57,  58
};

// simple example how to computer LBP-TOP and VLBP.
void Test();

// For computing the LBP-TOP.
void LBP_TOP(BYTE ***fg, int Length, int height,int width,
			 int FxRadius,int FyRadius,int TInterval,
			 int XYNeighborPoints,int XTNeighborPoints,int YTNeighborPoints,
			 int TimeLength,int BoderLength, int Bincount,
			 bool bBilinearInterpolation,float **Histogram);

// For computing basic VLBP and two rotation invariant VLBP.
void RIVLBP(BYTE ***fg, int Length, int height,int width,
			int TInterval,int FRadius,int NeighborPoints,
			int BoderLength,int TimeLength,int RotateIndex,
			bool bBilinearInterpolation,float *Histogram);

int RotLBP(int LBPCode,int NeighborPoints);
void ComputeRotationInvariance(int RotateIndex, int NeighborPoints,
							   int tempLBPpre, int tempLBPcur, int tempLBPpos,
							   int tempLBPpreC, int tempLBPposC, int binCount,
							   int& BasicLBP);



int main()
{
	Test();

	return 0;
}

void Test()
{
	char* file[]={
		{"C:\\test\\000.bmp"},
		{"C:\\test\\001.bmp"},
		{"C:\\test\\002.bmp"},
		{"C:\\test\\003.bmp"},
		{"C:\\test\\004.bmp"},
		{"C:\\test\\005.bmp"},
		{"C:\\test\\006.bmp"},
		{"C:\\test\\007.bmp"},
		{"C:\\test\\008.bmp"},
		{"C:\\test\\009.bmp"},
		{"C:\\test\\010.bmp"}
	};
	int height,width;
	CvSize imagesize;
    int widthstep,Channels;
	int i,j;

	int Length=11;
	IplImage **pFrame,**greyImg;
	BYTE **fg;

	// This is just an example. In your implementation, to avoid the memory problem,
	// it is better to keep only the [TInterval * 2 + 1] pFrame, greyImag, fg and data, which takes part in the computation in one iteration of "for(i = TimeLength; i < Length - TimeLength; i++)" inside LBP-TOP.
	// or keep [3] pFrame, greyImag, fg and data, which takes part in the computation in one iteration of "for(i = TimeLength; i < Length - TimeLength; i++)" inside RIVLBP.
	pFrame = new IplImage *[Length];
	greyImg = new IplImage *[Length];
	fg = new BYTE * [Length];

	BYTE ***data=NULL;
	float** Histogram = NULL;

	int k;
	data= new BYTE**[Length];

	for(i = 0; i < Length; i++)
	{
        pFrame[i] = cvLoadImage(file[i]);
		if((i == 0)&&(pFrame[0]))
		{
			height = pFrame[0]->height;
			width = pFrame[0]->width;
			imagesize = cvGetSize(pFrame[0]);
			Channels = pFrame[0]->nChannels;

		}
		greyImg[i] = cvCreateImage(imagesize,8,1);
		if(Channels == 3)
			cvCvtColor(pFrame[i],greyImg[i],CV_RGB2GRAY);
		else
			cvCopy(pFrame[i],greyImg[i]);

		widthstep = greyImg[0]->widthStep;
		fg[i] = (BYTE *)greyImg[i]->imageData;

	 }


	for(i=0; i<Length; i++)
	{
		data[i]=New2DPointer<BYTE>(height, width);
	}


	for(i = 0; i < Length; i++)
	{
		for(j=0; j<height; j++)
		{
			for(k=0; k<width; k++)
			{
				data[i][j][k]=*(fg[i] + widthstep * j + k);
			}
		}
	}

	// for computing LBP-TOP
	///////////////////////////////////////////////////////////

	   bool bBilinearInterpolation=0;

	   int FxRadius=1;
	   int FyRadius=1;
	   int TInterval=2;

	   int TimeLength=2;
	   int BoderLength=1;

	   int XYNeighborPoints=8;
	   int XTNeighborPoints=8;
	   int YTNeighborPoints=8;
	   int Bincount = 0; // 59 is only for neighboring points with 8. If won't compute unifrom patterns, please set it to 0, then basic LBP-TOP will be computed

	   int DimLBPTOP;// Actually, DimLBPTOP could be an array, to keep the length of the feature vectors from XY, XT, and YT planes, respectively.
	   // Here, to make it simple, we set them as same. That is to say, the number of the neighboring points are same for all three planes.
	   if(Bincount == 0)
	   {
		   DimLBPTOP = POW(XYNeighborPoints);
	   }
	   else
	   {
		   DimLBPTOP = Bincount;
	   }
	   Histogram = New2DPointer<float>(3, DimLBPTOP); // 3 planes with uniform patterns
	   for(j = 0; j < 3; j++)
	   {
		   for(k = 0; k < DimLBPTOP; k++)
		   {
			   Histogram[j][k]=0;
		   }
	   }



	   LBP_TOP(data, Length, height, width, FxRadius, FyRadius, TInterval,
		   XYNeighborPoints, XTNeighborPoints, YTNeighborPoints,
		   TimeLength, BoderLength,Bincount, bBilinearInterpolation,Histogram);

	   printf("LBP_TOP computation is finished. \n");
       ////////////////////////////////////////////////////////////////

	   ///////////////////////////////////////////////////////////////
	   // for computing VLBP
	   int RotateIndex=1;
	   int FRadius=1;
	   int NeighborPoints=4; // could be 2 or 4.
	   int binCount = (NeighborPoints + 1) * 2 + NeighborPoints;
	   int nDim=POW(binCount);
	   float *fHistogram=new float [nDim];

	   for(k=0; k<nDim; k++)
	   {
		   fHistogram[k]=0;
	   }

	   RIVLBP(data, Length, height,width, TInterval, FRadius, NeighborPoints,
		    BoderLength, TimeLength, RotateIndex, bBilinearInterpolation, fHistogram);

	   printf("RIVLBP computation is finished. \n");
	   /////////////////////////////////////////////////////////////////////////////

	   Delete2DPointer(Histogram, 3);
	   delete[] fHistogram;
	   for(i=0; i<Length; i++)
	   {
		   for(j = 0; j < height; j++)
			   delete []data[i][j];
		   delete []data[i];
	   }
	   delete []data;

	   for(i = 0; i < Length; i++)
	   {
			cvReleaseImage(&greyImg[i]);
			cvReleaseImage(&pFrame[i]);
	   }

	   FREE(pFrame);
	   FREE(greyImg);
	   FREE(fg);

	   printf("Deleted all arrays. \n");

}//testing


/////////////////////////////////////////////////========================================


/* This function is to compute the LBP-TOP features for a video sequence
 Reference:
 Guoying Zhao, Matti Pietikainen, "Dynamic texture recognition using local binary patterns
 with an application to facial expressions," IEEE Transactions on Pattern Analysis and Machine
 Intelligence, 2007, 29(6):915-928.

  Copyright 2009 by Guoying Zhao & Matti Pietikainen
*/

// =====================================================================================================//
// Function: Running this funciton each time to compute the LBP-TOP distribution of one video sequence.

// Inputs:

// "fg" keeps the grey level of all the pixels in sequences with [Length][height][width];
//      please note, all the images in one sequnces should have same size (height and weight).
//      But they don't have to be same for different sequences.

// "Length" is the length of the sequence;

// "height" is the height of the images;

// "width" is the width of the images;

// "FxRadius", "FyRadius" and "TInterval" are the radii parameter along X, Y and T axis; They can be 1, 2, 3 and 4. "1" and "3" are recommended.
// Pay attention to "TInterval". "TInterval * 2 + 1" should be smaller than the length of the input sequence "Length". For example, if one sequence includes seven frames, and you set TInterval to three, only the pixels in the frame 4 would be considered as central pixel and computed to get the LBP-TOP feature.


// "XYNeighborPoints", "XTNeighborPoints" and "YTNeighborPoints" are the number of the neighboring points
//     in XY plane, XT plane and YT plane; They can be 4, 8, 16 and 24. "8" is a good option.

// "TimeLength" and "BoderLength" are the parameters for bodering parts in time and space which would not
//     be computed for features. Usually they are same to TInterval and the bigger one of "FxRadius" and "FyRadius";

// "bBilinearInterpolation": if use bilinear interpolation for computing a neighboring point in a circle: 1 (yes), 0 (no).

// "Bincount": For example, if XYNeighborPoints = XTNeighborPoints = YTNeighborPoints = 8, you can set "Bincount" as "0" if you want to use basic LBP, or set "Bincount" as 59 if using uniform pattern of LBP,
//             If the number of Neighboring points is different than 8, you need to change it accordingly as well as change the above "UniformPattern59".


// Output:

// "Histogram": keeps LBP-TOP distribution of all the pixels in the current frame with [3][dim];
//     here, "3" deote the three planes of LBP-TOP, i.e., XY, XZ and YZ planes.
//     Each value of Histogram[i][j] is between [0,1]



// =====================================================================================================//

void LBP_TOP(BYTE ***fg, int Length, int height,int width,
			 int FxRadius,int FyRadius,int TInterval,
			 int XYNeighborPoints,int XTNeighborPoints,int YTNeighborPoints,
			 int TimeLength,int BoderLength, int Bincount,
			 bool bBilinearInterpolation,float **Histogram)

{
	int i,j;
	int xc,yc;			BYTE CenterByte;	BYTE CurrentByte;
	int BasicLBP = 0;	int FeaBin = 0;		int p;
	int X,Y,Z;			float x1,y1,z1;		float u,v;
	int ltx,lty;		int lbx,lby; 		int	rtx,rty;		int rbx,rby;

	if (false == bBilinearInterpolation)
	{
		for(i = TimeLength; i < Length - TimeLength; i++)
		{
			for(yc = BoderLength; yc < height - BoderLength; yc++)
			{
				for(xc = BoderLength; xc < width - BoderLength; xc++)
				{
					CenterByte = fg[i][yc][xc];
					BasicLBP = 0;	FeaBin = 0;

					// In XY plane
					for(p = 0; p < XYNeighborPoints; p++)
					{
						X = int (xc + FxRadius * cos((2 * PI * p) / XYNeighborPoints) + 0.5);
						Y = int (yc - FyRadius * sin((2 * PI * p) / XYNeighborPoints) + 0.5);
						BOUND(X,0,width-1); BOUND(Y,0,height-1);
						CurrentByte = fg[i][Y][X];
						if(CurrentByte >= CenterByte) BasicLBP += POW ( FeaBin);
						FeaBin++;
					}

					//     if "Bincount" is "0", it means basic LBP-TOP will be computed
					//         and "UniformPattern59" does not work in this case.
					//     Otherwise it should be the number of the uniform patterns,
					//         then "UniformPattern59" keeps the lookup-table of the
					//         basic LBP and unifrom LBP.
					if(Bincount == 0)
						Histogram[0][BasicLBP]++;
					else // uniform patterns
						Histogram[0][UniformPattern59[BasicLBP]]++;


					// In XT-plane
					BasicLBP = 0;	FeaBin = 0;
					for(p = 0; p < XTNeighborPoints; p++)
					{
						X = int (xc + FxRadius * cos((2 * PI * p) / XTNeighborPoints) + 0.5);
						Z = int (i + TInterval * sin((2 * PI * p) / XTNeighborPoints) + 0.5);
						BOUND(X,0,width-1); BOUND(Z,0,Length-1);

						CurrentByte = fg[Z][yc][X];
						if(CurrentByte >= CenterByte) BasicLBP += POW ( FeaBin);
						FeaBin++;
					}

                	if(Bincount == 0)
						Histogram[1][BasicLBP]++;
					else
						Histogram[1][UniformPattern59[BasicLBP]]++;

					// In YT-plane
					BasicLBP = 0;	FeaBin = 0;
					for(p = 0; p < YTNeighborPoints; p++)
					{
						Y = int (yc - FyRadius * sin((2 * PI * p) / YTNeighborPoints) + 0.5);
						Z = int (i + TInterval * cos((2 * PI * p) / YTNeighborPoints) + 0.5);
						BOUND(Y,0,height-1); BOUND(Z,0,Length-1);

						CurrentByte = fg[Z][Y][xc];
						if(CurrentByte >= CenterByte) BasicLBP += POW ( FeaBin);
						FeaBin++;
					}

					if(Bincount == 0)
						Histogram[2][BasicLBP]++;
					else
						Histogram[2][UniformPattern59[BasicLBP]]++;

				}//for(xc = BoderLength; xc < width - BoderLength; xc++)
			}//for(yc = BoderLength; yc < height - BoderLength; yc++)

		}//for(i = TimeLength; i < Length - TimeLength; i++)

	} // if (true == bUniformPattern)

	else
	{
		for(i = TimeLength; i < Length - TimeLength; i++)
		{
			for(yc = BoderLength; yc < height - BoderLength; yc++)
			{
				for(xc = BoderLength; xc < width - BoderLength; xc++)
				{
					CenterByte = fg[i][yc][xc];
					BasicLBP = 0;	FeaBin = 0;

					// In XY plane
					for(p = 0; p < XYNeighborPoints; p++)
					{
						//		bilinear interpolation
						x1 = float(xc + FxRadius * cos((2 * PI * p) / XYNeighborPoints));
						y1 = float(yc - FyRadius * sin((2 * PI * p) / XYNeighborPoints));


						u = x1 - INT(x1);
						v = y1 - INT(y1);
						ltx = INT(floor(x1)); lty = INT(floor(y1));
						lbx = INT(floor(x1)); lby = INT(ceil(y1));
						rtx = INT(ceil(x1)); rty = INT(floor(y1));
						rbx = INT(ceil(x1)); rby = INT(ceil(y1));
						// values of neighbors that do not fall exactly on pixels are estimated
						// by bilinear interpolation of four corner points near to it.


						CurrentByte = (BYTE) (	fg[i][lty][ltx] * (1 - u) * (1 - v) + \
												fg[i][lby][lbx] * (1 - u) * v + \
												fg[i][rty][rtx] * u * (1 - v) + \
												fg[i][rby][rbx]  * u * v);
						if(CurrentByte >= CenterByte) BasicLBP += POW ( FeaBin);
						FeaBin++;

					}


					//     if "Bincount" is "0", it means basic LBP-TOP will be computed
					//         and "UniformPattern59" does not work in this case.
					//     Otherwise it should be the number of the uniform patterns,
					//         then "UniformPattern59" keeps the lookup-table of the
					//         basic LBP and unifrom LBP.
					if(Bincount == 0)
						Histogram[0][BasicLBP]++;
					else // uniform patterns
						Histogram[0][UniformPattern59[BasicLBP]]++;


					// In XT-plane
					BasicLBP = 0;
					FeaBin = 0;
					for(p = 0; p < XTNeighborPoints; p++)
					{
					//	bilinear interpolation
						x1 =float(xc + FxRadius * cos((2 * PI * p) / XTNeighborPoints)) ;
						z1 =float(i + TInterval * sin((2 * PI * p) / XTNeighborPoints)) ;

						u = x1 - INT(x1);
						v = z1 - INT(z1);
						ltx = INT(floor(x1)); lty = INT(floor(z1));
						lbx = INT(floor(x1)); lby = INT(ceil(z1));
						rtx = INT(ceil(x1)); rty = INT(floor(z1));
						rbx = INT(ceil(x1)); rby = INT(ceil(z1));
						// values of neighbors that do not fall exactly on pixels are estimated
						// by bilinear interpolation of four corner points near to it.

						CurrentByte = (BYTE) (fg[lty][yc][ltx] * (1 - u) * (1-v) + \
											  fg[lby][yc][lbx] * (1 - u) * v + \
											  fg[rty][yc][rtx]* u * (1-v) + \
											  fg[rby][yc][rbx] * u * v);
						if(CurrentByte >= CenterByte) BasicLBP += POW ( FeaBin);
						FeaBin++;
					}

                	if(Bincount == 0)
						Histogram[1][BasicLBP]++;
					else
						Histogram[1][UniformPattern59[BasicLBP]]++;

					// In YT-plane
					BasicLBP = 0;
					FeaBin = 0;


					for(p = 0; p < YTNeighborPoints; p++)
					{
					//	bilinear interpolation
						y1 = float (yc - FyRadius * sin((2 * PI * p) / YTNeighborPoints));
						z1 = float (i + TInterval * cos((2 * PI * p) / YTNeighborPoints));

						u = y1 - INT(y1);
						v = z1 - INT(z1);

						ltx = INT(floor(y1)); lty = INT(floor(z1));
						lbx = INT(floor(y1)); lby = INT(ceil(z1));
						rtx = INT(ceil(y1)); rty = INT(floor(z1));
						rbx = INT(ceil(y1)); rby = INT(ceil(z1));
						// values of neighbors that do not fall exactly on pixels are estimated
						// by bilinear interpolation of four corner points near to it.
						CurrentByte = (BYTE) (fg[lty][ltx][xc] * (1 - u) * (1 - v) + \
							                  fg[lby][lbx][xc] * (1 - u) * v + \
											  fg[rty][rtx][xc] * u * (1 - v) + \
											  fg[rby][rbx][xc] * u * v);
						if(CurrentByte >= CenterByte) BasicLBP += POW ( FeaBin);
						FeaBin++;
					}

					if(Bincount == 0)
						Histogram[2][BasicLBP]++;
					else
						Histogram[2][UniformPattern59[BasicLBP]]++;

				}//for(xc = BoderLength; xc < width - BoderLength; xc++)
			}//for(yc = BoderLength; yc < height - BoderLength; yc++)
		}//for(i = TimeLength; i < Length - TimeLength; i++)
	} // if (true == bUniformPattern)

//-------------  Normalization ----------------------------//
	int binCount[3];
	if(Bincount == 0)
	{
		binCount[0]= POW(XYNeighborPoints);
		binCount[1]= POW(XTNeighborPoints);
		binCount[2]= POW(YTNeighborPoints);
	}
	else
	{
		// for case that XYNeighborPoints = XTNeighborPoints = XTNeighborPoints.
		// If they are not same, there should be three "Bincount".
		binCount[0] = Bincount;		binCount[1] = Bincount;		binCount[2] = Bincount;
	}

	// Normaliztion
	int Total = 0;
	for(j = 0; j < 3; j++)
	{
		Total = 0;
		for(i = 0; i < binCount[j]; i++)
			Total += int (Histogram[j][i]);
		for(i = 0; i < binCount[j]; i++)
		{
			Histogram[j][i] /= Total;

		}
	}
//-------------  Normalization ----------------------------//

}


/* This function is to compute the Basic VLBP and two kinds of rotation invariant VLBP features for a video sequence
 Reference:
 Guoying Zhao, Matti Pietikainen, "Dynamic texture recognition using local binary patterns
 with an application to facial expressions," IEEE Transactions on Pattern Analysis and Machine
 Intelligence, 2007, 29(6):915-928.

 Guoying Zhao, Matti Pietikainen, "Dynamic texture recognition using volume local binary patterns,"
In: Dynamical Vision, WDW 2005/2006 Proceedings, Lecture Notes in Computer Science 4358, 2006, 165-177.

 Copyright 2009 by Guoying Zhao & Matti Pietikainen
*/

// Inputs:

// "fg" keeps the grey level of all the pixels in sequences with [Length][height][width];
//      please note, all the images in one sequnces should have same size (height and weight).
//      But they don't have to be same for different sequences.

// "Length" is the length of the sequence;

// "height" is the height of the images;

// "width" is the width of the images;

// "FRadius", and "TInterval" are the radii parameter in space and Time axis; They could be 1, 2 or 3 or 4.
// Pay attention to "TInterval". "TInterval * 2 + 1" should be smaller than the length of the input sequence "Length". For example, if one sequence includes seven frames, and you set TInterval to three, only the pixels in the frame 4 would be considered as central pixel and computed to get the LBP-TOP feature.

// "NeighborPoints" is the number of the neighboring points; It can be 2 and 4.
// Since this "NeighborPoints" means the number of the neighboring points in one frame and the number of all the neighboring points for computing VLBP is pow(2,(NeighborPoints + 1) * 2 + NeighborPoints),
// so this value could not be very big. 2 and 4 are good options.

// "TimeLength" and "BoderLength" are the parameters for bodering parts in time and
//      space which would not be computed for features. Usually they are same to TInterval and
//      the bigger one of "FxRadius" and "FyRadius";

// "RotateIndex": 0: basic VLBP without rotation;
//                1: new Rotation invariant descriptor published in PAMI 2007;
//                2: old Rotation invariant descriptor published in ECCV workshop 2006

// "bBilinearInterpolation": if use bilinear interpolation for computing a neighboring point in a circle: 1 (yes), 0 (no).


// Output:

// "Histogram": LBP-TOP distribution. each value between [0,1]

void RIVLBP(BYTE ***fg, int Length, int height,int width,
			int TInterval,int FRadius,int NeighborPoints,
			int BoderLength,int TimeLength,int RotateIndex,
			bool bBilinearInterpolation,float *Histogram)

{
	int i;				int BasicLBP = 0;	int FeaBin = 0;		int p = 0;
	int xc,yc;			int X,Y;			BYTE CenterByte;	BYTE CurrentByte;
	int binCount = (NeighborPoints + 1) * 2 + NeighborPoints;

	// to store segmented LBP and to get rotation invariace code
	int tempLBPpreC, tempLBPpre;			int tempLBPcur;		int tempLBPpos,tempLBPposC;
	float x1,y1,u,v;
	int ltx,lty;		int lbx,lby; 		int	rtx,rty;		int rbx,rby;

	if (false == bBilinearInterpolation)
	{
		for(i = TimeLength; i < Length - TimeLength; i++)
		{
			for(yc = BoderLength; yc < height - BoderLength; yc++)
			{
				for(xc = BoderLength; xc < width - BoderLength; xc++)
				{
					CenterByte = fg[i][yc][xc];
					BasicLBP = 0;		FeaBin = 0;

					// In previous frame
					CurrentByte = fg[i-TInterval][yc][xc];
					if(CurrentByte >= CenterByte)
						BasicLBP += POW(FeaBin);
					tempLBPpreC = BasicLBP;
					FeaBin++;

                	p = 0;	tempLBPpre = 0;
					for(p = 0; p < NeighborPoints; p++)
					{
						X = int (xc + FRadius * cos((2 * PI * p) / NeighborPoints) + 0.5);
						Y = int (yc - FRadius * sin((2 * PI * p) / NeighborPoints) + 0.5);

						CurrentByte = fg[i-TInterval][Y][X];

						if(CurrentByte >= CenterByte) {BasicLBP += POW(FeaBin); tempLBPpre+= POW(p);}
						FeaBin++;
					}


					// In current frame
					tempLBPcur = 0;
					for(p = 0; p < NeighborPoints; p++)
					{
						X = int (xc + FRadius * cos((2 * PI * p) / NeighborPoints) + 0.5);
						Y = int (yc - FRadius * sin((2 * PI * p) / NeighborPoints) + 0.5);

						CurrentByte = fg[i][Y][X];

						if(CurrentByte >= CenterByte) {BasicLBP += POW(FeaBin);tempLBPcur+= POW(p);}
						FeaBin++;

					}

					// In post frame
					tempLBPpos = 0;
					for(p = 0; p < NeighborPoints; p++)
					{
						X = int (xc + FRadius * cos((2 * PI * p) / NeighborPoints) + 0.5);
						Y = int (yc - FRadius * sin((2 * PI * p) / NeighborPoints) + 0.5);

						CurrentByte = fg[i+TInterval][Y][X];

						if(CurrentByte >= CenterByte) {BasicLBP += POW(FeaBin);tempLBPpos+= POW(p);}
						FeaBin++;
					}


					tempLBPposC = 0;
					CurrentByte = fg[i+TInterval][yc][xc];
					if(CurrentByte >= CenterByte) {BasicLBP += POW(FeaBin);tempLBPposC = 1;}
					FeaBin++;

					if ((RotateIndex == 1)||(RotateIndex == 2))
					// if RotateIndex == 0, basic VLBP is computed.
					{
						// for rotation invariance code
						// for rotation invariance code
						ComputeRotationInvariance( RotateIndex,  NeighborPoints,  tempLBPpre,  tempLBPcur,
										 tempLBPpos, tempLBPpreC,  tempLBPposC,  binCount, BasicLBP);
					}
	  ///////////////////////////////////////////////////////////////////////////////////

					Histogram[BasicLBP]++;
				}//for(xc = BoderLength; xc < width - BoderLength; xc++)
			}//for(yc = BoderLength; yc < height - BoderLength; yc++)
		}//for(i = TimeLength; i < Length - TimeLength; i++)
	}

	else
	{
		for(i = TimeLength; i < Length - TimeLength; i++)
		{
			for(yc = BoderLength; yc < height - BoderLength; yc++)
			{
				for(xc = BoderLength; xc < width - BoderLength; xc++)
				{
					CenterByte = fg[i][yc][xc];
					BasicLBP = 0;	FeaBin = 0;

					// In previous frame
					CurrentByte = fg[i-TInterval][yc][xc];
					if(CurrentByte >= CenterByte)
						BasicLBP += POW(FeaBin);
					tempLBPpreC = BasicLBP;
					FeaBin++;

                	p = 0;		tempLBPpre = 0;
					for(p = 0; p < NeighborPoints; p++)
					{

						// bilinear interpolation
						x1 = float (xc + FRadius * cos((2 * PI * p) / NeighborPoints));
						y1 = float (yc - FRadius * sin((2 * PI * p) / NeighborPoints));

						u = x1 - INT(x1);
						v = y1 - INT(y1);
						int ltx,lty,lbx,lby,rtx,rty,rbx,rby;
						ltx = INT(floor(x1)); lty = INT(floor(y1));
						lbx = INT(floor(x1)); lby = INT(ceil(y1));
						rtx = INT(ceil(x1)); rty = INT(floor(y1));
						rbx = INT(ceil(x1)); rby = INT(ceil(y1));
						// values of neighbors that do not fall exactly on pixels are estimated
						// by bilinear interpolation of four corner points near to it.
						CurrentByte = (BYTE) (	fg[i-TInterval][lty][ltx] * (1 - u)*(1-v) + \
												fg[i-TInterval][lby][lbx] *(1 - u) * v + \
												fg[i-TInterval][rty][rtx]*u*(1-v) + \
												fg[i-TInterval][rby][rbx] * u * v);
						if(CurrentByte >= CenterByte) {BasicLBP += POW(FeaBin); tempLBPpre+= POW(p);}
						FeaBin++;
					}

					// In current frame
					tempLBPcur = 0;
					for(p = 0; p < NeighborPoints; p++)
					{

						// bilinear interpolation
						x1 = float (xc + FRadius * cos((2 * PI * p) / NeighborPoints));
						y1 = float (yc - FRadius * sin((2 * PI * p) / NeighborPoints));

						u = x1 - INT(x1);
						v = y1 - INT(y1);
						ltx = INT(floor(x1)); lty = INT(floor(y1));
						lbx = INT(floor(x1)); lby = INT(ceil(y1));
						rtx = INT(ceil(x1)); rty = INT(floor(y1));
						rbx = INT(ceil(x1)); rby = INT(ceil(y1));
						// values of neighbors that do not fall exactly on pixels are estimated
						// by bilinear interpolation of four corner points near to it.
						CurrentByte =  (BYTE) (	fg[i][lty][ltx] * (1 - u)*(1-v) + \
												fg[i][lby][lbx] *(1 - u) * v + \
												fg[i][rty][rtx]*u*(1-v) + \
												fg[i][rby][rbx] * u * v);
						if(CurrentByte >= CenterByte) {BasicLBP += POW(FeaBin);tempLBPcur+= POW(p);}
						FeaBin++;
					}

					// In post frame
					tempLBPpos = 0;
					for(p = 0; p < NeighborPoints; p++)
					{

						// bilinear interpolation
						x1 = float (xc + FRadius * cos((2 * PI * p) / NeighborPoints));
						y1 = float (yc - FRadius * sin((2 * PI * p) / NeighborPoints));

						u = x1 - INT(x1);
						v = y1 - INT(y1);
						ltx = INT(floor(x1)); lty = INT(floor(y1));
						lbx = INT(floor(x1)); lby = INT(ceil(y1));
						rtx = INT(ceil(x1)); rty = INT(floor(y1));
						rbx = INT(ceil(x1)); rby = INT(ceil(y1));
						// values of neighbors that do not fall exactly on pixels are estimated
						// by bilinear interpolation of four corner points near to it.
						CurrentByte =  (BYTE) (	fg[i+TInterval][lty][ltx] * (1 - u)*(1-v) + \
												fg[i+TInterval][lby][lbx] *(1 - u) * v + \
												fg[i+TInterval][rty][rtx]*u*(1-v) + \
												fg[i+TInterval][rby][rbx] * u * v);
						if(CurrentByte >= CenterByte) {BasicLBP += POW(FeaBin);  tempLBPpos+= POW(p);}
						FeaBin++;
					}


					tempLBPposC = 0;
					CurrentByte = fg[i+TInterval][yc][xc];
					if(CurrentByte >= CenterByte) {BasicLBP += POW(FeaBin);tempLBPposC = 1;}
					FeaBin++;

					if ((RotateIndex == 1)||(RotateIndex == 2))
					// if RotateIndex == 0, basic VLBP is computed.
					{
					// for rotation invariance code
						ComputeRotationInvariance( RotateIndex,  NeighborPoints,  tempLBPpre,  tempLBPcur,
									tempLBPpos, tempLBPpreC,  tempLBPposC,  binCount, BasicLBP);

					}


					Histogram[BasicLBP]++;
				}//for(xc = BoderLength; xc < width - BoderLength; xc++)
			}//for(yc = BoderLength; yc < height - BoderLength; yc++)

		}//for(i = TimeLength; i < Length - TimeLength; i++)
	}

	//-------------  Normalization ----------------------------//
	int Total = 0;
   int nDim=POW(binCount);

	for(i = 0; i < nDim; i++)
		Total += int (Histogram[i]);
	for(i = 0; i < nDim; i++)
	{
		Histogram[i] /= Total;
	}
	//-------------  Normalization ----------------------------//

}


// For a basic LBP code, this function is to get its rotation invariance corresponding code.
int RotLBP(int LBPCode,int NeighborPoints)
{
	int minLBP = LBPCode,	tempCode;
	for(int i = 1; i < NeighborPoints; i++)
   	{
		tempCode = (LBPCode>>i) | (((LBPCode & (INT(pow(2,i)) - 1)) << (NeighborPoints - i)));
		if (tempCode < minLBP) minLBP = tempCode;
	}

    return minLBP;
}


// for rotation invariance code
// Please note, the length of rotaion invariant VLBP is shorter than basic VLBP. For example, for "NeighborPoints = 4", the length of basic VLBP is 16384, while the length of new rotation invariant VLBP is 4176 and old rotation invariant VLBP 864.
// But in the following code, we keeps the length of histogram same to basic VLBP. Just the values of some VLBP are computed into their rotation invariant value and then original bits in histogram are zero.
// It is mainly to show how to compute the rotation invariant VLBP.

// You can make some postprocessing afterwards. Or you can make a lookup table according to the codes below to make the processing faster, like the uniform pattern we use in the LBP-TOP above.
void ComputeRotationInvariance(int RotateIndex, int NeighborPoints,
							   int tempLBPpre, int tempLBPcur, int tempLBPpos,
							   int tempLBPpreC, int tempLBPposC, int binCount,
							   int& BasicLBP)
{
	int minLBP = BasicLBP,	temp;
	int tempLBPpreT,tempLBPcurT,tempLBPposT;
	int p;
	// New rotation invariant descriptor which is published in PAMI 2007
	if (RotateIndex == 1)
	{
		for(p = 1; p < NeighborPoints; p++)
		{
			tempLBPpreT = (tempLBPpre>>p) | (((tempLBPpre & (INT(POW(p)) - 1)) << (NeighborPoints - p)));
			tempLBPcurT = (tempLBPcur>>p) | (((tempLBPcur & (INT(POW(p)) - 1)) << (NeighborPoints - p)));
			tempLBPposT = (tempLBPpos>>p) | (((tempLBPpos & (INT(POW(p)) - 1)) << (NeighborPoints - p)));
			temp = int ( tempLBPpreC + (tempLBPpreT << 1) + \
				(tempLBPcurT << (NeighborPoints + 1)) + \
				(tempLBPposT << (NeighborPoints * 2 + 1)) + \
				tempLBPposC * POW(binCount - 1));
			if (temp < minLBP) minLBP = temp;
		}
		BasicLBP = minLBP;
	}

	// Old rotation invariant descriptor which is published
	// in Dynamical Vision, WDW 2005/2006 Proceedings 2006
	else if(RotateIndex == 2)
	{

		tempLBPpreT = RotLBP(tempLBPpre,NeighborPoints);
		tempLBPcurT = RotLBP(tempLBPcur,NeighborPoints);
		tempLBPposT = RotLBP(tempLBPpos,NeighborPoints);

		temp = int (   tempLBPpreC + \
					  (tempLBPpreT << 1) + \
					  (tempLBPcurT << (NeighborPoints + 1)) + \
					  (tempLBPposT << (NeighborPoints * 2 + 1)) + \
					   tempLBPposC * POW(binCount - 1));
		BasicLBP = temp;
	}
}
