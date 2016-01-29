#include "MLibBasic.h"

#include "vector.h"
#include "matrix.h"

#define	MAX_VAR_NAME_LEN		10

#define TRUE	1
#define FALSE	0

#define FUNC	100
#define OPER	101
#define VAR		102
#define CONST_ELEM	103
#define BRACKET	104
#define LEFT_BRACKET	105
#define RIGHT_BRACKET	106

#define ADD_PRIOR	10
#define MUL_PRIOR	11
#define POW_PRIOR	12
#define UNARY_MINUS_PRIOR		13
#define FUNC_PRIOR	14

#define FUNC_NUM	19
#define	OPER_NUM	6
#define VAR_NUM		100

#define USER_FUNC_NUM		20
#define MAX_VARS_IN_FUNC	10

#define	FUNC_EXP		0
#define	FUNC_LN			1
#define	FUNC_LOG		2
#define	FUNC_SIN		3
#define	FUNC_COS		4
#define	FUNC_TG			5
#define	FUNC_CTG		6
#define	FUNC_ARCSIN		7
#define	FUNC_ARCCOS		8
#define	FUNC_ARCTG		9
#define	FUNC_ARCCTG		10
#define	FUNC_SH			11
#define	FUNC_CH			12
#define	FUNC_TH			13
#define	FUNC_CTH		14
#define	FUNC_ARSH			15
#define	FUNC_ARCH			16
#define	FUNC_ARTH			17
#define	FUNC_ARCTH		18

#define	CERR_PARNUM		-1			// broj otvorenih i zatvorenih zagrada nije u ravnoteži
#define	CERR_NUMCONST	-2			// numerièka konstanta nije dobro napisana
#define	CERR_NO_MORE_FUNCHANDLE		-3		// nema više mjesta za handle funkcija
#define	CERR_NO_MORE_VARHANDLE		-4		// nema više mjesta za handle varijable
#define	CERR_INVALID_FUNCHANDLE		-5		// pozvano izraèunavanje funkcije sa handleom za koji ne postoji funkcija
#define	CERR_TWO_OPER			-6		// dva operatora jedan do drugoga
#define	CERR_FUNC_OPER		-7		// operator odmah poslije imena funkcije
#define	CERR_NOT_ENOUGH_MEMORY		-8
#define	CERR_CONST_VAR		-9		// varijabla odmah poslije konstante
#define	CERR_INVALID_SIGN_POS		-10		// predznak na krivom mjestu
#define CERR_BALANCE						-11
//#define	CERR_

#define	CERR_ERROR_NUM		10

int	GetString( char *str, int start );
int GetNumConst( char *str, int start );

struct	VARIABLE
{
	double	dValue;
	char		strName[MAX_VAR_NAME_LEN];
	int			bFree;
};

enum	OPER_TYPE
{
	BIN_OPER,
	UNARY_OPER,
	UNKNOWN_OPER
};

struct	OPERATOR
	{
	char	cOperName;
	int		nPriority;
	OPER_TYPE		nOperType;
};

struct	FUNCTION
{
	char	strFuncName[10];
	int		nPriority;
	int		nParNum;
};

struct	USER_FUNC
{
	char	*Rpn;
	int		bFree;
	
	int		nVarsInFunc;
	int		anFuncOfVars[MAX_VARS_IN_FUNC];
};

struct DiffEqSystem
{
	int			m_nDim;
	int			m_anFuncHandle[10];		// funkcije koje cine sistem
	
	double	m_adStartValues[10];	// pocetni uvjeti
	double	*m_pdTime;						// pokazivac na nezavisnu varijablu
	double	*m_pdVars[10];				// pokazivaci na varijable sistema
};

class	MathSpace
{
public:
	struct	VARIABLE		Vars[VAR_NUM];

private:
	struct	OPERATOR		Opers[OPER_NUM];
	struct	FUNCTION		Funcs[FUNC_NUM];
	struct	USER_FUNC		UserFuncs[USER_FUNC_NUM];
	
	char		m_szError[CERR_ERROR_NUM][100];
public:
	MathSpace();
	~MathSpace();
	
	double*	DefineVar( char *Name );
	double*	DefineVar( char *Name, int *npVarNum );
	int			DefineFunction( char *Expression );

	void		UndefineFunction( int hFunc );
	void		UndefineAllFunctions( void );
	void		UndefineAllVars( void );
	
	double	Evaluate( int FuncHandle );

	int			FuncMinN( Vector startPoint3D, int nDim, int hFuncHandle, Vector *Min );
	double	FuncValInVector( int hFuncHandle, Vector *pVert );

	void		DiffSysIntegrate( struct DiffEqSystem *DES, double t1, double t2, int nSteps, double *x, double **y );

	int			GetNumOfVars( void );
	int			GetVarNum( const char *szVarName );
	
	const char		*GetVarName( int i );
	const char		*GetVarName( double *pdVar );
	
	double	*GetVar( char *szVarName );
	double	*GetVar( int nVarNum );
	
	virtual char	*ErrorText( int nErrorNum );

private:
	OPER_TYPE	OperType( char ch );
	int				IsOper( char ch );
	int				GetPriority( char *Stack, int nPos );

	void	PrintRpn( void );
};


#define	DES_MAXDIM		20
#define	DES_MAXPAR		10

#define	DESERR_NO_EQ_CHAR			-101			// U jednadžbi mora postojati znak = !!!
#define	DESERR_VAR_DEF_UNABLE	-102			// Ne mogu definirati varijablu !!!
#define	DESERR_SYN_ERR_START_VAL	-103		// Greška kod postavljanja poèetnog uvjeta (mora biti var=num.konst.)!!!
#define	DESERR_SYN_ERR_PAR_VAL		-104		// Greška kod postavljanja vrijednosti parametra
#define	DESERR_START_VAR_NAME_WRONG	-105		// Pogrešno ime varijable kod zadavanja poèetnog uvjeta
#define	DESERR_PAR_VAR_NAME_WRONG		-106		// Pogrešno ime varijable kod zadavanja parametra
#define	DESERR_NOT_ENOUGH_MEMORY		-107		// Nema više memorije
#define	DESERR_STEPSIZE_SMALL				-108		// Korak postupka premali
#define	DESERR_TO_MANY_STEPS				-109		// Previše koraka
#define	DESERR_NOT_ENOUGH_START_COND	-110	// Nije zadan dovoljan broj poèetnih uvjeta
#define	DESERR_INTERVAL_WRONG					-111	// Greška kod zadavanja intervala integracije
#define	DESERR_NOT_ENOUGH_PAR_VAL			-112
#define DESERR_CALC_LYAP_NO_CONVERGENCE		-113	// rutina za odreðivanje Lyap. exp. nije konvergirala
#define DESERR_VAR_NAME_WRONG					-114
//#define DESERR_

#define DESERR_ERROR_NUM				13

class	DiffEqvSystem : public	MathSpace
{
public:
	DiffEqvSystem();
	~DiffEqvSystem();

private:
	int			m_nN;
	int			m_nP;
	
	char		m_szErrorDES[DESERR_ERROR_NUM][100];

	char		m_szIndepVar[MAX_VAR_NAME_LEN];

	char		*m_lpszEq[DES_MAXDIM];				// pokazivaci na stringove koji sadrze jednadzbe sistema
	char		*m_lpszStartVal[DES_MAXDIM];	// pokazivaci na stringove koji sadrze pridjeljivanje poc. vrijednosti
	char		*m_lpszParamVal[DES_MAXDIM];
	
	int			m_anFuncHandle[DES_MAXDIM];		// funkcije koje cine sistem
	double	m_adStartValues[DES_MAXDIM];	// pocetni uvjeti
	double	m_adParValues[DES_MAXPAR];		// zadane vrijednosti parametara
	
	double	*m_pdTime;								// pokazivac na nezavisnu varijablu
	double	*m_pdVar[DES_MAXDIM];			// pokazivaci na varijable sistema
	double	*m_pdParam[DES_MAXPAR];		// pokazivaci na parametre sistema

	Vector	*m_vecRay;					//
	Vector	*m_vecStartPoint;		//

public:
	int		InputDES( char *lpszEquations, char *lpszIndepVar, char *lpszStartVal, char *lpszParam );
	int		ParseDES( char *lpszEquations, char *lpszIndepVar, char *lpszStartVal, char *lpszParam );
	int		GetData( void );

	int		GetDim() { return m_nN; };
	int		GetParNum() { return m_nP; };

	const char	*GetSysVarName( int i );
	const char	*GetParVarName( int i );

	int		IsSystemVar( char *szVarName );
	int		IsParameterVar( char *szVarName );
	int		IsSystemVar( int nVarNum );
	int		IsParameterVar( int nVarNum );
	
	int		GetOrderOfSysVar( char *szVarName );
	int		GetOrderOfSysVar( int nVarNum );
	int		GetOrderOfParVar( char *szParName );
	int		GetOrderOfParVar( int nParNum );

	int		FillVarValuesFromString( const char *szEq, Vector &VarVal );
	int		FillParValuesFromString( const char *szEq, Vector &VarVal );
	int		FillMixedValuesFromString( const char *szEq, Vector &SysVal, Vector &ParVal );

	double	GetStartValue( int i );
	double	GetParamValue( int i );
	int			SetInitialCond( Vector &vecInitCond );
	int			SetParamValues( Vector &vecParamVal );

	char*	ErrorText( int nErrorNum );
	
	void	SystemFunc( double x, Vector &X, Vector &Par, Vector *dydx );
	void	CalcJacobian( double t, Vector &vecX, Vector &vecPar, Matrix *Jacobian );
	void	CalcParameterJacobian( double t, Vector &X, Vector &P, Matrix *ParJacobian );

	int		CalcEqvSolution( Vector *X, Vector &P );
	void	EqvFunc( double t, Vector &X, Vector *dydx );

/******************************************************************************/
//						Rutine za odreðivanje stabilnosti sustava
/******************************************************************************/

	void	CalcEigenvalues( double t, Vector &X, Vector &P, Vector *EigenReal, Vector *EigenImag );
	void	CalcEigenvalues( double t, Vector &X, Vector &P, ComplexVector *Eigen  );

	int		CalcLyapExp( Vector &vecInitCond, Vector &vecPar, Vector &vecLyapExp, double T,
					double hMin=1e-20, double eps=1e-4, int nMaxIterNum=10000, double Er=0.5e-4, double Ea=0.0);
	void	VarFunc( double x, Vector &X, Vector &P, Vector *dydx );

//	ove rutine su nam potrebne kod integracije varijacijske funkcije sustava
	int		RKStep( Vector *y, Vector *vecPar, Vector *dxdy, int nDim, double *x, double htry, 
					double eps, Vector *yscal, double *hdid, double *hnext,
					void (DiffEqvSystem::*pfFunc)(double, Vector &, Vector &, Vector *) );
	void	RKAlg( Vector *y, Vector *vecPar,Vector *dxdy, int nDim, double x, double h, 				
					Vector *yout, Vector *yerr,
					void (DiffEqvSystem::*pfFunc)(double, Vector &, Vector &, Vector *) );
	int		ODEIntegrate( Vector ystart, Vector *vecPar, int nVar, double x1, double x2,
					double eps, double h1, double hmin, int *nok, int *nbad,
					void (DiffEqvSystem::*pfFunc)(double, Vector &, Vector &, Vector *),
 					int kmax, int *kount, double *xp, double **yp, double dxsav );

/******************************************************************************/
//						Rutine za nalaženje bifurkacijskih toèaka
/******************************************************************************/

	int		LocateBifurcationPoints( Vector *vecStart, Vector *vecParamOperatingPoint=NULL, Vector *vecLeftEigen=NULL );
	void	BifFunc( double x, Vector &X, Vector *dydx );
	void	MultiParBifFunc( double x, Vector &X, Vector *dydx );

	int 	LocateHopfBifurcationPoints( Vector &vecStart, double *omega );
	void	HopfBifFunc( double x, Vector &X, Vector *dydx );

	int		LocateBifPointsAlongRay( Vector *vecStart, Vector *vecRay, Vector *vecLeftEigen );
	void	BifFuncAlongRay( double x, Vector &X, Vector *dydx );

	int		LocateHopfBifPointsAlongRay( Vector *vecStart, Vector *vecRay, Vector *vecLeftEigen );
	void	HopfBifFuncAlongRay( double x, Vector &X, Vector *dydx );

/******************************************************************************/
//						Rutine za traženje rješenja sustava jednadžbi
/******************************************************************************/

	void	FindJacobian( int nDim, Vector &vecX, Vector *vertFuncVal, Matrix *Jacobian, 
						void (DiffEqvSystem::*pfVecFunc)(double, Vector&, Vector*) );
	void	LineSearch( int n, Vector &xold, double fold, Vector &g, Vector &p, 
						Vector &x, double *f, double stpmax, int *check, 
						void (DiffEqvSystem::*pfVecfunc)(double, Vector&, Vector*) );
	int		Newton( Vector &x, int n, int *check, 
						void (DiffEqvSystem::*pfVecfunc)(double, Vector&, Vector*) );

/******************************************************************************/
//									Rutine za integraciju sustava
/******************************************************************************/

	void	RKAlg( Vector &y, Vector &dydx, double x, double h, Vector &yout, Vector &yerr );
	int		RKStep( Vector &y, Vector &dydx, double *x, double htry, 
						double eps, Vector &yscal, double *hdid, double *hnext );
	int		Integrate( double x1, double x2, double eps, double h1, 
						double hmin, int *nok, int *nbad, int *kount, double dxsav,
						Vector *vecX, Matrix *matY );
};

