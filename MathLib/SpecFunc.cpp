#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

double	arsh(  double x )
{
	double	Tmp;

	Tmp = log(x + sqrt(1 + x*x));

	return Tmp;
}

double	arch(  double x )
{
	double	Tmp;

	if( x>=1 )
		Tmp = log(x + sqrt(x*x - 1));
	else
	{				
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}

	return Tmp;
}

double	arth(  double x )
{
	double	Tmp;

	if( x > -1 && x < 1 )
		Tmp = 0.5 * log( (1+x) / (1-x));
	else
	{				
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}

	return Tmp;
}

double	arcth(  double x )
{
	double Tmp;

	if( x >= -1 && x <=1 )
	{				
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}
	else
		Tmp = 0.5 * log( (x+1) / (x-1) );

	return Tmp;
}

// Gamma funkcija
// vraca vrijednost ln(Gamma(xx)) za xx > 0
double GammaLn( double xx )
{
	double x, y, tmp, ser;
	static double cof[6] = { 76.18009172947146, -86.50532032941677, 24.01409824083091,
		-1.231739572450155, 0.120860973866179e-2, -0.5395239384953e-5 };
	int		j;

	y = x = xx;
	tmp = x+5.5;
	tmp -= (x+0.5) * log(tmp);
	ser = 1.000000000190015;
	for( j=0; j<=5; j++ )
		ser += cof[j] / ++y;

	return -tmp+log(2.5066282746310005*ser/x);
}

// Beta funkcija
// vraca vrijednost beta funkcije B(z,w)
double Beta( double z, double w )
{
	return exp(GammaLn(z) + GammaLn(w) - GammaLn(z+w));
}

// Nepotpuna gamma funkcija P(a,x)
double GammaP( double a, double x )
{
	double gamser, gcf, gln;

	if( x < 0.0 || a <= 0.0 )
	{
		printf("Nekorektni argumenti ");
		throw MathLibException(FUNC_UNDEFINED);
	}
	if( x < (a+1.0) )
	{
		GammaSer( &gamser, a, x,&gln );
		return gamser;
	}
	else
	{
		GammaCFrac(&gcf, a, x, &gln );
		return 1.0 - gcf;
	}
}

// Nepotpuna gamma funkcija Q(a,x) = 1 - P(a,x)
double GammaQ( double a, double x )
{
	double	gamser, gcf, gln;

	if( x < 0.0 || a <= 0.0 )
	{
		printf("Nekorektni argumenti");
		throw MathLibException(FUNC_UNDEFINED);
	}
	if( x < (a+1.0))
	{
		GammaSer(&gamser, 1, x, &gln);
		return 1.0 - gamser;
	}
	else
	{
		GammaCFrac(&gcf, a, x, &gln);
		return gcf;
	}
}

#undef ITMAX
#undef EPS

#define ITMAX 100
#define EPS	3.0e-7

// Nepotpuna gamma funkcija izracunata pomocu njenog prikaza kao reda
void GammaSer(double *gamser, double a, double x, double *gln )
{
	int		n;
	double	sum, del, ap;

	*gln = GammaLn(a);
	if( x <= 0.0 )
	{
		if( x < 0.0 )
		{
			printf("x manji od 0 u funkcjii GammaSer");
			assert(0);
		}
		*gamser = 0.0;
		return;
	}
	else
	{
		ap = a;
		del = sum = 1.0 / a;
		for( n=1; n<=ITMAX; n++ )
		{
			++ap;
			del *= x / ap;
			sum += del;
			if( fabs(del) < fabs(sum) * EPS )
			{
				*gamser = sum * exp(-x+a*log(x) - (*gln));
				return;
			}
		}
		printf("a prevelik, ITMAX previse mali u funkcji GammaSer");
		assert(0);
		return;
	}
}
#undef ITMAX
#undef EPS

#define ITMAX 100
#define EPS	3.0e-7
#define FPMIN	1.0e-30

// Nepotpuna gamma funkcija Q(a,x) izracunata pomocu kontinuiranog razlomka
void GammaCFrac( double *gcf, double a, double x , double *gln )
{
	int			i;
	double	an, b, c, d, del, h;

	*gln = GammaLn(a);
	b = x+1.0 - a;
	c = 1.0 / FPMIN;
	d = 1.0 / b;
	h = d;
	for( i=1; i<=ITMAX; i++ )
	{
		an = -i*(i-a);
		b += 2.0;
		d = an*d + b;
		if( fabs(d) < FPMIN) 
			d = FPMIN;
		c = b + an/c;
		if( fabs(c) < FPMIN ) 
			c = FPMIN;
		d = 1.0/ d;
		del = d * c;
		h *= del;
		if( fabs(del - 1.0) < EPS) 
			break;
	}
	if( i > ITMAX)
	{
		printf("a prevelik, ITMAX previse mali u gcf");
		assert(0);
	}
	*gcf = exp(-x+a*log(x) - (*gln)) * h;
}

#undef ITMAX
#undef EPS	
#undef FPMIM


//  Error funkcija
double Errf( double x )
{
	return x < 0.0 ? -GammaP(0.5, x*x) : GammaP(0.5, x*x);
}

// Komplementarna error funkcija
double Errfc( double x )
{
	return x < 0.0 ? 1.0 + GammaP(0.5, x*x) : GammaQ(0.5, x*x);
}

#define MAXIT	100
#define FPMIN	1.0e-30
#define EPS	1.0e-7

// Racuna eksponencijalni integral En(x)
double ExpInt( int n, double x)
{
	int			i, ii, nm1;
	double	a, b, c, d, del, fact, h, psi, ans;

	nm1 = n-1;
	if( n < 0 || x < 0.0 || (x == 0.0 && (n==0 || n==1)) )
	{
		printf("Krivi argumenti u expin");
		assert(0);
	}
	else
	{
		if( n == 0 )
			ans = exp(-x) / x;
		else
		{
			if( x == 0.0 )
				ans = 1.0 / nm1;
			else
			{
				if( x > 1.0 )
				{
					b = x + n;
					c = 1.0 / FPMIN;
					d = 1.0 / b;
					h = d;
					for( i=1; i<=MAXIT; i++ )
					{
						a = -i * (nm1 + i);
						b += 2.0;
						d = 1.0 / (a*d + b);
						c = b + a/c;
						del = c * d;
						h *= del;

						if( fabs(del - 1.0) < EPS )
						{
							ans = h*exp(-x);
							return ans;
						}
					}
					printf("Greska kod kontinuiranog razloma u epxin");
					assert(0);
				}
				else
				{
					ans = (nm1 != 0 ? 1.0 /nm1 : -log(x)-EULER);
					fact = 1.0;
					for( i=1; i<=MAXIT; i++ )
					{
						fact *= -x / i;
						if( i!= nm1 )
							del = -fact/(i-nm1);
						else
						{
							psi = -EULER;
							for( ii=1; ii<nm1; ii++)
								psi += 1.0 / ii;
							del = fact * (-log(x) + psi);
						}
						ans += del;
						if( fabs(del) < fabs(ans)*EPS)
							return ans;
					}
					printf("Greska u redu kod ExpInt");
					assert(0);
				}
			}
		}
	}
	return ans;
}


double Ei( double x)
{
	int			k;
	double		fact, prev, sum, term;

	if( x<=0.0 )
	{
		printf("Krivi argument u Ei");
		assert(0);
	}
	if( x < FPMIN )
		return log(x)+EULER;
	if( x <= -log(EPS) )
	{
		sum = 0.0;
		fact = 1.0;
		for( k=1; k<=MAXIT; k++ )
		{
			fact *= x / k;
			term = fact / k;
			sum += term;
			if( term < EPS*sum)
				break;
		}
		if (k > MAXIT )
		{
			printf("Greska reda u Ei");
			assert(0);
		}
		return sum +log(x)+EULER;
	}
	else
	{
		sum =0.0;
		term = 1.0;
		for( k=1; k<=MAXIT; k++ )
		{
			prev = term;
			term *= k / x;
			if( term < EPS )
				break;

			if( term < prev )
				sum += term;
			else
			{
				sum -= prev;
				break;
			}
		}
		return exp(x)*(1.0+sum) / x;
	}
}

#undef MAXIT
#undef FPMIN
#undef EPS	

// Nekompletna beta funkcija Ix(a,b)
double betai( double a, double b, double x )
{
	double		bt;

	if( x < 0.0 || x > 1.0 )
	{
		printf(" Krivi x u betai");
		assert(0);
	}

	if( x == 0.0 || x == 1.0 )
		bt = 0.0;
	else
		bt = exp(GammaLn(a+b) - GammaLn(a) - GammaLn(b) + a*log(x) + b * log(1.0-x));

	if( x < (a+1.0) / (a+b+2.0))
		return bt * BetaCFrac(a,b,x) / a;
	else
		return 1.0 - bt* BetaCFrac(b,a,1.0-x)/b;
}

#define MAXIT 100
#define	EPS		3.0e-7
#define FPMIN	1.0e-30

// Racuna kontinuirani razlomak za nekompletnu beta funkciju pomocu Lentzovog metoda
double BetaCFrac( double a, double b, double x )
{
	int		m, m2;
	double		aa, c, d, del, h, qab, qam, qap;

	qab = a + b;
	qap = a + 1.0;
	qam = a - 1.0;
	c = 1.0;
	d = 1.0 - qab*x/qap;
	if( fabs(d) < FPMIN)
		d = FPMIN;
	d = 1.0 / d;
	h = d;

	for( m=1; m<=MAXIT; m++ )
	{
		m2 = 2 * m;
		aa = m * (b-a) * x / ((qam+m2) * (a+m2));
		d = 1.0 + aa*d;
		if( fabs(d) < FPMIN )
			d = FPMIN;
		c = 1.0 + aa/c;
		if( fabs(c) < FPMIN)
			c = FPMIN;
		d = 1.0 / d;

		h *= d*c;
		aa = -(a+m) * (qab +m)*x/((a+m2)*(qap-m2));
		d = 1.0 + aa*d;
		if( fabs(d) < FPMIN)
			d = FPMIN;
		c = 1.0 + aa/c;
		if( fabs(c) < FPMIN )
			c = FPMIN;
		d = 1.0 / d;

		del = d*c;
		h *= del;
		if( fabs(del-1.0) < EPS)
			break;
	}
	if( m > MAXIT)
	{
		printf("a ili b preveliki, ili MAXIT premali u BetaCFrac");
		assert(0);
	}
	return h;
}

#undef MAXIT
#undef FPMIN
#undef EPS	

double	BesselJ0( double x )
{
	double		ax, z;
	double	xx, y, ans, ans1, ans2;

	if( (ax = fabs(x)) < 8.0 )
	{
		y = x*x;
		ans1 = 57568490574.0 + y*(-13362590354.0 + y*(651619640.7 + y*(-11214424.18 + y*(77392.33017 + y*(-184.9052456)))));
		ans2 = 57568490411.0 + y*(1029532985.0 + y*(9494680.718 + y*(59272.64853 + y*(267.8532712 + y*1.0))));
		ans = ans1 / ans2;
	}
	else
	{
		z = 8.0 / ax;
		y = z*z;
		xx = ax - 0.785398164;

		ans1 = 1.0 + y*(-0.1098628627e-2 + y*(0.2734510407e-4 + y*(-0.2073370639e-5 + y*0.2093887211e-6)));
		ans2 = -0.1562499995e-1 + y*(0.1430488765e-3 + y*(-0.6911147651e-5 + y*(0.7621095161e-6 - y*0.934935152e-7)));

		ans = sqrt(0.636619722 / ax) * (cos(xx) * ans1 - z*sin(xx)*ans2);
	}

	return ans;
}

double BesselY0( double x )
{
	double		z;
	double		xx, y, ans, ans1, ans2;

	if( x < 8.0 )
	{
		y = x*x;
		ans1 = -2957821389.0 + y*(7062834065.0 + y*(-512359803.6 + y*(10879881.29 +y*(-86327.92757 + y*228.4622733))));
		ans2 = 40076544269.0 + y*(745249964.8 + y*(7189466.438 + y*(47447.26470 + y*(226.1030244 + y*1.0))));
		ans = (ans1 / ans2) + 0.636619772 * BesselJ0(x) * log(x);
	}
	else
	{
		z = 8.0 / x;
		y = z*z;
		xx = x - 0.785398164;
		ans1 = 1.0 + y*(-0.1098628627e-2 + y*(0.2734510407e-4 + y*(-0.2073370639e-5 + y*0.2093887211e-6)));
		ans2 = -0.1562499995e-1 + y*(0.1430488765e-3 + y*(-0.6911147651e-5 + y*(0.7621095161e-6 + y*(-0.934945152e-7))));
		ans = sqrt(0.636619772/x) * (sin(xx) * ans1 + z*cos(xx)*ans2);
	}
	return ans;
}

#define EPS		1.0e-16
#define FPMIN	1.0e-30
#define MAXIT	20000
#define XMIN		2.0
#define PI     3.141592653589793

void	BesselJY( double x, double xnu, double *rj, double *ry, double *rjp, double *ryp )
{
	int			i, isign, l, nl;
	double	a, b, br, bi, c, cr, ci, d, del, del1, den, di, dlr, dli, dr, e, f, fact, fact2,
		      fact3, ff, gam, gam1, gam2, gammi, gampl, h, p, pimu, pimu2, q, r, rjl,
					rjl1, rjmu, rjp1, rjpl, rjtemp, ry1, rymu, rymup, rytemp, sum, sum1,
					temp, w, x2, xi, xi2, xmu, xmu2;

	if( x <= 0.0 || xnu < 0.0 )
	{
		printf("Krivi argumenti u BesselJY");
		assert(0);
	}
	nl = ( x < XMIN ? (int)(xnu+0.5) : IMAX(0,(int)(xnu-x+1.5)));

	xmu = xnu - nl;
	xmu2 = xmu * xmu;
	xi = 1.0 / x;
	xi2 = 2.0 * xi;
	w = xi2 / PI;
	isign = 1;
	h = xnu * xi;
	if( h < FPMIN )
		h = FPMIN;
	b = xi2 * xnu;
	d = 0.0;
	c = h;
	for( i=1; i<=MAXIT; i++ )
	{
		b += xi2;
		d = b - d;
		if( fabs(d) < FPMIN )
			d = FPMIN;
		c = b - 1.0/c;
		if( fabs(c) < FPMIN )
			c = FPMIN;
		d = 1.0 / d;
		del = c * d;
		h = del * h;
		if( d < 0.0 ) isign = -isign;
		if( fabs(del-1.0) < EPS )
			break;
	}

	if( i > MAXIT )
	{
		printf("x prevelik u BesselJY, probaj asimptotsku ekspanziju");
		assert(0);
	}
	rjl = isign * FPMIN;
	rjpl = h * rjl;
	rjl1 = rjl;
	rjp1 = rjpl;
	fact = xnu * xi;
	for( l=nl; l>=1; l-- )
	{
		rjtemp = fact * rjl + rjpl;
		fact -= xi;
		rjpl = fact * rjtemp - rjl;
		rjl = rjtemp;
	}
	if( rjl == 0.0 ) 
		rjl = EPS;
	f = rjpl / rjl;
	
	if( x < XMIN )
	{
		x2 = 0.5 * x;
		pimu = PI * xmu;
		fact = (fabs(pimu) < EPS ? 1.0 : pimu/sin(pimu) );
		d = -log(x2);
		e = xmu * d;
		fact2 = (fabs(e) < EPS ? 1.0 : sinh(e) / e);
		beschb(xmu, &gam1, &gam2, &gampl, &gammi );
		ff = 2.0 / PI * fact * (gam1 * cosh(e) + gam2*fact2*d);
		e = exp(e);
		p = e / (gampl*PI);
		q = 1.0 / (e*PI*gammi);
		pimu2 = 0.5 * pimu;
		fact3 = (fabs(pimu2) < EPS ? 1.0 : sin(pimu2)/pimu2);
		r = PI * pimu2 * fact3 * fact3;
		c = 1.0;
		d = -x2*x2;
		sum = ff + r*q;
		sum1 = p;
		for( i=1; i<=MAXIT; i++ )
		{
			ff = (i*ff + p + q) / (i*i - xmu2);
			c *= (d/i);
			p /= (i-xmu);
			q /= (i+xmu);
			del = c*(ff + r*q);
			sum += del;
			del1 = c *p - i*del;
			sum1 += del1;
			if( fabs(del1) < (1.0 + fabs(sum)) * EPS)
				break;
		}
		if( i > MAXIT )
		{
			printf("Red bessy nije konvergirao");
			assert(0);
		}
		rymu = -sum;
		ry1 = -sum1*xi2;
		rymup = xmu * xi * rymu - ry1;
		rjmu = w/(rymup - f*rymu);
	}
	else
	{
		a = 0.25 - xmu2;
		p = -0.5*xi;
		q = 1.0;
		br = 2.0 * x;
		bi = 2.0;
		fact = a*xi/(p*p + q*q);
		cr = br + q*fact;
		ci = bi + p*fact;
		den = br * br + bi * bi;
		dr = br / den;
		di = -bi / den;
		dlr = cr*dr - ci*di;
		dli = cr*di + ci*dr;
		temp = p*dlr - q*dli;
		q = p*dli + q*dlr;
		p = temp;
		for( i=2; i<=MAXIT; i++ )
		{
			a += 2*(i-1);
			bi += 2.0;
			dr = a*dr + br;
			di = a*di + bi;
			if( fabs(dr) + fabs(di) < FPMIN )
				dr = FPMIN;
			fact = a/(cr*cr + ci*ci);
			cr = br + cr*fact;
			ci = bi - ci*fact;
			if( fabs(cr) + fabs(ci) < FPMIN )
				cr = FPMIN;
			den = dr*dr + di*di;
			dr /= den;
			di /= -den;
			dlr = cr*dr - ci*di;
			dli = cr*di + ci*dr;
			temp = p*dlr - q*dli;
			q = p*dli + q*dlr;
			p = temp;
			if( fabs(dlr-1.0) + fabs(dli) < EPS)
				break;
		}
		if( i > MAXIT )
		{
			printf("cf2 nije prosao u BesselJY");
			assert(0);
		}
		gam = (p-f)/q;
		rjmu = sqrt(w/((p-f)*gam+q));
		rjmu = SIGN(rjmu, rjl);
		rymu = rjmu * gam;
		rymup = rymu *(p + q/gam);
		ry1 = xmu * xi * rymu - rymup;
	}
	fact = rjmu / rjl;
	*rj = rjl1 * fact;
	*rjp = rjp1 * fact;
	for( i=1; i<=nl; i++ )
	{
		rytemp = (xmu+i) * xi2 * ry1 - rymu;
		rymu = ry1;
		ry1 = rytemp;
	}
	*ry = rymu;
	*ryp = xnu*xi*rymu - ry1;
}
#undef EPS		
#undef FPMIN	
#undef MAXIT	
#undef XMIN		
#undef PI 


#define NUSE1		7
#define NUSE2		8

void beschb( double x, double *gam1, double *gam2, double *gampl, double *gammi )
{
	double		xx;
	static double c1[] = { -1.142022680371172e0, 6.516511267076e-3,
		3.08709017308e-4, -3.470626964e-6, 6.943764e-9,
		3.6780e-11, -1.36e-13 };
	static double c2[] = { 1.843740587300906e0, -0.076852840844786e0,
		1.271927136655e-3, -4.971736704e-6, -3.3126120e-8,
		2.42310e-10, -1.70e-13, -1.0e-15 };

	xx = 8.0*x*x - 1.0;
	*gam1 = chebev(-1.0, 1.0, c1, NUSE1, xx);
	*gam2 = chebev(-1.0, 1.0, c2, NUSE2, xx);
	*gampl = *gam2 - x*(*gam1);
	*gammi = *gam2 + x*(*gam1);
}

double chebev( double a, double b, double c[], int m, double x )
{
	double d=0.0, dd=0.0, sv, y, y2;
	int		j;
	
	if((x-a)*(x-b) > 0.0 )
	{
		printf("x nije u rasponu u chebev");
		assert(0);
	}
	y2 = 2.0 * (y=(2.0*x - a - b) / (b-a));
	for( j=m-1; j>=1; j--)
	{
		sv = d;
		d = y2*d - dd + c[j];
		dd = sv;
	}
	return y*d - dd + 0.5 * c[0];
}

#define RTPI02 1.2533141

void SphericalBessel( int n, double x, double *sj, double *sy, double *sjp, double *syp )
{
	double	factor, order, rj, rjp, ry, ryp;

	if( n < 0 || x <= 0.0 )
	{
		printf("Krivi argumenti u SphericalBessel");
		assert(0);
	}
	order = n + 0.5;
	BesselJY( x, order, &rj, &ry, &rjp, &ryp );
	factor = RTPI02 / sqrt(x);
	*sj = factor * rj;
	*sy = factor * ry;
	*sjp = factor * rjp - (*sj) / (2.0*x);
	*syp = factor * ryp - (*sy) / (2.0*x);
}

double LegandrePoly( int l, int m, double x)
{
	double	fact, pll, pmm, pmmp1, somx2;
	int		i, ll;
	
	if( m < 0 ||  m > 1 || fabs(x) > 1.0 )
	{
		printf("Krivi argumenti u plgnd");
		assert(0);
	}
	pmm = 1.0;
	if( m > 0)
	{
		somx2 = sqrt((1.0-x) * (1.0+x));
		fact = 1.0;
		for( i=1; i<=m; i++ )
		{
			pmm *= -fact*somx2;
			fact += 2.0;
		}
	}
	if( l == m )
		return pmm;
	else
	{
		pmmp1 = x*(2*m+1) * pmm;
		if( l == (m+1))
			return pmmp1;
		else
		{
			for( ll=m+2; ll <=l; ll++ )
			{
				pll = (x*(2*ll-1)*pmmp1 - (ll+m-1)*pmm) / (ll-m);
				pmm = pmmp1;
				pmmp1 = pll;
			}
			return pll;
		}
	}
}

#define EPS		6.0e-8
#define	MAXIT	100
#define PIBY2	1.57721566
#define	FPMIN	1.0e-30
#define	TMIN		2.0
#define	TRUE		1
#define	ONE		Complex(1.0,0.0)

void CiSi( double x, double *ci, double *si )
{
	int			i, k, odd;
	double		a, err, fact, sign, sum, sumc, sums, t, term;
	Complex	h, b, c, d, del;

	t = fabs(x);
	if( t == 0.0 )
	{
		*si = 0.0;
		*ci = -1.0 / FPMIN;
		return;
	}
	if( t > TMIN )
	{
		b = Complex(1.0, t);
		c = Complex(1.0/FPMIN, 0.0);
		d = h = ONE / b;
		for( i=2; i<=MAXIT; i++ )
		{
			a = -(i-1) * ( i-1);
			b += Complex(2.0, 0.0);
			d = ONE / ( a * d + b );
			c = b + (Complex(a, 0.0) / c);
			del = c * d;
			h *= del;
			if( fabs(Re(del) - 1.0) + fabs(Im(del)) < EPS)
				break;
		}
		if( i > MAXIT )
		{
			printf("cf failed u CiSi");
			assert(0);
		}
		h *= Complex(cos(t),sin(t));
		*ci = -Re(h);
		*si = PIBY2 + Im(h);
	}
	else
	{
		if( t < sqrt(FPMIN) )
		{
			sumc = 0.0;
			sums = t;
		}
		else
		{
			sum = sumc = sums = 0.0;
			sign = fact = 1.0;
			odd = TRUE;
			for( k=1; k<=MAXIT; k++ )
			{
				fact *= t / k;
				term = fact / k;
				sum += sign*term;
				err = term/fabs(sum);
				if( odd)
				{
					sign = -sign;
					sums = sum;
					sum = sumc;
				}
				else
				{
					sumc = sum;
					sum = sums;
				}
				if( err < EPS )
					break;
				odd = !odd;
			}
			if( k > MAXIT )
			{
				printf("Maxits exceeded ");
				assert(0);
			}
		}
		*si = sums;
		*ci = sumc + log(t) + EULER;
	}

	if( x < 0.0 )
		*si = -(*si);
}

#undef EPS		
#undef MAXIT	
#undef PIBY2	
#undef FPMIN	
#undef TMIN	
#undef TRUE
#undef ONE		
