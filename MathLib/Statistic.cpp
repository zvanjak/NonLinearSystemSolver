#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

void fit( double x[], double y[], int ndata, double sig[], int mwt, double *a, double *b, 
				 double *siga, double *sigb, double *chi2, double *q )
{
	int		i;
	double	wt, t, sxoss, sx=0.0, sy=0.0, st2=0.0, ss, sigdat;

	*b = 0.0;
	if( mwt)
	{
		ss = 0.0;
		for( i=1; i<=ndata; i++ )
		{
			wt = 1.0 / DSQR(sig[i]);
			ss += wt;
			sx += x[i] * wt;
			sy += y[i] * wt;
		}
	}
	else
	{
		for( i=1; i<=ndata; i++ )
		{
			sx += x[i];
			sy += y[i];
		}
		ss = ndata;
	}
	sxoss = sx/ss;
	if( mwt)
	{
		for( i=1; i<=ndata; i++ )
		{
			t = (x[i] - sxoss) / sig[i];
			st2 += t*t;
			*b += t*y[i] / sig[i];
			}
		}
	else
	{
		for( i=1; i<=ndata; i++ )
		{
			t = x[i] - sxoss;
			st2 += t*t;
			*b += t*y[i];
			}
		}
	*b /= st2;
	*a = (sy - sx* (*b) ) / ss;
	*siga = sqrt((1.0 + sx*sx/(ss*st2)) / ss);
	*sigb = sqrt(1.0 / st2 );
	*chi2 = 0.0;
	
	if( mwt == 0 )
	{
		for( i=1; i<=ndata; i++ )
			*chi2 += DSQR(y[i] - (*a) - (*b) * x[i]);
		*q = 1.0;
		sigdat = sqrt((*chi2) / (ndata-2));
		*siga *= sigdat;
		*sigb *= sigdat;
	}
	else
	{
		for( i=1; i<=ndata; i++ )
			*chi2 += DSQR((y[i] - (*a) - (*b) * x[i]) / sig[i]);

		*q = GammaQ(0.5 * (ndata-2), 0.5*(*chi2));
	}
}
