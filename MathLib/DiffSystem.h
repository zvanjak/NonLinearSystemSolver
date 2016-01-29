#include "MLibBasic.h"

class	DiffSystem
{
	public:
		DiffSystem();
		DiffSystem( int nDim, int nParam, 
								void (*pfSysFunc)(double,Vector&,Vector&,Vector*),
								void (*pfJacFunc)(double,Vector&,Vector&,Matrix*),
								Vector *vecStartVal, Vector *vecSystParam );
		~DiffSystem();

	private:
		int		m_nN;			// broj zavisnih varijabli sustava
		int		m_nP;			// broj parametara sustava

		enum	{ SYS_DEFINED, SYS_UNDEFINED }	m_nSysDefined;

		Vector	*m_vecStartVal;		// pokazivaè na niz poèetnih vrijednosti
		Vector	*m_vecSysParam;		// pokazivaè na vrijednosti parametara sustava

		Vector	*m_vecRay;				// sadrži vektor duž kojega se traže bifurkacije
		Vector	*m_vecStartPoint;	// sadrži poèetnu toèku od koje kreæe traženje bifurkacije

		// pointer na funkciju koja definira jednadžbe sustava
		void	(*m_pfSysFunc)(double t, Vector &X, Vector &P, Vector *DyDx );		
		// pointer na funkciju koja raèuna Jacobian sustava u toèki zadanoj sa X i vrijednostima parametra P
		void	(*m_pfJacFunc)(double t, Vector &X, Vector &P, Matrix *mJac);			

	public:
		int		GetSysDim( void )		{	return m_nN; };
		void	SetSysDim( int nDim )	{ m_nN = nDim; };
		int		GetSysParNum( void )	{ return m_nP; };
		void	SetSysParNum( int nPar ) { m_nP = nPar; };

		int		CalcEqvSolution( Vector *X, Vector &P );
		void	EqvFunc( double t, Vector &X, Vector *dydx );

/******************************************************************************/
//						Rutine za traženje rješenja sustava jednadžbi
/******************************************************************************/
		
		void	FindJacobian( int nDim, Vector &vecX, Vector &vecFuncVal, Matrix *matJacobian, 
								void (DiffSystem::*pfVecFunc)(double, Vector&, Vector*) );
		void	LineSearch( int nDim, Vector &xOld, double fOld, Vector &Grad, Vector &p, Vector &x, 
								double *dFuncVal, double StepMax, int *check, void (DiffSystem::*vecfunc)(double, Vector&, Vector*) );
		
		int		NewtonRaphson( int nTrialNum, Vector *vecX, int nDim, 
								void (DiffSystem::*pfVecFunc)(double, Vector&, Vector*) );
		int		GlobalNewton( Vector *x, int nDim, int *check, 
								void (DiffSystem::*pfVecfunc)(double, Vector&, Vector*) );

/******************************************************************************/
//						Rutine za odreðivanje stabilnosti sustava
/******************************************************************************/
		
		void	CalcJacobian( double t, Vector &X, Vector &P, Matrix *Jacobian );
		void	CalcParameterJacobian( double t, Vector &X, Vector &P, Matrix *ParJacobian );
		
		void	CalcEigenvalues( double t, Vector &X, Vector &P, Vector *EigenReal, Vector *EigenImag );
		void	CalcEigenvalues( double t, Vector &X, Vector &P, ComplexVector *Eigen  );

		void	CalcLyapExp( void );
		void	VarFunc( double x, Vector &X, Vector &P, Vector *dydx );

/******************************************************************************/
//						Rutine za nalaženje bifurkacijskih toèaka
/******************************************************************************/

		int		LocateBifurcationPoints( Vector *vecStart, Vector *vecParamOperatingPoint=NULL, Vector *vecLeftEigen=NULL );
		void	BifFunc( double x, Vector &X, Vector *dydx );
		void	MultiParBifFunc( double x, Vector &X, Vector *dydx );
		
		int 	LocateHopfBifurcationPoints( Vector *vecStart, double *omega );
		void	HopfBifFunc( double x, Vector &X, Vector *dydx );

		int		LocateBifPointsAlongRay( Vector *vecStart, Vector *vecRay, Vector *vecLeftEigen );
		void	BifFuncAlongRay( double x, Vector &X, Vector *dydx );
		
		int		LocateHopfBifPointsAlongRay( Vector *vecStart, Vector *vecRay, Vector *vecLeftEigen );
		void	HopfBifFuncAlongRay( double x, Vector &X, Vector *dydx );

/******************************************************************************/
//									Rutine za integraciju sustava
/******************************************************************************/

		void RKStep( Vector *y, Vector *dxdy, int nDim, double *x, double htry, 
				double eps, Vector *yscal, double *hdid, double *hnext,
				void (DiffSystem::*pfFunc)(double, Vector &, Vector &, Vector *) );
		void RKAlg( Vector *y, Vector *dxdy, int nDim, double x, double h, 				
				Vector *yout, Vector *yerr,
				void (DiffSystem::*pfFunc)(double, Vector &, Vector &, Vector *) );
		void ODEIntegrate( Vector ystart, int nVar, double x1, double x2,
				double eps, double h1, double hmin, int *nok, int *nbad,
				void (DiffSystem::*pfFunc)(double, Vector &, Vector &, Vector *),
 				int kmax, int *kount, double *xp, double **yp, double dxsav );

		void BSStep( double *y, double *dydx, int nv, double *xx, double htry,
				double eps, double *yscal, double *hdid, double *hnext,
				void (*derivs)(double, double[], double[]) );
		void PolyExtr(int iest, double xest, double yest[], double yz[], 
				double dy[], int nv, Matrix *d, double *x );
		void RatExtr(int iest, double xest, double yest[], double yz[], 
				double dy[], int nv, Matrix *d, double *x );
		void ModMidPoint( double y[], double dydx[], int nvar, double xs, double htot, 
				int nstep, double yout[], void (*derivs)(double, double [], double[]) );
};

