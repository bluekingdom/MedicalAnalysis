#include "defineall.h"

double max1(double a, double b)
{
	return (a + b + fabs(a - b)) / 2;
}

double min1(double a, double b)
{
	return (a + b - fabs(a - b)) / 2;
}

double* G_globalSimilarity = nullptr;
int G_initSim = 0;

