/**
 * floating.c
 *
 * 64-bit floating point operations used by Hall of Tortured Souls and other parts of Excel 95.
 * 
 * This is not what the original code looks like! The original code was very
 * complex, presumably because it had to do 64-bit floating point operations on
 * 32-bit machines that may not have even had built-in support for floating
 * point operations.
 * 
 * Because this isn't the focus of this project, I've avoided the details of
 * these functions and rewrote them by hand. The results of the rewritten
 * functions may not be 100% accurate to the behaviour of the original
 * executable. (For instance, the fp_overflow flag is never set here.)
 * 
 * If someone is interested in decompiling the original functions, the addresses
 * of each of them are in the header file.
 */

#include "floating.h"

#include <math.h>

uint fp_overflow;
uint fp_fpuflags;
double fp_value;

double fp_scalar0 = 0.0;
double fp_scalar1 = 1.0;
double fp_scalar2 = 2.0;
double fp_scalarHalf = 0.5;
double fp_scalar10 = 10.0;

void FP_Set(double *param_1)
{
	fp_value = *param_1;
}

void FP_SetInteger(int param_1)
{
	fp_value = (double)param_1;
}

void FP_SetZero(void)
{
	fp_value = 0.0;
}

void FP_CopyTo(double *param_1)
{
	*param_1 = fp_value;
}

int FP_AsInteger(void)
{
	return (int)fp_value;
}

void FP_Add(double *param_1)
{
	fp_value += *param_1;
}

void FP_Sub(double *param_1)
{
	fp_value -= *param_1;
}

void FP_Mul(double *param_1)
{
	fp_value *= *param_1;
}

void FP_Div(double *param_1)
{
	fp_value /= *param_1;
}

void FP_Abs(void)
{
	fp_value = fabs(fp_value);
}

void FP_Negate(void)
{
	fp_value = -fp_value;
}

void FP_PowerOfTen(int n)
{
	fp_value = pow(10.0, (double)n);
}

int FP_CompareTo(double *param_1)
{
	if (fp_value == *param_1) 
		return 0;
	if (fp_value < *param_1)
		return -1;
	return 1;
}

/* returns -1 for negative numbers, 1 for positive numbers, and 0 for 0 */
int FP_Sign(void)
{
	if (fp_value == 0.0) 
		return 0;
	if (fp_value < 0.0)
		return -1;
	return 1;
}

void FP_Sin(void)
{
	fp_value = sin(fp_value);
}

void FP_Cos(void)
{
	fp_value = cos(fp_value);
}

void FP_Sqrt(void)
{
	fp_value = sqrt(fp_value);
}
