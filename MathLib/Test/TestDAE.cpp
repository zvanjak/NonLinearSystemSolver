#include <math.h>
#include <stdio.h>
#include <time.h>

#include <mathlib.h>

void func( double t, double x[], double y[], double xout[], double yout[] )
{
	xout[0] = -x[1] + x[0] * sin(y[0]);
	xout[1] = x[0] + y[1] * y[1];

	yout[0] = x[0] * y[1] - x[1] + 1.5;
	yout[1] = y[0] * y[0] + y[1] * cos(x[1]);
}

void vecfunc( Vector &x, Vector &out )
{
	out[0] = x[1] * 2 - x[2];
	out[1] = x[0] + x[1]*x[3];
	out[2] = -x[0] * 3 + 4;
	out[3] = x[1] * x[1] + x[3];
}

void vecfunc2( double x[], double out[] )
{
	out[0] = x[1] * 2 - x[2];
	out[1] = x[0] + x[1]*x[3];
	out[2] = -x[0] * 3 + 4;
	out[3] = x[1] * x[1] + x[3];
}
/*
void main( void )
{
	int			i, nkount, nok, nbad;
	double	x[2], ystart[2];
	double	pdt[500], **pdx, **pdy;

	pdx = new double*[2];
	pdy = new double*[2];

	for( i=0; i<2; i++ )
	{
		pdx[i] = new double[500];
		pdy[i] = new double[500];
	}

	x[0] = 1.5;
	x[1] = 0.5;
	ystart[0] = 0.5;
	ystart[1] = 0.0;

	// da saznamo pocetne vrijednosti za 'y'
	if( simple_newton(100, 2, 2, 0.0, x, ystart, func) )
	{
		puts("Nisam nasao pocetno rjesenje za y[] ");
	}

	for( i=0; i<2; i++ )
		printf("\n y[%d] = %20.10e", i, ystart[i] );

 	dae_integrate(2, 2, x, ystart, 0, 2, 0.001, 1e-10, 1e-3, &nok, &nbad, 500, &nkount, 
								pdt, pdx, pdy, 0.05, func, dae_rkqs);

	for( i=0; i<nkount; i++ )
		printf("\n t=%f,  x[0] = %f   -   x[1] = %f   ::::   y[0]  %f   -   y[1] = %f", pdt[i], pdx[0][i], pdx[1][i], pdy[0][i], pdy[1][i] );
}
*/