#ifndef _H_FLOATING
#define _H_FLOATING

#include "core_types.h"

/* 50455730 */	extern uint fp_overflow;
/* 50455734 */	extern uint fp_fpuflags;
/* 50455738 */	extern double fp_value;

/* 504525e4 */	extern double fp_scalar0;
/* 504557a8 */	extern double fp_scalar1;
/* 5045577c */	extern double fp_scalar2;
/* 504557dc */	extern double fp_scalarHalf;
/* 50452560 */	extern double fp_scalar10;

/* 50158e65 */	void FP_Set(double *param_1);
/* 501590c7 */	void FP_SetInteger(int param_1);
/* 50158f09 */	void FP_SetZero(void);
/* 50158e7c */	void FP_CopyTo(double *param_1);
/* 50159166 */	int FP_AsInteger(void);

/* 50158e93 */	void FP_Add(double *param_1);
/* 50158f1e */	void FP_Sub(double *param_1);
/* 50158f56 */	void FP_Mul(double *param_1);
/* 50158f94 */	void FP_Div(double *param_1);

/* 50158f16 */	void FP_Abs(void);
/* 50158ef7 */	void FP_Negate(void);
/* 5015487a */	void FP_PowerOfTen(int n);

/* 500511c3 */	int FP_CompareTo(double *param_1);
/* 50158ecb */	int FP_Sign(void);

/* 501592d9 */	void FP_Sin(void);
/* 501592b1 */	void FP_Cos(void);
/* 5015982f */	void FP_Sqrt(void);

#endif
