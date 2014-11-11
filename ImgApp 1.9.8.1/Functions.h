#include <atlimage.h>
#include "MainFrm.h"

int fnCopyCImage(CImage *pDestination, CImage *pSource);
bool fnCheckCImageIs24Bits(CImage *pImage);
int fnFixImage24Bits(CImage *pImage);
bool fnIsEqualImage(CImage *pImageA, CImage *pImageB);
int fnGray(CImage *pImage);
int fnNegative(CImage *pImage);
int fnGetRChannel(CImage *pDestination, CImage *pSource);
int fnGetGChannel(CImage *pDestination, CImage *pSource);
int fnGetBChannel(CImage *pDestination, CImage *pSource);
char *fnCStringToChar(CString str);
void fnSetStatusBar(CString str);
void fnSetStatusBar(CString str, int index);

void DWT_Once(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
void DWT_Two(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
void DWT_Tri(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
void DWT_Four(double** spOriginData, double** spTransData0, double** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);

void DWTi_Once(double **spData, double **spTransData0, double **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius);
double iweight(double x,double y,double sigma ,double num);
void bi_filter(double **spData, double **spTransData0, double **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, double layer, float fRadius);
double varsigma(double **spData, int nHeight_H, int nWidth_H);
void thresholding(double **spData, int nHeight_H, int nWidth_H, double sigma);
void lastthresholding(double **spData, int nHeight_H, int nWidth_H, double sigma);
double getlocalsigma(double *spData,int max);
void fnTest0();
void fnTest1();
void fnTest2();
void fnTest3();
void fnTest4();
void fnTest5();
void fnTest6();
void fnTest7();
void fnTest8();
void fnTest9();