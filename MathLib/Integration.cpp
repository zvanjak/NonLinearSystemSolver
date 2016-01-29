#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	<limits>

#include	"Integrate.h"
#include	"mathlib.h"

template float		trapzd<float> ( float (*)(float), float, float, int );
template double		trapzd<double> ( double (*)(double), double, double, int );
template Complex	trapzd<Complex> ( Complex (*)(Complex), Complex, Complex, int );

template float		qtrap<float> ( float (*)(float), float, float );
template double		qtrap<double> ( double (*)(double), double, double );
template Complex	qtrap<Complex> ( Complex (*)(Complex), Complex, Complex );

template float		qsimp<float> ( float (*)(float), float, float );
template double		qsimp<double> ( double (*)(double), double, double );

template float		qromb<float>( float (*)(float), float,float );
template double		qromb<double>( double (*)(double), double,double );

template float		midpnt<float> ( float (*)(float), float, float, int );
template double		midpnt<double> ( double (*)(double), double, double, int );

template float		qromo<float>( float (*)(float), float, float, float (*)( float(*)(float), float, float, int) );
template double		qromo<double>( double (*)(double), double, double, double (*)( double(*)(double), double, double, int) );

/************************************************************************/
/*						Numericka integracija					*/
/************************************************************************/

#define	FUNC_INT(X) ((*func)(x))

template < class T > T	trapzd( T (* func) (T), T a, T b, int n )
{
/*
		Funkcija raèuna n-ti korak proširenog trapezoidalnog pravila
		
		n - broj toèaka koji se koristi kod inteprolacije unutar intervala
*/
	T					x=0, tnm, sum, del;
	static T	s;
	int				it, j;

	if( n == 1 )
		return (s=0.5*(b-a)*(FUNC_INT(a)+FUNC_INT(b)));
	else
	{
		for( it=1,j=1; j<n-1; j++ )
			it <<=1;

		tnm = it;
		del = (b-a)/tnm;
		x = a+0.5*del;
		
		for( sum=0.0, j=1; j<=it; j++, x+=del )
			sum +=FUNC_INT(x);

		s=0.5 * (s+(b-a)*sum/tnm);
		return s;
	}
}

#define	NINTEGR_EPS		1.0e-5
#define	JMAX					20

template < class T > T	qtrap(T (* func) (T), T a, T b )
{
/*
		Integrira funkciju 'func' koristeæi trapezoidalno pravilo

		NINETGR_EPS - željena relativna toènost
		JMAX - max. broj koraka
*/
	int			j;
	T	s, olds;


	olds = T(-1.e30);
	for( j=1; j<=JMAX; j++ )
	{
		s = trapzd(func,a,b,j);
		if( fabs(s-olds) < NINTEGR_EPS*fabs(olds) )
			return s;
		if( s == 0.0 && olds == 0.0 && j > 6 )
			return s;
		olds = s;
	}

	printf("To many steps in NIntegrate !!!");

	return T();

}

#define EPS		1.0e-5

template < class T > T qsimp( T (*func)(T), T a, T b )
{
	int	j;
	T		s, st, ost, os;

	ost = os = T(-1.0e30);
	for( j=1; j<=JMAX; j++ )
	{
		st = trapzd(func, a, b, j);
		s = ( 4.0 * st - ost) / 3.0;
		if( fabs(s-os) < EPS * fabs(os) )
			return	s;
		if( s == 0.0 && os == 0.0 && j > 6 )
			return s;
		os = s;
		ost = st;
	}

	printf("Previse koraka u qsimp");
	return T(0.0);
}

#define JMAXP	(JMAX+1)
#define	K 5
#define	EPS		1.0e-5

template < class T > T qromb( T (*func)(T), T a, T b )
{
	T		ss, dss;
	T		s [JMAXP+1], h[JMAXP+1];
	int	j;
	
	h[1-1] = T(1.0);
	for( j=1; j<=JMAX; j++ )
	{
		s[j-1] = trapzd(func, a, b, j);
		if( j >= K )
		{
			polint(&h[j-K-1], &s[j-K-1], K, T(0.0), &ss, &dss );
			if( fabs(dss) <= EPS*fabs(ss) )
				return ss;
		}

		s[j+1-1] = s[j-1];
		h[j+1-1] = 0.25 * h[j-1];
	}

	puts("To many steps in qromb");
	return T(0.0);
}

#undef JMAX

template < class T > T midpnt( T (*func)(T), T a, T b, int n )
{
	T		x, tnm, sum, del, ddel;
	static T	s;
	int		it, j;

	if( n == 1 )
		return ( s=(b-a) * FUNC_INT(0.5*(a+b)));
	else
	{
		for( it=1, j=1; j<n-1; j++ )
			it += 3;

		tnm = it;
		del = (b-a) / (3.0 * tnm);
		ddel = del + del;
		x = a + 0.5*del;
		sum = 0.0;
		for( j=1; j<=it; j++ )
		{
			sum += FUNC_INT(x);
			x += ddel;
			sum += FUNC_INT(x);
			x += del;
		}
		s = ( s+ (b-a) * sum / tnm ) / 3.0;
		
		return s;
	}
}

#define	EPS	1.0e-6
#define	JMAX	14
#define JMAXP	(JMAX+1)

template < class T > T qromo( T (*func)(T), T a, T b, T (*choose)(T (*f)(T), T, T, int) )
{
	int		j;
	T			ss, dss, h[JMAXP+1], s[JMAXP+1];

	h[1] = T(1.0);
	for( j=1; j<=JMAX; j++ )
	{
		s[j] = (*choose)(func,a,b,j);
		if( j>= K )
		{
//			polint(&h[h-K], &s[j-K], K, T(0.0), &ss, &dss );
			if( fabs(dss) <= EPS * fabs(ss) ) 
				return ss;
		}

		s[j+1] = s[j];
		h[j+1] = h[j] / 9.0;
	}
	puts("To many steps in qromo");
	return T(0.0);
}
