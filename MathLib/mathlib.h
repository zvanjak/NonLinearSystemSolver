#if !defined  __MATHLIB_H
#define __MATHLIB_H

#include "calc.h"
#include "complex.h"
#include "geometry.h"
#include "polynom.h"
#include "vector.h"
#include "matrix.h"
#include "DiffSystem.h"
#include "SpecFunc.h"
#include "SparseMatrix.h"
#include "Function.h"
#include "VectorFunction.h"
#include "ODEIntegration.h"
#include "RootFinding.h"
#include "Interpolation.h"
#include "Integrate.h"

#define	M_PI	3.141592654589793
#define PI    3.141592653589793
#define EULER	0.5772156649

#define SIGN(a,b) ( (b) >= 0.0 ? fabs(a): -fabs(a) )
#define SIMPLEX_MAX_ITER_NUM	500

static double dsqrarg;
#define DSQR(a) ((dsqrarg=(a)) == 0.0 ? 0.0 : dsqrarg*dsqrarg )

static float sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg )

static double dMaxArg1, dMaxArg2;
#define DMAX(a,b) (dMaxArg1=(a),dMaxArg2=(b),(dMaxArg1)>(dMaxArg2) ? (dMaxArg1) : (dMaxArg2) )

static double dMinArg1, dMinArg2;
#define DMIN(a,b) (dMinArg1=(a),dMinArg2=(b),(dMinArg1)<(dMinArg2) ? (dMinArg1) : (dMinArg2) )

static float maxarg1, maxarg2;
#define FMAX(a,b) (maxarg1=(a), maxarg2=(b), (maxarg1) > (maxarg2) ? (maxarg1) : (maxarg2) )

static float minarg1, minarg2;
#define FMIN(a,b) (minarg1=(a), minarg2=(b), (minarg1) > (minarg2) ? (minarg1) : (minarg2) )

static int nMinArg1, nMinArg2;
#define IMIN(a,b) (nMinArg1=(a),nMinArg2=(b),(nMinArg1)<(nMinArg2) ? (nMinArg1) : (nMinArg2) )

static int iMaxArg1, iMaxArg2;
#define IMAX(a,b) (iMaxArg1=(a), iMaxArg2=(b), (iMaxArg1) > (iMaxArg2) ? (iMaxArg1) : (iMaxArg2) )

#define ALPHA	1
#define BETA	0.5
#define GAMA	2
#define EPS		1e-6
 
void	FuncMinN( Vector StartPoint3D, int nDim, double (*func) (double *), Vector *Min );

void period( double x[], double y[], int n, double ofac, double hifac, double px[],
						double py[], int np, int *nout, int *jmax, double *prob );
void avevar( double data[], unsigned long n, double *ave, double *var );

#define		DIV_ZERO				1
#define		FUNC_UNDEFINED	2
#define		UNDEFINED_ARG		3

/*
#define		SINGULAR_MATRIX
#define		NO_CONVERG
#define		SVD_VECTOR_W_DIM		1
#define		SVD_MATRIX_V_DIM		2
#define		NO_ALG_SOLUTION			

	
#define		INT_OK		0
#define		CONVERGED	
*/

class	MathLibException
{
public:
	MathLibException();
	MathLibException( int nType );
	~MathLibException();

	int		m_nType;
};

#endif