#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <valarray>

#include <mathlib.h>

int		nCurrCoef=0;

float	f1( float x )
{
	return x*x;
}

double	f2( double x )
{
	return x*cos(x);
}

float func( float psi )
{
	float	a[6] = { 0.5, 0.24, 0.1, 0.09, 0.05, 0.01 };
	float	rez, x;

	rez = a[0];
	x = acos(psi / 3.141593);
	for( int i=1; i<6; i++ )
		rez += a[i] * pow(x, i);

	return rez;
}

float FCos( float psi )
{
	return (func(psi) * cos(nCurrCoef * psi) );
}

float FSin( float psi )
{
	return (func(psi) * sin(nCurrCoef * psi) );
}

void main( void )
{
	float	a[30], b[30];
	float	rez;

	for( int i=0; i<30; i++ )
	{
		nCurrCoef = i;
		
 		a[i] = 1/(2*3.14159) * qromb<float>(FCos, -3.141593, 3.141593);
//		printf("\n");;
		puts("  ");
		b[i] = 1/(2*3.14159) * qromb<float>(FSin, -3.141593, 3.141593);
		
		printf(" %2d,  %15.10f ,  %15.10f", i, a[i], b[i] );
//		printf(" %d = %f ,  %d = %f\n", i, a[i], i, b[i] );
	}
}