
// Refference: http://www.easyrgb.com/index.php?X=MATH

#ifndef COLOR_SPACE_H
#define COLOR_SPACE_H

// Observer. = 2¢X, Illuminant = D65
// X = 0 ~  95.047
// Y = 0 ~ 100.000
// Z = 0 ~ 108.883

#define REF_X_VALUE  95.047
#define REF_Y_VALUE 100.000
#define REF_Z_VALUE 108.883

// R = 0 ~ 255
// G = 0 ~ 255
// B = 0 ~ 255

void XYZ_RGB(double X, double Y, double Z, BYTE &R, BYTE &G, BYTE &B);
void RGB_XYZ(BYTE R, BYTE G, BYTE B, double &X, double &Y, double &Z);

// Y = 0 ~ 100.0
// x = 0 ~ 1.0
// y = 0 ~ 1.0

void XYZ_Yxy(double X, double Y1, double Z, double &Y2, double &x, double &y);
void Yxy_XYZ(double Y1, double x, double y, double &X, double &Y2, double &Z);

// Hunter-L = 0 ~ 100.0
// Hunter-a = 0 ~ Unknown
// Hunter-b = 0 ~ Unknown

void XYZ_HunterLab(double X, double Y, double Z, double &L, double &a, double &b);
void HunterLab_XYZ(double L, double a, double b, double &X, double &Y, double &Z);

// L = 0 ~ 100.0
// a = 0 ~ Unknown
// b = 0 ~ Unknown

void XYZ_Lab(double X, double Y, double Z, double &L, double &a, double &b);
void Lab_XYZ(double L, double a, double b, double &X, double &Y, double &Z);

// L = 0 ~ 100.0
// C = 0 ~ Unknown
// H = 0 ~ 360¢X

void Lab_LCH(double L1, double a, double b, double &L2, double &C, double &H);
void LCH_Lab(double L1, double C, double H, double &L2, double &a, double &b);

#define REF_U_VALUE 0.197839825
#define REF_V_VALUE 0.468336303

void XYZ_Luv(double X, double Y, double Z, double &L, double &u, double &v);
void Luv_XYZ(double L, double u, double v, double &X, double &Y, double &Z);

// H = 0 ~ 1.0
// S = 0 ~ 1.0
// L = 0 ~ 1.0

double Hue_2_RGB(double v1, double v2, double vH);
void RGB_HSL(BYTE R, BYTE G, BYTE B, double &H, double &S, double &L);
void HSL_RGB(double H, double S, double L, BYTE &R, BYTE &G, BYTE &B);

// H = 0 ~ 1.0
// S = 0 ~ 1.0
// V = 0 ~ 1.0

void RGB_HSV(BYTE R, BYTE G, BYTE B, double &H, double &S, double &V);
void HSV_RGB(double H, double S, double V, BYTE &R, BYTE &G, BYTE &B);

// C = 0 ~ 1.0
// M = 0 ~ 1.0
// Y = 0 ~ 1.0

void RGB_CMY(BYTE R, BYTE G, BYTE B, double &C, double &M, double &Y);
void CMY_RGB(double C, double M, double Y, BYTE &R, BYTE &G, BYTE &B);

// Y  = 0 ~ 255
// Cb = 0 ~ 255
// Cr = 0 ~ 255
void RGB_YCbCr(BYTE R, BYTE G, BYTE B, BYTE &Y, BYTE &Cb, BYTE &Cr);

void RGB_Lab(BYTE R, BYTE G, BYTE B, double &L, double &a, double &b);
void Lab_RGB(double L, double a, double b, BYTE &R, BYTE &G, BYTE &B);

#endif