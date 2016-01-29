#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"


MathLibException::MathLibException()
{
}

MathLibException::MathLibException( int nType )
{
	m_nType = nType;
}

MathLibException::~MathLibException()
{
}

/*******************************************************************/
/*						FUNKCIJE								   */
/*******************************************************************/



/*****************************************************************/
//			Fourier

#define TWOPID  6.2831853071795865

void period( double x[], double y[], int n, double ofac, double hifac, double px[],
						double py[], int np, int *nout, int *jmax, double *prob )
{
	// - raèuna Lombov periodogram za nejednoliko razmaknute toèke
	//   x - niz apscisa u kojima imamo vrijednosti
	//   y - niz vrijednosti promatrane varijable u danim toèkama apscise
	//   n - broj toèaka
	//   ofac - oversampling faktor (tipièno 4 ili više )
	//   hifac - do koje frekvencije idemo ( do hifac * Nyquistova frekvencija )
	//   np = ofac * hifac / 2 * N
	//   px[1..np] - niz frekvencija za koje je izraèunat periodogram
	//   py[1..np] - vrijednosti periodograma
	//   jmax - py[jmax] je maksimalni element u py[]
	//   prob -  procjena znaèenja tog maksimuma prema hipotetskom sluèajnm šumu 
	//           ( mala vrijednost indicira da postoji znaèajan periodièki signal)
	int			i,j;
	double		ave, c, cc, cwtau, effm, expy, pnow, pymax, s, ss, sumc, sumcy, sums, sumsh,
			sumsy, swtau, var, wtau, xave, xdif, xmax, xmin, yy;
	double	arg, wtemp, *wi, *wpi, *wpr, *wr;

	wi = new double[n+1];
	wpi = new double[n+1];
	wpr = new double[n+1];
	wr = new double[n+1];

	*nout = (int) (0.5 * ofac * hifac * n);

	if( *nout > np )
	{
		printf("output arrays too short in period ");
		assert(0);
	}
	avevar(y,n, &ave, &var);
	xmax = xmin = x[1];
	for( j=1; j<=n; j++ )
	{
		if( x[j] > xmax )
			xmax = x[j];
		if( x[j] < xmin )
			xmin = x[j];
	}
	xdif = xmax - xmin;
	xave = 0.5 * (xmax + xmin);
	pymax = 0.0;
	pnow = 1.0 / (xdif * ofac);
	for( j=1; j<=n; j++ )
	{
		arg = TWOPID * ((x[j] - xave)*pnow);
		wpr[j] = -2.0 * DSQR(sin(0.5*arg));
		wpi[j] = sin(arg);
		wr[j] = cos(arg);
		wi[j] = wpi[j];
	}
	for( i=1; i<=(*nout); i++ )
	{
		px[i] = pnow;
		sumsh = sumc = 0.0;
		for( j=1; j<=n; j++ )
		{
			c = wr[j];
			s = wi[j];
			sumsh += s * c;
			sumc += (c-s)*(c+s);
		}
		wtau = 0.5 * atan2(2.0*sumsh, sumc);
		swtau = sin(wtau);
		cwtau = cos(wtau);
		sums = sumc = sumsy = sumcy = 0.0;
		for( j=1; j<=n; j++ )
		{
			s = wi[j];
			c = wr[j];
			ss = s * cwtau - c * swtau;
			cc = c * cwtau + s * swtau;
			sums += ss*ss;
			sumc += cc*cc;
			yy = y[j] - ave;
			sumsy += yy*ss;
			sumcy += yy*cc;
			wr[j] = ((wtemp=wr[j])* wpr[j] - wi[j] * wpi[j]) + wr[j];
			wi[j] = (wi[j] * wpr[j] + wtemp * wpi[j]) + wi[j];
		}
		py[i] = 0.5*(sumcy * sumcy/ sumc + sumsy*sumsy/sums) / var;
		if( py[i] >= pymax )
			pymax = py[(*jmax=i)];
		pnow += 1.0 / (ofac * xdif);
	}
	expy = exp(-pymax);
	effm = 2.0*(*nout) / ofac;
	*prob = effm * expy;
	if( *prob > 0.01 ) 
		*prob = 1.0-pow(1.0-expy, effm);

	delete []wr;
	delete []wi;
	delete []wpi;
	delete []wpr;
}

void avevar( double data[], unsigned long n, double *ave, double *var )
{
	unsigned long j;
	double s, ep;

	for( *ave=0.0, j=1; j<=n; j++ )
		*ave += data[j];
	*ave /= n;

	*var = ep = 0.0;
	for( j=1; j<=n; j++ )
	{
		s=data[j] - (*ave);
		ep += s;
		*var += s*s;
	}
	*var = (*var - ep*ep/n)/(n-1);
}

