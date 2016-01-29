#include <math.h>
#include <stdio.h>

#include	"mathlib.h"

template	void		polint<float>( float [], float [], int, float, float *, float * );
template	void		polint<double>( double [], double [], int, double, double *, double * );

template < class T > void polint( T xa[], T ya[], int n, T x, T *y, T *dy )
//void polint( double xa[], double ya[], int n, double x, double *y, double *dy )
{
/*
		Za dana polja xa[] i ya[] u kojima su tabelirane vrijednosti funkcije, 
		i zadanu vrijednost 'x', funkcija vraæa interpoliranu vrijednost 'y' i 
		procijenjenu grešku 'dy'
*/
	int			i, m, ns = 1;
	T	den, dif, dift, ho, hp, w;
	T *c, *d;

	dif = (T) fabs(x-xa[1-1]);
	c = new T[n];
	d = new T[n];

	for( i=1; i<=n; i++ )
	{
		if( (dift = (T) fabs(x-xa[i-1])) < dif )
		{
			ns = i;
			dif = dift;
		}
		c[i-1] = ya[i-1];
		d[i-1] = ya[i-1];
	}
	*y = ya[ns -1]; ns--;
	for( m=1; m<n; m++ )
	{
		for( i=1; i<=n-m; i++ )
		{
			ho = xa[i-1] - x;
			hp = xa[i+m-1] - x;
			w = c[i+1-1] - d[i-1];

			if( (den = ho - hp) == 0.0 )
			{
				printf("Error in routine polint");
				// do ove greške može doæi samo ako su dva ulaza u 'xa' identièna
//				assert(0);
			}
			den = w / den;
			d[i-1] = hp * den;
			c[i-1] = ho * den;
		}
		*y += (*dy = (2*ns < (n-m) ? c[ns+1-1] : d[ns-- - 1] ));
	}
	delete []d;
	delete []c;
}

void spline( double x[], double y[], int n, double yp1, double ypn, double y2[] )
{
/* Za dana polja x[1..n], y[1..n] koja sadrze tabeliranu funkciju, x1<x2<...<xn, i za
   dane prve derivacije yp1 i ypn u tockama 1 i n, funkcija vraca polje y2[1..n] koje 
	 sadrzi druge derivacije interpolirajuce funkcije u tabeliranim tocakama x[i].
	 Ako su yp1 i/ili ypn jednaki 10E30,ili veci, funkcija postavlja odgovarajuci granicni
	 uvjet tak oda se dobije prirodni spline, sa drugom derivacijom jednakom nuli */
	
	int		i, k;
	double p, qn, sig, un;
	
	Vector	*u;

	u = new Vector(n-1);
	if( yp1 > 0.99e30 )
		y2[1] = (*u)[1] = 0.0;
	else 
	{
		y2[1] = -0.5;
		(*u)[1] = ( 3.0 / (x[2]-x[1]) ) * ( (y[2]-y[1]) / (x[2]-x[1]) - yp1 );
	}
	for( i=2; i<=n-1; i++ )
	{
		sig = (x[i]-x[i-1]) / (x[i+1]-x[i-1]);
		p = sig*y2[i-1] + 2.0;
		y2[i] = (sig-1.0) / p;
		(*u)[i] = (y[i+1]-y[i]) / (x[i+1]-x[i])  -  (y[i]-y[i-1]) / (x[i]-y[i-1]);
		(*u)[i] = ( 6.0 * (*u)[i] / (x[i+1]-x[i-1]) - sig*(*u)[i-1] ) / p;
	}
	if( ypn > 0.99e30)
		qn = un = 0.0;
	else
	{
		qn = 0.5;
		un = (3.0 / (x[n]-x[n-1]) ) * (ypn - (y[n]-y[n-1]) / (x[n]-x[n-1]) );
	}
	y2[n] = (un - qn*(*u)[n-1]) / (qn*y2[n-1] + 1.0);
	for( k=n-1; k>=1; k--)
		y2[k] = y2[k] * y2[k+1] + (*u)[k];

	delete u;
}

void splint( double xa[], double ya[], double y2a[], int n, double x, double *y )
{
/*
    Za dana polja xa[1..n] i ya[1..n], koja tabeliraju funkciju (  s time da su xa-ovi poredani )
		i dano polje y2a[1..n], koje je izlaz funkcije spline, i za dani x, funkcija vraca
		vrijednost y interpoliranu  pomocu kubicnog spline-a
*/
	int		klo, khi, k;
	double	h, b, a;

	klo = 1;
	khi = n;
	while( khi - klo > 1 )
	{
		k = (khi + klo) >> 1;
		if( xa[k] > x )
			khi = k;
		else
			klo = k;
	}
	h = xa[khi] - xa[klo];
	if( h == 0.0 )
	{
		printf("Bad xa input to routine splint !!!");
//		assert(0);
	}
	a = (xa[khi] - x) / h;
	b = (x - xa[klo]) / h;
	*y = a * ya[klo] + b*ya[khi] + (( a*a*a - a) * y2a[klo] + (b*b*b - b)*y2a[khi]) * (h*h) / 6.0;
}