#define CONVERGED		0
#define	NO_CONVERG	1	
 
int simple_newton( int nTrial, int nDim, double adXStart[], void (*pfFunc)(double[], double[]) );
int	simple_newton( int nTrial, int nN, int nM, double t, double X[], double YStart[], 
								void (*pfFunc)(double, double[], double[], double[], double[]) );

void		line_search( int n, double xold[], double fold, double grad[], double p[], double x[], 
							double *f, double stpmax, int *check, double (*func)(double []) );
int			global_newton( double x[], int n, void (*vecfunc)(double[], double[]) );
double	fmin( double x[] );

int	broydn( double x[], int n, void (*vecfunc)(double[], double[]) );

void findjac( int nDim, double adX[], double adFuncVal[], Matrix &Jacobian, 
						  void (*pfVecFunc)(double[], double[]) );
void findjac_dae( int nN, int nM, double t, double adX[], double adY[], double adFuncVal[], Matrix &Jacobian, 
								  void (*pfVecFunc)(double, double[], double[], double[], double[]) );



int		SimpleNewton( int nTrialNum, int nDim, Vector *vecX, 
									    void (*pfVecFunc)( Vector&, Vector&) );
int		SimpleNewton( int nTrial, int nN, int nM, double t, Vector &X, Vector &YStart, 
								void (*pfSysFunc)(double, Vector &, Vector &, Vector &, Vector &) );

void	FindJacobian( int nDim, double t, Vector &vecX, Matrix &Jacobian, 
									  void (*pfVecFunc)(double, Vector&, Vector&) );

void	FindJacobian( int nDim, Vector &vecX, Vector &vecFuncVal, Matrix &Jacobian, 
										void (*pfVecFunc)( Vector&, Vector&) );

void		LineSearch( int n, Vector &xold, double fold, Vector &g, Vector &p, Vector &x, 
										double *f, double stpmax, int *check, double (*func)(Vector &) );
void		GlobalNewton( Vector &x, int n, int *check, void (*vecfunc)(Vector&, Vector&) );
double	FMin( Vector &x );

int	Broydn( Vector &x, int n, void (*vecfunc)(Vector&, Vector&) );
