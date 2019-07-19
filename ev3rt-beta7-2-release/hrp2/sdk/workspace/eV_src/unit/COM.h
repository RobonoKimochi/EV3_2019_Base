#ifndef EV3_UNIT_COM_H_
#define EV3_UNIT_COM_H_

#include "ColorSensor.h"
#include <math.h>

typedef struct{
	int h;
	int s;
	int v;
}Hsv;

double MAX(double a, double b);
double MIN(double a, double b);
double MAX(double a, double b, double c);
double MIN(double a, double b, double c);
double MED(double a, double b, double c);
double DIST(double a, double b);
double DIST2POINT(double a1, double a2,double b1, double b2);
double ABS(double a);
Hsv RGBtoHSV(rgb_raw_t in);


#endif  // EV3_UNIT_COM_H_
