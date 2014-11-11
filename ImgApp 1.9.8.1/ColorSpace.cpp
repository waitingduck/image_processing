#include "stdafx.h"
#include "ColorSpace.h"

#include <cmath>
using namespace std;

double USER_DEFINE_PI = 4.0 * atan(1.0);


void XYZ_RGB(double X, double Y, double Z, BYTE &R, BYTE &G, BYTE &B)
{
	double xyz[3];
	xyz[0] = X / 100.0;
	xyz[1] = Y / 100.0;
	xyz[2] = Z / 100.0;

	double rgb[3];
	rgb[2] = xyz[0] * ( 3.2406) + xyz[1] * (-1.5372) + xyz[2] * (-0.4986);
	rgb[1] = xyz[0] * (-0.9689) + xyz[1] * ( 1.8758) + xyz[2] * ( 0.0415);
	rgb[0] = xyz[0] * ( 0.0557) + xyz[1] * (-0.2040) + xyz[2] * ( 1.0570);

	for(int k = 0; k < 3; k++){
		if(rgb[k] > 0.0031308)
			rgb[k] = 1.055 * pow(rgb[k], 1.0 / 2.4) - 0.055;
		else
			rgb[k] = 12.92 * rgb[k];
		rgb[k] *= 255.0;
	}

	R = (BYTE)(rgb[2] > 255 ? 255 : (rgb[2] < 0 ? 0 : rgb[2]));
	G = (BYTE)(rgb[1] > 255 ? 255 : (rgb[1] < 0 ? 0 : rgb[1]));
	B = (BYTE)(rgb[0] > 255 ? 255 : (rgb[0] < 0 ? 0 : rgb[0]));
}


void RGB_XYZ(BYTE R, BYTE G, BYTE B, double &X, double &Y, double &Z)
{
	double rgb[3];
	rgb[2] = (double)R / 255.0;
	rgb[1] = (double)G / 255.0;
	rgb[0] = (double)B / 255.0;

	for(int k = 0; k < 3; k++){
		if(rgb[k] > 0.04045)
			rgb[k] = pow((rgb[k] + 0.055) / 1.055, 2.4);
		else
			rgb[k] = rgb[k] / 12.92;
		rgb[k] *= 100.0;
	}

	X = rgb[2] * 0.4124 + rgb[1] * 0.3576 + rgb[0] * 0.1805;
	Y = rgb[2] * 0.2126 + rgb[1] * 0.7152 + rgb[0] * 0.0722;
	Z = rgb[2] * 0.0193 + rgb[1] * 0.1192 + rgb[0] * 0.9505;
}


void XYZ_Yxy(double X, double Y1, double Z, double &Y2, double &x, double &y)
{
	Y2 = Y1;
	x = X / (X + Y1 + Z);
	y = Y1 / (X + Y1 + Z);
}


void Yxy_XYZ(double Y1, double x, double y, double &X, double &Y2, double &Z)
{
	X = x * (Y1 / y);
	Y2 = Y1;
	Z = (1.0 - x - y) * (Y1 / y);
}


void XYZ_HunterLab(double X, double Y, double Z, double &L, double &a, double &b)
{
	L = 10.0 * sqrt(Y);
	a = 17.5 * (1.02 * X - Y) / sqrt(Y);
	b = 7.0 * (Y - 0.847 * Z) / sqrt(Y);
}


void HunterLab_XYZ(double L, double a, double b, double &X, double &Y, double &Z)
{
	Y = pow(L / 10.0, 2.0);
	X = (a / 17.5 * L / 10.0 + Y) / 1.02;
	Z = -(b / 7.0 * L / 10.0 - Y) / 0.847;
}


void XYZ_Lab(double X, double Y, double Z, double &L, double &a, double &b)
{
	double xyz[3];
	xyz[0] = X / REF_X_VALUE;
	xyz[1] = Y / REF_Y_VALUE;
	xyz[2] = Z / REF_Z_VALUE;

	for(int k = 0; k < 3; k++){
		if(xyz[k] > 0.008856)
			xyz[k] = pow(xyz[k], 1.0 / 3.0);
		else
			xyz[k] = 7.787 * xyz[k] + 16.0 / 116.0;
	}

	L = 116.0 * xyz[1] - 16.0;
	a = 500.0 * (xyz[0] - xyz[1]);
	b = 200.0 * (xyz[1] - xyz[2]);
}


void Lab_XYZ(double L, double a, double b, double &X, double &Y, double &Z)
{
	double xyz[3];
	xyz[1] = (L + 16.0) / 116.0;
	xyz[0] = a / 500.0 + xyz[1];
	xyz[2] = xyz[1] - b / 200.0;

	for(int k = 0; k < 3; k++){
		if(pow(xyz[k], 3.0) > 0.008856) // ~ 改 ~
			xyz[k] = pow(xyz[k], 3.0);
		else
			//xyz[k] = (xyz[k] - 16.0) / 116.0 / 7.787;
			xyz[k] = (xyz[k] - 16.0 / 116.0) / 7.787; // ~ 改 ~
	}

/*
	for(int k = 0; k < 3; k++){
		if(xyz[k] > 0.008856) // ~ 改 ~
			xyz[k] = pow(xyz[k], 3.0);
		else
			//xyz[k] = (xyz[k] - 16.0) / 116.0 / 7.787;
			xyz[k] = (xyz[k] - 16.0 / 116.0) / 7.787; // ~ 改 ~
	}
*/

	X = xyz[0] * REF_X_VALUE;
	Y = xyz[1] * REF_Y_VALUE;
	Z = xyz[2] * REF_Z_VALUE;
}


void Lab_LCH(double L1, double a, double b, double &L2, double &C, double &H)
{
	L2 = L1;
	C = sqrt(a * a + b * b);
	H = atan(b / a) * 180.0 / USER_DEFINE_PI;
	if(H < 0) H = 360.0 + H;
}


void LCH_Lab(double L1, double C, double H, double &L2, double &a, double &b)
{
	L2 = L1;
	double radian = H * USER_DEFINE_PI / 180.0;
	a = cos(radian) * C;
	b = sin(radian) * C;
}


void XYZ_Luv(double X, double Y, double Z, double &L, double &u, double &v)
{
	double varU = (4.0 * X) / (X + (15.0 * Y) + (3.0 * Z));
	double varV = (9.0 * Y) / (X + (15.0 * Y) + (3.0 * Z));

	double varY = Y / 100.0;
	if(varY > 0.008856) varY = pow(varY, 1.0 / 3.0);
	else varY = (7.787 * varY) + (16.0 / 116.0);

	L = (116.0 * varY ) - 16.0;
	u = 13.0 * L * (varU - REF_U_VALUE);
	v = 13.0 * L * (varV - REF_V_VALUE);
}


void Luv_XYZ(double L, double u, double v, double &X, double &Y, double &Z)
{
	Y = (L + 16.0) / 116.0;
	if(pow(Y, 3.0) > 0.008856) Y = pow(Y, 3.0);
	else Y = (Y - 16.0 / 116.0) / 7.787;

	double varU = u / (13.0 * L) + REF_U_VALUE;
	double varV = v / (13.0 * L) + REF_V_VALUE;

	Y = Y * 100.0;
	X = -(9.0 * Y * varU) / ((varU - 4.0) * varV - varU * varV);
	Z = (9.0 * Y - (15.0 * varV * Y) - (varV * X)) / (3.0 * varV);
}


double Hue_2_RGB(double v1, double v2, double vH)
{
	if(vH < 0) vH += 1.0;
	if(vH > 1) vH -= 1.0;
	if(vH < 1.0 / 6.0) return (v1 + (v2 - v1) * 6.0 * vH);
	if(vH < 3.0 / 6.0) return (v2);
	if(vH < 4.0 / 6.0) return (v1 + (v2 - v1) * ((4.0 / 6.0) - vH) * 6.0);
	return (v1);
}


void RGB_HSL(BYTE R, BYTE G, BYTE B, double &H, double &S, double &L)
{
	double rgb[3];
	rgb[2] = (double)R / 255.0;
	rgb[1] = (double)G / 255.0;
	rgb[0] = (double)B / 255.0;

	double Max = rgb[0], Min = rgb[0];
	int index = 0;
	for(int k = 1; k < 3; k++){
		if(rgb[k] > Max){
			Max = rgb[k];
			index = k;
		}
		if(rgb[k] < Min) Min = rgb[k];
	}
	double Delta = Max - Min;

	L = (Max + Min) / 2.0;

	if(Delta == 0.0){
		H = S = 0.0;
	}
	else{
		if(L < 0.5) S = Delta / (Max + Min);
		else S = Delta / (2.0 - Max - Min);

		for(int k = 1; k < 3; k++)
			rgb[k] = ((Max - rgb[k]) / 6.0 + Delta / 2.0) / Delta;

		if(index == 2) H = rgb[0] - rgb[1];
		if(index == 1) H = rgb[2] - rgb[0] + 1.0 / 3.0;
		if(index == 0) H = rgb[1] - rgb[2] + 2.0 / 3.0;

		if(H < 0.0) H += 1.0;
		if(H > 1.0) H -= 1.0;
	}
}


void HSL_RGB(double H, double S, double L, BYTE &R, BYTE &G, BYTE &B)
{
	if(S == 0.0){
		double gray = L * 255.0;
		R = G = B = (BYTE)(gray > 255 ? 255 : (gray < 0 ? 0 : gray));
	}
	else{
		double v1, v2;
		
		if(L < 0.5) v2 = L * (1.0 + S);
		else v2 = (L + S) - (S * L);

		v1 = 2.0 * L - v2;

		double hue;

		hue = 255.0 * Hue_2_RGB(v1, v2, H + 1.0 / 3.0);
		R = (BYTE)(hue > 255 ? 255 : (hue < 0 ? 0 : hue));
		hue = 255.0 * Hue_2_RGB(v1, v2, H);
		G = (BYTE)(hue > 255 ? 255 : (hue < 0 ? 0 : hue));
		hue = 255.0 * Hue_2_RGB(v1, v2, H - 1.0 / 3.0);
		B = (BYTE)(hue > 255 ? 255 : (hue < 0 ? 0 : hue));
	}
}


void RGB_HSV(BYTE R, BYTE G, BYTE B, double &H, double &S, double &V)
{
	double rgb[3];
	rgb[2] = (double)R / 255.0;
	rgb[1] = (double)G / 255.0;
	rgb[0] = (double)B / 255.0;

	double Max = rgb[0], Min = rgb[0];
	int index = 0;
	for(int k = 1; k < 3; k++){
		if(rgb[k] > Max){
			Max = rgb[k];
			index = k;
		}
		if(rgb[k] < Min) Min = rgb[k];
	}
	double Delta = Max - Min;

	V = Max;

	if(Delta == 0.0){
		H = S = 0.0;
	}
	else{
		S = Delta / Max;

		for(int k = 1; k < 3; k++)
			rgb[k] = ((Max - rgb[k]) / 6.0 + Delta / 2.0) / Delta;

		if(index == 2) H = rgb[0] - rgb[1];
		if(index == 1) H = rgb[2] - rgb[0] + 1.0 / 3.0;
		if(index == 0) H = rgb[1] - rgb[2] + 2.0 / 3.0;

		if(H < 0.0) H += 1.0;
		if(H > 1.0) H -= 1.0;
	}
}


void HSV_RGB(double H, double S, double V, BYTE &R, BYTE &G, BYTE &B)
{
	if(S == 0.0){
		double gray = V * 255.0;
		R = G = B = (BYTE)(gray > 255 ? 255 : (gray < 0 ? 0 : gray));
	}
	else{
		double varH = H * 6.0;
		while(varH < 0 || varH >= 6.0){
			if(varH < 0) varH += 6.0;
			if(varH >= 6.0) varH -= 6.0;
		}

		int varI = (int)varH;

		double var1, var2, var3;
		var1 = V * (1.0 - S);
		var2 = V * (1.0 - S * (varH - (double)varI));
		var3 = V * (1.0 - S * (1.0 - (varH - (double)varI)));

		double rgb[3];

		if		(varI == 0) { rgb[2] = V	; rgb[1] = var3	; rgb[0] = var1	; }
		else if	(varI == 1)	{ rgb[2] = var2	; rgb[1] = V	; rgb[0] = var1	; }
		else if	(varI == 2)	{ rgb[2] = var1	; rgb[1] = V	; rgb[0] = var3	; }
		else if	(varI == 3)	{ rgb[2] = var1	; rgb[1] = var2	; rgb[0] = V	; }
		else if	(varI == 4)	{ rgb[2] = var3	; rgb[1] = var1	; rgb[0] = V	; }
		else				{ rgb[2] = V	; rgb[1] = var1	; rgb[0] = var2	; }

		for(int k = 1; k < 3; k++)
			rgb[k] *= 255.0;

		R = (BYTE)(rgb[2] > 255 ? 255 : (rgb[2] < 0 ? 0 : rgb[2]));
		G = (BYTE)(rgb[1] > 255 ? 255 : (rgb[1] < 0 ? 0 : rgb[1]));
		B = (BYTE)(rgb[0] > 255 ? 255 : (rgb[0] < 0 ? 0 : rgb[0]));
	}
}


void RGB_CMY(BYTE R, BYTE G, BYTE B, double &C, double &M, double &Y)
{
	C = 1.0 - (double)R / 255.0;
	M = 1.0 - (double)G / 255.0;
	Y = 1.0 - (double)B / 255.0;
}


void CMY_RGB(double C, double M, double Y, BYTE &R, BYTE &G, BYTE &B)
{
	R = (BYTE)((1.0 - (C > 1 ? 1.0 : (C < 0 ? 0.0 : C))) * 255.0);
	G = (BYTE)((1.0 - (M > 1 ? 1.0 : (M < 0 ? 0.0 : M))) * 255.0);
	B = (BYTE)((1.0 - (Y > 1 ? 1.0 : (Y < 0 ? 0.0 : Y))) * 255.0);
}


void RGB_YCbCr(BYTE R, BYTE G, BYTE B, BYTE &Y, BYTE &Cb, BYTE &Cr)
{
	Y  =       (BYTE)(0.299000 * (double)R + 0.587000 * (double)G + 0.114000 * (double)B);
	Cb = 128 - (BYTE)(0.168736 * (double)R - 0.331264 * (double)G + 0.500000 * (double)B);
	Cr = 128 + (BYTE)(0.500000 * (double)R - 0.418688 * (double)G - 0.081312 * (double)B);
}


void RGB_Lab(BYTE R, BYTE G, BYTE B, double &L, double &a, double &b)
{
	double X, Y, Z;
	RGB_XYZ(R, G, B, X, Y, Z);
	XYZ_Lab(X, Y, Z, L, a, b);
}


void Lab_RGB(double L, double a, double b, BYTE &R, BYTE &G, BYTE &B)
{
	double X, Y, Z;
	Lab_XYZ(L, a, b, X, Y, Z);
	XYZ_RGB(X, Y, Z, R, G, B);
}