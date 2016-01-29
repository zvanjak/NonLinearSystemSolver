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


/********************************************************************/
/*						COMPLEX			MATRIX				*/
/********************************************************************/


ComplexMatrix::ComplexMatrix()
{
}

ComplexMatrix::ComplexMatrix( UINT RowNum, UINT ColNum )
{
	int     i;

	m_nRowNum = (RowNum > 0) ? RowNum : 0;
	m_nColNum = (ColNum > 0) ? ColNum : 0;

	if( m_nRowNum>0 )
	{
		if( !(m_pdElem = new Complex  *[m_nRowNum]) )         /* rezerviraj mjesto za 'm' pokazivaüa na 'm' redaka */
		{				// nema memorije
			fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
			assert(0);
		}
		else
			for( i=0; i<m_nRowNum; i++ )        
				if( m_nColNum > 0 )     
				{
					if( !(m_pdElem[i] = new Complex[m_nColNum] ) )
					{			// nema memorije
						fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
						assert(0);
					}
					else
						for( int j=0; j<m_nColNum; j++ )
							m_pdElem[i][j] = Complex(0,0);
				}
				else
					m_pdElem[i] = NULL;
	}
	else                         
	{	// krivi broj redaka
		fprintf(stderr,"\n Broj redaka mora biti veci od 0 !!!");
		assert(0);
	}
}

ComplexMatrix::ComplexMatrix(	const ComplexMatrix &Copy )
{
	int     i;

	if( Copy.m_nRowNum <= 0 )
	{	// krive dimenzije matrice
		fprintf(stderr, "\n Krive dimenzije matrice kod pridruzivanja !!!");
		assert(0);
	}

	if( Copy.m_pdElem != NULL )
    {
		m_nRowNum = (Copy.m_nRowNum > 0) ? Copy.m_nRowNum : 0;
		m_nColNum = (Copy.m_nColNum > 0) ? Copy.m_nColNum : 0;

		if( !(m_pdElem = new Complex *[m_nRowNum]) )         /* rezerviraj mjesto za 'm' pokazivaüa na 'm' redaka */
		{				// nema memorije
			fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
			assert(0);
		}
		else
		{
			for( i=0; i<m_nRowNum; i++ )
				if( Copy.m_nColNum > 0 )
				{
					if( !(m_pdElem[i] = new Complex[m_nColNum] ) )
					{				// nema memorije
						fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
						assert(0);
					}
					else
						memmove( m_pdElem[i], Copy.m_pdElem[i], sizeof(Complex) * Copy.m_nColNum );
				}
				else
					m_pdElem[i] = NULL;
		}
	}                                                   
}

ComplexMatrix::~ComplexMatrix()
{                       
	int		i;
    	 
	if( m_pdElem != NULL )
	{                             
		if( m_nColNum != 0 )
		{
			for( i=0; i<m_nRowNum; i++ )        
				if( m_pdElem[i] != NULL )
				{
					delete []m_pdElem[i];
				}
		}
		if ( m_nRowNum != 0 )
		{
			delete []m_pdElem;
			m_pdElem = NULL;
		}                                    
	}
}

UINT	RowNum( const ComplexMatrix &a )
{
	return	a.m_nRowNum;
}

UINT	ColNum( const ComplexMatrix &a )
{
	return	a.m_nColNum;
}

Complex	Elem( const ComplexMatrix &a, UINT i, UINT j )
{
	if( i>=0 && i<RowNum(a) && j>=0 && j<ColNum(a) )
		return ( a.m_pdElem[i][j] );
	else
	{
		fprintf(stderr, "\n Krivi indeks za element matrice !!!");
		return	a.m_pdElem[0][0];
	}
}

Complex **ComplexMatrix::GetPtr( void )
{
	return m_pdElem;
}

/********************************************************************************************/

ComplexMatrix  ComplexMatrix::operator+( const ComplexMatrix &b ) const
{
	ComplexMatrix  ret(m_nRowNum,m_nColNum);

	if( m_nRowNum == b.m_nRowNum && m_nColNum==b.m_nColNum )
	{
		for( int i=0; i<m_nRowNum; i++ )
			for( int j=0; j<m_nColNum; j++ )
				ret.m_pdElem[i][j] = m_pdElem[i][j] + b.m_pdElem[i][j];
	}
	else
	{			// kriva dimenzija matrice
		fprintf(stderr, "\n Krive dimenzije matrice kod pridruzivanja !!!");
		assert(0);
	}

	return ret;
}

ComplexMatrix  ComplexMatrix::operator-( const ComplexMatrix &b ) const
{
	ComplexMatrix  ret(m_nRowNum,m_nColNum);

	if( m_nRowNum == b.m_nRowNum && m_nColNum==b.m_nColNum )
	{
		for( int i=0; i<m_nRowNum; i++ )
			for( int j=0; j<m_nColNum; j++ )
				ret.m_pdElem[i][j] = m_pdElem[i][j] - b.m_pdElem[i][j];
	}
	else
	{			// kriva dimenzija matrice
		fprintf(stderr, "\n Krive dimenzije matrice kod pridruzivanja !!!");
		assert(0);
	}

	return ret;
}

ComplexMatrix  ComplexMatrix::operator*( const	ComplexMatrix &b ) const
{
	ComplexMatrix	ret(m_nRowNum, b.m_nColNum);

	if( m_nColNum==b.m_nRowNum )
	{
		for( int i=0; i<ret.m_nRowNum; i++ )
			for( int j=0; j<ret.m_nColNum; j++ )
			{
				ret.m_pdElem[i][j] = 0;
				for( int k=0; k<m_nColNum; k++ )
					ret.m_pdElem[i][j] += m_pdElem[i][k] * b.m_pdElem[k][j];
			}
	}
	else
	{				// krive dimenzije matrice
		fprintf(stderr, "\n Krive dimenzije matrice kod pridruzivanja !!!");
		assert(0);
	}

	return	ret;
}

ComplexVector	ComplexMatrix::operator*( Vector &b )
{
	int			i, j;
	ComplexVector	ret(m_nRowNum);

	if( m_nColNum == b.GetDim() )
	{
		for( i=0; i<m_nRowNum; i++ )
		{
			ret[i] = 0;
			for( j=0; j<m_nColNum; j++ )
				ret[i] += m_pdElem[i][j] * b[j];
		}
	}
	else
	{
		printf("Krive dimenzije kod mnozenja matrice i vektora !!!");
		assert(0);
	}

	return ret;
}

ComplexVector	ComplexMatrix::operator*( ComplexVector &b )
{
	int			i, j;
	ComplexVector	ret(m_nRowNum);

	if( m_nColNum == b.GetDim() )
	{
		for( i=0; i<m_nRowNum; i++ )
		{
			ret[i] = 0;
			for( j=0; j<m_nColNum; j++ )
				ret[i] += m_pdElem[i][j] * b[j];
		}
	}
	else
	{
		printf("Krive dimenzije kod mnozenja matrice i vektora !!!");
		assert(0);
	}

	return ret;
}

ComplexMatrix  ComplexMatrix::operator+=( const ComplexMatrix &b )
{
	if( m_nRowNum == b.m_nRowNum && m_nColNum==b.m_nColNum )
	{
		for( int i=0; i<m_nRowNum; i++ )
			for( int j=0; j<m_nColNum; j++ )
				m_pdElem[i][j] += b.m_pdElem[i][j];
	}
	else
	{			// kriva dimenzija matrice
		fprintf(stderr, "\n Krive dimenzije matrice kod pridruzivanja !!!");
		assert(0);
	}

	return *this;
}

ComplexMatrix  ComplexMatrix::operator-=( const ComplexMatrix &b )
{
	if( m_nRowNum == b.m_nRowNum && m_nColNum==b.m_nColNum )
	{
		for( int i=0; i<m_nRowNum; i++ )
			for( int j=0; j<m_nColNum; j++ )
				m_pdElem[i][j] -= b.m_pdElem[i][j];
	}
	else
	{			// kriva dimenzija matrice
		fprintf(stderr, "\n Krive dimenzije matrice kod pridruzivanja !!!");
		assert(0);
	}

	return *this;
}

ComplexMatrix  ComplexMatrix::operator*=( const	ComplexMatrix &b )
{
	ComplexMatrix	ret(m_nRowNum, b.m_nColNum);
	int	i, j;

	if( m_nColNum==b.m_nRowNum )
	{
		// najprije izmnoûi matrice
		for( i=0; i<ret.m_nRowNum; i++ )
			for( j=0; j<ret.m_nColNum; j++ )
			{
				ret.m_pdElem[i][j] = 0;
				for( int k=0; k<m_nColNum; k++ )
					ret.m_pdElem[i][j] += m_pdElem[i][k] * b.m_pdElem[k][j];
			}
		
		// a sada to sve stavi u onu s lijeve strane
		for( i=0; i<ret.m_nRowNum; i++ )
			for( j=0; j<ret.m_nColNum; j++ )
				m_pdElem[i][j] = ret.m_pdElem[i][j];
	}
	else
	{				// krive dimenzije matrice
		fprintf(stderr, "\n Krive dimenzije matrice kod pridruzivanja !!!");
		assert(0);
	}

	return	*this;
}

ComplexMatrix	ComplexMatrix::operator*=( double b )
{
	for( int i=0; i<this->m_nRowNum; i++ )
		for( int j=0; j<this->m_nColNum; j++ )
			m_pdElem[i][j] *= b;

	return *this;
}

ComplexMatrix	operator*( const double a, const ComplexMatrix &b ) 
{   
	ComplexMatrix	ret(b.m_nRowNum, b.m_nColNum);
	int		i,j;

	for( i=0; i<b.m_nRowNum; i++ )
		for( j=0; j<b.m_nColNum; j++ )
			ret.m_pdElem[i][j] = b.m_pdElem[i][j] * a;

	return	ret;
}

ComplexMatrix	operator*( const ComplexMatrix &a, const double b )
{   
	ComplexMatrix	ret(a.m_nRowNum, a.m_nColNum);
	int		i,j;

	for( i=0; i<a.m_nRowNum; i++ )
		for( j=0; j<a.m_nColNum; j++ )
			ret.m_pdElem[i][j] = a.m_pdElem[i][j] * b;

	return	ret;
}
	
/*ComplexMatrix  ComplexMatrix::operator/( const ComplexMatrix &b ) const
	{
		ComplexMatrix	Ret(m,m,"Div");
		
		if( m==n && b.m==m && b.n==n )
			Ret = *this * b.Inv();
		else
			Error(_MATRIX_DIMENSION_WRONG);
		
		return	Ret;
    }*/

ComplexMatrix& ComplexMatrix::operator=( const	ComplexMatrix &b )
{
	int		i;

	if( m_pdElem != NULL && b.m_pdElem != NULL )
	{
		if( m_nRowNum != b.m_nRowNum || m_nColNum != b.m_nColNum )       	/* ako matrice koje se pridruûuju nisu iste dimenzije */
		{
			for( i=0; i < m_nRowNum; i++ )		/* stare dimenzije izbriöi i ustanovi nove */
				if( m_pdElem[i] != NULL )
					delete m_pdElem[i];
			if( m_pdElem != NULL )
				delete m_pdElem;
						
			m_nRowNum = b.m_nRowNum;
			m_nColNum = b.m_nColNum;
						
			if( !(m_pdElem= new Complex *[m_nRowNum]) )
				for( i=0; i < m_nRowNum; i++ )
					if( (m_pdElem[i] = new Complex[m_nColNum]) == NULL)
					{				// nema memorije
						for( int j=0; j < i; j++ )
							delete m_pdElem[j];
						delete m_pdElem;
					}
			else
			{				// nema memorije
			}
		}
		for(i=0; i<m_nRowNum; i++ )
			memmove(m_pdElem[i], b.m_pdElem[i], sizeof(Complex) * b.m_nColNum );
	}

	return *this;
}

/*********************************************************************/

Complex ComplexMatrix::GetElem( int i, int j )
{
	return m_pdElem[i][j];
}

void ComplexMatrix::SetElem( int i, int j, Complex dCValue )
{
	m_pdElem[i][j] = dCValue;
}

void ComplexMatrix::SetElem( int i, int j, double dReal, double dImag )
{
	m_pdElem[i][j].m_dReal = dReal;
	m_pdElem[i][j].m_dImag = dImag;
}


void	Print( const ComplexMatrix &a)
{
	int		i,j;

	if( a.m_pdElem != NULL && a.m_nRowNum!=0 && a.m_nColNum!=0 )
 	{
		for( i=0; i<a.m_nRowNum; i++ )
		{
			fprintf(stdout, "\n");
			for( j=0; j<a.m_nColNum; j++ )
				fprintf(stdout, "%10.3f + i %-10.3f", Re(a.m_pdElem[i][j]), Im(a.m_pdElem[i][j]));
		}
	}                   
}

void	ComplexMatrix::MakeUnitMatrix( void )
{
	if( m_nRowNum == m_nColNum )
	{
		for( int i=0; i<m_nRowNum; i++ )
			for( int j=0; j<m_nColNum; j++ )
				if( i == j )
				{
					m_pdElem[i][j].m_dReal = 1;
					m_pdElem[i][j].m_dImag = 1;
				}
				else
					m_pdElem[i][j].m_dReal = m_pdElem[i][j].m_dImag = 0;
	}
}

void ComplexMatrix::MakeRandomElements( double dMin, double dMax )
{
	int			i,j;
	double	transf;

	srand( (unsigned)time( NULL ) );
	transf = (dMax - dMin) / RAND_MAX;

	for( i=0; i<m_nRowNum; i++ )
		for( j=0; j<m_nColNum; j++ )
		{
			m_pdElem[i][j].m_dReal = rand() * transf + dMin;
			m_pdElem[i][j].m_dImag = rand() * transf + dMin;
		}
}

void	ComplexMatrix::Clear( void )
{
	for( int i=0; i<m_nRowNum; i++ )
		for( int j=0; j<m_nColNum; j++ )
			m_pdElem[i][j].m_dReal = m_pdElem[i][j].m_dImag = 0;
}

ComplexMatrix  Invert( const ComplexMatrix &T )
{
	ComplexMatrix	Ret(T.m_nRowNum,T.m_nRowNum), a(T.m_nRowNum, T.m_nColNum);
	
	int				i,j,k, m, n, i1, l;
	Complex		s, help;
	double		a1;
	
	m = T.m_nRowNum;
	n = T.m_nColNum;

	a = T;

	if( m == n )
	{
		//matricu Ret izjednaËiti sa jediniËnom matricom
		for( i=0; i<n; i++ )
		{
			Ret.m_pdElem[i][i].Set(1,0);
		}

		for( k=0; k<n; k++ )
		{
//			if( k != n-1 )
			{
				i1 = k;
				a1 = Module(a.m_pdElem[k][k]);

				// u k-tom stupcu nai najveÊi element po apsolutnoj vrijednosti
				for( i=k+1; i<n; i++ )
				{
					if( a1 < Module(a.m_pdElem[i][k] ) )
					{
						i1 = i;
						a1 = Module(a.m_pdElem[i][k]);
					}
					if( i1 != k )
					{
						//zamjeniti retke i1 i k u matricama 'a' i 'Ret'
						for( l=0; l<n; l++ )
						{
							help = a.m_pdElem[i1][l];
							a.m_pdElem[i1][l] = a.m_pdElem[k][l];
							a.m_pdElem[k][l] = help;

							help = Ret.m_pdElem[i1][l];
							Ret.m_pdElem[i1][l] = Ret.m_pdElem[k][l];
							Ret.m_pdElem[k][l] = help;
						}
					}
				}

				if( a1 == 0 )		// znaËi singularna matrica
				{
					fprintf(stderr, "\n Invert : singularna matrica !!!");
					assert(0);
					goto _matInvExit;
				}

				// sada dijagonalni element svodimo na 1
				s = a.m_pdElem[k][k] / pow(Module(a.m_pdElem[k][k]),2);
				for( j=0; j<n; j++ )
				{
					a.m_pdElem[k][j] /= s;
					Ret.m_pdElem[k][j] /= s;
				}
				
				for( i=k; i<n; i++ )
				{
					s = a.m_pdElem[i][k];
					if( i != k )
						for( j=0; j<n; j++ )
						{
							a.m_pdElem[i][j] -= a.m_pdElem[k][j] * s;
							Ret.m_pdElem[i][j] -= Ret.m_pdElem[k][j] * s;
						}
				}

			}
		}
		// sada treba eliminirati i gornji desni trokutni dio 
		for( k=n-1; k>0 ; k-- )
		{
			for( i=k-1; i>=0; i-- )
			{
				s = a.m_pdElem[i][k];
				for( j=0; j<n; j++ )
				{
					a.m_pdElem[i][j] -= a.m_pdElem[k][j] * s;
					Ret.m_pdElem[i][j] -= Ret.m_pdElem[k][j] * s;
				}
			}
		}
	}

_matInvExit:
		return Ret;
}

/**************************************************************/
//			LU DEKOMPOZICIJA

void LUDecomp( ComplexMatrix &a, int *indx, double *d )
{
	int i, j, k, imax;
	int			ret = 1;
	double	big, temp, dum;
	double	*vv;
	Complex		sum, chg;

	assert( a.m_nRowNum == a.m_nColNum );

	vv = (double *) calloc( a.m_nRowNum, sizeof(double) );
	*d = 1.0;
	for( i=0; i<a.m_nRowNum; i++ )
	{
		big = 0.0;
		for( j=0; j<a.m_nRowNum; j++ )
			if( (temp=Module(a.m_pdElem[i][j])) > big )
				big = temp;
		if( big == 0.0)
		{
			printf("\n Singularna matrica u LUDecomp !!!");
			ret = 0;
			free(vv);
			return;
//			assert(0);
		}
		vv[i] = 1.0 / big;
	}
	for( j=0; j<a.m_nRowNum; j++ )
	{
		for( i=0; i<j; i++ )
		{
			sum = a.m_pdElem[i][j];
			for( k=0; k<i; k++ )
				sum = sum - a.m_pdElem[i][k] * a.m_pdElem[k][j];
			a.m_pdElem[i][j] = sum;
		}
		big = 0.0;
		for( i=j; i<a.m_nRowNum; i++ )
		{
			sum = a.m_pdElem[i][j];
			for( k=0; k<j; k++ )
				sum = sum - a.m_pdElem[i][k] * a.m_pdElem[k][j];
			a.m_pdElem[i][j] = sum;
			if( (dum=vv[i] * Module(sum)) >= big )
			{
				big = dum;
				imax = i;
			}
		}
		if( j != imax )
		{
			for( k=0; k<a.m_nRowNum; k++ )
			{
				chg = a.m_pdElem[imax][k];
				a.m_pdElem[imax][k] = a.m_pdElem[j][k];
				a.m_pdElem[j][k] = chg;
			}
			*d = -(*d);
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if( a.m_pdElem[j][j] == Complex(0.0, 0.0) )
		{
			a.m_pdElem[j][j] = Complex(1e-20, 1e-20);
			printf("\n Singularna matrica u LUDecomp !!!!!");
			ret = 0;
		}
		if( j != a.m_nRowNum-1 )
		{
			chg = 1.0 / a.m_pdElem[j][j];
			for( i=j+1; i<a.m_nRowNum; i++ )
				a.m_pdElem[i][j] *= chg;
		}
	}
	free(vv);
	return;
}

void LUBackSubst( ComplexMatrix &a, int *indx, ComplexMatrix &b )
{
	int		ii=-1, ip;
	int		i, j;
	Complex	sum;

	assert( a.m_nRowNum == b.m_nRowNum );

	for( i=0; i<a.m_nRowNum; i++ )
	{
		ip = indx[i];
		sum = b.m_pdElem[ip][0];
		b.m_pdElem[ip][0] = b.m_pdElem[i][0];
		if( ii != -1 )
			for( j=ii; j<=i-1; j++ )
				sum = sum - a.m_pdElem[i][j] * b.m_pdElem[j][0];
		else if ( Module(sum) )
			ii = i;
		b.m_pdElem[i][0] = sum;
	}
	for( i=a.m_nRowNum-1; i>=0; i-- )
	{
		sum = b.m_pdElem[i][0];
		for( j=i+1; j<a.m_nRowNum; j++ )
			sum = sum - a.m_pdElem[i][j] * b.m_pdElem[j][0];
		b.m_pdElem[i][0] = sum / a.m_pdElem[i][i];
	}
}

void LUBackSubst( ComplexMatrix &a, int *indx, ComplexVector &b )
{
	int		ii=-1, ip;
	int		i, j;
	Complex	sum;

	assert( a.m_nRowNum == b.GetDim() );

	for( i=0; i<a.m_nRowNum; i++ )
	{
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if( ii != -1 )
			for( j=ii; j<=i-1; j++ )
				sum = sum - a.m_pdElem[i][j] * b[j];
		else if ( Module(sum) )
			ii = i;
		b[i] = sum;
	}
	for( i=a.m_nRowNum-1; i>=0; i-- )
	{
		sum = b[i];
		for( j=i+1; j<a.m_nRowNum; j++ )
			sum = sum - a.m_pdElem[i][j] * b[j];
		b[i] = sum / a.m_pdElem[i][i];
	}
}

