#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

//***************************************************************/
//						POLYNOM
//***************************************************************/

Polynom::Polynom()
{
	m_nDegree = -1;
	m_pdCoef = NULL;
}

Polynom::~Polynom()
{
	if( m_pdCoef != NULL )
		delete	[]m_pdCoef;
}

Polynom::Polynom( const Polynom &Copy )
{
	m_nDegree = GetDegree(Copy);

	if( Copy.m_nDegree >= 0 )
	{
		m_pdCoef = new double[GetDegree(Copy) + 1];

		for( int i=0; i <= GetDegree(Copy); i++ )
			m_pdCoef[i] = Copy.m_pdCoef[i];
	}
	else
		m_pdCoef = NULL;
}

Polynom::Polynom( int n )
{
	m_nDegree = n;
	if( n >= 0 )
	{
		m_pdCoef = new double[n+1];
		for( int i=0; i<=n; i++ )
			m_pdCoef[i] = 0;
	}
	else
	{			// krivi stupanj
		m_nDegree = -1;
		m_pdCoef = NULL;
	}
}

int	GetDegree( const Polynom &a )
{
	return	a.m_nDegree;
}

int GetRealDegree( const Polynom &a )
{
	for( int i=a.m_nDegree; i>=0; i-- )
		if( a.m_pdCoef[i] != 0 )
			return i;
	
		return 0;
}

double	GetCoef( const Polynom &a, int CoefNum )
{
	double	dRet;

	if( CoefNum <= GetDegree(a) )
	{
		dRet = a.m_pdCoef[CoefNum];
	}
	else
	{			// krivi koeficijent
	}

	return	dRet;
}

double	GetLeadingCoef( const Polynom &a )
{
	return a.m_pdCoef[GetDegree(a)];
}

double Polynom::Val( double x )
{
	double		p;

	p = m_pdCoef[m_nDegree];
	
	for( int j=m_nDegree-1; j>=0; j-- )
		p = p*x + m_pdCoef[j];

	return p;
}

/*************************************************************************************/
/*********							OPERATORI						****/
/*************************************************************************************/

Polynom	Polynom::operator+( const Polynom &b ) const
{
	Polynom Ret(GetDegree(*this) > GetDegree(b) ? GetDegree(*this) : GetDegree(b));

	for( int i=0; i<=GetDegree(Ret); i++ )
	{
		if( GetDegree(*this) >= i )
			Ret.m_pdCoef[i] += this->m_pdCoef[i];

		if( GetDegree(b) >= i )
			Ret.m_pdCoef[i] += b.m_pdCoef[i];
	}

	return	Ret;
}

Polynom	Polynom::operator-( const Polynom &b ) const
{
	Polynom Ret(GetDegree(*this) > GetDegree(b) ? GetDegree(*this) : GetDegree(b));

	for( int i=0; i<=GetDegree(Ret); i++ )
	{
		if( GetDegree(*this) >= i )
			Ret.m_pdCoef[i] += this->m_pdCoef[i];

		if( GetDegree(b) >= i )
			Ret.m_pdCoef[i] -= b.m_pdCoef[i];
	}

	return	Ret;
}

Polynom	Polynom::operator*( const Polynom &b ) const
{
	Polynom	Ret(GetDegree(*this) + GetDegree(b) );

	for( int i=0; i <= GetDegree(*this); i++ )
		for( int j=0; j <= GetDegree(b); j++ )
			Ret.m_pdCoef[i+j] += this->m_pdCoef[i] * b.m_pdCoef[j];

	return	Ret;
}

Polynom	Polynom::operator/( const Polynom &b ) const
{
	int			k, j;
	int			n, nv;

	n = GetRealDegree(*this);
	nv = GetRealDegree(b);

	Polynom q(n), r(n);

	for( j=0; j<=n; j++ )
	{
		r[j] = this->m_pdCoef[j];
		q[j] = 0.0;
	}
	for( k=n-nv; k>=0; k-- )
	{
		q[k] = r[nv+k] / b.m_pdCoef[nv];
		for( j=nv+k-1; j>=k; j-- )
			r[j] -= q[k] * b.m_pdCoef[j-k];
	}
	for( j=nv; j<=n; j++ )
		r[j] = 0.0;

	return q;
}

Polynom	Polynom::operator%( const Polynom &b ) const
{
	int			k, j;
	int			n, nv;
	
	n = GetRealDegree(*this);
	nv = GetRealDegree(b);

	Polynom q(n), r(n);

	for( j=0; j<=n; j++ )
	{
		r[j] = this->m_pdCoef[j];
		q[j] = 0.0;
	}
	for( k=n-nv; k>=0; k-- )
	{
		q[k] = r[nv+k] / b.m_pdCoef[nv];
		for( j=nv+k-1; j>=k; j-- )
			r[j] -= q[k] * b.m_pdCoef[j-k];
	}
	for( j=nv; j<=n; j++ )
		r[j] = 0.0;

	return r;
}

Polynom	operator*( double a, const Polynom &b )
{
	Polynom Ret(GetDegree(b) );

	for( int i=0; i<=GetDegree(b); i++ )
		Ret.m_pdCoef[i] = a * b.m_pdCoef[i];

	return	Ret;
}

Polynom	operator*( const Polynom &a, double b )
{
	Polynom Ret(GetDegree(a) );

	for( int i=0; i<=GetDegree(a); i++ )
		Ret.m_pdCoef[i] = b * a.m_pdCoef[i];

	return	Ret;
}

Polynom	Polynom::operator+=( const Polynom &b )
{
	// uzmi za red polinoma onaj s visim indeksom
	double	*pdTmp;
	int		nDeg, i;
	
	if( m_nDegree < b.m_nDegree )
	{
		// znaci da je onaj kojem pribrajamo manjeg reda i zato ga
		// treba prosiriti
		pdTmp = (double *) new double[b.m_nDegree];
		memmove( pdTmp, m_pdCoef, sizeof(double)*(m_nDegree+1) );
		
		delete	m_pdCoef;
		m_pdCoef = pdTmp;
		nDeg = b.m_nDegree;
	}
	else
		nDeg = m_nDegree;

	for( i=0; i<=nDeg; i++ )
	{
		if( b.m_nDegree >= i )
			m_pdCoef[i] += b.m_pdCoef[i];
	}

	return	*this;
}

Polynom	Polynom::operator-=( const Polynom &b )
{
	// uzmi za red polinoma onaj s visim indeksom
	double	*pdTmp;
	int		nDeg, i;
	
	if( m_nDegree < b.m_nDegree )
	{
		// znaci da je onaj od kojeg oduzimamo manjeg reda i zato ga
		// treba prosiriti
		pdTmp = (double *) new double[b.m_nDegree];
		memmove( pdTmp, m_pdCoef, sizeof(double)*(m_nDegree+1) );
		
		delete	m_pdCoef;
		m_pdCoef = pdTmp;
		nDeg = b.m_nDegree;
	}
	else
		nDeg = m_nDegree;

	for( i=0; i<=nDeg; i++ )
	{
		if( b.m_nDegree >= i )
			m_pdCoef[i] -= b.m_pdCoef[i];
	}
	return *this;
}

Polynom	Polynom::operator*=( const Polynom &b )
{
	double	*pdTmp;

	pdTmp = (double *) new double[m_nDegree + b.m_nDegree];

	for( int i=0; i <= GetDegree(*this); i++ )
		for( int j=0; j <= GetDegree(b); j++ )
			pdTmp[i+j] += m_pdCoef[i] * b.m_pdCoef[j];

	delete m_pdCoef;
	m_pdCoef = pdTmp;
	m_nDegree += b.m_nDegree;

	return *this;
}

Polynom	Polynom::operator*=( const double b )
{
	int		i;

	for( i=0; i<m_nDegree; i++ )
		m_pdCoef[i] *= b;

	return *this;
}

Polynom	Polynom::operator/=( const double &b )
{
	int		i;

	for( i=0; i<m_nDegree; i++ )
		m_pdCoef[i] /= b;

	return *this;
}

double&	Polynom::operator[]( int i )
{
	int	nTmp;

	if( i<0 || i>(nTmp=GetDegree(*this)) )
		fprintf(stderr, "\n Indeks polinoma izvan opsega !!!");

	assert( i>=0 );
	assert( i<=nTmp );

	return m_pdCoef[i];
}

Polynom& Polynom::operator=( const Polynom &b )
{
	if( m_nDegree < b.m_nDegree )
	{
		delete m_pdCoef;				// oslobodi memoriju zauzetu "starim" polinomom
		m_pdCoef  = new double[b.m_nDegree];
		m_nDegree = b.m_nDegree;
	}
	
	if( m_nDegree > b.m_nDegree )
		for( int i=b.m_nDegree + 1; i <= m_nDegree; i++ )	// postavi na 0 one koeficijente koji su višeg reda od onog koji se pridružuje
			m_pdCoef[i] = 0;
	
	for( int i=0; i <= b.m_nDegree; i++ )
		m_pdCoef[i] = b.m_pdCoef[i];

	return	*this;
}

void	ReducePolynom( Polynom *a )
{
	int		OldDegree;
	double	*pTmp;

	OldDegree = GetDegree(*a);
	for( unsigned i=GetDegree(*a); i>= 0; i-- )
		if( a->m_pdCoef[i] == 0 )
			a->m_nDegree--;
		else
			break;
	
	if( a->m_nDegree != OldDegree )
	{
		pTmp = new double[a->m_nDegree];
		for( int i=0; i <= a->m_nDegree; i++ )
			pTmp[i] = a->m_pdCoef[i];

		delete	[]a->m_pdCoef;
		a->m_pdCoef = pTmp;
	}

}

void	Input( Polynom *a )
{
	for( int i=0; i<=GetDegree(*a); i++ )
	{
		fprintf(stdout, "\n unesi %d.-ti koeficijent : ", i );
		fscanf(stdin, "%lf", &(a->m_pdCoef[i]) );
	}
}

void	Print( const Polynom &a )
{
	printf("\n  ");
	
	for( int i=GetDegree(a); i>=0; i-- )
		fprintf(stdout, "%4.2f + X^%d + ", a.m_pdCoef[i], i );
}
