#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

#define FREERETURN {delete matJac; delete pdFuncVal; delete pdP; free(indx); }
#define TOLX	1e-8
#define TOLF	1e-8

int			nDim;
void		(*pfHelpFunc)( Vector &x, Vector &f );
Vector	*vecFuncVal;

int			help_nn;
void		(*help_func)( double x[], double f[] );
double	*help_fvec;

/*************************************************************************************/
//							IMPLEMENTACIJA OBICNOG NEWTON-RAPHSONovog POSTUPKA
/*************************************************************************************/

// Nalazenje rjesenja sustava algebarskih jednazdbi

int simple_newton( int nTrialNum, int nDim, double adXStart[], 
									void (*pfVecFunc)(double[], double[]) )
{
	int		i, k, *indx;
	double	errx, errf, d;
	Matrix	*matJac;
	double  *pdFuncVal, *pdP;

	indx = (int *) calloc( nDim, sizeof(int) );
	
	matJac		= new Matrix(nDim, nDim);
	pdFuncVal = new double[nDim];
	pdP				= new double[nDim];

	for( k=0; k<nTrialNum; k++ )
	{
		// izracunaj vrijednosti funkcije za danu tocku 'x'
		(*pfVecFunc)(adXStart, pdFuncVal);
		// izracunaj jakobijan u toj tocki
		findjac(nDim, adXStart, pdFuncVal, *matJac, pfVecFunc );

		// provjeri konvergenciju funkcijskih vrijednosti
 		errf = 0.0;
		for( i=0; i<nDim; i++ )
			errf += fabs(pdFuncVal[i]);
		if( errf <= TOLF )
		{
			FREERETURN;
			return CONVERGED;
		}
		
		// izracunaj desnu stranu sustava jednadzbi za izracunavanje 'dx'
		for( i=0; i<nDim; i++ )
			pdP[i] = -1 * pdFuncVal[i];

		if( LUDecomp(*matJac, indx, &d ) == LU_SING )
		{
			FREERETURN;
			return NO_CONVERG;
		}
		LUBackSubst(*matJac, indx, pdP);

		// provjeri konvergenciju korijena
		errx = 0.0;
		for( i=0; i<nDim; i++ )
		{
			errx += fabs(pdP[i]);
			adXStart[i] = adXStart[i] + pdP[i];				// update rjesenje
		}

		if( errx <= TOLX )
		{
			FREERETURN;
			return CONVERGED;
		}
	}
	FREERETURN;
	return NO_CONVERG;
}

#undef FREERETURN
#define FREERETURN {delete matJac; delete xout; delete yout; delete pdP; free(indx); }

// Nalazenje ravnoteznog rjesenja algebarskih jednadzbi za DAE sistem

int simple_newton( int nTrialNum, int n, int m, double t, double x[], double y[], 
								void (*pfSysFunc)(double, double[], double[], double[], double[]) )
{
	int			i, k, *indx;
	double	errx, errf, d;
	double  *yout, *pdP, *xout;
	Matrix	*matJac;

	indx = (int *) calloc( m, sizeof(int) );
	
	matJac		= new Matrix(m, m);
	yout	 = new double[m];
	pdP				= new double[m];
	xout			= new double[n];

	for( k=0; k<nTrialNum; k++ )
	{
		// izracunaj vrijednosti funkcije za danu tocku 'x'
		(*pfSysFunc)(t, x, y, xout, yout);
		
		// izracunaj jakobijan u toj tocki
		findjac_dae(n, m, t, x, y, yout, *matJac, pfSysFunc );
//		Print(*matJac);

		// provjeri konvergenciju funkcijskih vrijednosti
 		errf = 0.0;
		for( i=0; i<m; i++ )
			errf += fabs(yout[i]);
		if( errf <= TOLF )
		{
			FREERETURN;
			return CONVERGED;
		}
		
		// izracunaj desnu stranu sustava jednadzbi za izracunavanje 'dx'
		for( i=0; i<m; i++ )
			pdP[i] = -1 * yout[i];

		if( LUDecomp(*matJac, indx, &d ) == LU_SING )
		{
			FREERETURN;
			return NO_CONVERG;
		}
		LUBackSubst(*matJac, indx, pdP);

		// provjeri konvergenciju korijena
		errx = 0.0;
		for( i=0; i<m; i++ )
		{
			errx += fabs(pdP[i]);
			y[i] = y[i] + pdP[i];				// update rjesenje
		}

		if( errx <= TOLX )
		{
			FREERETURN;
			return CONVERGED;
		}
	}
	FREERETURN;
	return NO_CONVERG;
}

#undef FREERETURN
#define FREERETURN {delete matJac; delete vecFuncVal; delete vecP; free(indx); }

int SimpleNewton( int nTrialNum, int nDim, Vector *vecX, 
								    void (*pfVecFunc)(Vector&, Vector&) )
{
	int		i, k, *indx;
	double	errx, errf, d;
	Matrix	*matJac;
	Vector	*vecFuncVal, *vecP;

	indx = (int *) calloc( nDim, sizeof(int) );
	
	matJac = new Matrix(nDim, nDim);
	vecP  = new Vector(nDim);
	vecFuncVal = new Vector(nDim);

	for( k=0; k<nTrialNum; k++ )
	{
		(*pfVecFunc)(*vecX, *vecFuncVal);
		FindJacobian(nDim, *vecX, *vecFuncVal, *matJac, pfVecFunc);

 		errf = 0.0;
		for( i=0; i<nDim; i++ )
			errf += fabs((*vecFuncVal)[i]);
		if( errf <= TOLF )
		{
			FREERETURN;
			return CONVERGED;
		}
		
		*vecP = -1 * (*vecFuncVal);

		if( LUDecomp(*matJac, indx, &d ) == LU_SING )
		{
			FREERETURN;
			return NO_CONVERG;
		}
		LUBackSubst(*matJac, indx, *vecP );
		errx = 0.0;

		for( i=0; i<nDim; i++ )
			errx += fabs((*vecP)[i]);

		(*vecX) = (*vecX) + (*vecP);

		if( errx <= TOLX )
		{
			FREERETURN;
			return CONVERGED;
		}
	}
	delete matJac; 
	delete vecFuncVal; 
	delete vecP; 
	free(indx); 

	return NO_CONVERG;
}

int		SimpleNewton( int nTrial, int nN, int nM, double t, Vector &X, Vector &YStart, 
								void (*pfSysFunc)(double, Vector &, Vector &, Vector &, Vector &) )
{
	return 1;
}

/***************************************************************************************/
//												GLOBALNO KONVERGENTA NEWTONOVA METODA
/***************************************************************************************/

#define	ALF		1.0e-4
#define TOLX	1.0e-7

void line_search( int n, double xold[], double fold, double grad[], double p[], double x[], 
							double *f, double stpmax, int *check, double (*func)(double []) )
{
	int			i;
	double	a, alam, alam2, alamin, b, disc, f2, fold2, rhs1, rhs2, slope;
	double	sum, temp, test, tmplam;

	*check = 0;
	for( sum=0.0, i=1; i<=n; i++ )
		sum += p[i-1] * p[i-1];
	sum = sqrt(sum);
	if( sum > stpmax )
		for( i=1; i<=n; i++ )
			p[i-1] *= stpmax / sum;

	for( slope=0.0, i=1; i<=n; i++ )
		slope += grad[i-1] * p[i-1];

	test = 0.0;

	for( i=1; i<=n; i++ )
	{
		temp = fabs(p[i-1]) / DMAX(fabs(xold[i-1]),1.0);
		if( temp > test )
			test = temp;
	}
	alamin = TOLX / test;
	alam = 1.0;
	for( ; ; )
	{
		for( i=1; i<=n; i++ )
			x[i-1] = xold[i-1] + alam*p[i-1];
		*f = (*func)(x);

		if( alam < alamin )
		{
			for( i=1; i<=n; i++ )
				x[i-1] = xold[i-1];
			*check = 1;
			return;
		}
		else if( *f <= fold+ALF*alam*slope) return;
		else
		{
			if( alam == 1.0 )
				tmplam = -slope / (2.0*(*f - fold - slope));
			else
			{
				rhs1 = *f - fold - alam*slope;
				rhs2 = f2 - fold - alam2*slope;

				a = (rhs1/(alam*alam) - rhs2/(alam2*alam2))/(alam - alam2);
				b = (-alam2*rhs1 / (alam*alam) + alam*rhs2/(alam2*alam2))/(alam - alam2);

				if( a == 0.0 )
					tmplam = -slope/(2.0*b);
				else
				{
					disc = b*b - 3.0*a*slope;
					if( disc < 0.0 )
					{
						printf("Roundoff problem in lnsrch !!!");
						assert(0);
					}
					else
						tmplam = (-b+sqrt(disc)) / (3.0*a);
				}
				if( tmplam > 0.5*alam)
					tmplam = 0.5*alam;
			}
		}
		alam2 = alam;
		f2 = *f;
		fold2 = fold;
		alam = DMAX(tmplam, 0.1*alam);
	}
}

#define MAXITS		200
#define	TOLF			1.0E-4
#define	TOLMIN		1.0E-6
#define	TOLX			1.0E-7
#define	STPMX			100.0


int global_newton( double x[], int n, void (*vecfunc)(double[], double[]) )
{
	int			i, its, j, *indx, check;
	double	d, den, f, fold, stpmax, sum, temp, test;
	Matrix	*fjac;
	double	*g, *p, *xold;

	indx = new int[n];
	fjac = new Matrix(n,n);
	g = new double[n];
	p = new double[n];
	xold = new double[n];
	help_fvec = new double[n];

	help_nn = n;
	help_func = vecfunc;

	f = fmin(x);
	test = 0.0;
	for( i=1; i<=n; i++ )
		if( fabs(help_fvec[i-1]) > test )
			test = fabs(help_fvec[i-1]);

	if( test< 0.01*TOLF )
	{
		check = CONVERGED;
		delete help_fvec; delete xold; delete p; delete g;
		delete fjac; delete []indx; 
		
		return check;
	}
	for( sum=0.0, i=1; i<=n; i++ )
		sum += DSQR(x[i-1]);
	stpmax = STPMX * DMAX(sqrt(sum),(double)n);

	for( its=1; its<=MAXITS; its++ )
	{
		findjac(n,x,help_fvec,*fjac,vecfunc);
		for( i=1; i<=n; i++ )
		{
			for( sum=0.0, j=1; j<=n; j++ )
				sum += (*fjac)[j-1][i-1] * help_fvec[j-1];
			g[i-1] = sum;
		}
		for( i=1; i<=n; i++ )
			xold[i-1] = x[i-1];
		fold = f;
		for( i=1; i<=n; i++ )
			p[i-1] = -help_fvec[i-1];
		
		if( LUDecomp(*fjac,indx,&d) == LU_SING )
		{
			delete help_fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; 
			
			return NO_CONVERG;
		}
		LUBackSubst(*fjac,indx,p);
		line_search(n,xold,fold,g,p,x,&f,stpmax,&check,fmin);

		test = 0.0;
		for( i=1; i<=n; i++ )
			if( fabs(help_fvec[i-1]) > test )
				test = fabs(help_fvec[i-1]);

		if( test < TOLF )
		{
			check = CONVERGED;
			
			delete help_fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; 
			
			return check;
		}
		if( check == NO_CONVERG )
		{
			test = 0.0;
			den = DMAX(f, 0.5*n);
			for( i=1; i<=n; i++ )
			{
				temp = fabs(g[i-1]) * DMAX(fabs(x[i-1]), 1.0) / den;
				if( temp > test )
					test = temp;
			}
			check = (test < TOLMIN ? NO_CONVERG : CONVERGED );
			delete help_fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; 
			
			return check;
		}
		test = 0.0;
		for( i=1; i<=n; i++ )
		{
			temp = (fabs(x[i-1] - xold[i-1])) / DMAX(fabs(x[i-1]), 1.0);
			if( temp > test )
				test = temp;
		}
		if( test < TOLX )
		{
			delete help_fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; 
			
			return check;
		}
	}
	printf("MAXITS exceeded in newt ");
	
	return NO_CONVERG;
}

double	fmin( double x[] )
{
	int			i;
	double	sum;
	
	(*help_func)(x,help_fvec);
	for( sum=0.0, i=1; i<=help_nn; i++ )
		sum += DSQR(help_fvec[i-1]);

	return 0.5*sum;
}

/******************************************************************************************/

void LineSearch( int n, Vector &xold, double fold, Vector &g, Vector &p, Vector &x, 
						double *f, double stpmax, int *check, double (*func)(Vector&) )
{
	int			i;
	double	a, alam, alam2, alamin, b, disc, f2, fold2, rhs1, rhs2, slope;
	double	sum, temp, test, tmplam;

	*check = 0;
	for( sum=0.0, i=1; i<=n; i++ )
		sum += p[i-1] * p[i-1];
	sum = sqrt(sum);
	if( sum > stpmax )
		for( i=1; i<=n; i++ )
			p[i-1] *= stpmax / sum;

	for( slope=0.0, i=1; i<=n; i++ )
		slope += g[i-1] * p[i-1];

	test = 0.0;

	for( i=1; i<=n; i++ )
	{
		temp = fabs(p[i-1]) / DMAX(fabs(xold[i-1]),1.0);
		if( temp > test )
			test = temp;
	}
	alamin = TOLX / test;
	alam = 1.0;
	for( ; ; )
	{
		for( i=1; i<=n; i++ )
			x[i-1] = xold[i-1] + alam*p[i-1];
		*f = (*func)(x);

		if( alam < alamin )
		{
			for( i=1; i<=n; i++ )
				x[i-1] = xold[i-1];
			*check = 1;
			return;
		}
		else if( *f <= fold+ALF*alam*slope) return;
		else
		{
			if( alam == 1.0 )
				tmplam = -slope / (2.0*(*f - fold - slope));
			else
			{
				rhs1 = *f - fold - alam*slope;
				rhs2 = f2 - fold - alam2*slope;

				a = (rhs1/(alam*alam) - rhs2/(alam2*alam2))/(alam - alam2);
				b = (-alam2*rhs1 / (alam*alam) + alam*rhs2/(alam2*alam2))/(alam - alam2);

				if( a == 0.0 )
					tmplam = -slope/(2.0*b);
				else
				{
					disc = b*b - 3.0*a*slope;
					if( disc < 0.0 )
					{
						printf("Roundoff problem in lnsrch !!!");
						assert(0);
					}
					else
						tmplam = (-b+sqrt(disc)) / (3.0*a);
				}
				if( tmplam > 0.5*alam)
					tmplam = 0.5*alam;
			}
		}
		alam2 = alam;
		f2 = *f;
		fold2 = fold;
		alam = DMAX(tmplam, 0.1*alam);
	}
}

#define MAXITS		200
#define	TOLF			1.0E-4
#define	TOLMIN		1.0E-6
#define	TOLX			1.0E-7
#define	STPMX			100.0


int GlobalNewton( Vector &x, int n, void (*vecfunc)(Vector&, Vector&) )
{
	int			i, its, j, *indx, check;
	double	d, den, f, fold, stpmax, sum, temp, test;
	Matrix	*fjac;
	Vector	*g, *p, *xold;

	indx = new int[n];
	fjac = new Matrix(n,n);
	g = new Vector(n);
	p = new Vector(n);
	xold = new Vector(n);
	vecFuncVal = new Vector(n);

	nDim = n;
	pfHelpFunc = vecfunc;

	f = FMin(x);
	test = 0.0;
	for( i=1; i<=n; i++ )
		if( fabs((*vecFuncVal)[i-1]) > test )
			test = fabs((*vecFuncVal)[i-1]);

	if( test< 0.01*TOLF )
	{
		check = CONVERGED;
		
		delete vecFuncVal; delete xold; delete p; delete g;
		delete fjac; delete []indx; 
		
		return check;
	}
	for( sum=0.0, i=1; i<=n; i++ )
		sum += DSQR(x[i-1]);
	stpmax = STPMX * DMAX(sqrt(sum),(double)n);

	for( its=1; its<=MAXITS; its++ )
	{
		FindJacobian(n,x,*vecFuncVal,*fjac,vecfunc);
		for( i=1; i<=n; i++ )
		{
			for( sum=0.0, j=1; j<=n; j++ )
				sum += (*fjac)[j-1][i-1] * (*vecFuncVal)[j-1];
			(*g)[i-1] = sum;
		}
		for( i=1; i<=n; i++ )
			(*xold)[i-1] = x[i-1];
		fold = f;
		for( i=1; i<=n; i++ )
			(*p)[i-1] = -(*vecFuncVal)[i-1];
		
		if( LUDecomp(*fjac,indx,&d) == LU_SING )
		{
			delete vecFuncVal; delete xold; delete p; delete g;
			delete fjac; delete []indx; 
			
			return NO_CONVERG;
		}
		LUBackSubst(*fjac,indx,*p);
		LineSearch(n,*xold,fold,*g,*p,x,&f,stpmax,&check,FMin);

		test = 0.0;
		for( i=1; i<=n; i++ )
			if( fabs((*vecFuncVal)[i-1]) > test )
				test = fabs((*vecFuncVal)[i-1]);

		if( test < TOLF )
		{
			check = CONVERGED;
			
			delete vecFuncVal; delete xold; delete p; delete g;
			delete fjac; delete []indx; 
			
			return check;
		}
		if( check == NO_CONVERG )
		{
			test = 0.0;
			den = DMAX(f, 0.5*n);
			for( i=1; i<=n; i++ )
			{
				temp = fabs((*g)[i-1]) * DMAX(fabs(x[i-1]), 1.0) / den;
				if( temp > test )
					test = temp;
			}
			check = (test < TOLMIN ? NO_CONVERG : CONVERGED );
			delete vecFuncVal; delete xold; delete p; delete g;
			delete fjac; delete []indx; 
			
			return check;
		}
		test = 0.0;
		for( i=1; i<=n; i++ )
		{
			temp = (fabs(x[i-1] - (*xold)[i-1])) / DMAX(fabs(x[i-1]), 1.0);
			if( temp > test )
				test = temp;
		}
		if( test < TOLX )
		{
			delete vecFuncVal; delete xold; delete p; delete g;
			delete fjac; delete []indx; 
			
			return check;
		}
	}
	printf("MAXITS exceeded in newt ");

	return NO_CONVERG;
}

double	FMin( Vector &x )
{
	int			i;
	double	sum;
	
	(*pfHelpFunc)(x,*vecFuncVal);
	for( sum=0.0, i=0; i<nDim; i++ )
		sum += DSQR((*vecFuncVal)[i]);

	return 0.5*sum;
}

/*****************************************************************************/
//													BROYDEN-OVA METODA
/*****************************************************************************/

#define MAXITS		200
#define	TOLF			1.0E-4
#define	TOLMIN		1.0E-6
#define	TOLX			EPS
#define	STPMX			100.0

int	broydn( double x[], int n, void (*vecfunc)(double[], double[]) )
{
	int			i, its, j, k, restrt, sing, skip, check;
	double	den, f, fold, stpmax, sum, temp, test;
	Vector	*c, *d, *fvcold, *g, *p, *s, *t, *w, *xold;
	Matrix	*qt, *r;
	
	c = new Vector(n);
	d = new Vector(n);
	g = new Vector(n);
	p = new Vector(n);
	s = new Vector(n);
	t = new Vector(n);
	w = new Vector(n);
	xold = new Vector(n);
	help_fvec = new double[n];
	fvcold = new Vector(n);
	qt = new Matrix(n,n);
	r = new Matrix(n,n);

	help_nn = n;
	help_func = vecfunc;
	f = fmin(x);
	test = 0.0;
	for( i=1; i<=n; i++ )
		if( fabs(help_fvec[i-1]) > test) 
			test = fabs(help_fvec[i-1]);
	
	if( test < 0.01*TOLF )
	{
		check = CONVERGED;
		
		delete help_fvec; delete xold; delete w; delete t; delete s; delete r;	
		delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
		
		return check;
	}

	for( sum=0.0, i=1; i<=n; i++ )
		sum += DSQR(x[i-1]);
	stpmax = STPMX * DMAX(sqrt(sum), (double)n);
	restrt = 1;

	for( its=1; its<=MAXITS; its++)
	{
		if( restrt)
		{
			findjac(n,x,help_fvec,*r,vecfunc);
			QRDecomp(*r,*c,*d,&sing);
			if( sing )
			{
				printf("singular Jac in broydn");
				
				delete help_fvec; delete xold; delete w; delete t; delete s; delete r;	
				delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
				
				return NO_CONVERG;
			}

			for( i=1; i<=n; i++ )
			{
				for( j=1; j<=n; j++ )
					(*qt)[i-1][j-1] = 0.0;
				(*qt)[i-1][i-1] = 1.0;
			}
			for( k=1; k<n; k++ )
			{
				if( (*c)[k-1] )
				{
					for( j=1; j<=n; j++ )
					{
						sum = 0.0;
						for( i=k; i<=n; i++ )
							sum += (*r)[i-1][k-1] * (*qt)[i-1][j-1];
						sum /= (*c)[k-1];
						for( i=k; i<=n; i++ )
							(*qt)[i-1][j-1] -= sum * (*r)[i-1][k-1];
					}
				}
			}
			for( i=1; i<=n; i++ )
			{
				(*r)[i-1][i-1] = (*d)[i-1];
				for( j=1; j<i; j++ )
					(*r)[i-1][j-1] = 0.0;
			}
		}
		else
		{
			for( i=1; i<=n; i++ )
				(*s)[i-1] = x[i-1] - (*xold)[i-1];
			for( i=1; i<=n; i++ )
			{
				for( sum=0.0, j=i; j<=n; j++ )
					sum += (*r)[i-1][j-1] * (*s)[j-1];
				(*t)[i-1] = sum;
			}
			skip = 1;
			for( i=1; i<=n; i++ )
			{
				for( sum=0.0, j=1; j<=n; j++ )
					sum += (*qt)[j-1][i-1] * (*t)[j-1];
				(*w)[i-1] = help_fvec[i-1] - (*fvcold)[i-1] - sum;

				if( fabs((*w)[i-1]) >= EPS*(fabs(help_fvec[i-1])+fabs((*fvcold)[i-1]))) skip=0;
				else
					(*w)[i-1] = 0.0;
			}
			if( !skip )
			{
				for( i=1; i<=n; i++ )
				{
					for( sum=0.0, j=1; j<=n; j++ )
						sum += (*qt)[i-1][j-1] * (*w)[j-1];
					(*t)[i-1] = sum;
				}
				for( den=0.0, i=1; i<=n; i++ )
					den += DSQR((*s)[i-1]);
				for( i=1; i<=n; i++ )
					(*s)[i-1] /= den;
				QRUpdate(*r,*qt,*t,*s);
				for( i=1; i<=n; i++ )
				{
					if( (*r)[i-1][i-1] == 0.0 )
					{
						printf("r singular in broydn");

						delete help_fvec; delete xold; delete w; delete t; delete s; delete r;	
						delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
						
						return NO_CONVERG;
					}
					(*d)[i-1] = (*r)[i-1][i-1];
				}
			}
		}
		for( i=1; i<=n; i++ )
		{
			for( sum=0.0, j=1; j<=n; j++ )
				sum += (*qt)[i-1][j-1] * help_fvec[j-1];
			(*g)[i-1] = sum;
		}
		for( i=n; i>=1; i-- )
		{
			for( sum=0.0, j=1; j<=i; j++ )
				sum += (*r)[j-1][i-1] * (*g)[j-1];
			(*g)[i-1] = sum;
		}
		for( i=1; i<=n; i++ )
		{
			(*xold)[i-1] = x[i-1];
			(*fvcold)[i-1] = help_fvec[i-1];
		}
		fold = f;
		for( i=1; i<=n; i++ )
		{
			for( sum=0.0, j=1; j<=n; j++ )
				sum += (*qt)[i-1][j-1] * help_fvec[j-1];
			(*p)[i-1] = -sum;
		}

		RSolve(*r,*d,*p);
		line_search(n,xold->GetPtr(),fold,g->GetPtr(),p->GetPtr(),x,&f,stpmax,&check,fmin);

		test = 0.0;
		for( i=1; i<=n; i++ )
			if( fabs(help_fvec[i-1]) > test )
				test = fabs(help_fvec[i-1]);
		if( test < TOLF )
		{
			check = CONVERGED;
			
			delete help_fvec; delete xold; delete w; delete t; delete s; delete r;	
			delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
			
			return check;
		}
		if( check == NO_CONVERG )
		{
			if( restrt)
			{
				delete help_fvec; delete xold; delete w; delete t; delete s; delete r;	
				delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
			
				return check;
			}
			else
			{
				test = 0.0;
				den = DMAX(f, 0.5*n);
				for( i=1; i<=n; i++ )
				{
					temp = fabs((*g)[i-1]) * DMAX(fabs(x[i-1]), 1.0) / den;
					if( temp > test )
						test = temp;
				}
				if( test < TOLMIN )
				{
					delete help_fvec; delete xold; delete w; delete t; delete s; delete r;	
					delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
				
					return check;
				}
				else
					restrt = 1;
			}
		}
		else
		{
			restrt = 0;
			test = 0.0;
			for( i=1; i<=n; i++ )
			{
				temp = ( fabs(x[i-1] - (*xold)[i-1])) / DMAX(fabs(x[i-1]), 1.0);
				if( temp > test )
					test = temp;
			}
			if( test < TOLX )
			{
				delete help_fvec; delete xold; delete w; delete t; delete s; delete r;	
				delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
				
				return check;
			}
		}
	}
	printf("MAXITS exceeded in broydn");
	
	delete help_fvec; delete xold; delete w; delete t; delete s; delete r;	
	delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
//	assert(0);
	
	return check;
}

int	Broydn( Vector &x, int n, void (*vecfunc)(Vector&, Vector& ) )
{
	int			i, its, j, k, restrt, sing, skip, check;
	double	den, f, fold, stpmax, sum, temp, test;
	Vector	*c, *d, *fvcold, *g, *p, *s, *t, *w, *xold;
	Matrix	*qt, *r;
	
	c = new Vector(n);
	d = new Vector(n);
	g = new Vector(n);
	p = new Vector(n);
	s = new Vector(n);
	t = new Vector(n);
	w = new Vector(n);
	xold = new Vector(n);
	vecFuncVal = new Vector(n);
	fvcold = new Vector(n);
	qt = new Matrix(n,n);
	r = new Matrix(n,n);

	nDim = n;
	pfHelpFunc = vecfunc;
	f = FMin(x);
	test = 0.0;
	for( i=1; i<=n; i++ )
		if( fabs((*vecFuncVal)[i-1]) > test) 
			test = fabs((*vecFuncVal)[i-1]);
	
	if( test < 0.01*TOLF )
	{
		check = CONVERGED;
		
		delete vecFuncVal; delete xold; delete w; delete t; delete s; delete r;	
		delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
		
		return check;
	}

	for( sum=0.0, i=1; i<=n; i++ )
		sum += DSQR(x[i-1]);
	stpmax = STPMX * DMAX(sqrt(sum), (double)n);
	restrt = 1;

	for( its=1; its<=MAXITS; its++)
	{
		if( restrt )
		{
			FindJacobian(n,x,*vecFuncVal,*r,vecfunc);
			QRDecomp(*r,*c,*d,&sing);
			if( sing )
			{
				printf("singular Jac in broydn");
				delete vecFuncVal; delete xold; delete w; delete t; delete s; delete r;	
				delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
				
				return NO_CONVERG;
			}

			for( i=1; i<=n; i++ )
			{
				for( j=1; j<=n; j++ )
					(*qt)[i-1][j-1] = 0.0;
				(*qt)[i-1][i-1] = 1.0;
			}
			for( k=1; k<n; k++ )
			{
				if( (*c)[k-1] )
				{
					for( j=1; j<=n; j++ )
					{
						sum = 0.0;
						for( i=k; i<=n; i++ )
							sum += (*r)[i-1][k-1] * (*qt)[i-1][j-1];
						sum /= (*c)[k-1];
						for( i=k; i<=n; i++ )
							(*qt)[i-1][j-1] -= sum * (*r)[i-1][k-1];
					}
				}
			}
			for( i=1; i<=n; i++ )
			{
				(*r)[i-1][i-1] = (*d)[i-1];
				for( j=1; j<i; j++ )
					(*r)[i-1][j-1] = 0.0;
			}
		}
		else
		{
			for( i=1; i<=n; i++ )
				(*s)[i-1] = x[i-1] - (*xold)[i-1];
			for( i=1; i<=n; i++ )
			{
				for( sum=0.0, j=i; j<=n; j++ )
					sum += (*r)[i-1][j-1] * (*s)[j-1];
				(*t)[i-1] = sum;
			}
			skip = 1;
			for( i=1; i<=n; i++ )
			{
				for( sum=0.0, j=1; j<=n; j++ )
					sum += (*qt)[j-1][i-1] * (*t)[j-1];
				(*w)[i-1] = (*vecFuncVal)[i-1] - (*fvcold)[i-1] - sum;

				if( fabs((*w)[i-1]) >= EPS*(fabs((*vecFuncVal)[i-1])+fabs((*fvcold)[i-1]))) skip=0;
				else
					(*w)[i-1] = 0.0;
			}
			if( !skip )
			{
				for( i=1; i<=n; i++ )
				{
					for( sum=0.0, j=1; j<=n; j++ )
						sum += (*qt)[i-1][j-1] * (*w)[j-1];
					(*t)[i-1] = sum;
				}
				for( den=0.0, i=1; i<=n; i++ )
					den += DSQR((*s)[i-1]);
				for( i=1; i<=n; i++ )
					(*s)[i-1] /= den;
				QRUpdate(*r,*qt,*t,*s);
				for( i=1; i<=n; i++ )
				{
					if( (*r)[i-1][i-1] == 0.0 )
					{
						printf("r singular in broydn");
						delete vecFuncVal; delete xold; delete w; delete t; delete s; delete r;	
						delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
						
						return NO_CONVERG;
					}
					(*d)[i-1] = (*r)[i-1][i-1];
				}
			}
		}
		for( i=1; i<=n; i++ )
		{
			for( sum=0.0, j=1; j<=n; j++ )
				sum += (*qt)[i-1][j-1] * (*vecFuncVal)[j-1];
			(*g)[i-1] = sum;
		}
		for( i=n; i>=1; i-- )
		{
			for( sum=0.0, j=1; j<=i; j++ )
				sum += (*r)[j-1][i-1] * (*g)[j-1];
			(*g)[i-1] = sum;
		}
		for( i=1; i<=n; i++ )
		{
			(*xold)[i-1] = x[i-1];
			(*fvcold)[i-1] = (*vecFuncVal)[i-1];
		}
		fold = f;
		for( i=1; i<=n; i++ )
		{
			for( sum=0.0, j=1; j<=n; j++ )
				sum += (*qt)[i-1][j-1] * (*vecFuncVal)[j-1];
			(*p)[i-1] = -sum;
		}

		RSolve(*r,*d,*p);
		LineSearch(n,*xold,fold,*g,*p,x,&f,stpmax,&check,FMin);

		test = 0.0;
		for( i=1; i<=n; i++ )
			if( fabs((*vecFuncVal)[i-1]) > test )
				test = fabs((*vecFuncVal)[i-1]);
		if( test < TOLF )
		{
			check = CONVERGED;
			
			delete vecFuncVal; delete xold; delete w; delete t; delete s; delete r;	
			delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
			
			return check; 
		}
		if( check == NO_CONVERG )
		{
			if( restrt)
			{
				delete vecFuncVal; delete xold; delete w; delete t; delete s; delete r;	
				delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
			
				return check;
			}
			else
			{
				test = 0.0;
				den = DMAX(f, 0.5*n);
				for( i=1; i<=n; i++ )
				{
					temp = fabs((*g)[i-1]) * DMAX(fabs(x[i-1]), 1.0) / den;
					if( temp > test )
						test = temp;
				}
				if( test < TOLMIN )
				{
					delete vecFuncVal; delete xold; delete w; delete t; delete s; delete r;	
					delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
				
					return check;
				}
				else
					restrt = 1;
			}
		}
		else
		{
			restrt = 0;
			test = 0.0;
			for( i=1; i<=n; i++ )
			{
				temp = ( fabs(x[i-1] - (*xold)[i-1])) / DMAX(fabs(x[i-1]), 1.0);
				if( temp > test )
					test = temp;
			}
			if( test < TOLX )
			{
				delete vecFuncVal; delete xold; delete w; delete t; delete s; delete r;	
				delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
				
				return check;
			}
		}
	}
	printf("MAXITS exceeded in broydn");
	
	delete vecFuncVal; delete xold; delete w; delete t; delete s; delete r;	
	delete qt; delete p; delete g; delete fvcold; delete d; delete c; 
//	assert(0);
	
	return NO_CONVERG;
}

/***********************************************************************************/
//														RACUNANJE JAKOBIANA
/***********************************************************************************/

void findjac( int nDim, double adX[], double adFuncVal[], Matrix &Jacobian, 
				  void (*pfVecFunc)(double[], double[]) )
{
	int		i, j;
	double	h, temp;
	double	*f;

	f = new double[nDim];

	for( j=0; j<nDim; j++ )
	{
		temp = adX[j];
		h = 1e-5 * fabs(temp);
		if( h == 0.0 )
			h = 1e-5;

		adX[j] = temp + h;
		h = adX[j] - temp;

		(*pfVecFunc)(adX,f);
		adX[j] = temp;

		for( i=0; i<nDim; i++ )
			Jacobian.SetElem(i,j, (f[i] - adFuncVal[i]) / h );
	}

	free(f);
}

void findjac_dae( int nN, int nM, double t, double adX[], double adY[], double adFuncVal[], Matrix &Jacobian, 
				  void (*pfVecFunc)(double, double[], double[], double[], double[]) )
{
	int		i, j;
	double	h, temp;
	double	*f, *xout;

	f = new double[nM];
	xout = new double[nN];

	for( j=0; j<nM; j++ )
	{
		temp = adY[j];
		h = 1e-5 * fabs(temp);
		if( h == 0.0 )
			h = 1e-5;

		adY[j] = temp + h;
		h = adY[j] - temp;

		(*pfVecFunc)(t, adX, adY, xout, f);
		adY[j] = temp;

		for( i=0; i<nM; i++ )
			Jacobian.SetElem(i,j, (f[i] - adFuncVal[i]) / h );
	}

	free(f);
}

void FindJacobian( int nDim, Vector &vectX, Vector &vectFuncVal, Matrix &Jacobian, 
										void (*pfVecFunc)(Vector&, Vector&) )
{
	int		i, j;
	double	h, temp;
	Vector	*f;

	f = new Vector(nDim);

	for( j=0; j<nDim; j++ )
	{
		temp = vectX[j];
		h = 1e-8 * fabs(temp);
		if( h == 0.0 )
			h = 1e-8;

		vectX[j] = temp + h;
		h = vectX[j] - temp;

		(*pfVecFunc)(vectX,*f);
		vectX[j] = temp;

		for( i=0; i<nDim; i++ )
			Jacobian.SetElem(i,j, ((*f)[i] - vectFuncVal[i]) / h );
	}

	free(f);
}

void	FindJacobian( int nDim, double t, Vector &vecX, Matrix &Jacobian, 
									  void (*pfVecFunc)(double, Vector&, Vector&) )
{
	int		i, j;
	double	h, temp;
	Vector	*f, *vecFuncVal;

	f = new Vector(nDim);
	vecFuncVal = new Vector(nDim);

	// najprije izraèunamo funkcije u traženoj toèci
	(*pfVecFunc)(t,vecX,*vecFuncVal);

	for( j=0; j<nDim; j++ )
	{
		temp = vecX[j];
		h = 1e-8 * fabs(temp);
		if( h == 0.0 )
			h = 1e-8;

		//a zatim se pomaknemo malo dalje i izraèunamo vrijednost funkcije
		vecX[j] = temp + h;
		h = vecX[j] - temp;

		(*pfVecFunc)(t,vecX,*f);
		vecX[j] = temp;

		for( i=0; i<nDim; i++ )
			Jacobian.SetElem(i,j, ((*f)[i] - (*vecFuncVal)[i]) / h );
	}

	delete f;
}

