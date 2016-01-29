#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mathlib.h>

int nvar;
double	x1, x2;

int kmax, kount;
double *xp, **yp, dxsav;

void load( double x1, double v[], double y[] )
{
	y[0] = 0.0;
	y[1] = 1.5;
	y[2] = v[0];
}

void score( double x2, double y[], double f[] )
{
	f[0] = y[0];
}

void derivs( double x, double y[], double dydx[] )
{
	double		m = 9.11e-31;
	double		V = 5*1.6e-19;
	double		h = 6.626e-34;

	dydx[0] = y[1];
	dydx[1] = -(y[2]-50)/0.2*y[0];
	dydx[2] = 0.0;
}

#undef EPS
#define EPS		1.0e-6

void shoot( double v[], double f[] )
{
	int			nbad, nok;
	double	h1, hmin=1.0e-12, *y;

	y = new double[nvar];
	kmax = 0;
	h1 = (x2-x1)/100.0;

	load(x1,v,y);
	
	odeint(y,nvar,x1,x2, EPS, h1, hmin, &nok, &nbad, derivs, rkqs,
					kmax, &kount, xp, yp, dxsav );
	
	score(x2,y,f);

	delete []y;
}

#define N2	1
/*
void main( void )
{
	int			check;
	double	v[N2], ystart[2];

	nvar = 3;
	x1 = 0;
	x2 = 1;
	xp = new double [100];
	yp = new double*[nvar];
	yp[0] = new double[100];
	yp[1] = new double[100];
	yp[2] = new double[100];

	for( int i=0; i<50; i++ )
	{
		v[0] = -50 + ( (double) rand())/32767*100;
		printf("v = %lf    ", v[0] );

		check = global_newton(v, N2, shoot );
		if( check == CONVERGED )
		{
			printf("v = %lf\n", v[0] );

	/*		ystart[0] = 0.0;
			ystart[1] = 1.5;
			ystart[2] = v[0];
			
			simple_integrate(ystart, 3, 0, 1.0, 0.001, 0.1, xp, yp, &kount, derivs );
			for( int i=0; i<11; i++ )
				printf("x[%d] = %lf -  y0 = %lf ,  y1 = %lf  , y2 = %lf\n", i, xp[i], yp[0][i], yp[1][i], yp[2][i] );
	*/
/*	}
	}
}
*/