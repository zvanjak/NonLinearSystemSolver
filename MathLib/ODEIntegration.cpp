#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

/******************************************************************/
/****   Integracija sistema obiènih diferencijalnih jednadžbi  ****/
/******************************************************************/


int simple_integrate( double pdYStart[], int nVar, double dX1, double dX2, 
			double	dStep, double dXSave, double *pdX, double **pdY, int *nKount,
			void (*derivs)(double, double [], double []) )
{
	int			i, kount, nStepPerSave, num;
	long		nstep,k;
	double	x, xsav;
	double	*v, *vout, *dv, *verr;

	v = new double[nVar];
	dv = new double[nVar];
	vout = new double[nVar];
	verr = new double[nVar];

	for( i=0; i<nVar; i++ )
	{
		v[i] = pdYStart[i];
		pdY[i][0] = v[i];
	}

	pdX[0] = dX1;
	x = dX1;
	nstep = (long) ((dX2-dX1) / dStep);
	nStepPerSave = (int) (dXSave / dStep);

	num = 0;
	kount = 0;
	xsav = x;
	
	for( k=0; k<nstep; k++ )
	{
		(*derivs)(x,v,dv);
		rkck(v,dv,nVar,x,dStep,vout,verr,derivs);

		if( (double)(x+dStep) == x )
		{
			printf("Stepsize too small in RKDumb!!");

			return STEPSIZE_UNDERFLOW;
		}
		num++;

		x += dStep;
		for( i=0; i<nVar; i++ )
			v[i] = vout[i];
	
		if( num == nStepPerSave )
		{
			num = 0;
			pdX[kount+1] = x;
			for( i=0; i<nVar; i++ )
			{
				pdY[i][kount+1] = v[i];
			}
			xsav = x;
			kount++;
		}
	}

	*nKount = kount;

	delete []v;
	delete []dv;
	delete []vout;
	delete []verr;

	return INT_OK;
}

int SimpleODEIntegrate( Vector &ystart, int nVar, double x1, double x2, 
			double	h, double dxsave, Vector &xp, Matrix &yp, int *nKount,
			void (*derivs)(double, Vector &, Vector &) )
{
	int			i, kount, nStepPerSave, num;
	long		nstep,k;
	double	x, xsav;
	Vector	*v, *vout, *dv, *verr;

	v = new Vector(nVar);
	dv = new Vector(nVar);
	vout = new Vector(nVar);
	verr = new Vector(nVar);

	for( i=0; i<nVar; i++ )
	{
		(*v)[i] = ystart[i];
		yp[i][0] = (*v)[i];
	}

	xp[0] = x1;
	x = x1;
	nstep = (long) ((x2-x1) / h);
	nStepPerSave = (int) (dxsave / h);

	num = 0;
	kount = 0;
	xsav = x;
	
	for( k=0; k<nstep; k++ )
	{
		(*derivs)(x,*v,*dv);
		RKAlgorithm(*v,*dv,nVar,x,h,*vout,*verr,derivs);

		if( (double)(x+h) == x )
		{
			printf("Stepsize too small in RKDumb!!");

			return STEPSIZE_UNDERFLOW;
		}
		num++;

		x += h;
		for( i=0; i<nVar; i++ )
			(*v)[i] = (*vout)[i];
	
		if( num == nStepPerSave )
		{
			num = 0;
			xp[kount+1] = x;
			for( i=0; i<nVar; i++ )
			{
				yp[i][kount+1] = (*v)[i];
			}
			xsav = x;
			kount++;
		}
	}

	*nKount = kount;

	delete v;
	delete dv;
	delete vout;

	return INT_OK;
}


//	Stepper rutina za RK-metodu
//  odredjuje korak postupka
int RKStepper( Vector &y, Vector &dydx, int n, double *x, double htry, 
					 double eps, Vector &yscal, double *hdid, double *hnext,
					 void (*derivs)(double, Vector &, Vector &) )
/*	RK 5-og reda sa pracenjem lokalne greske zaokruzivanja da bi se osigurala tocnost i 
	prilagodila duljina koraka.
	ULAZ:
		vektor zavisnih varijabli  -  y[1..n]
		i njegove derivacije   -  dydx[1..n],   i to u pocetnoj tocki nezavisne varijable x
		duljina koraka(pocetna za probu)  -  htry
		zahtijevana tocnost	 -  eps
		i vektor sa kojim se skalira greska  -  yscal[1..n]
	IZLAZ:
		y i x -  zamijenjeni sa novim vrijednostima
		hdid  -  postignuta duljina koraka
		hnext -  predvidjena duljina koraka u slijedecem koraku

	(*derivs) - korisnicka rutina koja racuna derivacije (desne strane sistema jednadzbi)
	
	Primjer za derivs:

	dy1/dx = f1(x,y1,y2)
	dy2/dx = f2(x,y1,y2)

	void System(double x, double *y, double *dxdy )
	{
		dxdy[0] = f1(x, y[0], y[1] );
		dxdy[1] = f2(x, y[0], y[1] );
	}
*/
{
	int		i;
	double	errmax, h, htemp, xnew;
	Vector	*yerr, *ytemp;

	yerr = new Vector(n);
	ytemp = new Vector(n);
	h = htry;

	for(;;)
	{
		RKAlgorithm(y,dydx,n,*x,h,*ytemp,*yerr,derivs);
		errmax = 0.0;
		for( i=0; i<n; i++ )
			errmax = DMAX(errmax, fabs((*yerr)[i]/yscal[i]));
		errmax /= eps;

		if( errmax <= 1.0 )
			break;

		htemp = SAFETY * h * pow(errmax,PSHRNK);
		h = (h >= 0.0 ? DMAX(htemp, 0.1*h) : DMIN(htemp, 0.1*h));

		xnew = (*x) + h;
		if( xnew == *x )
		{
			fprintf(stderr, "\n stepsize underflow in rkqs!!!");
			return STEPSIZE_UNDERFLOW;
		}
	}
	if( errmax > ERRCON )
		*hnext = SAFETY * h * pow(errmax, PGROW);
	else
		*hnext = 5.0 * h;

	*x += (*hdid=h);
	for( i=0; i<n; i++ )
		y[i] = (*ytemp)[i];
	
	free(ytemp);
	free(yerr);

	return INT_OK;
}

//	Algoritam rutina na RK-metodu
//	implementira osnovnu formulu metodu i pocinjuci od zavisnih varijabli y[i] u x
//	racuna y u x+h
void RKAlgorithm( Vector &y, Vector &dydx, int n, double x, double h, Vector &yout,
		   Vector &yerr, void (*derivs)(double, Vector &, Vector &) )
/*
	Dobivsi vrijednosti za n varijabli y[1..n] i njihovih derivacija 
	dydx[1..n] poznatih u x, koristi cash-Karpovu RK metodu da napreduje 
	s rijesenjem preko intervala h i vraca varijable u yout[1..n]
	Takodjer vraca lokalnu gresku zaokruzivanja u yerr
*/
{
	int		i;
	static double a2=0.2, a3=0.3, a4=0.6, a5=1, a6=0.875, 
		b21=0.2, b31=3.0/40.0, b32=9.0/40.0, b41=0.3, b42=-0.9, b43=1.2,
		b51=-11.0/54.0, b52=2.5, b53=-70.0/27.0, b54=35.0/27.0,
		b61=1631.0/55296.0, b62=175.0/512.0, b63=575.0/13824.0,
		b64=44275.0/110592.0, b65=253.0/4096.0, 
		c1=37.0/378.0, c3=250.0/621.0, c4=125.0/594.0, c6=512.0/1771.0,
		dc5=-277.00/14336.0;
	double	dc1=c1-2825.0/27648.0, dc3=c3-18575.0/48384.0,
		dc4=c4-13525.0/55296.0, dc6=c6-0.25;

	Vector	*ak2, *ak3, *ak4, *ak5, *ak6, *ytemp;
	
	ak2 = new Vector(n);
	ak3 = new Vector(n);
	ak4 = new Vector(n);
	ak5 = new Vector(n);
	ak6 = new Vector(n);
	ytemp = new Vector(n);

	for( i=0; i<n; i++ )
		(*ytemp)[i] = y[i] + b21*h*dydx[i];

	(*derivs)(x+a2*h, *ytemp, *ak2);
	for( i=0; i<n; i++ )
		(*ytemp)[i] = y[i] + h*(b31*dydx[i] + b32*(*ak2)[i]);

	(*derivs)(x+a3*h, *ytemp, *ak3);
	for( i=0; i<n; i++ )
		(*ytemp)[i] = y[i] + h*(b41*dydx[i] + b42*(*ak2)[i] + b43*(*ak3)[i]);

	(*derivs)(x+a4*h, *ytemp, *ak4);
	for( i=0; i<n; i++ )
		(*ytemp)[i] = y[i] + h*(b51*dydx[i] + b52*(*ak2)[i] + b53*(*ak3)[i] + b54*(*ak4)[i]);

	(*derivs)(x+a5*h, *ytemp, *ak5);
	for( i=0; i<n; i++ )
		(*ytemp)[i] = y[i] + h*(b61*dydx[i] + b62*(*ak2)[i] + b63*(*ak3)[i] + b64*(*ak4)[i] + b65*(*ak5)[i]);

	(*derivs)(x+a6*h, *ytemp, *ak6);
	for( i=0; i<n; i++ )
		yout[i] = y[i] + h*(c1*dydx[i] + c3*(*ak3)[i] + c4*(*ak4)[i] + c6*(*ak6)[i]);

	for( i=0; i<n; i++ )
		yerr[i] = h*(dc1*dydx[i] + dc3*(*ak3)[i] + dc4*(*ak4)[i] + dc5*(*ak5)[i] + dc6*(*ak6)[i]);

	delete ak2;
	delete ak3;
	delete ak4;
	delete ak5;
	delete ak6;
	delete ytemp;
}

int RKIntegrate( Vector &ystart, int nvar, double x1, double x2, 
	 double eps, double h1, double hmin, int *nok, int *nbad,
	 void (*derivs)(double, Vector &, Vector &),
	 void (*RKStepper)(Vector &, Vector &, int, double *, double, double, 
										 Vector &, double *, double *, void (*)(double, Vector &, Vector &)),
	 int kmax, int *kount, Vector &xp, Matrix &yp, double dxsav)
//	ystart[] - vektor poèetnih vrijednosti
//  nvar  - koliko ima dif. jednadžbi
//  nok, nbad - broj dobrih i loših poduzetih koraka
//  h1 - poèetna duljina koraka
//  hmin - minimalna dozvoljena duljina koraka ( može biti 0 )
//  derivs - korisnikova rutina koja raèuna derivacije u danoj toèki
//  rkqs - stepper rutina
//  kmax - koliko maksimalno izraèunatih vrijednosti y da se zapatmi
//  dxsav - razmak izmeðu dvije vrijednosti koje su zapamæene
//		POVRATNE VRIJEDNOSTI
//  xp[1..kmax] - u kojim toèkama su izraèunate vrijednosti y
//  yp[1..nvar][1..kmax] - vrijednosti varijabli
{
	int		nstp, i;
	double	xsav, x, hnext, hdid, h;
	Vector	*yscal, *y, *dydx;

	yscal = new Vector(nvar);
	y = new Vector(nvar);
	dydx = new Vector(nvar);
	
	x=x1;
	h = SIGN(h1, x2-x1);
	*nok = (*nbad) = *kount = 0;
	for( i=0; i<nvar; i++ )
		(*y)[i] = ystart[i];
	if( kmax > 0 ) xsav = x - dxsav*2.0;

	for( nstp=1; nstp<=MAXSTP; nstp++ )
	{
		(*derivs)(x,*y,*dydx);
		for( i=0; i<nvar; i++ )
		{
			(*yscal)[i] = fabs((*y)[i]) + fabs((*dydx)[i]*h) + TINY;
			if( (*yscal)[i] < 1 )
				(*yscal)[i] = 1;
		}

		if( kmax > 0 && *kount < kmax-1 && fabs(x-xsav) > fabs(dxsav) )
		{
			xp[(*kount)] = x;
			for( i=0; i<nvar; i++ )
				yp[i][*kount] = (*y)[i];
			(*kount)++;
			xsav = x;
		}
		if( (x+h-x2)*(x+h-x1) > 0.0 ) 
			h=x2 - x;

		(*RKStepper)(*y,*dydx,nvar,&x,h,eps,*yscal,&hdid,&hnext,derivs);
		if( hdid == h)
			++(*nok);
		else
			++(*nbad);

		if( (x-x2)*(x2-x1) >= 0.0 )
		{
			for( i=0; i<nvar; i++ )
				ystart[i] = (*y)[i];
			if( kmax )
			{
				xp[(*kount)++] = x;
				for( i=0; i<nvar; i++ )
					yp[i][*kount] = (*y)[i];
			}

			free(dydx);
			free(y);
			free(yscal);
			
			return INT_OK;
		}
		if( fabs(hnext) <= hmin )
		{
			fprintf(stdout, "\n Step size too small in odeint!!!");
			printf("\n trenutni x = %12.8f", x );

			return STEPSIZE_UNDERFLOW;
		}
		h = hnext;
	}
	fprintf(stdout, "\nToo many steps in odeint!!!");
	printf("\n trenutni x = %12.8f", x );

	return TO_MANY_STEPS;
}


#define		SAFETY		0.9
#define		GROW			1.5
#define		PGROW_STIFF		-.25
#define		PSHRNK_STIFF	(-1.0/3.0)
#define		ERRCON_STIFF	0.1296
#define		SHRNK			0.5
#define		MAXTRY		500

#define		GAM		(1.0/2.0)
#define		A21		2.0
#define		A31		(48.0/25.0)
#define		A32		(6.0/25.0)
#define		C21		-8.0
#define		C31		(372/25.0)
#define		C32		(12.0/5.0)
#define		C41		(-112.0/125.0)
#define		C42		(-54.0/125.0)
#define		C43		(-2.0/5.0)
#define		B1		(19.0/9.0)
#define		B2		(1.0/2.0)
#define		B3		(25.0/108.0)
#define		B4		(125.0/108.0)
#define		E1		(17.0/54.0)
#define		E2		(7.0/36.0)
#define		E3		0.0
#define		E4		(125.0/108.0)
#define		C1X		(1.0/2.0)
#define		C2X		(-3.0/2.0)
#define		C3X		(121.0/50.0)
#define		C4X		(29.0/250.0)
#define		A2X		1.0
#define		A3X		(3.0/5.0)

#define		STIFF_RETURN  {	delete ysav;	delete g4;	delete g3;	delete g2;	delete g1;	delete err;	delete dysav; delete dfdy;	delete dfdx;	delete a;	delete []indx; }

int StiffODEIntegrate( Vector &ystart, int n, double x1, double x2, 
							double eps, double h1, double hmin, int *nok, int *nbad,
							void (*derivs)(double, Vector &, Vector &),
							void (*jacobn)(double, Vector &, Vector *, Matrix *, int),
														 int kmax, int *kount, double *xp, double **yp, double dxsav)
//	ystart[] - vektor poèetnih vrijednosti
//  n  - koliko ima dif. jednadžbi
//  nok, nbad - broj dobrih i loših poduzetih koraka
//  h1 - poèetna duljina koraka
//  hmin - minimalna dozvoljena duljina koraka ( može biti 0 )
//  derivs - korisnikova rutina koja raèuna derivacije u danoj toèki
//  rkqs - stepper rutina
//  kmax - koliko maksimalno izraèunatih vrijednosti y da se zapatmi
//  dxsav - razmak izmeðu dvije vrijednosti koje su zapamæene
//		POVRATNE VRIJEDNOSTI
//  xp[1..kmax] - u kojim toèkama su izraèunate vrijednosti y
//  yp[1..n][1..kmax] - vrijednosti varijabli
{
	int		nstp, i;
	double	xsav, x, hnext, hdid, h;
	Vector	*yscal, *y, *dydx;

	int			j, jtry, *indx;
	double	d, errmax, sxsav, htry, hstiff;
	Matrix	*a, *dfdy;
	Vector	*dfdx, *dysav, *err;
	Vector	*g1, *g2, *g3, *g4, *ysav;

	indx = new int[n];
	a = new Matrix(n,n);
	dfdx = new Vector(n);
	dfdy = new Matrix(n,n);
	dysav = new Vector(n);
	err = new Vector(n);
	g1 = new Vector(n);
	g2 = new Vector(n);
	g3 = new Vector(n);
	g4 = new Vector(n);
	ysav = new Vector(n);

	yscal = new Vector(n);
	y = new Vector(n);
	dydx = new Vector(n);
	
	x=x1;
	h = SIGN(h1, x2-x1);
	*nok = (*nbad) = *kount = 0;
	for( i=0; i<n; i++ )
		(*y)[i] = ystart[i];
	if( kmax > 0 ) 
		xsav = x - dxsav*2.0;

	for( nstp=1; nstp<=MAXSTP; nstp++ )
	{
		(*derivs)(x,*y,*dydx);
		for( i=0; i<n; i++ )
		{
			(*yscal)[i] = fabs((*y)[i]) + fabs((*dydx)[i]*h) + TINY;
			if( (*yscal)[i] < 1 )
				(*yscal)[i] = 1;
		}

		if( kmax > 0 && *kount < kmax-1 && fabs(x-xsav) > fabs(dxsav) )
		{
			xp[(*kount)] = x;
			for( i=0; i<n; i++ )
				yp[i][*kount] = (*y)[i];
			(*kount)++;
			xsav = x;
		}
		if( (x+h-x2)*(x+h-x1) > 0.0 ) 
			h=x2 - x;

		htry = h;

		sxsav = x;
		for( i=0; i<n; i++ )
		{
			(*ysav)[i] = (*y)[i];
			(*dysav)[i] = (*dydx)[i];
		}

		jacobn(xsav, *ysav, dfdx, dfdy, n);

		hstiff=htry;
		for( jtry=0; jtry<MAXTRY; jtry++)
		{
			for(i=0; i<n; i++ )
			{
				for(j=0; j<n; j++ )
					(*a)[i][j] = -(*dfdy)[i][j];
				(*a)[i][i] += 1.0 / (GAM*hstiff);
			}
			LUDecomp( *a, indx, &d);
			for( i=0; i<n; i++ )
				(*g1)[i] = (*dysav)[i] + hstiff * C1X * (*dfdx)[i];

			LUBackSubst(*a, indx, *g1 );

			for(i=0; i<n; i++ )
				(*y)[i] = (*ysav)[i] + A21*(*g1)[i];

			x = sxsav + A2X * hstiff;

			(*derivs)(x, *y, *dydx);

			for( i=0; i<n; i++ )
				(*g2)[i] = (*dydx)[i] + hstiff * C2X * (*dfdx)[i] + C21*(*g1)[i] / hstiff;
			LUBackSubst(*a, indx, *g2);

			for( i=0; i<n; i++ )
				(*y)[i] = (*ysav)[i] + A31 * (*g1)[i] + A32*(*g2)[i];
			x = sxsav + A3X * hstiff;
			(*derivs)(x, *y, *dydx);

			for(i=0; i<n; i++ )
				(*g3)[i] = (*dydx)[i] + hstiff * C3X*(*dfdx)[i] + (C31*(*g1)[i] + C32*(*g2)[i]) / hstiff;
			LUBackSubst(*a, indx, *g3);

			for(i=0; i<n; i++ )
				(*g4)[i] = (*dydx)[i] + hstiff * C4X*(*dfdx)[i] + (C41*(*g1)[i] + C41*(*g2)[i] + C43*(*g3)[i]) / hstiff;
			LUBackSubst(*a, indx, *g4);
			
			for(i=0; i<n; i++ )
			{
				(*y)[i] = (*ysav)[i] + B1*(*g1)[i] + B2*(*g2)[i] + B3*(*g3)[i] + B4*(*g4)[i];
				(*err)[i] = E1*(*g1)[i] + E2*(*g2)[i] + E3*(*g3)[i] + E4*(*g4)[i];
			}

			x = sxsav + hstiff;
			if( x == sxsav )
			{
//		puts("Stepsize not significant in stiff");

				return STEPSIZE_UNDERFLOW;
			}
			
			errmax = 0.0;
			for( i=0; i<n; i++ )
				errmax = DMAX(errmax, fabs((*err)[i]/(*yscal)[i]));
			errmax /= eps;

			if( errmax <= 1.0 )
			{
				hdid = hstiff;
				hnext = (errmax > ERRCON_STIFF ? SAFETY * hstiff * pow(errmax,PGROW_STIFF) : GROW*hstiff );

				break;
			}
			else
			{
				hnext = SAFETY * hstiff * pow(errmax, PSHRNK_STIFF);
				hstiff = (hstiff >= 0.0 ? DMAX(hnext, SHRNK*hstiff) : DMIN(hnext, SHRNK*hstiff));
			}
		}
		if( jtry >= MAXTRY )
		{
//	puts("Exceeded MAXTRY in stiff");
			return	TO_MANY_STEPS;
		}

		if( hdid == h )
			++(*nok);
		else
			++(*nbad);

		if( (x-x2)*(x2-x1) >= 0.0 )
		{
			for( i=0; i<n; i++ )
				ystart[i] = (*y)[i];
			if( kmax )
			{
				xp[(*kount)++] = x;
				for( i=0; i<n; i++ )
					yp[i][*kount] = (*y)[i];
			}

			free(dydx);
			free(y);
			free(yscal);

			STIFF_RETURN;

			return INT_OK;
		}
		if( fabs(hnext) <= hmin )
		{
//			fprintf(stdout, "\n Step size too small in odeint!!!");
//			printf("\n trenutni x = %12.8f", x );

			STIFF_RETURN;

			return STEPSIZE_UNDERFLOW;
		}
		h = hnext;
	}
//	fprintf(stdout, "\nToo many steps in odeint!!!");
//	printf("\n trenutni x = %12.8f", x );

	STIFF_RETURN;
	return TO_MANY_STEPS;
}

/****************************************************************/
//							Simple ODE integration
/****************************************************************/

int rkqs( double y[], double dydx[], int n, double *x, double htry, 
					 double eps, double yscal[], double *hdid, double *hnext,
					 void (*derivs)(double, double[], double[]) )
/*	RK 5-og reda sa pracenjem lokalne greske zaokruzivanja da bi se osigurala tocnost i 
	prilagodila duljina koraka.
	ULAZ:
		vektor zavisnih varijabli  -  y[1..n]
		i njegove derivacije   -  dydx[1..n],   i to u pocetnoj tocki nezavisne varijable x
		duljina koraka(pocetna za probu)  -  htry
		zahtijevana tocnost	 -  eps
		i vektor sa kojim se skalira greska  -  yscal[1..n]
	IZLAZ:
		y i x -  zamijenjeni sa novim vrijednostima
		hdid  -  postignuta duljina koraka
		hnext -  predvidjena duljina koraka u slijedecem koraku

	(*derivs) - korisnicka rutina koja racuna derivacije (desne strane sistema jednadzbi)
	
	Primjer za derivs:

	dy1/dx = f1(x,y1,y2)
	dy2/dx = f2(x,y1,y2)

	void System(double x, double *y, double *dxdy )
	{
		dxdy[0] = f1(x, y[0], y[1] );
		dxdy[1] = f2(x, y[0], y[1] );
	}
*/
{
	int			i;
	double	errmax, h, htemp, xnew;
	double	*yerr, *ytemp;

	yerr = new double[n];
	ytemp = new double[n];
	h = htry;

	for(;;)
	{
		rkck(y,dydx,n,*x,h,ytemp,yerr,derivs);
		errmax = 0.0;
		for( i=0; i<n; i++ )
			errmax = DMAX(errmax, fabs(yerr[i]/yscal[i]));
		errmax /= eps;

		if( errmax <= 1.0 )
			break;

		htemp = SAFETY * h * pow(errmax,PSHRNK);
		h = (h >= 0.0 ? DMAX(htemp, 0.1*h) : DMIN(htemp, 0.1*h));

		xnew = (*x) + h;
		if( xnew == *x )
		{
			fprintf(stderr, "\n stepsize underflow in rkqs!!!");

			return STEPSIZE_UNDERFLOW;
		}
	}
	if( errmax > ERRCON )
		*hnext = SAFETY * h * pow(errmax, PGROW);
	else
		*hnext = 5.0 * h;

	*x += (*hdid=h);
	for( i=0; i<n; i++ )
		y[i] = ytemp[i];
	
	delete [] ytemp;
	delete []yerr;

	return INT_OK;
}

//	Algoritam rutina na RK-metodu
//	implementira osnovnu formulu metodu i pocinjuci od zavisnih varijabli y[i] u x
//	racuna y u x+h
void rkck( double y[], double dydx[], int n, double x, double h, double yout[],
		   double yerr[], void (*derivs)(double, double[], double[]) )
/*
	Dobivsi vrijednosti za n varijabli y[1..n] i njihovih derivacija 
	dydx[1..n] poznatih u x, koristi cash-Karpovu RK metodu da napreduje 
	s rijesenjem preko intervala h i vraca varijable u yout[1..n]
	Takodjer vraca lokalnu gresku zaokruzivanja u yerr
*/
{
	int		i;
	static double a2=0.2, a3=0.3, a4=0.6, a5=1, a6=0.875, 
		b21=0.2, b31=3.0/40.0, b32=9.0/40.0, b41=0.3, b42=-0.9, b43=1.2,
		b51=-11.0/54.0, b52=2.5, b53=-70.0/27.0, b54=35.0/27.0,
		b61=1631.0/55296.0, b62=175.0/512.0, b63=575.0/13824.0,
		b64=44275.0/110592.0, b65=253.0/4096.0, 
		c1=37.0/378.0, c3=250.0/621.0, c4=125.0/594.0, c6=512.0/1771.0,
		dc5=-277.00/14336.0;
	double	dc1=c1-2825.0/27648.0, dc3=c3-18575.0/48384.0,
		dc4=c4-13525.0/55296.0, dc6=c6-0.25;

	double	*ak2, *ak3, *ak4, *ak5, *ak6, *ytemp;
	
	ak2 = new double[n];
	ak3 = new double[n];
	ak4 = new double[n];
	ak5 = new double[n];
	ak6 = new double[n];
	ytemp = new double[n];

	for( i=0; i<n; i++ )
		ytemp[i] = y[i] + b21*h*dydx[i];

	(*derivs)(x+a2*h, ytemp, ak2);
	for( i=0; i<n; i++ )
		ytemp[i] = y[i] + h*(b31*dydx[i] + b32*ak2[i]);

	(*derivs)(x+a3*h, ytemp, ak3);
	for( i=0; i<n; i++ )
		ytemp[i] = y[i] + h*(b41*dydx[i] + b42*ak2[i] + b43*ak3[i]);

	(*derivs)(x+a4*h, ytemp, ak4);
	for( i=0; i<n; i++ )
		ytemp[i] = y[i] + h*(b51*dydx[i] + b52*ak2[i] + b53*ak3[i] + b54*ak4[i]);

	(*derivs)(x+a5*h, ytemp, ak5);
	for( i=0; i<n; i++ )
		ytemp[i] = y[i] + h*(b61*dydx[i] + b62*ak2[i] + b63*ak3[i] + b64*ak4[i] + b65*ak5[i]);

	(*derivs)(x+a6*h, ytemp, ak6);
	for( i=0; i<n; i++ )
		yout[i] = y[i] + h*(c1*dydx[i] + c3*ak3[i] + c4*ak4[i] + c6*ak6[i]);

	for( i=0; i<n; i++ )
		yerr[i] = h*(dc1*dydx[i] + dc3*ak3[i] + dc4*ak4[i] + dc5*ak5[i] + dc6*ak6[i]);

	delete []ak2;
	delete []ak3;
	delete []ak4;
	delete []ak5;
	delete []ak6;
	delete []ytemp;
}

int odeint( double ystart[], int nvar, double x1, double x2, 
						double eps, double h1, double hmin, int *nok, int *nbad,
						void (*derivs)(double, double[], double[]),
	 
						int rkqs( double y[], double dydx[], int n, double *x, double htry, 
											double eps, double yscal[], double *hdid, double *hnext,
											void (*derivs)(double, double[], double[]) ),
	 
						int kmax, int *kount, double *xp, double **yp, double dxsav)
//	ystart[] - vektor poèetnih vrijednosti
//  nvar  - koliko ima dif. jednadžbi
//  nok, nbad - broj dobrih i loših poduzetih koraka
//  h1 - poèetna duljina koraka
//  hmin - minimalna dozvoljena duljina koraka ( može biti 0 )
//  derivs - korisnikova rutina koja raèuna derivacije u danoj toèki
//  rkqs - stepper rutina
//  kmax - koliko maksimalno izraèunatih vrijednosti y da se zapatmi
//  dxsav - razmak izmeðu dvije vrijednosti koje su zapamæene
//		POVRATNE VRIJEDNOSTI
//  xp[1..kmax] - u kojim toèkama su izraèunate vrijednosti y
//  yp[1..nvar][1..kmax] - vrijednosti varijabli
{
	int		nstp, i;
	double	xsav, x, hnext, hdid, h;
	double	*yscal, *y, *dydx;

	yscal = new double[nvar];
	y = new double[nvar];
	dydx = new double[nvar];
	
	x=x1;
	h = SIGN(h1, x2-x1);
	*nok = (*nbad) = *kount = 0;
	for( i=0; i<nvar; i++ )
		y[i] = ystart[i];
	if( kmax > 0 ) xsav = x - dxsav*2.0;

	for( nstp=1; nstp<=MAXSTP; nstp++ )
	{
		(*derivs)(x,y,dydx);
		for( i=0; i<nvar; i++ )
		{
			yscal[i] = fabs(y[i]) + fabs(dydx[i]*h) + TINY;
			if( yscal[i] < 1 )
				yscal[i] = 1;
		}

		if( kmax > 0 && *kount < kmax-1 && fabs(x-xsav) > fabs(dxsav) )
		{
			xp[(*kount)] = x;
			for( i=0; i<nvar; i++ )
				yp[i][*kount] = y[i];
			(*kount)++;
			xsav = x;
		}
		if( (x+h-x2)*(x+h-x1) > 0.0 ) 
			h=x2 - x;

		(*rkqs)(y,dydx,nvar,&x,h,eps,yscal,&hdid,&hnext,derivs);
		if( hdid == h)
			++(*nok);
		else
			++(*nbad);

		if( (x-x2)*(x2-x1) >= 0.0 )
		{
			for( i=0; i<nvar; i++ )
				ystart[i] = y[i];
			if( kmax )
			{
				xp[(*kount)++] = x;
				for( i=0; i<nvar; i++ )
					yp[i][*kount] = y[i];
			}

			delete [] dydx;
			delete []y;
			delete []yscal;
			return INT_OK;
		}
		if( fabs(hnext) <= hmin )
		{
//			fprintf(stdout, "\n Step size too small in odeint!!!");
//			printf("\n trenutni x = %12.8f", x );

			return STEPSIZE_UNDERFLOW;
		}
		h = hnext;
	}
//	fprintf(stdout, "\nToo many steps in odeint!!!");
//	printf("\n trenutni x = %12.8f", x );

	delete [] dydx;
	delete []y;
	delete []yscal;
	
	return TO_MANY_STEPS;
}

/****************************************************************/
//               BULIRSCH-STOER METHOD
/****************************************************************/


#define	KMAXX			8
#define	IMAXX			(KMAXX+1)
#define	SAFE1			0.25
#define	SAFE2			0.7
#define	REDMAX		1.0e-5
#define	REDMIN		0.7
#define	SCALMX		0.1


int bsstep( double *y, double *dydx, int nv, double *xx, double htry,
						 double eps, double *yscal, double *hdid, double *hnext,
						 void (*derivs)(double, double[], double[]) )
{
	int			i, iq, k, kk, km, reduct, exitflag=0;
	double	eps1, errmax, fact, h, red, scale, work, wrkmin, xest;
	double	*err, *yerr, *ysav, *yseq, *x;
	
	static double epsold=-1.0, xnew;
	static double a[IMAXX+1];
	static double alf[KMAXX+1][KMAXX+1];

	static int		nseq[IMAXX+1]={2,4,6,8,10,12,14,16,18,0};
	static int		first =1, kmax, kopt;

	Matrix	d(nv,KMAXX);

	x = (double *) calloc(KMAXX, sizeof(double) );
	err = (double *) calloc( KMAXX, sizeof(double) );
	yerr = (double *) calloc(nv, sizeof(double) );
	ysav = (double *) calloc(nv, sizeof(double) );
	yseq = (double *) calloc(nv, sizeof(double) );

	if( eps != epsold )
	{
		*hnext = xnew = -1.0e29;
		eps1 = SAFE1 * eps;
		a[0] = nseq[0] + 1;

		for( k=0; k<KMAXX; k++ )
			a[k+1] = a[k] + nseq[k+1];
		
		for( iq=1; iq<KMAXX; iq++ )
		{
			for( k=0; k<iq; k++ )
				alf[k][iq] = pow(eps1, (a[k+1]-a[iq+1]) / ((a[iq+1]-a[0]+1.0)*(2*k+1)));
		}
		epsold = eps;
		for( kopt=1; kopt<KMAXX-1; kopt++ )
			if( a[kopt+1] > a[kopt]*alf[kopt-1][kopt]) 
				break;
		kmax = kopt;
	}
	h = htry;
	for( i=0; i<nv; i++ )
		ysav[i] = y[i];
	
	if( *xx != xnew || h!= (*hnext) )
	{
		first = 1;
		kopt = kmax;
	}
	reduct = 0;
	for(;;)
	{
		for( k=0; k<=kmax; k++ )
		{
			xnew =(*xx)+h;
			if( xnew == (*xx) )
			{
//				printf("Step size underflow in bsstep!!!");
				
				return	STEPSIZE_UNDERFLOW;
			}
			
			mmid(ysav, dydx, nv, *xx, h, nseq[k], yseq, derivs);

			xest=DSQR(h/nseq[k]);
			pzextr(k, xest, yseq, y, yerr, nv, &d, x);

			if( k != 0 )
			{
				errmax = TINY;
				for( i=0; i<nv; i++ )
					errmax=DMAX(errmax, fabs(yerr[i]/yscal[i]));
				errmax /= eps;
				km = k - 1;
				err[km] = pow(errmax/SAFE1, 1.0/(2*km+1));
			}
			if( k!=0 && (k >= kopt-1 || first )) 
			{
				if( errmax < 1.0 )
				{
					exitflag = 1;
					break;
				}
				if( k==kmax || k==kopt+1)
				{
					red = SAFE2 / err[km];
					break;
				}
				else if( k==kopt && alf[kopt-1][kopt] < err[km] )
				{
					red = 1.0 / err[km];
					break;
				}
				else if ( kopt == kmax && alf[km][kmax-1] < err[km] )
				{
					red = alf[km][kmax-1]*SAFE2/err[km];
					break;
				}
				else if( alf[km][kopt] < err[km] )
				{
					red = alf[km][kopt-1] / err[km];
					break;
				}
			}
		}
		if( exitflag)
			break;

		red = DMIN(red, REDMIN );
		red = DMAX(red, REDMAX);
		h *= red;
		reduct = 1;
	}
	*xx = xnew;
	*hdid = h;
	first = 0;
	wrkmin = 1.0e35;
	
	for( kk=0; kk<=km; kk++ )
	{
		fact = DMAX(err[kk], SCALMX);
		work = fact*a[kk+1];
	
		if( work < wrkmin )
		{
			scale = fact;
			wrkmin = work;
			kopt = kk+1;
		}
	}
	
	*hnext = h/scale;
	if( kopt >= k && kopt != kmax && !reduct )
	{
		fact = DMAX(scale/alf[kopt-1][kopt], SCALMX);
		if( a[kopt+1]*fact <= wrkmin )
		{
			*hnext = h/fact;
			kopt++;
		}
	}
	free(x);
	free(err);
	free(ysav);
	free(yerr);
	free(yseq);

	return INT_OK;
}

void pzextr(int iest, double xest, double yest[], double yz[], double dy[], int nv, Matrix *d, double *x )
{
	int			k1, j;
	double	q, f2, f1, delta, *c;

	c = (double *) calloc( nv, sizeof(double) );

	x[iest] = xest;	
	for( j=0; j<nv; j++ )
		dy[j] = yz[j] = yest[j];
	
	if( iest == 0 )
	{
		for(j=0; j<nv; j++ )
			(*d)[j][0] = yest[j];
		}
	else
	{
		for( j=0; j<nv; j++ )
			c[j] = yest[j];
	
		for( k1=0; k1<iest; k1++ )
		{
			delta = 1.0/(x[iest-k1-1] - xest);
			f1 = xest * delta;
			f2 = x[iest-k1]*delta;
			for( j=0; j<nv; j++ )
			{
				q = (*d)[j][k1];
				(*d)[j][k1] = dy[j];
				
				delta = c[j]-q;
				dy[j] = f1*delta;
				c[j] = f2*delta;
				yz[j] += dy[j];
			}
		}
		for( j=0; j<nv; j++ )
			(*d)[j][iest] = dy[j];
	}
	free(c);
}

void rzextr(int iest, double xest, double yest[], double yz[], double dy[], int nv, Matrix *d, double *x )
{
	int			k, j;
	double	yy, v, ddy, c, b1, b, *fx;

	fx = (double *) calloc( iest, sizeof(double) );

	x[iest] = xest;
	if( iest == 0 )
		for( j=0; j<nv; j++ )
		{
			yz[j] = yest[j];
			(*d)[j][0] = yest[j];
			dy[j] = yest[j];
		}
	else
	{
		for( k=0; k<iest-1; k++ )
			fx[k+1] = x[iest-k] / xest;
		for( j=0; j<nv; j++ )
		{
			v=(*d)[j][0];
			(*d)[j][0] = yy = c = yest[j];
			for( k=1; k<iest; k++ )
			{
				b1 = fx[k] * v;
				b = b1 - c;
				if( b ) 
				{
					b = (c - v)/b;
					ddy = c*b;
					c=b1*b;
				}
				else
					ddy=v;

				if( k != iest )
					v=(*d)[j][k];
				
				(*d)[j][k] = ddy;
				yy+=ddy;
			}
			dy[j] = ddy;
			yz[j] = yy;
		}
	}
	free(fx);
}


void mmid( double y[], double dydx[], int nvar, double xs, double htot, 
					 int nstep, double yout[],
					 void (*derivs)(double, double [], double[]) )
{
	int			n, i;
	double x, swap, h2, h, *ym, *yn;

	ym = (double *) calloc( nvar, sizeof(double) );
	yn = (double *) calloc( nvar, sizeof(double) );

	h = htot/nstep;
	for( i=0; i<nvar; i++ )
	{
		ym[i] = y[i];
		yn[i] = y[i] + h*dydx[i];
	}
	
	x = xs+h;
	(*derivs)(x, yn, yout);
	h2 = 2.0 * h;
	
	for( n=2; n<=nstep; n++ )
	{
		for ( i=0; i<nvar; i++ )
		{
			swap = ym[i] + h2*yout[i];
			ym[i] = yn[i];
			yn[i] = swap;
		}
		x += h;
		(*derivs)(x, yn, yout);
	}
	
	for( i=0; i<nvar; i++ )
		yout[i] = 0.5 * (ym[i] + yn[i] + h*yout[i]);

	free(ym);
	free(yn);
}

/*****************************************************************************************/
//								DAE sistemi ( diferencijalno-algebarski sustav jednadzbi )
/*****************************************************************************************/

#define	SIMPLE_DAE_RETURN	{ 	delete []dxdt;	delete []dxm;	delete []dxt;	delete []xt;	delete []xout;	delete []y;			delete []yout;	delete []x; }

int simple_dae_integrate( int nN, int nM, double pdXStart[], double pdYStart[],  
			double dT1, double dT2, double	dStep, double dTSave, 
			double *pdT, double **pdX, double **pdY, int *nKount,
			void (*pfSysFunc)(double, double [], double [], double[], double[]) )
{
	int			i, kount, nStepPerSave, num;
	long		nstep,k;
	double	t, tsav, hh, th, h6;
	double	*x, *y, *dxm, *dxt, *xt, *yout, *dxdt,*xout;

	dxdt = new double[nN];
	dxm	 = new double[nN];
	dxt  = new double[nN];
	xt   = new double[nN];
	xout = new double[nN];

	y = new double[nM];
	yout = new double[nM];

	x = new double[nN];

	pdT[0] = dT1;
	for( i=0; i<nN; i++ )
	{
		x[i] = pdXStart[i];
		pdX[i][0] = x[i];
	}
	for( i=0; i<nM; i++ )
	{
		y[i] = pdYStart[i];
		pdY[i][0] = pdYStart[i];
	}

	t = dT1;
	nstep = (long) ((dT2-dT1) / dStep);
	nStepPerSave = (int) (dTSave / dStep);

	hh = dStep*0.5;
	h6 = dStep / 6.0;

	num = 0;
	kount = 0;
	tsav = t;
	
	for( k=0; k<nstep; k++ )
	{
		(*pfSysFunc)(t, x, y, dxdt, yout);

		th = t + hh;

		for( i=0; i<nN; i++ )
			xt[i] = x[i] + hh * dxdt[i];

		if( simple_newton(100, nN, nM, th, xt, y, pfSysFunc) != 1 )
		{
			SIMPLE_DAE_RETURN;
			return	NO_ALG_SOLUTION;
		}
		(*pfSysFunc)(th, xt, y, dxt, yout);

		for( i=0; i<nN; i++ )
			xt[i] = x[i] + hh * dxt[i];

		if( simple_newton(100, nN, nM, th, xt, y, pfSysFunc) != 1 )
		{
			SIMPLE_DAE_RETURN;
			return	NO_ALG_SOLUTION;
		}
		(*pfSysFunc)(th, xt, y, dxm, yout );

		for( i=0; i<nN; i++ )
		{
			xt[i] = x[i] + dStep * dxm[i];
			dxm[i] += dxt[i];
		}

		if( simple_newton(100, nN, nM, th, xt, y, pfSysFunc) != 1 )
		{
			SIMPLE_DAE_RETURN;
			return	NO_ALG_SOLUTION;
		}
		(*pfSysFunc)(t+dStep, xt, y, dxt, yout );

		for( i=0; i<nN; i++ )
			xout[i] = x[i] + h6*(dxdt[i] + dxt[i] + 2.0*dxm[i]);

		if( (double)(t+dStep) == t )
		{
			printf("Stepsize too small in RKDumb!!");

			return	STEPSIZE_UNDERFLOW;
		}
		num++;

		t += dStep;
		for( i=0; i<nN; i++ )
			x[i] = xout[i];
	
		if( num == nStepPerSave )
		{
			num = 0;
			pdT[kount+1] = t;
			for( i=0; i<nN; i++ )
			{
				pdX[i][kount+1] = x[i];
				pdY[i][kount+1] = y[i];
			}
			tsav = t;
			kount++;
		}
	}

	*nKount = kount;

	SIMPLE_DAE_RETURN;
	return INT_OK;
}

#undef SIMPLE_DAE_RETURN
#define	SIMPLE_DAE_RETURN	{ 	delete dxdt;	delete dxm;	delete dxt;	delete xt;	delete xout;	delete y;			delete yout;	delete x; }

int SimpleDAEIntegrate( int nN, int nM, Vector &vecXStart, Vector &vecYStart,
					double dT1, double dT2, double	dStep, double dTSave, 
					Vector &vecT, Matrix &matX, Matrix &matY, int *nKount,
					void (*pfSysFunc)(double t, Vector &x, Vector &y, Vector &xout, Vector &yout) )
{
	int			i, kount, nStepPerSave, num;
	long		nstep,k;
	double	t, tsav, hh, th, h6;
	Vector	*x, *y, *dxm, *dxt, *xt, *yout, *dxdt,*xout;

	dxdt = new Vector(nN);
	dxm	 = new Vector(nN);
	dxt  = new Vector(nN);
	xt   = new Vector(nN);
	xout = new Vector(nN);

	y = new Vector(nM);
	yout = new Vector(nM);

	x = new Vector(nN);

	vecT[0] = dT1;
	for( i=0; i<nN; i++ )
	{
		(*x)[i] = vecXStart[i];
		matX[i][0] = (*x)[i];
	}
	for( i=0; i<nM; i++ )
	{
		(*y)[i] = vecYStart[i];
		matY[i][0] = vecYStart[i];
	}

	t = dT1;
	nstep = (long) ((dT2-dT1) / dStep);
	nStepPerSave = (int) (dTSave / dStep);

	hh = dStep*0.5;
	h6 = dStep / 6.0;

	num = 0;
	kount = 0;
	tsav = t;
	
	for( k=0; k<nstep; k++ )
	{
		(*pfSysFunc)(t, *x, *y, *dxdt, *yout);

		th = t + hh;

		for( i=0; i<nN; i++ )
			(*xt)[i] = (*x)[i] + hh * (*dxdt)[i];

		if( SimpleNewton(100, nN, nM, th, *xt, *y, pfSysFunc) != 1 )
		{
			SIMPLE_DAE_RETURN;
			return NO_ALG_SOLUTION;
		}
		(*pfSysFunc)(th, *xt, *y, *dxt, *yout);

		for( i=0; i<nN; i++ )
			(*xt)[i] = (*x)[i] + hh * (*dxt)[i];

		if( SimpleNewton(100, nN, nM, th, *xt, *y, pfSysFunc) != 1 )
		{
			SIMPLE_DAE_RETURN;
			return NO_ALG_SOLUTION;
		}
		(*pfSysFunc)(th, *xt, *y, *dxm, *yout );

		for( i=0; i<nN; i++ )
		{
			(*xt)[i] = (*x)[i] + dStep * (*dxm)[i];
			(*dxm)[i] += (*dxt)[i];
		}

		if( SimpleNewton(100, nN, nM, th, *xt, *y, pfSysFunc) != 1 )
		{
			SIMPLE_DAE_RETURN;
			return NO_ALG_SOLUTION;
		}
		(*pfSysFunc)(t+dStep, *xt, *y, *dxt, *yout );

		for( i=0; i<nN; i++ )
			(*xout)[i] = (*x)[i] + h6*( (*dxdt)[i] + (*dxt)[i] + 2.0*(*dxm)[i]);

		if( (double)(t+dStep) == t )
		{
			printf("Stepsize too small in RKDumb!!");

			return STEPSIZE_UNDERFLOW;
		}
		num++;

		t += dStep;
		for( i=0; i<nN; i++ )
			(*x)[i] = (*xout)[i];
	
		if( num == nStepPerSave )
		{
			num = 0;
			vecT[kount+1] = t;
			for( i=0; i<nN; i++ )
			{
				matX[i][kount+1] = (*x)[i];
				matY[i][kount+1] = (*y)[i];
			}
			tsav = t;
			kount++;
		}
	}
	*nKount = kount;

	SIMPLE_DAE_RETURN;
	return INT_OK;
}

#define	RKCK_RETURN	{ 	delete []ak2;	delete []ak3;	delete []ak4;	delete []ak5;	delete []ak6; delete []yout; delete []xtemp; }

int dae_rkck(	double x[], double y[], double dxdt[], int n, int m, 
								double t, double h, double xout[], double xerr[], 
								void (*pfSysFunc)(double, double[], double[], double[], double[]) )
/*
	Dobivsi vrijednosti za n varijabli x[1..n] i njihovih derivacija 
	dxdt[1..n] poznatih u t, koristi cash-Karpovu RK metodu da napreduje 
	s rijesenjem preko intervala h i vraca varijable u xout[1..n]
	Takodjer vraca lokalnu gresku zaokruzivanja u xerr
*/
{
	int		i;
	static double a2=0.2, a3=0.3, a4=0.6, a5=1, a6=0.875, 
		b21=0.2, b31=3.0/40.0, b32=9.0/40.0, b41=0.3, b42=-0.9, b43=1.2,
		b51=-11.0/54.0, b52=2.5, b53=-70.0/27.0, b54=35.0/27.0,
		b61=1631.0/55296.0, b62=175.0/512.0, b63=575.0/13824.0,
		b64=44275.0/110592.0, b65=253.0/4096.0, 
		c1=37.0/378.0, c3=250.0/621.0, c4=125.0/594.0, c6=512.0/1771.0,
		dc5=-277.00/14336.0;
	double	dc1=c1-2825.0/27648.0, dc3=c3-18575.0/48384.0,
		dc4=c4-13525.0/55296.0, dc6=c6-0.25;

	double	*ak2, *ak3, *ak4, *ak5, *ak6, *xtemp, *yout;
	
	ak2 = new double[n];
	ak3 = new double[n];
	ak4 = new double[n];
	ak5 = new double[n];
	ak6 = new double[n];
	
	yout = new double[m];
	xtemp = new double[n];

	for( i=0; i<n; i++ )
		xtemp[i] = x[i] + b21*h*dxdt[i];

	if( simple_newton(100, n, m, t+a2*h, xtemp, y, pfSysFunc) )
	{	
		RKCK_RETURN;
		return NO_ALG_SOLUTION;
	}
	(*pfSysFunc)(t+a2*h, xtemp, y, ak2, yout);

	for( i=0; i<n; i++ )
		xtemp[i] = x[i] + h*(b31*dxdt[i] + b32*ak2[i]);

	if( simple_newton(100, n, m, t+a3*h, xtemp, y, pfSysFunc) )
	{	
		RKCK_RETURN;
		return NO_ALG_SOLUTION;
	}
	(*pfSysFunc)(t+a3*h, xtemp, y, ak3, yout);

	for( i=0; i<n; i++ )
		xtemp[i] = x[i] + h*(b41*dxdt[i] + b42*ak2[i] + b43*ak3[i]);

	if( simple_newton(100, n, m, t+a4*h, xtemp, y, pfSysFunc) )
	{	
		RKCK_RETURN;
		return NO_ALG_SOLUTION;
	}
	(*pfSysFunc)(t+a4*h, xtemp, y, ak4, yout);

	for( i=0; i<n; i++ )
		xtemp[i] = x[i] + h*(b51*dxdt[i] + b52*ak2[i] + b53*ak3[i] + b54*ak4[i]);

	if( simple_newton(100, n, m, t+a5*h, xtemp, y, pfSysFunc) )
	{	
		RKCK_RETURN;
		return NO_ALG_SOLUTION;
	}
	(*pfSysFunc)(t+a5*h, xtemp, y, ak5, yout);

	for( i=0; i<n; i++ )
		xtemp[i] = x[i] + h*(b61*dxdt[i] + b62*ak2[i] + b63*ak3[i] + b64*ak4[i] + b65*ak5[i]);

	if( simple_newton(100, n, m, t+a6*h, xtemp, y, pfSysFunc) )
	{	
		RKCK_RETURN;
		return NO_ALG_SOLUTION;
	}
	(*pfSysFunc)(t+a6*h, xtemp, y, ak6, yout);

	for( i=0; i<n; i++ )
		xout[i] = x[i] + h*(c1*dxdt[i] + c3*ak3[i] + c4*ak4[i] + c6*ak6[i]);

	for( i=0; i<n; i++ )
		xerr[i] = h*(dc1*dxdt[i] + dc3*ak3[i] + dc4*ak4[i] + dc5*ak5[i] + dc6*ak6[i]);

	RKCK_RETURN;
	return INT_OK;
}

int	 dae_rkqs(	double x[], double dxdt[], double y[], int n, int m, double *t, 
								double htry, double eps, double xscal[], double *hdid, double *hnext,
								void (*pfSysFunc)(double, double[], double[], double[], double[]) )
{
	int			i, ret;
	double	errmax, h, htemp, tnew;
	double	*xerr, *xtemp;

	xerr = new double[n];
	xtemp = new double[n];
	h = htry;

	for(;;)
	{
		ret = dae_rkck(x, y, dxdt, n, m, *t, h, xtemp, xerr, pfSysFunc);
		
		if( ret == NO_ALG_SOLUTION )
		{
			// s obzirom da algoritam nije nasao algebarsko rjesenje za dani korak
			// smanjiti cemo korak postupka na pola
			h = h * 0.5;
		}
		else
		{
			errmax = 0.0;
			for( i=0; i<n; i++ )
				errmax = DMAX(errmax, fabs(xerr[i]/xscal[i]));
			errmax /= eps;

			if( errmax <= 1.0 )
				break;

			htemp = SAFETY * h * pow(errmax,PSHRNK);
			h = (h >= 0.0 ? DMAX(htemp, 0.1*h) : DMIN(htemp, 0.1*h));
		}

		tnew = (*t) + h;
		if( tnew == *t )
		{
			fprintf(stderr, "\n stepsize underflow in rkqs!!!");

			return STEPSIZE_UNDERFLOW;
		}
	}
	if( errmax > ERRCON )
		*hnext = SAFETY * h * pow(errmax, PGROW);
	else
		*hnext = 5.0 * h;

	*t += (*hdid=h);
	for( i=0; i<n; i++ )
		x[i] = xtemp[i];
	
	delete []xtemp;
	delete []xerr;

	return INT_OK;
}
/*
void odeint( double ystart[], int nvar, double x1, double x2, 
	 double eps, double h1, double hmin, int *nok, int *nbad,
	 void (*derivs)(double, double[], double[]),
	 
	 void rkqs( double y[], double dydx[], int n, double *x, double htry, 
					 double eps, double yscal[], double *hdid, double *hnext,
					 void (*derivs)(double, double[], double[]) ),
	 
		int kmax, int *kount, double *xp, double **yp, double dxsav)
*/
int dae_integrate(	int n, int m, double xstart[], double ystart[], double t1, double t2, 
								double h1, double hmin, double eps, int *nok, int *nbad,
								int kmax, int *kount, double *pdT, double **pdX, double **pdY, double dtsav,
								void (*pfSysFunc)(double, double[], double[], double[], double[]),
								int (*pfRkqs)(double x[], double dxdt[], double y[], int n, int m, double *t, 
														double htry, double eps, double xscal[], double *hdid, double *hnext,
														void (*pfSysFunc)(double, double[], double[], double[], double[])) )
//	ystart[] - vektor poèetnih vrijednosti
//  nvar  - koliko ima dif. jednadžbi
//  nok, nbad - broj dobrih i loših poduzetih koraka
//  h1 - poèetna duljina koraka
//  hmin - minimalna dozvoljena duljina koraka ( može biti 0 )
//  derivs - korisnikova rutina koja raèuna derivacije u danoj toèki
//  rkqs - stepper rutina
//  kmax - koliko maksimalno izraèunatih vrijednosti y da se zapatmi
//  dxsav - razmak izmeðu dvije vrijednosti koje su zapamæene
//		POVRATNE VRIJEDNOSTI
//  pdT[1..kmax] - u kojim toèkama su izraèunate vrijednosti y
//  pdX[1..nvar][1..kmax] - vrijednosti varijabli
{
	int		nstp, i;
	double	tsav, t, hnext, hdid, h;
	double	*xscal, *x, *y, *dxdt, *yout;

	xscal = new double(n);
	x = new double(n);
	dxdt = new double(n);
	yout = new double[m];
	y = new double[m];
	
	t=t1;
	h = SIGN(h1, t2-t1);
	*nok = (*nbad) = *kount = 0;
	for( i=0; i<n; i++ )
		x[i] = xstart[i];
	for( i=0; i<m; i++ )
		y[i] = ystart[i];

	if( kmax > 0 ) tsav = t - dtsav*2.0;

	for( nstp=1; nstp<=MAXSTP; nstp++ )
	{
		(*pfSysFunc)(t, x, y, dxdt, yout);
		for( i=0; i<n; i++ )
		{
			xscal[i] = fabs(x[i]) + fabs(dxdt[i]*h) + TINY;
			if( xscal[i] < 1 )
				xscal[i] = 1;
		}

		if( kmax > 0 && *kount < kmax-1 && fabs(t-tsav) > fabs(dtsav) )
		{
			pdT[(*kount)] = t;
			for( i=0; i<n; i++ )
				pdX[i][*kount] = x[i];
			(*kount)++;
			tsav = t;
		}
		if( (t+h-t2)*(t+h-t1) > 0.0 ) 
			h=t2 - t;

		(*pfRkqs)(x, dxdt, y, n, m, &t, h, eps, xscal, &hdid, &hnext, pfSysFunc);
		if( hdid == h)
			++(*nok);
		else
			++(*nbad);

		if( (t-t2)*(t2-t1) >= 0.0 )
		{
			for( i=0; i<n; i++ )
				xstart[i] = x[i];
			if( kmax )
			{
				pdT[(*kount)++] = t;
				for( i=0; i<n; i++ )
					pdX[i][*kount] = x[i];
			}

			delete []dxdt;
			delete []x;
			delete []xscal;
			delete []yout;
	
			return INT_OK;
		}
		if( fabs(hnext) <= hmin )
		{
			fprintf(stdout, "\n Step size too small in odeint!!!");
			printf("\n trenutni t = %12.8f", t );

			return STEPSIZE_UNDERFLOW;
		}
		h = hnext;
	}
	fprintf(stdout, "\nToo many steps in odeint!!!");
	printf("\n trenutni t = %12.8f", t );

	delete []dxdt;
	delete []x;
	delete []xscal;

	return TO_MANY_STEPS;
}




template <class T, class T2> int SimpleInt( T& ystart, int nVar, double x1, double x2, 
																						double h, double dxsave, T& xp, T2& yp, int *nKount,
																						void (*derivs)(double, T&, T&) )
{
	int			i, kount, nStepPerSave, num;
	long		nstep,k;
	double	x, xsav;
	T				*v, *vout, *dv, *verr;
	double	*k;

// = new double(10);

	v = new T(nVar);
	dv = new T(nVar);
	vout = new T(nVar);
	verr = new T(nVar);

	for( i=0; i<nVar; i++ )
	{
		(*v)[i] = ystart[i];
		yp[i][0] = (*v)[i];
	}

	xp[0] = x1;
	x = x1;
	nstep = (long) ((x2-x1) / h);
	nStepPerSave = (int) (dxsave / h);

	num = 0;
	kount = 0;
	xsav = x;
	
	for( k=0; k<nstep; k++ )
	{
		(*derivs)(x,*v,*dv);
//RKAlgorithm(*v,*dv,nVar,x,h,*vout,*verr,derivs);

		if( (double)(x+h) == x )
		{
			printf("Stepsize too small in RKDumb!!");

			return STEPSIZE_UNDERFLOW;
		}
		num++;

		x += h;
		for( i=0; i<nVar; i++ )
			(*v)[i] = (*vout)[i];
	
		if( num == nStepPerSave )
		{
			num = 0;
			xp[kount+1] = x;
			for( i=0; i<nVar; i++ )
			{
				yp[i][kount+1] = (*v)[i];
			}
			xsav = x;
			kount++;
		}
	}

	*nKount = kount;

	delete v;
	delete dv;
	delete vout;

	return INT_OK;
}

template int SimpleInt<double *, double **>(double *,int,double,double,double,double,double *,double * *,int *);
template int SimpleInt<Vector &,Matrix &>(Vector &,int,double,double,double,double,Vector &,Matrix &, int *);

