#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>
#include <time.h>

#include	"mathlib.h"

DiffSystem::DiffSystem()
{
	m_nN = 0;
	m_nP = 0;
	m_nSysDefined = SYS_UNDEFINED;

	m_vecStartVal = NULL;
	m_vecSysParam = NULL;

	m_vecRay = NULL;
	m_vecStartPoint = NULL;

	m_pfSysFunc = NULL;
	m_pfJacFunc = NULL;
}

DiffSystem::DiffSystem( int nDim, int nParam, 
			void (*pfSysFunc)(double,Vector&,Vector&,Vector*),
			void (*pfJacFunc)(double,Vector&,Vector&,Matrix*),
			Vector *vecStartVal, Vector *vecSystParam )
{
	m_pfSysFunc = pfSysFunc;
	m_pfJacFunc = pfJacFunc;

	SetSysDim(nDim);
	SetSysParNum(nParam);

	m_vecStartVal = new Vector(vecStartVal->GetDim());
	*m_vecStartVal = *vecStartVal;
	
	m_vecRay = NULL;
	m_vecStartPoint = NULL;

	m_vecSysParam = new Vector(vecSystParam->GetDim());
	*m_vecSysParam = *vecSystParam;

	m_nSysDefined = SYS_DEFINED;
}

DiffSystem::~DiffSystem()
{
	delete m_vecStartVal;
	delete m_vecSysParam;

	if( m_vecRay != NULL )
		delete m_vecRay;
	if( m_vecStartPoint != NULL )
		delete m_vecStartPoint;
}

int	DiffSystem::CalcEqvSolution( Vector *vecX, Vector &vecP )
{
	// raèuna ravnotežno rješenje sustava; za vrijednosti parametara zadanih sa vecP,
	// i zadanom poèetnom toèkom za traženje vecX
	// PARAMETRI
	//		vecX - poèetna vrijednost od koje kreæe traženje, u vecX se vraæa naðena toèka
	//		vecP - vrijednosti parametara sustava za koje se traži rav. rješenje
	// POVRATNA VRIJEDNOST
	//		ret - 0 ako je ravnotežna toèka uspješno pronaðena
	int			i, nDim, ret, check;
	Vector	*val;

	nDim = GetSysDim();
	val = new Vector( nDim+GetSysParNum() );

	for( i=0; i<nDim; i++ )
		(*val)[i] = (*vecX)[i];
	for( i=0; i<GetSysParNum(); i++ )
		(*val)[i+nDim] = vecP[i];

	ret = GlobalNewton(val, nDim, &check, &DiffSystem::EqvFunc );
	
	if( ret == 0 )
	{
		for( i=0; i<nDim; i++ )
			(*vecX)[i] = (*val)[i];
	}
	delete val;

	return ret;
}

void DiffSystem::EqvFunc( double t, Vector &X, Vector *dydx )
{
	// Raèuna vrijednosti funkcija sustava za odreðenu vrijednost varijabli sustava i parametara
	// Predaje se rutini GlobalNewton kao funkcija èije se nule traže kog traženja ravnotežnog rješenja
	// - potrebna je zato što GlobalNewton traži nule funkcije kojoj se kao parametar predaje samo 
	//   jedan vektor, dok mi imamo dva, vektor varijabli sustava i vektor parametara
	// PARAMETRI
	//	X - vektor koji sadrži i vrijednosti varijabli sustava i vrijednosti parametara
	//  *dydx - vektor izraèunatih desnih strana jednadžbi
	int			i, nDim=GetSysDim();
	Vector	myX(nDim), myP(GetSysParNum());

	for( i=0; i<nDim; i++ )
		myX[i] = X[i];
	for( i=0; i<GetSysParNum(); i++ )
		myP[i] = X[i+nDim];

	t = 0;
	(*m_pfSysFunc)(t, myX, myP, dydx );
}

void DiffSystem::CalcJacobian( double t, Vector &X, Vector &P, Matrix *Jacobian )
{
	// numerièki izraèunava jacobian sistema u zadanoj toèci
	//	X - toèka u kojoj se raèuna
	//	P - vrijednost parametara za koje se raèuna
	//	Jacobian - pointer na matricu u koju æe se smjestiti rezultat
	int			i, j, nDim;
	double	h, temp;
	Vector	*f1, *f2;

	if( m_pfJacFunc == NULL )
	{
		nDim = GetSysDim();
		f1 = new Vector(nDim);		//	ovdje æemo smjestiti vrijednosti u x1
		f2 = new Vector(nDim);		//  a ovdje u x2
		
		(*m_pfSysFunc)(t,X,P,f1);		// raèunamo vrijednost funkcije u x1

		for( j=0; j<nDim; j++ )
		{
			temp = X[j];
			h = 1e-5 * fabs(temp);		// ovdje odredjujemo najmanji h uz koji
			if( h == 0.0 )						// æemo zadržati toènost numerièki odreðene
				h = 1e-5;								// derivacije

			X[j] = temp + h;					// sada raèunamo koordinate toèke x2
			h = X[j] - temp;

			(*m_pfSysFunc)(t,X,P,f2);		// raèunamo vrijednost funkcije u x2
			X[j] = temp;

			for( i=0; i<nDim; i++ )
				(*Jacobian)[i][j] = ((*f2)[i] - (*f1)[i]) / h;
		}
		
		delete f1;
		delete f2;
	}
	else
		(*m_pfJacFunc)(t,X,P,Jacobian);
}

void DiffSystem::CalcParameterJacobian( double t, Vector &X, Vector &P, Matrix *matParJac )
{
	// Raèuna jakobijan sustava s obzirom na vektor parametara
	//	X - toèka u kojoj se raèuna
	//	P - vrijednost parametara za koje se raèuna
	//	Jacobian - pointer na matricu u koju æe se smjestiti rezultat
	int			i, j, nDim, nParNum;
	double	h, temp;
	Vector	*f1, *f2;

	nDim = GetSysDim();
	nParNum = GetSysParNum();
	
	f1 = new Vector(nDim);		//	ovdje æemo smjestiti vrijednosti u x1
	f2 = new Vector(nDim);		//  a ovdje u x2
	
	(*m_pfSysFunc)(t,X,P,f1);		// raèunamo vrijednost funkcije u x1

	for( j=0; j<nParNum; j++ )
	{
		temp = P[j];
		h = 1e-5 * fabs(temp);		// ovdje odredjujemo najmanji h uz koji
		if( h == 0.0 )						// æemo zadržati toènost numerièki odreðene
			h = 1e-5;								// derivacije

		P[j] = temp + h;					// sada raèunamo koordinate toèke x2
		h = P[j] - temp;

		(*m_pfSysFunc)(t,X,P,f2);		// raèunamo vrijednost funkcije u x2
		P[j] = temp;

		for( i=0; i<nDim; i++ )
			(*matParJac)[i][j] = ((*f2)[i] - (*f1)[i]) / h;
	}
	
	delete f1;
	delete f2;
}

void	DiffSystem::CalcEigenvalues( double t, Vector &X, Vector &P, Vector *EigenReal, Vector *EigenImag )
{
	// Raèuna svojstvene vrijednosti sustava
	// PARAMETRI
	//   X - vektor varijabli sustava u kojima se raèunaju sv. vrijednosti
	//   P - vektor parametara sustava
	// Svojstvene vrijednosti se vraæaju u EigenReal (realni dio) i EigenImag(imag. dio)
	Matrix		*mJac, *mHess;
	int				nDim;

	nDim = GetSysDim();
	mJac = new Matrix(nDim, nDim);
	mHess = new Matrix(nDim, nDim);
	
	if( m_pfJacFunc != NULL )		// znaèi da je definirana funkcija koja raèuna jacobian
		(*m_pfJacFunc)(t,X,P,mJac);
	else
		CalcJacobian(t,X,P,mJac);

	(*mHess) = ReductionToHess(*mJac);		// najprije vršimo redukciju matrice na Hessenbergovu formu
	HessEigenvalues(*mHess, EigenReal, EigenImag);
}

void	DiffSystem::CalcEigenvalues( double t, Vector &X, Vector &P, ComplexVector *Eigen  )
{
	// Raèuna svojstvene vrijednosti sustava isto kao i gornja funkcija, ali rezultat vraæa u 
	// jedinstvenom ComplexVector-u
	Vector	EReal(GetSysDim()), EImag(GetSysDim());

	CalcEigenvalues(t, X, P, &EReal, &EImag);

	ComplexVector	compl(EReal, EImag );
	*Eigen = compl;
}

void	DiffSystem::CalcLyapExp( void )
{
	// Raèuna sve Lyapunovljeve eksponente zadanog sustava, za vrijednost poèetnih uvjeta zadanih
	// u èlanskoj varijabli sustava m_vecStartVal i vrijednosti parametara u m_vecSysParam
	Matrix		*matU, *matX, *matV;
	Vector		*vecSum, *vecLambda, *vecLambdaOld, *vecStartVal;
	int				nDim, i, j, k, l, nIterNumMax, nNumOK, nNumBad, nKount, nIterNum;
	double		Er, Ea, T;
	double		eps, h, hmin, t1, t2, scalProd, norm, *pdXp, **pdYp;

	nDim = GetSysDim();

	Er =0.5e-5;
	Ea = 0;
	T = 0.5;
	nIterNumMax = 5500;
	
	pdXp = (double *) calloc(20, sizeof(double) );
	pdYp = (double **) calloc(nDim+nDim*nDim, sizeof(double *) );
	for( i=0; i<nDim+nDim*nDim; i++ )
		pdYp[i] = (double *) calloc(20, sizeof(double) );

	matU			= new Matrix(nDim, nDim);
	matX			= new Matrix(nDim, nDim);
	matV			= new Matrix(nDim, nDim);
	vecSum		= new Vector(nDim);
	vecLambda = new Vector(nDim);
	vecLambdaOld = new Vector(nDim);
	// u vecStartVal prvi 'nDim' su poèetne vrijednosti za jednadžbe sistema
	// a ostalih nDim*nDim za poè. vrijed. varijacijske jednadžbe
	vecStartVal	 = new Vector(nDim + nDim*nDim);
	
	matU->MakeUnitMatrix();				// u[][] mora imati ortonormalne stupce
	matX->Clear();
	matV->Clear();
	for( i=0; i<nDim; i++ )
	{
		(*vecSum)[i] = 0;
		(*vecLambda)[i] = 0;
		(*vecStartVal)[i] = (*m_vecStartVal)[i];	// ovo su poèetne vrijednosti za integraciju jednadžbe sistema
		
		// a ovdje popunjavamo poèetne vrijednosti za integraciju varijacijske jednadžbe
		for( j=0; j<nDim; j++ )
			(*vecStartVal)[nDim + nDim*i + j] = (*matU)[i][j];
	}
	
	nIterNum = 0;
	t1 = 0;
	eps = 1e-6;
	h = 1e-5;
	hmin = 1e-15;
	do
	{
		nIterNum++;
		if( nIterNumMax == nIterNum )
		{
			printf("Nije postignuta konvergencija !!!");
			break;
		}

		*vecLambdaOld = *vecLambda;

		t2 = t1 + T;
		ODEIntegrate( *vecStartVal, nDim+nDim*nDim, t1, t2, eps,
			h, hmin, &nNumOK, &nNumBad, &DiffSystem::VarFunc,
			2, &nKount, pdXp, pdYp, T/2 );

		// stavljamo rezultat u matricu X
		for( i=0; i<nDim; i++ )
			for( j=0; j<nDim; j++ )
				(*matX)[i][j] = pdYp[nDim + nDim*i + j][nKount];

		for( i=0; i<nDim; i++ )
		{
			for( j=0; j<nDim; j++ )
				(*matV)[j][i] = (*matX)[j][i];		// v[][i] = x[][i]

			for( j=0; j<i; j++ )
			{
				scalProd = 0;
				for( l=0; l<nDim; l++ )
					scalProd += (*matV)[l][i] * (*matU)[l][j];

				for( k=0; k<nDim; k++ )
					(*matV)[k][i] -= scalProd * (*matU)[k][j];
			}

			norm=0;
			for( j=0; j<nDim; j++ )
				norm += (*matV)[j][i] * (*matV)[j][i];
			norm = sqrt(norm);

			for( j=0; j<nDim; j++ )
				(*matU)[j][i] = (*matV)[j][i] / norm;
			
			(*vecSum)[i] += log(norm);
			(*vecLambda)[i] = (*vecSum)[i] / (nIterNum*T);
		}

		// treba ponovo popuniti poèetne vrijednosti za sljedeæi korak integracije
		for( i=0; i<nDim; i++ )
		{
			(*vecStartVal)[i] = pdYp[i][nKount];	// ovo su poèetne vrijednosti za integraciju jednadžbe sistema
			// a ovdje popunjavamo poèetne vrijednosti za integraciju varijacijske jednadžbe
			for( j=0; j<nDim; j++ )
				(*vecStartVal)[nDim + nDim*i + j] = (*matU)[i][j];
		}
		printf("\n  %d   l1 = %12.5f     l2 = %12.5f    l3 = %12.5f", nIterNum, (*vecLambda)[0], (*vecLambda)[1], (*vecLambda)[2] );

	} while( nIterNum<5 || Norm(*vecLambdaOld - *vecLambda) > Er * Norm(*vecLambda) + Ea );

	free(pdXp);
	for(i=0; i<nDim; i++ )
		free(pdYp[i]);
	free(pdYp);

	delete matU;
	delete matV;
	delete matX;
	delete vecSum;
	delete vecLambda;
	delete vecLambdaOld;
}

void	DiffSystem::VarFunc( double t, Vector &X, Vector &P, Vector *dydx )
{
	// Raèuna vrijednosti varijacijske funkcije sustava;

	// najprije treba izraèunati derivacije funkcija sistema
	(*m_pfSysFunc)(t,X,P,dydx);

	int				nDim=GetSysDim(), i, j;
	Matrix		matJac(nDim,nDim), matX(nDim,nDim), matRez(nDim,nDim);
	
	// stavljamo pripadne vrijednosti varijabli varijacijske jednadžbe u matricu
	for( i=0; i<nDim; i++ )
		for( j=0; j<nDim; j++ )
			matX[i][j] = X[nDim + nDim*i + j];

		// izraèunamo derivacije varijacijske jednadžbe
	CalcJacobian(t,X,P,&matJac);

	matRez = matJac * matX;

	// a onda ih treba prebaciti u niz dydx
	for( i=0; i<nDim; i++ )
		for( j=0; j<nDim; j++ )
			(*dydx)[nDim + nDim*i + j] = matRez[i][j];
}

int	DiffSystem::LocateBifurcationPoints( Vector *vecStart, Vector *vecParamOperPoint, Vector *vecLeftEigen )
{
	// Raèuna toèke u kojima se pojavljuje saddle-node bifurkacija
	// PARAMETRI
	//	 vecStart - poèetne vrijednosti za iteracijski postupak
	//     vecStart[0..nDim-1] - poèetne vrijednosti varijabli sustava
	//     vecStart[nDim..nDim+ParNum-1] - poèetne vrijednosti parametara
	//	 vecParamOperPoint - vrijednosti parametara za toèku za koju se traži najbliža bif. toèka (višeparametarski sluèaj)
	//   vecLeftEigen - pokazivaè na vektor u koji æe se pohraniti izraèunati lijevi svojstveni
	//								vektor za danu bifurkacijsku toèku
	// POVRATNA VRIJEDNOST
	//	 ret = 0 za uspješno naðenu bif. toèku
	int			i, nDim, ret=-1, check;
	Vector	*val;

	if( GetSysParNum() == 1 )
	{
		nDim = GetSysDim();		
		val = new Vector(2*nDim+1);

		for( i=0; i<nDim; i++ )
			(*val)[i] = (*vecStart)[i];

		// poèetne vrijednosti za 'v'
		srand( (unsigned) time( NULL ) );
		for( i=0; i<nDim; i++ )
			(*val)[nDim+i] = ( (double) rand() ) / RAND_MAX - 0.5;
	
		(*val)[2*nDim] = (*vecStart)[nDim];

		ret = GlobalNewton(val, 2*nDim+1, &check, &DiffSystem::BifFunc );

		if( ret == 0 )
		{
			for( i=0; i<nDim; i++ )
				(*vecStart)[i] = (*val)[i];
			(*vecStart)[nDim] = (*val)[2*nDim];

			if( vecLeftEigen != NULL )
				for( i=0; i<nDim; i++ )
					(*vecLeftEigen)[i] = (*val)[i+nDim];
		}
		delete val;
	}
	else if( GetSysParNum() > 1 )
	{
		if( m_vecStartPoint != NULL )
			delete m_vecStartPoint;

		nDim = GetSysDim();		
		val = new Vector(2*nDim+GetSysParNum()+1);

		// poèetne vrijednosti za varijable sustava
		for( i=0; i<nDim; i++ )
			(*val)[i] = (*vecStart)[i];

		// poèetne vrijednosti za komponente lijevog svojstvenog vektora
		for( i=0; i<nDim; i++ )
			(*val)[nDim+i] = ( (double) rand() ) / RAND_MAX - 0.5;

		// toèka za koju tražimo najbližu bifurkacijsku toèku
		m_vecStartPoint = new Vector(GetSysParNum());
		if( vecParamOperPoint != NULL )
			*m_vecStartPoint = *vecParamOperPoint;
		else
			// ukoliko nije zadana, onda tražimo bifurkaciju najbližu ishodištu
			m_vecStartPoint->Clear();

		// a ovdje popunjavamo našu procjenu gdje bi se otprilike mogla nalaziti ta najbliža bif. toèka
		for( i=0; i<GetSysParNum(); i++ )
			(*val)[2*nDim+i] = (*vecStart)[nDim+i];
		
		// poèetna vrijednost za koeficijent k
		(*val)[2*nDim+GetSysParNum()] = 1;

		ret = GlobalNewton(val, 2*nDim+GetSysParNum()+1, &check, &DiffSystem::MultiParBifFunc );

		if( ret == 0 )
		{
			for( i=0; i<nDim; i++ )
				(*vecStart)[i] = (*val)[i];
			for( i=0; i<GetSysParNum(); i++ )
				(*vecStart)[nDim+i] = (*val)[2*nDim+i];

			if( vecLeftEigen != NULL )
				for( i=0; i<nDim; i++ )
					(*vecLeftEigen)[i] = (*val)[i+nDim];
		}
		delete val;
	}
	else
	{
		// sustav ne ovisi o parametrima, pa prema tome nema ni bifurkacije
		ret = -1;
	}

	return ret;
}

void	DiffSystem::BifFunc( double x, Vector &X, Vector *dydx )
{
	// Raèuna jednadžbe sustava za saddle-node bifurkaciju (1-parametarski sluèaj)
	//    X[0..n-1] - varijable sustava
	//    X[n..2n-1] - komponente lijevog svojstvenog vektora
	//    X[2n] - parametar sustava
	int			i, j, nDim;

	nDim = GetSysDim();
	Vector	myX(nDim), myP(GetSysParNum()), myV(nDim), myRes(nDim);
	Matrix	matJac(nDim, nDim);

	for( i=0; i<nDim; i++ )
		myX[i] = X[i];
	for( i=nDim; i<2*nDim; i++ )
		myV[i-nDim] = X[i];
	for( i=2*nDim; i<2*nDim+GetSysParNum(); i++ )
		myP[i-2*nDim] = X[i];

	(*m_pfSysFunc)(x, myX, myP, dydx);
	CalcJacobian(x, myX, myP, &matJac );

	for( i=0; i<nDim; i++ )
	{
		(*dydx)[i+nDim] = 0;
		for( j=0; j<nDim; j++ )
			(*dydx)[i+nDim] += myV[j] * matJac[j][i];
	}
	
	(*dydx)[2*nDim] = -1;
	for( j=0; j<nDim; j++ )
		(*dydx)[2*nDim] += myV[j] * myV[j];
}

void	DiffSystem::MultiParBifFunc( double x, Vector &X, Vector *dydx )
{
	// Raèuna jednadžbe sustava za saddle-node bifurkaciju (višeparametarski sluèaj)
	//		X[0..n-1] - varijable sustava
	//		X[n..2n-1] - komponente lijevog svojstvenog vektora
	//		X[2n..2n+p-1] - 'p' komponenata parametara sustava
	//		X[2n+p] - koeficijent k
	int			i, j, nDim, nParNum;
	double	dCoef;

	nDim = GetSysDim();
	nParNum = GetSysParNum();
	Vector	myX(nDim), myP(nParNum), myW(nDim), myRes(nParNum);
	Matrix	matJac(nDim, nDim), matParJac(nDim, nParNum);

	for( i=0; i<nDim; i++ )
		myX[i] = X[i];
	for( i=nDim; i<2*nDim; i++ )
		myW[i-nDim] = X[i];
	for( i=2*nDim; i<2*nDim+nParNum; i++ )
		myP[i-2*nDim] = X[i];
	
	dCoef = X[2*nDim+nParNum];

	(*m_pfSysFunc)(x, myX, myP, dydx);
	CalcJacobian(x, myX, myP, &matJac );
//	printf("\njacobian");
//	Print(matJac);
	CalcParameterJacobian(x, myX, myP, &matParJac );
//	printf("\nParametar jacobian");
//	Print(matParJac);

	for( i=0; i<nDim; i++ )
	{
		(*dydx)[i+nDim] = 0;
		for( j=0; j<nDim; j++ )
			(*dydx)[i+nDim] += myW[j] * matJac[j][i];
	}
	
	for( i=0; i<nParNum; i++ )
	{
		(*dydx)[i+2*nDim] = dCoef * ( myP[i] - (*m_vecStartPoint)[i] );
		
		// na kraju ovog izraèunavanja myRs sadrži vektor normale na bifurkacijsku hiperplohu
		myRes[i] = 0;
		for( j=0; j<nDim; j++ )
			myRes[i] += myW[j] * matParJac[j][i];

		(*dydx)[i+2*nDim] += myRes[i];
	}

//	printf("\nMyRes u MultiBifFunc");
//	Print(myRes);
	
	(*dydx)[2*nDim+nParNum] = -1;
	for( j=0; j<nParNum; j++ )
		(*dydx)[2*nDim+nParNum] += myRes[j] * myRes[j];

/*	printf("\ndydx U MultiBifFunc");
	Print(*dydx);
	getch();*/
}

int	DiffSystem::LocateBifPointsAlongRay( Vector *vecStart, Vector *vecRay, Vector *vecLeftEigen )
{
	// Traži bifurkacijske toèke duzž dane zrake
	// PARAMETRI
	//	 vecStart - poèetne vrijednosti za iteracijski postupak
	//     vecStart[0..nDim-1] - poèetne vrijednosti varijabli sustava
	//     vecStart[nDim..nDim+ParNum-1] - poèetne vrijednosti parametara
	//	 vecRay - vektor smjera zrake duž koje se traži
	//   vecLeftEigen - pokazivaè na vektor u koji æe se pohraniti izraèunati lijevi svojstveni
	//								vektor za danu bifurkacijsku toèku
	// POVRATNA VRIJEDNOST
	//	 ret = 0 za uspješno naðenu bif. toèku
	int			i, nDim, ret=-1, check;
	Vector	*val;

	// da bi tražili bifurkaciju duž zrake, sustav treba biti barem 2-parametarski
	if( GetSysParNum() > 1 )
	{
		if( vecRay == NULL )
			return -2;

		if( m_vecRay != NULL )
			delete m_vecRay;
		if( m_vecStartPoint != NULL )
			delete m_vecStartPoint;
		
		nDim = GetSysDim();		
		val = new Vector(2*nDim+1);

		m_vecRay = new Vector(GetSysParNum());
		*m_vecRay = *vecRay;
		Normalize(*m_vecRay);

		// poèetne vrijednosti za toèku od koje se kreæe u traženju su zadane u vecStart
		m_vecStartPoint = new Vector(GetSysParNum());
		for( i=0; i<GetSysParNum(); i++ )
			(*m_vecStartPoint)[i] = (*vecStart)[nDim+i];

		for( i=0; i<nDim; i++ )
			(*val)[i] = (*vecStart)[i];

		// poèetne vrijednosti za 'v'
		for( i=0; i<nDim; i++ )
			(*val)[nDim+i] = ( (double) rand() ) / RAND_MAX - 0.5;

		// uzimamo jedinièni pomak duž vektora u kojem tražimo
		(*val)[2*nDim] = 1;

		ret = GlobalNewton(val, 2*nDim+1, &check, &DiffSystem::BifFuncAlongRay );

		if( ret == 0 )
		{
			for( i=0; i<nDim; i++ )
				(*vecStart)[i] = (*val)[i];
			for( i=0; i<GetSysParNum(); i++ )
				(*vecStart)[nDim+i] = (*val)[2*nDim] * (*m_vecRay)[i];

			if( vecLeftEigen != NULL )
				for( i=0; i<nDim; i++ )
					(*vecLeftEigen)[i] = (*val)[i+nDim];
		}
		delete val;
	}
	else
		ret = -2;

	return ret;
}

void	DiffSystem::BifFuncAlongRay( double x, Vector &X, Vector *dydx )
{
	// Raèuna jednadžbe sustava za saddle-node bifurkaciju, duž vektora parametara
	int			i, j, nDim;

	nDim = GetSysDim();
	Vector	myX(nDim), myP(GetSysParNum()), myV(nDim), myRes(nDim);
	Matrix	matJac(nDim, nDim);

	for( i=0; i<nDim; i++ )
		myX[i] = X[i];
	for( i=nDim; i<2*nDim; i++ )
		myV[i-nDim] = X[i];
	
	for( i=0; i<GetSysParNum(); i++ )
		myP[i] = X[2*nDim] * (*m_vecRay)[i];

	(*m_pfSysFunc)(x, myX, myP, dydx);
	CalcJacobian(x, myX, myP, &matJac );

/*	// za rad sa desnim svojstvenim vektorom
	for( i=0; i<nDim; i++ )
	{
		(*dydx)[i+nDim] = 0;
		for( j=0; j<nDim; j++ )
			(*dydx)[i+nDim] += matJac[i][j] * myV[j];
	}
	
	(*dydx)[2*nDim] = -1;
	for( j=0; j<nDim; j++ )
		(*dydx)[2*nDim] += myV[j] * myV[j];*/

	// za rad sa lijevim svojstvenim vektorom
	for( i=0; i<nDim; i++ )
	{
		(*dydx)[i+nDim] = 0;
		for( j=0; j<nDim; j++ )
			(*dydx)[i+nDim] += myV[j] * matJac[j][i];
	}
	
	(*dydx)[2*nDim] = -1;
	for( j=0; j<nDim; j++ )
		(*dydx)[2*nDim] += myV[j] * myV[j];
}

int	DiffSystem::LocateHopfBifurcationPoints( Vector *vecStart, double *omega )
{
	// Raèuna toèke u kojima se pojavljuje Hopfova bifurkacija, 
	// PARAMETRI
	//	 vecStart - poèetne vrijednosti za iteracijski postupak
	//     vecStart[0..nDim-1] - poèetne vrijednosti varijabli sustava
	//     vecStart[nDim] - poèetna vrijednost parametra
	// POVRATNA VRIJEDNOST
	//	 ret = 0 za uspješno naðenu bif. toèku
	int			i, nDim, ret=-1, check;
	double	v;
	Vector	*val;

	if( GetSysParNum() == 1 )
	{
		nDim = GetSysDim();		
		// ovaj 1 je za parametar omega (iznos imaginarne komponente svojstvene vrijednosti
		val = new Vector(3*nDim+GetSysParNum()+1);

		if( vecStart->GetDim() == GetSysDim() + GetSysParNum() )
		{
			for( i=0; i<nDim; i++ )
				(*val)[i] = (*vecStart)[i];

			// poèetne vrijednosti za 'v'
		//	v = sqrt( 1 / (double)nDim );
			srand( (unsigned) time( NULL ) );

			for( i=0; i<nDim; i++ )
			{
				v = ((double) rand()) / RAND_MAX - 0.5;
				(*val)[nDim+i] = v;				// ovdje postavljamo realni dio
				v = ((double) rand()) / RAND_MAX - 0.5;
				(*val)[2*nDim+i] = v;			// a ovdje imaginarni
			}

			// ovdje ubacujemo vrijednost parametra
			(*val)[3*nDim] = (*vecStart)[nDim];
			// a ovdje poèetnu vrijednost iznosa svojstvene vrijednosti
			(*val)[3*nDim+1] = 1;
		}
		else
			for( i=0; i<3*nDim+GetSysParNum()+1; i++ )
				(*val)[i] = (*vecStart)[i];

		ret = GlobalNewton(val, 3*nDim+GetSysParNum()+1, &check, &DiffSystem::HopfBifFunc );

		if( ret == 0 )
		{
			for( i=0; i<nDim; i++ )
				(*vecStart)[i] = (*val)[i];
			
			(*vecStart)[nDim] = (*val)[3*nDim];
			*omega = (*val)[3*nDim+1];
		}
		delete val;
	}
	else if( GetSysParNum() > 1 )
	{
	}
	else
		ret = -1;

	return ret;
}

void	DiffSystem::HopfBifFunc( double x, Vector &X, Vector *dydx )
{
	// Raèuna jednadžbe sustava za Hopfovu bifurkaciju, za jednoparametarski sustav
	int			i, j, nDim;
	double	omega;

	nDim = GetSysDim();
	Vector	myX(nDim), myP(1), myVReal(nDim), myVImag(nDim), myRes(nDim);
	Matrix	matJac(nDim, nDim);

	for( i=0; i<nDim; i++ )
		myX[i] = X[i];
	
	for( i=nDim; i<2*nDim; i++ )
		myVReal[i-nDim] = X[i];
	for( i=2*nDim; i<3*nDim; i++ )
		myVImag[i-2*nDim] = X[i];
	
	for( i=3*nDim; i<3*nDim+1; i++ )
		myP[i-3*nDim] = X[i];

	omega = X[3*nDim+1];

	(*m_pfSysFunc)(x, myX, myP, dydx);
	CalcJacobian(x, myX, myP, &matJac );
	// sada se u prvih 'n' èlanova vektora 'dydx' nalaze vrijednosti funkcija sistema u zadanoj toèki
	
	// a sada treba naæi i vrijednosti ostalih funkcija sustava
	for( i=0; i<nDim; i++ )
	{
		(*dydx)[i+nDim] = 0;
		// množimo J * Re(v)
		for( j=0; j<nDim; j++ )
			(*dydx)[i+nDim] += matJac[i][j] * myVReal[j];
		
		// pošto množimo jediniènu matricu sa vektorom Im(v), isto je kao da samo 
		// zbrojimo odgovarajuæu komponentu tog vektora
		(*dydx)[i+nDim] += omega * myVImag[i];
	}
	
	for( i=0; i<nDim; i++ )
	{
		(*dydx)[i+2*nDim] = 0;
		// množimo J * Im(v)
		for( j=0; j<nDim; j++ )
			(*dydx)[i+2*nDim] += matJac[i][j] * myVImag[j];
		
		// pošto množimo jediniènu matricu sa vektorom Im(v), isto je kao da samo 
		// zbrojimo odgovarajuæu komponentu tog vektora
		(*dydx)[i+2*nDim] -= omega * myVReal[i];
	}

	(*dydx)[3*nDim] = -1;
	for( j=0; j<nDim; j++ )
		(*dydx)[3*nDim] += myVReal[j]*myVReal[j] + myVImag[j]*myVImag[j];
	
	(*dydx)[3*nDim+1] = 0;
	for( j=0; j<nDim; j++ )
		(*dydx)[3*nDim+1] += atan2(myVImag[j], myVReal[j]);
}

int	DiffSystem::LocateHopfBifPointsAlongRay( Vector *vecStart, Vector *vecRay, Vector *vecLeftEigen )
{
	// Traži Hopfove bifurkacijske toèke duž dane zrake
	// PARAMETRI
	//	 vecStart - poèetne vrijednosti za iteracijski postupak
	//     vecStart[0..nDim-1] - poèetne vrijednosti varijabli sustava
	//     vecStart[nDim..nDim+ParNum-1] - poèetne vrijednosti parametara
	//	 vecRay - vektor smjera zrake duž koje se traži
	//   vecLeftEigen - pokazivaè na vektor u koji æe se pohraniti izraèunati lijevi svojstveni
	//								vektor za danu bifurkacijsku toèku
	// POVRATNA VRIJEDNOST
	//	 ret = 0 za uspješno naðenu bif. toèku
	int			i, nDim, ret=-1, check;
	Vector	*val;

	// da bi tražili bifurkaciju duž zrake, sustav treba biti barem 2-parametarski
	if( GetSysParNum() > 1 )
	{
		if( vecRay == NULL )
			return -2;

		if( m_vecRay != NULL )
			delete m_vecRay;
		if( m_vecStartPoint != NULL )
			delete m_vecStartPoint;
		
		nDim = GetSysDim();		
		val = new Vector(3*nDim+1);

		m_vecRay = new Vector(GetSysParNum());
		*m_vecRay = *vecRay;
		Normalize(*m_vecRay);

		// poèetne vrijednosti za toèku od koje se kreæe u traženju su zadane u vecStart
		m_vecStartPoint = new Vector(GetSysParNum());
		for( i=0; i<GetSysParNum(); i++ )
			(*m_vecStartPoint)[i] = (*vecStart)[nDim+i];

		for( i=0; i<nDim; i++ )
			(*val)[i] = (*vecStart)[i];

		// poèetne vrijednosti za 'v'
		for( i=0; i<nDim; i++ )
			(*val)[nDim+i] = ( (double) rand() ) / RAND_MAX - 0.5;

		// uzimamo jedinièni pomak duž vektora u kojem tražimo
		(*val)[2*nDim] = 1;

		ret = GlobalNewton(val, 2*nDim+1, &check, &DiffSystem::BifFuncAlongRay );

		if( ret == 0 )
		{
			for( i=0; i<nDim; i++ )
				(*vecStart)[i] = (*val)[i];
			for( i=0; i<GetSysParNum(); i++ )
				(*vecStart)[nDim+i] = (*val)[2*nDim] * (*m_vecRay)[i];

			if( vecLeftEigen != NULL )
				for( i=0; i<nDim; i++ )
					(*vecLeftEigen)[i] = (*val)[i+nDim];
		}
		delete val;
	}
	else
		ret = -2;

	return ret;
}

void	DiffSystem::HopfBifFuncAlongRay( double x, Vector &X, Vector *dydx )
{
	int			i, j, nDim;
	double	omega;

	nDim = GetSysDim();
	Vector	myX(nDim), myP(GetSysParNum()), myVReal(nDim), myVImag(nDim), myRes(nDim);
	Matrix	matJac(nDim, nDim);

	for( i=0; i<nDim; i++ )
		myX[i] = X[i];
	
	for( i=nDim; i<2*nDim; i++ )
		myVReal[i-nDim] = X[i];
	for( i=2*nDim; i<3*nDim; i++ )
		myVImag[i-2*nDim] = X[i];
	
	for( i=0; i<GetSysParNum(); i++ )
		myP[i] = X[3*nDim] * (*m_vecRay)[i];

	omega = X[3*nDim+GetSysParNum()];

	(*m_pfSysFunc)(x, myX, myP, dydx);
	CalcJacobian(x, myX, myP, &matJac );
	// sada se u prvih 'n' èlanova vektora 'dydx' nalaze vrijednosti funkcija sistema u zadanoj toèki
	
	// a sada treba naæi i vrijednosti ostalih funkcija sustava
	for( i=0; i<nDim; i++ )
	{
		(*dydx)[i+nDim] = 0;
		// množimo J * Re(v)
		for( j=0; j<nDim; j++ )
			(*dydx)[i+nDim] += matJac[i][j] * myVReal[j];
		
		// pošto množimo jediniènu matricu sa vektorom Im(v), isto je kao da samo 
		// zbrojimo odgovarajuæu komponentu tog vektora
		(*dydx)[i+nDim] += omega * myVImag[i];
	}
	
	for( i=0; i<nDim; i++ )
	{
		(*dydx)[i+2*nDim] = 0;
		// množimo J * Im(v)
		for( j=0; j<nDim; j++ )
			(*dydx)[i+2*nDim] += matJac[i][j] * myVImag[j];
		
		// pošto množimo jediniènu matricu sa vektorom Im(v), isto je kao da samo 
		// zbrojimo odgovarajuæu komponentu tog vektora
		(*dydx)[i+2*nDim] -= omega * myVReal[i];
	}

	(*dydx)[3*nDim] = -1;
	for( j=0; j<nDim; j++ )
		(*dydx)[3*nDim] += myVReal[j]*myVReal[j] + myVImag[j]*myVImag[j];
	
	(*dydx)[3*nDim+1] = 0;
	for( j=0; j<nDim; j++ )
		(*dydx)[3*nDim+1] += atan2(myVImag[j], myVReal[j]);
}

/******************************************************************/
/****   Integracija sistema obiènih diferencijalnih jednadžbi  ****/
/******************************************************************/


//	Stepper rutina za RK-metodu - odredjuje korak postupka
void DiffSystem::RKStep( Vector *y, Vector *dydx, int n, double *x, double htry, 
			double eps, Vector *yscal, double *hdid, double *hnext,
			void (DiffSystem::*pfFunc)(double, Vector &, Vector &, Vector *) )

/*	RK 5-og reda sa pracenjem lokalne greske zaokruzivanja da bi se osigurala tocnost i 
prilagodila duljina koraka.
ULAZ:
	vektor zavisnih varijabli  -  y[1..n]
	i njegove derivacije   -  dydx[1..n],   i to u poèetnoj tocki nezavisne varijable x
	duljina koraka(poèetna za probu)  -  htry
	zahtijevana toènost	 -  eps
	i vektor sa kojim se skalira greška  -  yscal[1..n]
IZLAZ:
	y i x -  zamijenjeni sa novim vrijednostima
	hdid  -  postignuta duljina koraka
	hnext -  predvidjena duljina koraka u sljedeæem koraku
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
		RKAlg(y,dydx,n,*x,h,ytemp,yerr,pfFunc);
		errmax = 0.0;
		for( i=0; i<n; i++ )
			errmax = DMAX(errmax, fabs((*yerr)[i]/(*yscal)[i]));
		errmax /= eps;

		if( errmax <= 1.0 )
			break;

		htemp = SAFETY * h * pow(errmax,PSHRNK);
		h = (h >= 0.0 ? DMAX(htemp, 0.1*h) : DMIN(htemp, 0.1*h));

		xnew = (*x) + h;
		if( xnew == *x )
		{
			fprintf(stderr, "\n stepsize underflow in rkqs!!!");
			assert(0);
		}
	}
	if( errmax > ERRCON )
		*hnext = SAFETY * h * pow(errmax, PGROW);
	else
		*hnext = 5.0 * h;

	*x += (*hdid=h);
	for( i=0; i<n; i++ )
		(*y)[i] = (*ytemp)[i];

	delete yerr;
	delete ytemp;
}

//	Algoritam rutina na RK-metodu -	implementira osnovnu formulu metodu 
//		pocinjuci od zavisnih varijabli y[i] u x racuna y(x+h)
void	DiffSystem::RKAlg( Vector *y, Vector *dydx, int n, double x, double h, Vector *yout, Vector *yerr,
 												 void (DiffSystem::*pfFunc)(double, Vector &, Vector &, Vector *) )
/*
	Dobivsi vrijednosti za n varijabli y[1..n] i njihovih derivacija 
	dydx[1..n] poznatih u x, koristi Cash-Karpovu RK metodu da napreduje 
	s rješenjem preko intervala h i vraca varijable u yout[1..n]
	Takodjer vraca lokalnu gresku zaokruzivanja u yerr[]
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
	Vector	ak2(n), ak3(n), ak4(n), ak5(n), ak6(n), ytemp(n);
 
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + b21*h*(*dydx)[i];

	(this->*pfFunc)(x+a2*h, ytemp, *m_vecSysParam, &ak2);
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + h*(b31*(*dydx)[i] + b32*ak2[i]);

	(this->*pfFunc)(x+a3*h, ytemp, *m_vecSysParam, &ak3);
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + h*(b41*(*dydx)[i] + b42*ak2[i] + b43*ak3[i]);

	(this->*pfFunc)(x+a4*h, ytemp, *m_vecSysParam, &ak4);
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + h*(b51*(*dydx)[i] + b52*ak2[i] + b53*ak3[i] + b54*ak4[i]);

	(this->*pfFunc)(x+a5*h, ytemp, *m_vecSysParam, &ak5);
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + h*(b61*(*dydx)[i] + b62*ak2[i] + b63*ak3[i] + b64*ak4[i] + b65*ak5[i]);

	(this->*pfFunc)(x+a6*h, ytemp, *m_vecSysParam, &ak6);
	for( i=0; i<n; i++ )
		(*yout)[i] = (*y)[i] + h*(c1*(*dydx)[i] + c3*ak3[i] + c4*ak4[i] + c6*ak6[i]);

	for( i=0; i<n; i++ )
		(*yerr)[i] = h*(dc1*(*dydx)[i] + dc3*ak3[i] + dc4*ak4[i] + dc5*ak5[i] + dc6*ak6[i]);

}

void DiffSystem::ODEIntegrate( Vector ystart, int nVar, double x1, double x2,
			double eps, double h1, double hmin, int *nok, int *nbad,
			void (DiffSystem::*pfFunc)(double, Vector &, Vector &, Vector *),
			int kmax, int *kount, double *xp, double **yp, double dxsav )

//	ystart[] - vektor poèetnih vrijednosti
//  nVar  - broj dif. jednadžbi
//  nok, nbad - broj dobrih i loših poduzetih koraka
//  h1 - poèetna duljina koraka
//  hmin - minimalna dozvoljena duljina koraka ( može biti 0 )
//  m_pfSysFunc - korisnikova rutina koja raèuna derivacije u danoj toèki
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

	yscal = new Vector(nVar);
	y			= new Vector(nVar);
	dydx	= new Vector(nVar);

	x=x1;
	h = SIGN(h1, x2-x1);
	*nok = (*nbad) = 0;
	*kount = -1;
	for( i=0; i<nVar; i++ )
		(*y)[i] = ystart[i];
	if( kmax > 0 ) xsav = x - dxsav*2.0;

	for( nstp=1; nstp<=MAXSTP; nstp++ )
	{
		(this->*pfFunc)(x, *y, *m_vecSysParam, dydx);
		for( i=0; i<nVar; i++ )
			(*yscal)[i] = fabs((*y)[i]) + fabs((*dydx)[i]*h) + TINY;
		if( kmax > 0 && *kount < kmax-1 && fabs(x-xsav) > fabs(dxsav) )
		{
			xp[++(*kount)] = x;
			for( i=0; i<nVar; i++ )
				yp[i][*kount] = (*y)[i];
			xsav = x;
		}
		if( (x+h-x2)*(x+h-x1) > 0.0 ) 
			h=x2 - x;

		RKStep(y,dydx,nVar,&x,h,eps,yscal,&hdid,&hnext,pfFunc);
		if( hdid == h)
			++(*nok);
		else
			++(*nbad);

		if( (x-x2)*(x2-x1) >= 0.0 )
		{
			for( i=0; i<nVar; i++ )
				ystart[i] = (*y)[i];
			if( kmax )
			{
				xp[++(*kount)] = x;
				for( i=0; i<nVar; i++ )
					yp[i][*kount] = (*y)[i];
			}

			delete yscal;
			delete y;
			delete dydx;

			return;
		}
		if( fabs(hnext) <= hmin )
		{
			fprintf(stderr, "\n Step size too small in odeint!!!");
			assert(0);
		}
		h = hnext;
	}
	fprintf(stderr, "\nToo many steps in odeint!!!");
	assert(0);
}

/****************************************************************/
//    BULIRSCH-STOER METHOD

#define	KMAXX			8
#define	IMAXX			(KMAXX+1)
#define	SAFE1			0.25
#define	SAFE2			0.7
#define	REDMAX		1.0e-5
#define	REDMIN		0.7
#define	SCALMX		0.1

void DiffSystem::BSStep( double *y, double *dydx, int nv, double *xx, double htry,
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
				printf("Step sizue underflow in bsstep!!!");
				assert(0);
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
}

void DiffSystem::PolyExtr(int iest, double xest, double yest[], double yz[], double dy[], int nv, Matrix *d, double *x )
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

void DiffSystem::RatExtr(int iest, double xest, double yest[], double yz[], double dy[], int nv, Matrix *d, double *x )
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


void DiffSystem::ModMidPoint( double y[], double dydx[], int nvar, double xs, double htot, 
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


/**************************************************************************************/
/**************************************************************************************/

#define FREERETURN {delete matJac; delete vectFuncVal; delete vectP; delete []indx; return 1; }
#define TOLX	1e-10
#define TOLF	1e-8

int DiffSystem::NewtonRaphson( int nTrialNum, Vector *vectX, int nDim, 
				void (DiffSystem::*pfVecFunc)(double, Vector&, Vector*) )
{
	// Obièni Newton-Raphsonov postupak
	//   nTrialNum - maksimalan broj iteracija
	//   vectX - poèetne vrijednosti za varijable, u njima se i vraæa naðeno rješenje
	//   nDim - dimenzija sustava
	//   pfVecFunc - korisnièki definirana funkcija koja vraæa vrijednosti jednadžbi sustava u zadanoj toèci
	int			i, k, *indx;
	double	errx, errf, d;
	Matrix	*matJac;
	Vector	*vectFuncVal=NULL, *vectP;

	indx = new int[nDim];
	matJac = new Matrix(nDim, nDim);
	vectP  = new Vector(nDim);
	vectFuncVal = new Vector(nDim);

	for( k=0; k<nTrialNum; k++ )
	{
		(this->*pfVecFunc)(nDim, *vectX, vectFuncVal);
		
		FindJacobian(nDim, *vectX, *vectFuncVal, matJac, pfVecFunc);

 		errf = 0.0;
		for( i=0; i<nDim; i++ )
			errf += fabs((*vectFuncVal)[i]);
		if( errf <= TOLF )
			FREERETURN;
		
		*vectP = -1 * (*vectFuncVal);

		LUDecomp(*matJac, indx, &d );
		LUBackSubst(*matJac, indx, *vectP );
		errx = 0.0;

		for( i=0; i<nDim; i++ )
			errx += fabs((*vectP)[i]);

		(*vectX) += (*vectP);

		if( errx <= TOLX )
			FREERETURN;
	}
	delete matJac; 
	delete vectFuncVal; 
	delete vectP; 
	delete []indx;

	return 0;
}

void DiffSystem::FindJacobian( int nDim, Vector &vecX, Vector &vecFuncVal, Matrix *Jacobian, 
			void (DiffSystem::*pfVecFunc)(double, Vector&, Vector*) )
{
	// Raèuna jacobian sustava u zadanoj toèci
	//   nDim - broj jednadžbi
	//   vecX - toèka u kojoj se raèuna jakobian
	//   vecFuncVal - veæ izraèunate vrijednosti jednadžbi sustava u poèetnoj toèci
	//   Jacobian - matrica u koju se smješta izraèunati jakobijan
	int			i, j;
	double	h, temp;
	Vector	*f=NULL;

	f = new Vector(nDim);

	for( j=0; j<nDim; j++ )
	{
		temp = vecX[j];
		h = 1e-5 * fabs(temp);
		if( h == 0.0 )
			h = 1e-5;

		vecX[j] = temp + h;
		h = vecX[j] - temp;

		(this->*pfVecFunc)(0,vecX,f);
		vecX[j] = temp;

		for( i=0; i<vecFuncVal.GetDim(); i++ )
			Jacobian->SetElem(i,j, ((*f)[i] - vecFuncVal[i]) / h );
	}
}
							
#define	ALF		1.0e-4
//#define TOLX	1.0e-7

void DiffSystem::LineSearch( int n, Vector &xold, double fold, Vector &g, Vector &p, Vector &x, 
						double *f, double stpmax, int *check, void (DiffSystem::*vecfunc)(double, Vector&, Vector*) )
{
	// Raèuna toèku x duž smjera zadanog sa p, poèevši od toèke xold, tako da se vrijednost 
	// 	funkcije F*F dovoljno smanjila
	// PARAMETRI
	//   n - broj jednadžbi sustava
	//   xold - poèetna toèka
	//   x - novoizraèunata toèka
	//   fold - vrijednost funkcije u poèetnojtoèci
	//   g - gradijent jednadžbi sustava u poeètnoj toèci
	//   f - vraæena vrijednost funkcije u novoj toèci
	//   stpmax - maksimalan dozvoljena dužina koraka
	//   check - 0 kod normalnog izlaza, 1 kada je x previše blizu xold
	int			i;
	double	a, alam, alam2, alamin, b, disc, f2, fold2, rhs1, rhs2, slope;
	double	sum, temp, test, tmplam;
	Vector	*fvec;

	fvec = new Vector(n);

	*check = 0;
	for( sum=0.0, i=1; i<=n; i++ )
		sum += p[i-1] * p[i-1];
	sum = sqrt(sum);
	if( sum > stpmax )
		for( i=1; i<=n; i++ )
			p[i-1] *= stpmax / sum;

	for( slope=0.0, i=1; i<=n; i++ )
		slope += g[i-1] * p[i-1];

	test = 0.0;

	for( i=1; i<=n; i++ )
	{
		temp = fabs(p[i-1]) / DMAX(fabs(xold[i-1]),1.0);
		if( temp > test )
			test = temp;
	}
	alamin = TOLX / test;
	alam = 1.0;
	for( ; ; )
	{
		for( i=1; i<=n; i++ )
			x[i-1] = xold[i-1] + alam*p[i-1];

//		*f = fmin(x);
		(this->*vecfunc)(n,x,fvec);
		for( sum=0.0, i=0; i<n; i++ )
			sum += DSQR((*fvec)[i]);

		*f = 0.5*sum;

		if( alam < alamin )
		{
			for( i=1; i<=n; i++ )
				x[i-1] = xold[i-1];
			*check = 1;
			return;
		}
		else if( *f <= fold+ALF*alam*slope) return;
		else
		{
			if( alam == 1.0 )
				tmplam = -slope / (2.0*(*f - fold - slope));
			else
			{
				rhs1 = *f - fold - alam*slope;
				rhs2 = f2 - fold - alam2*slope;

				a = (rhs1/(alam*alam) - rhs2/(alam2*alam2))/(alam - alam2);
				b = (-alam2*rhs1 / (alam*alam) + alam*rhs2/(alam2*alam2))/(alam - alam2);

				if( a == 0.0 )
					tmplam = -slope/(2.0*b);
				else
				{
					disc = b*b - 3.0*a*slope;
					if( disc < 0.0 )
					{
						printf("Roundoff problem in lnsrch !!!");
						assert(0);
					}
					else
						tmplam = (-b+sqrt(disc)) / (3.0*a);
				}
				if( tmplam > 0.5*alam)
					tmplam = 0.5*alam;
			}
		}
		alam2 = alam;
		f2 = *f;
		fold2 = fold;
		alam = DMAX(tmplam, 0.1*alam);
	}
}

#define MAXITS_GN		250
#define	TOLF_GN			1.0E-5
#define	TOLMIN_GN		1.0E-6
#define	TOLX_GN			1.0E-9
#define	STPMX_GN			100.0

int DiffSystem::GlobalNewton( Vector *x, int n, int *check, void (DiffSystem::*vecfunc)(double, Vector&, Vector*) )
{
	// Globalno konvergentan Newtonov postupak
	// PARAMETRI
	//    x - poèetna vrijednost
	//    n - broj jednadžbi
	//		vecfunc - pokazivaè na funkciju koja definira jedadžbe sustava
	//    check - da li je program konvergirao u lokalni minimum (koji nije rješenje)
	// POVRATNA VRIJEDNOST
	//		ret = 0 ako je rješenje uspješno pronaðeno
	int			i, its, j, *indx;
	double	d, den, f, fold, stpmax, sum, temp, test;
	Matrix	*fjac;
	Vector	*g, *p, *xold, *fvec;

	indx = new int[n];
	fjac = new Matrix(n,n);
	g = new Vector(n);
	p = new Vector(n);
	xold = new Vector(n);
	fvec = new Vector(n);

//	f = fmin(x);
	(this->*vecfunc)(n,*x,fvec);
	for( sum=0.0, i=0; i<n; i++ )
		sum += DSQR((*fvec)[i]);

	f = 0.5*sum;

	// najprije æemo provjeriti da li smo odmah s poèetnim vrijednostima pogodili nulu
	test = 0.0;
	for( i=1; i<=n; i++ )
		if( fabs((*fvec)[i-1]) > test )
			test = fabs((*fvec)[i-1]);

	// koristimo malo jaèi uvjet
	if( test< 0.01*TOLF_GN )
	{
		*check = 0;
		delete fvec; delete xold; delete p; delete g;
		delete fjac; delete []indx; return 0;
	}
	// raèunamo stpmax za pretraživanje po pravcu
	for( sum=0.0, i=1; i<=n; i++ )
		sum += DSQR((*x)[i-1]);
	stpmax = STPMX_GN * DMAX(sqrt(sum),(double)n);

	for( its=1; its<=MAXITS_GN; its++ )
	{
		FindJacobian(n,*x,*fvec,fjac,vecfunc);
//		Print(*fjac);
		
		// raèunamo gradijent jednadžbi sustava
		for( i=1; i<=n; i++ )
		{
			for( sum=0.0, j=1; j<=n; j++ )
				sum += (*fjac)[j-1][i-1] * (*fvec)[j-1];
			(*g)[i-1] = sum;
		}
		// pohanjujemo x i f
		for( i=1; i<=n; i++ )
			(*xold)[i-1] = (*x)[i-1];
		fold = f;
		
		// raèunamo desnu stranu za sustav linearnih jedandžbi
		for( i=1; i<=n; i++ )
			(*p)[i-1] = -(*fvec)[i-1];
		
		LUDecomp(*fjac,indx,&d);
		LUBackSubst(*fjac,indx,*p);
		LineSearch(n,*xold,fold,*g,*p,*x,&f,stpmax,check,vecfunc);

		// potrebno je izraèunati fvec u novoj toèci
		(this->*vecfunc)(n, *x, fvec );
		// testiramo konvergenciju funkcijskih vrijednosti
		test = 0.0;
		for( i=1; i<=n; i++ )
			if( fabs((*fvec)[i-1]) > test )
				test = fabs((*fvec)[i-1]);

		if( test < TOLF_GN )
		{
			*check = 0;
			delete fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; return 0;
		}
		// provjeravamo da li je gradijent od f nula, znaèi da smo u lokalnom minimumu
		if( *check )
		{
			test = 0.0;
			den = DMAX(f, 0.5*n);
			for( i=1; i<=n; i++ )
			{
				temp = fabs((*g)[i-1]) * DMAX(fabs((*x)[i-1]), 1.0) / den;
				if( temp > test )
					test = temp;
			}
			*check = (test < TOLMIN_GN ? 1 : 0 );
			delete fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; return -1;
		}
		// provjeravamo konvergenciju za vrijednosti od x
		test = 0.0;
		for( i=1; i<=n; i++ )
		{
			temp = (fabs((*x)[i-1] - (*xold)[i-1])) / DMAX(fabs((*x)[i-1]), 1.0);
			if( temp > test )
				test = temp;
		}
		if( test < TOLX_GN )
		{
			*check = 1;
			delete fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; return -1;
		}
	}
	*check = 1;
	printf("MAXITS exceeded in newt ");
	return -2;
}
