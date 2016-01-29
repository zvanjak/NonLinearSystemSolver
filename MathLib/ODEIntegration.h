#define SAFETY	0.9
#define PGROW	-0.2
#define PSHRNK	-0.25
#define ERRCON	1.89e-4

#define MAXSTP	500000
#define TINY	1e-30

// Povratne vrijednosti funkcija za integraciju
#define		INT_OK								0
#define		STEPSIZE_UNDERFLOW		-1
#define		TO_MANY_STEPS					-2
#define		NO_ALG_SOLUTION				-3


int simple_integrate( double ystart[], int nvar, double x1, double x2, 
			double h, double dxsave, 	double *xp, double **yp, int *nKount,
			void (*derivs)(double, double [], double[]) );

int SimpleODEIntegrate( Vector &ystart, int nvar, double x1, double x2, 
														double h, double dxsave,Vector &xp, Matrix &yp, int *nKount,
														void (*derivs)(double, Vector &, Vector &) );
 														

int	 rkqs( double y[], double dydx[], int n, double *x, double htry, 
					 double eps, double yscal[], double *hdid, double *hnext,
					 void (*derivs)(double, double[], double[]) );

void rkck( double y[], double dydx[], int n, double x, double h, double yout[],
						double yerr[], void (*derivs)(double, double[], double[]) );

int  odeint( double ystart[], int nvar, double x1, double x2, 
								double eps, double h1, double hmin, int *nok, int *nbad,
								void (*derivs)(double, double[], double[]),
								int rkqs( double y[], double dydx[], int n, double *x, double htry, 
													double eps, double yscal[], double *hdid, double *hnext,
													void (*derivs)(double, double[], double[]) ),
								int kmax, int *kount, double *xp, double **yp, double dxsav);


int RKStepper( Vector &y, Vector &dydx, int n, double *x, double htry, 
								double eps, Vector &yscal, double *hdid, double *hnext,
								void (*derivs)(double, Vector &, Vector &) );

void RKAlgorithm( Vector &y, Vector &dydx, int n, double x, double h, Vector &yout,
									Vector &yerr, void	(*derivs)(double, Vector &, Vector &) );

int	 RKIntegrate( Vector &ystart, int nvar, double x1, double x2, 
									double eps, double h1, double hmin, int *nok, int *nbad,
									void (*derivs)(double, Vector &, Vector &),
									void (*rkqs)(Vector &, Vector &, int, double *, double, double, 
														Vector &, double *, double *, void (*)(double, Vector &, Vector &)),
									int kmax, int *kount, Vector &xp, Matrix &yp, double dxsav);


/*void	stiff( Vector &y, Vector &dydx, int n, double *x, double htry, double eps,
  					 Vector &yscal, double *hdid, double *hnext,
						 void (*derivs)(double, Vector &, Vector &) );*/
int StiffODEIntegrate( Vector &ystart, int n, double x1, double x2, 
												double eps, double h1, double hmin, int *nok, int *nbad,
												void (*derivs)(double, Vector &, Vector &),
												void (*jacobn)(double, Vector &, Vector *, Matrix *, int),
																	int kmax, int *kount, double *xp, double **yp, double dxsav);



int	 bsstep( double *y, double *dydx, int nv, double *xx, double htry,
						double eps, double *yscal, double *hdid, double *hnext,
						void (*derivs)(double, double[], double[]) );

void pzextr(int iest, double xest, double yest[], double yz[], 
						double dy[], int nv, Matrix *d, double *x );

void rzextr(int iest, double xest, double yest[], double yz[], 
						double dy[], int nv, Matrix *d, double *x );

void mmid(double y[], double dydx[], int nvar, double xs, double htot, 
					int nstep, double yout[], void (*derivs)(double, double [], double[]) );



int simple_dae_integrate( int nN, int nM, double pdXStart[], double pdYStart[],  
					double dT1, double dT2, double	dStep, double dTSave, 
					double *pdT, double **pdX, double **pdY, int *nKount,
					void (*pfSysFunc)(double t, double x[], double y[], double xout[], double yout[]) );

int SimpleDAEIntegrate( int nN, int nM, Vector &vecXStart, Vector &vecYStart,
					double dT1, double dT2, double	dStep, double dTSave, 
					Vector &vecT, Matrix &matX, Matrix &matY, int *nKount,
					void (*pfSysFunc)(double t, Vector &x, Vector &y, Vector &xout, Vector &yout) );

int dae_rkck(	double x[], double y[], double dxdt[], int n, int m, 
								double t, double h, double xout[], double xerr[], 
								void (*pfSysFunc)(double, double[], double[], double[], double[]) );

int	dae_rkqs(	double x[], double dxdt[], double y[], int n, int m, double *t, 
							double htry, double eps, double xscal[], double *hdid, double *hnext,
							void (*pfSysFunc)(double, double[], double[], double[], double[]) );

int	dae_integrate(	int n, int m, double xstart[], double ystart[], double t1, double t2, 
								double h, double hmin, double eps, int *nok, int *nbad,
								int kmax, int *kount, double *pdT, double **pdX, double **pdY, double dxsav,
								void (*pfSysFunc)(double, double[], double[], double[], double[]),
								int (*pfRkqs)(double x[], double dxdt[], double y[], int n, int m, double *t, 
														double htry, double eps, double xscal[], double *hdid, double *hnext,
														void (*pfSysFunc)(double, double[], double[], double[], double[])) );



/*void simple_integrate( double ystart[], int nvar, double x1, double x2, 
			double h, double dxsave, 	double *xp, double **yp, int *nKount,
			void (*derivs)(double, Vector &, Vector &) );

void SimpleODEIntegrate( Vector &ystart, int nvar, double x1, double x2, 
														double h, double dxsave,
														void (*derivs)(double, Vector &, Vector &),
 														double *xp, double **yp, int *nKount );
*/

template <class T, class T2> int SimpleInt( T& ystart, int nVar, double x1, double x2, double h, double dxsave,
																	T& xp, T2& yp, int *nKount,
																	void (*derivs)(double, T&, T&) );

