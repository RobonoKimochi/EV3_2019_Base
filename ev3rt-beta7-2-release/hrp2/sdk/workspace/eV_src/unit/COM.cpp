/**********************************************/
/* 共通演算に使用する関数をまとめる           */
/**********************************************/


#include "COM.h"

double MAX(double a, double b)
{
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

double MIN(double a, double b)
{
	if (a > b) {
		return b;
	} else {
		return a;
	}
}

double MAX(double a, double b, double c)
{
	if (a > b) {
		if (a > c) {
			return a;
		} else {
			return c;
		}
	} else {
		if (b > c) {
			return b;
		} else {
			return c;
		}
	}
}

double MIN(double a, double b, double c)
{
	if (a > b) {
		if (b > c) {
			return c;
		} else {
			return b;
		}
	} else {
		if (a > c) {
			return c;
		} else {
			return a;
		}
	}
}

double MED(double a, double b, double c)
{
	if (a > b) {
		if (b > c) {
			return b;
		} else {
			if (a > c) {
				return b;
			} else {
				return a;
			}
		}
	} else {
		if (a > c) {
			return a;
		} else {
			if (b > c) {
				return c;
			} else {
				return b;
			}
		}
	}
}

double ABS(double a)
{
	if (a > 0) {
		return a;
	} else if (a < 0) {
		return -a;
	} else {
		return 0;
	}
}

double DIST(double a, double b)
{
	return (sqrt((a * a) + (b * b)));
}

double DIST2POINT(double a1, double a2,double b1, double b2)
{

	double dista,distb,ret;
	dista = a1 - a2;
	distb = b1 - b2;
	ret = DIST(dista,distb);

	return (ret);
}

Hsv RGBtoHSV(rgb_raw_t in)
{
	double max = (double)MAX(in.r, in.g, in.b);
	double min = (double)MIN(in.r, in.g, in.b);
	double h = 0.0;
	double s = 0.0;
	Hsv hsv = {}; // V(明度)

   if (max != min) {
     // H(色相)
     if (max == in.r) h = 60 * (in.g - in.b) / (max-min);
     if (max == in.g) h = 60 * (in.b - in.r) / (max-min) + 120;
     if (max == in.b) h = 60 * (in.r - in.g) / (max-min) + 240;

     // S(彩度)
     s = ((double)max - (double)min) / (double)max;
   }

   while (h < 0){
     h = h + 360;
   }

   hsv.h =  h;
   hsv.s =  (s * 100);
   hsv.v =  ((max / (double)255) * (double)100);

   return hsv;

}
