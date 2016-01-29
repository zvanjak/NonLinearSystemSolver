#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>
#include	<time.h>

#include	"mathlib.h"


//******************************************************************
//					VECTOR
//******************************************************************

Vector::Vector()
{
	m_nDim = 0;
	m_pdCoef = NULL;
}

Vector::Vector( int nDim )
{
	m_nDim = nDim;
	m_pdCoef = new double[m_nDim];
}

Vector::Vector( int nDim, char *name )
{
	m_nDim = nDim;
	m_pdCoef = new double[m_nDim];
	strcpy( strName, name );
}

Vector::Vector( int nDim, double *pdValues,int Copy )
{
	m_nDim = nDim;
	m_pdCoef = new double[nDim];

	for( int i=0; i<nDim; i++ )
		m_pdCoef[i] = pdValues[i];
}

Vector::~Vector()
{
	delete []m_pdCoef;
}

Vector::Vector( Vector &Copy )
{
	register int	i;

	m_nDim = Copy.m_nDim;
	m_pdCoef = new double[m_nDim];
	
	for( i=0; i<m_nDim; i++ )
		m_pdCoef[i] = Copy.m_pdCoef[i];
}

void Vector::InitValues( double *x )
{
	register int i;
	
	for( i=0; i<m_nDim; i++ )
		m_pdCoef[i] = x[i];
}

void Vector::InitValues( double dFirst, ... )
{
	double			dNext;
	int	i;
	va_list			marker;

	va_start(marker, dFirst);

	m_pdCoef[0] = dFirst;
	for( i=1; i<m_nDim; i++ )
	{
		dNext = va_arg(marker, double);
		m_pdCoef[i] = dNext;
	}

	va_end(marker);
}

void Vector::MakeRandomElements( double dMin, double dMax )
{
	int			i;
	double	transf;

	srand( (unsigned)time( NULL ) );
	transf = (dMax - dMin) / RAND_MAX;

	for( i=0; i<m_nDim; i++ )
			m_pdCoef[i] = rand() * transf + dMin;
}

void Vector::Reinitialize( int nNewDim )
{
	delete []m_pdCoef;

	m_pdCoef = new double[nNewDim];
	
	m_nDim = nNewDim;
}

void Vector::Realloc( int nNewDim )
{
	double	*pdTmp;
	int			till;

	pdTmp = new double[nNewDim];
	till = (m_nDim > nNewDim) ? nNewDim : m_nDim;
	for( int i=0; i < till; i++ )
		pdTmp[i] = m_pdCoef[i];

	delete []m_pdCoef;
	m_pdCoef = pdTmp;
}


double Vector::Get( int i )
{
	if( i >= 0 && i < m_nDim )
		return m_pdCoef[i];
	else
	{
		assert(0);
		return -1;
	}
}

int	Vector::GetDim( void )
{
	return m_nDim;
}

double* Vector::GetPtr( void )
{
	return m_pdCoef;
}

void	Vector::Clear( void )
{
	for( int i=0; i<m_nDim; i++ )
		m_pdCoef[i] = 0.0;
}

Vector& Vector::operator=( const Vector &b )
{
	int i;

	// ovjde treba definirati kakvo ce biti ponasanje funkcije u slucaju da se pridjeljuju
	// vektori koji nisu istih dimenzija
	//   1) - moze se javiti greska
	//   2) - moze se reinicijalizirati vektor s lijeve strane tako da budu istih dimenzija
	//   3) - moze vektor s lijeve strane popuniti dokle ide
	if( m_nDim != b.m_nDim )
	{
		assert(0);
	}
	else
	{
		m_nDim = b.m_nDim;
		for( i=0; i<b.m_nDim; i++ )
			m_pdCoef[i] = b.m_pdCoef[i];
	}

	return *this;
}

Vector Vector::operator+(  Vector &b ) 
{
	Vector	retVector(m_nDim, "OP+");
	int	i;
	
	if( m_nDim == b.m_nDim )
	{
		for( i=0; i<b.m_nDim; i++ )
			retVector.m_pdCoef[i] = m_pdCoef[i] + b.m_pdCoef[i];
	}
	else
		assert(0);
	
	return retVector;
}

Vector Vector::operator-(  Vector &b )  
{
	Vector			retVector(m_nDim, "OP-");
	int	i;

	if( m_nDim == b.m_nDim )
	{
		for( i=0; i<b.m_nDim; i++ )
			retVector.m_pdCoef[i] = m_pdCoef[i] - b.m_pdCoef[i];
	}
	else
		assert(0);
	
	return retVector;
}

Vector Vector::operator*(  double b ) 
{
	Vector			retVector(m_nDim, "OP*");
	int	i;
	
	for( i=0; i<m_nDim; i++ )
		retVector.m_pdCoef[i] = m_pdCoef[i] * b;
	
	return retVector;
}

Vector Vector::operator+=(  Vector &b )
{
	int	i;
	
	if( m_nDim == b.m_nDim )
	{
		for( i=0; i<b.m_nDim; i++ )
			m_pdCoef[i] += b.m_pdCoef[i];
	}
	else
		assert(0);
	
	return *this;
}

Vector Vector::operator-=(  Vector &b ) 
{
	int	i;
	
	if( m_nDim == b.m_nDim )
	{
		for( i=0; i<b.m_nDim; i++ )
			m_pdCoef[i] -= b.m_pdCoef[i];
	}
	
	return *this;
}

Vector Vector::operator*=(  double b )
{
	int	i;
	
	for( i=0; i<m_nDim; i++ )
		m_pdCoef[i] *= b;
	
	return *this;
}

bool	Vector::operator==( Vector &b )
{
	if( m_nDim == b.m_nDim )
	{
		for( int i=0; i<m_nDim; i++ )
			if( m_pdCoef[i] != b.m_pdCoef[i] )
				return false;
	}
	else
		return false;
	
	return true;
}

double& Vector::operator[]( int i )
{
	if( i >= 0 && i < m_nDim )
		return m_pdCoef[i];
	else
	{
		assert(0);
		return m_pdCoef[0];
	}
}

/*************		friend funkcije			****************/

Vector	operator+(  double a,  Vector &b )
{
	Vector			retVector(b.m_nDim, "OPd+");
	int	i;
	
	for( i=0; i<b.m_nDim; i++ )
		retVector.m_pdCoef[i] = a + b.m_pdCoef[i];
	
	return retVector;
}

Vector	operator-(  double a,  Vector &b )
{
	Vector			retVector(b.m_nDim, "OPd-");
	int	i;
	
	for( i=0; i<b.m_nDim; i++ )
		retVector.m_pdCoef[i] = a - b.m_pdCoef[i];
	
	return retVector;
}

Vector	operator*(  double a,  Vector &b )
{
	Vector			retVector(b.m_nDim, "OPd*");
	int	i;
	
	for( i=0; i<b.m_nDim; i++ )
		retVector.m_pdCoef[i] = b.m_pdCoef[i] * a;
	
	return retVector;
}

Vector	operator+(  Vector &a,  double b )
{
	Vector			retVector(a.m_nDim, "OP+d");
	int	i;
	
	for( i=0; i<a.m_nDim; i++ )
		retVector.m_pdCoef[i] = a.m_pdCoef[i] + b;
	
	return retVector;
}

Vector	operator-(  Vector &a,  double b )
{
	Vector			retVector(a.m_nDim, "OP-d");
	int	i;
	
	for( i=0; i<a.m_nDim; i++ )
		retVector.m_pdCoef[i] = a.m_pdCoef[i] - b;
	
	return retVector;
}

Vector	operator*(  Vector &a,  double b )
{
	Vector			retVector(a.m_nDim, "OP*d");
	int	i;
	
	for( i=0; i<a.m_nDim; i++ )
		retVector.m_pdCoef[i] = a.m_pdCoef[i] * b;
	
	return retVector;
}

void	Print( Vector &b )
{
	int		i;

	for( i=0; i<b.m_nDim; i++ )
		printf("\n[%d] = %15.8f ", i, b.m_pdCoef[i] );
}

double	Norm(  Vector &a )
{
	int	i;
	double		dRet=0;

	for( i=0; i<a.m_nDim; i++ )
		dRet += a.m_pdCoef[i] * a.m_pdCoef[i];

	return (sqrt(dRet));
}

void		Normalize( Vector &a )
{
	double		norm;

	norm = Norm(a);
	for( int i=0; i<a.GetDim(); i++ )
		a[i] /= norm;
}

double FindMin( Vector &a )
{
	double	min = 1e30;

	for( int i=0; i<a.m_nDim; i++ )
		if( a[i] < min )
			min = a[i];

	return min;
}

double FindMax( Vector &a )
{
	double	max = -1e30;

	for( int i=0; i<a.m_nDim; i++ )
		if( a[i] > max )
			max = a[i];

	return max;
}

//******************************************************************
//					COMPLEX VECTOR
//******************************************************************

ComplexVector::ComplexVector()
{
	m_nDim = 2;
	m_pdCoef = new Complex[m_nDim]();
//	strcpy(strName, "Temp");
}

ComplexVector::ComplexVector( int nDim )
{
	m_nDim = nDim;
	m_pdCoef = new Complex[m_nDim]();
//	strcpy( strName, "HELP" );
}

ComplexVector::ComplexVector( Vector &vecR, Vector &vecI )
{
	m_nDim = vecR.GetDim();

	m_pdCoef = new Complex[m_nDim]();
	for( int i=0; i<m_nDim; i++ )
	{
		m_pdCoef[i].Set(vecR[i], vecI[i]);
	}
}

ComplexVector::~ComplexVector()
{
	delete []m_pdCoef;
}

ComplexVector::ComplexVector( ComplexVector &Copy )
{
	int	i;

	m_nDim = Copy.m_nDim;
	m_pdCoef = new Complex[m_nDim]();
//	strcpy( strName, Copy.strName );
	
	for( i=0; i<m_nDim; i++ )
		m_pdCoef[i] = Copy.m_pdCoef[i];
}

void ComplexVector::InitValues( Complex *x )
{
	int i;
	
	for( i=0; i<m_nDim; i++ )
		m_pdCoef[i] = x[i];
}

void ComplexVector::InitValues( Complex dFirst, ... )
{
	Complex			dNext;
	int	i;
	va_list			marker;

	va_start(marker, dFirst);

	m_pdCoef[0] = dFirst;
	for( i=1; i<m_nDim; i++ )
	{
		dNext = va_arg(marker, Complex);
		m_pdCoef[i] = dNext;
	}

	va_end(marker);
}

void ComplexVector::MakeRandomElements( double dMin, double dMax )
{
	int			i;
	double	transf;

	srand( (unsigned)time( NULL ) );
	transf = (dMax - dMin) / RAND_MAX;

	for( i=0; i<m_nDim; i++ )
	{
		m_pdCoef[i].m_dReal = rand() * transf + dMin;
		m_pdCoef[i].m_dImag = rand() * transf + dMin;
	}
}

void ComplexVector::Reinitialize( int nNewDim )
{
	delete []m_pdCoef;

	m_pdCoef = new Complex[nNewDim]();
	
	m_nDim = nNewDim;
}

Complex ComplexVector::Get( unsigned int i )
{
	return m_pdCoef[i];
}

int	ComplexVector::GetDim( void )
{
	return m_nDim;
}

Complex* ComplexVector::GetPtr( void )
{
	return m_pdCoef;
}

ComplexVector& ComplexVector::operator=(  ComplexVector &b )
{
	int i;
	
	m_nDim = b.m_nDim;
	for( i=0; i<b.m_nDim; i++ )
		m_pdCoef[i] = b.m_pdCoef[i];

	return *this;
}

ComplexVector ComplexVector::operator+(  ComplexVector &b ) 
{
	ComplexVector			retComplexVector(m_nDim);
	int	i;
	
	for( i=0; i<m_nDim; i++ )
		retComplexVector.m_pdCoef[i] = m_pdCoef[i] + b.m_pdCoef[i];
	
	return retComplexVector;
}

ComplexVector ComplexVector::operator-(  ComplexVector &b )  
{
	ComplexVector			retComplexVector(m_nDim);
	int	i;
	
	for( i=0; i<m_nDim; i++ )
		retComplexVector.m_pdCoef[i] = m_pdCoef[i] - b.m_pdCoef[i];
	
	return retComplexVector;
}

ComplexVector ComplexVector::operator*(  double b ) 
{
	ComplexVector			retComplexVector(m_nDim);
	int	i;
	
	for( i=0; i<m_nDim; i++ )
		retComplexVector.m_pdCoef[i] = m_pdCoef[i] * b;
	
	return retComplexVector;
}

ComplexVector ComplexVector::operator+=(  ComplexVector &b )
{
	int	i;
	
	for( i=0; i<m_nDim; i++ )
		m_pdCoef[i] += b.m_pdCoef[i];
	
	return *this;
}

ComplexVector ComplexVector::operator-=(  ComplexVector &b ) 
{
	int	i;
	
	for( i=0; i<m_nDim; i++ )
		m_pdCoef[i] -= b.m_pdCoef[i];
	
	return *this;
}

ComplexVector ComplexVector::operator*=(  double b )
{
	int	i;
	
	for( i=0; i<m_nDim; i++ )
		m_pdCoef[i] *= b;
	
	return *this;
}

Complex& ComplexVector::operator[]( int i )
{
	assert( i >= 0 );
	assert( i <= m_nDim );
	
	return m_pdCoef[i];
}

/*************		friend funkcije			****************/

ComplexVector	operator+(  double a,  ComplexVector &b )
{
	ComplexVector			retComplexVector(b.m_nDim);
	int	i;
	
	for( i=0; i<b.m_nDim; i++ )
		retComplexVector.m_pdCoef[i] = a + b.m_pdCoef[i];
	
	return retComplexVector;
}

ComplexVector	operator-(  double a,  ComplexVector &b )
{
	ComplexVector			retComplexVector(b.m_nDim);
	int	i;
	
	for( i=0; i<b.m_nDim; i++ )
		retComplexVector.m_pdCoef[i] = a - b.m_pdCoef[i];
	
	return retComplexVector;
}

ComplexVector	operator*(  double a,  ComplexVector &b )
{
	ComplexVector			retComplexVector(b.m_nDim);
	int	i;
	
	for( i=0; i<b.m_nDim; i++ )
		retComplexVector.m_pdCoef[i] = b.m_pdCoef[i] * a;
	
	return retComplexVector;
}

ComplexVector	operator+(  ComplexVector &a,  double b )
{
	ComplexVector			retComplexVector(a.m_nDim);
	int	i;
	
	for( i=0; i<a.m_nDim; i++ )
		retComplexVector.m_pdCoef[i] = a.m_pdCoef[i] + b;
	
	return retComplexVector;
}

ComplexVector	operator-(  ComplexVector &a,  double b )
{
	ComplexVector			retComplexVector(a.m_nDim);
	int	i;
	
	for( i=0; i<a.m_nDim; i++ )
		retComplexVector.m_pdCoef[i] = a.m_pdCoef[i] - b;
	
	return retComplexVector;
}

ComplexVector	operator*(  ComplexVector &a,  double b )
{
	ComplexVector			retComplexVector(a.m_nDim);
	int	i;
	
	for( i=0; i<a.m_nDim; i++ )
		retComplexVector.m_pdCoef[i] = a.m_pdCoef[i] * b;
	
	return retComplexVector;
}

void	Print( ComplexVector &b )
{
	int		i;

	for( i=0; i<b.m_nDim; i++ )
		printf("\n[%d] = %15.8f + i %15.8f", i, Re(b.m_pdCoef[i]), Im(b.m_pdCoef[i]) );
}

double	Norm(  ComplexVector &a )
{
	int	i;
	Complex		dRet(0,0);

	for( i=0; i<a.m_nDim; i++ )
		dRet += a.m_pdCoef[i] * a.m_pdCoef[i];

	return (Module(dRet));
}