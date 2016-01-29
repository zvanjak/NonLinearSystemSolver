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


/*******************************************************************/
/*					MATRIX								*/
/*******************************************************************/

Matrix::Matrix()
{
}

Matrix::Matrix( int RowNum, int ColNum )
{
	int     i;

	m_nRowNum = (RowNum > 0) ? RowNum : 0;
	m_nColNum = (ColNum > 0) ? ColNum : 0;

	if( m_nRowNum>0 )
	{
		if( !(m_pdElem = new double  *[m_nRowNum]) )         /* rezerviraj mjesto za 'm' pokazivaüa na 'm' redaka */
		{				// nema memorije
			fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
			assert(0);
		}
		else
			for( i=0; i<m_nRowNum; i++ )        
				if( m_nColNum > 0 )     
				{
					if( !(m_pdElem[i] = new double[m_nColNum] ) )
					{			// nema memorije
						fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
						assert(0);
					}
					else
						for( int j=0; j<m_nColNum; j++ )
							m_pdElem[i][j] = 0;
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

Matrix::Matrix( int RowNum, int ColNum, int nParam )
{
	int     i;

	m_nRowNum = (RowNum > 0) ? RowNum : 0;
	m_nColNum = (ColNum > 0) ? ColNum : 0;

	if( m_nRowNum>0 )
	{
		if( !(m_pdElem = new double  *[m_nRowNum]) )         // rezerviraj mjesto za 'm' pokazivaüa na 'm' redaka 
		{				// nema memorije
			fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
			assert(0);
		}
		else
		{
			if( nParam == CONT_ALLOCATION )
			{
				// ovdje Êemo alocirati cijelu matricu tako da zauzima kontinuirani
				// blok memorije
				double		*pdTmp;

				pdTmp = new double[m_nRowNum * m_nColNum];
				
				for( i=0; i<m_nRowNum; i++ )
					m_pdElem[i] = pdTmp + m_nColNum*i;
			}
			else
				for( i=0; i<m_nRowNum; i++ )        
					if( m_nColNum > 0 )     
					{
						if( !(m_pdElem[i] = new double[m_nColNum] ) )
						{			// nema memorije
							fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
							assert(0);
						}
						else
							for( int j=0; j<m_nColNum; j++ )
								m_pdElem[i][j] = 0;
					}
					else
						m_pdElem[i] = NULL;
		}
	}
	else                         
	{	// krivi broj redaka
		fprintf(stderr,"\n Broj redaka mora biti veci od 0 !!!");
		assert(0);
	}
}

Matrix::Matrix(	const Matrix &Copy )
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

		if( !(m_pdElem = new double    *[m_nRowNum]) )         /* rezerviraj mjesto za 'm' pokazivaüa na 'm' redaka */
		{				// nema memorije
			fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
			assert(0);
		}
		else
		{
			for( i=0; i<m_nRowNum; i++ )
				if( Copy.m_nColNum > 0 )
				{
					if( !(m_pdElem[i] = new double[m_nColNum] ) )
					{				// nema memorije
						fprintf(stderr, "\n Nema memorije za alociranje matrice !!!");
						assert(0);
					}
					else
						memmove( m_pdElem[i], Copy.m_pdElem[i], sizeof(double) * Copy.m_nColNum );
				}
				else
					m_pdElem[i] = NULL;
		}
	}                                                   
}

Matrix::~Matrix()
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

int	RowNum( const Matrix &a )
{
	return	a.m_nRowNum;
}

int	ColNum( const Matrix &a )
{
	return	a.m_nColNum;
}

double	Elem( const Matrix &a, int i, int j )
{
	if( i>=0 && i<RowNum(a) && j>=0 && j<ColNum(a) )
		return ( a.m_pdElem[i][j] );
	else
	{
		fprintf(stderr, "\n Krivi indeks za element matrice !!!");
		return	a.m_pdElem[0][0];
	}
}

double **Matrix::GetPtr( void )
{
	return m_pdElem;
}

int	Matrix::IsSquare( void )
{
	if( m_nRowNum == m_nColNum )
		return 1;
	else
		return 0;
}

int	Matrix::IsSymetric( double dTiny )
{
	int		i, j, ret = 1;

	// da bi bila simetricna, matrica najprije mora biti kvadratna
	if( !IsSquare() )
		return 0;

	for( i=0; i<m_nRowNum; i++ )
		for( j=0; j<i; j++ )
			if( fabs(m_pdElem[i][j] - m_pdElem[j][i]) > dTiny )
				ret = 0;

	return ret;
}

/********************************************************************************************/

Matrix  Matrix::operator+( const Matrix &b ) const
{
	Matrix  ret(m_nRowNum,m_nColNum);

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

Matrix  Matrix::operator-( const Matrix &b ) const
{
	Matrix  ret(m_nRowNum,m_nColNum);

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

Matrix  Matrix::operator*( const	Matrix &b ) const
{
	Matrix	ret(m_nRowNum, b.m_nColNum);

	if( m_nColNum==b.m_nRowNum )
	{
		for( int i=0; i<ret.m_nRowNum; i++ )
			for( int j=0; j<ret.m_nColNum; j++ )
			{
				ret.m_pdElem[i][j] = 0;
				for(int k=0; k<m_nColNum; k++ )
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

Vector	Matrix::operator*( Vector &b )
{
	int			i, j;
	Vector	ret(m_nRowNum);

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

Matrix  Matrix::operator+=( const Matrix &b )
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

Matrix  Matrix::operator-=( const Matrix &b )
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

Matrix  Matrix::operator*=( const	Matrix &b )
{
	Matrix	ret(m_nRowNum, b.m_nColNum);
	int			i, j;

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

Matrix	Matrix::operator*=( double b )
{
	for( int i=0; i<this->m_nRowNum; i++ )
		for( int j=0; j<this->m_nColNum; j++ )
			m_pdElem[i][j] *= b;

	return *this;
}

Matrix	operator*( const double a, const Matrix &b ) 
{   
	Matrix	ret(b.m_nRowNum, b.m_nColNum);
	int		i,j;

	for( i=0; i<b.m_nRowNum; i++ )
		for( j=0; j<b.m_nColNum; j++ )
			ret.m_pdElem[i][j] = b.m_pdElem[i][j] * a;

	return	ret;
}

Matrix	operator*( const Matrix &a, const double b )
{   
	Matrix	ret(a.m_nRowNum, a.m_nColNum);
	int		i,j;

	for( i=0; i<a.m_nRowNum; i++ )
		for( j=0; j<a.m_nColNum; j++ )
			ret.m_pdElem[i][j] = a.m_pdElem[i][j] * b;

	return	ret;
}
	
/*Matrix  Matrix::operator/( const Matrix &b ) const
	{
		Matrix	Ret(m,m,"Div");
		
		if( m==n && b.m==m && b.n==n )
			Ret = *this * b.Inv();
		else
			Error(_MATRIX_DIMENSION_WRONG);
		
		return	Ret;
    }*/

Matrix& Matrix::operator=( const	Matrix &b )
{
	int		i;

	if( m_pdElem != NULL && b.m_pdElem != NULL )
	{
		if( m_nRowNum != b.m_nRowNum || m_nColNum != b.m_nColNum )       	/* ako matrice koje se pridruûuju nisu iste dimenzije */
		{
			fprintf(stdout, "Pridruzuju se matrice razlicitih dimenzija (%d,%d)->(%d,%d)!!!", m_nRowNum, m_nColNum, b.m_nRowNum, b.m_nColNum);
//			assert(0);
			for( i=0; i < m_nRowNum; i++ )		/* stare dimenzije izbriöi i ustanovi nove */
				if( m_pdElem[i] != NULL )
					delete m_pdElem[i];
			if( m_pdElem != NULL )
				delete []m_pdElem;
						
			m_nRowNum = b.m_nRowNum;
			m_nColNum = b.m_nColNum;
						
			if( !(m_pdElem= new double   *[m_nRowNum]) )
				for( i=0; i < m_nRowNum; i++ )
					if( (m_pdElem[i] = new double[m_nColNum]) == NULL)
					{				// nema memorije
						for( int j=0; j < i; j++ )
							delete m_pdElem[j];
						delete m_pdElem;
					}
			else
			{				// nema memorije
				fprintf(stderr, "Nema memorije !!!");
				assert(0);
			}
		}
		for(i=0; i<m_nRowNum; i++ )
			memmove(m_pdElem[i], b.m_pdElem[i], sizeof(double) * b.m_nColNum );
	}

	return *this;
}

double*	Matrix::operator[]( int i )
{
	if( i<0 || i> (int) m_nRowNum )
	{
		fprintf(stderr, "Array bound violation i matrix[] !!!");
		assert(0);
		return NULL;
	}
	else
		return m_pdElem[i];
}

/*****************************************************************/

double	Matrix::GetElem( int i, int j )
{
	return m_pdElem[i][j];
}

void	Matrix::SetElem( int i, int j, double dValue )
{
	if( i< m_nRowNum && j <m_nColNum )
		m_pdElem[i][j] = dValue;
	else
	{
		printf("Prekoracenje dimenzije matrice u SetElem !!!");
		assert(0);
	}
}

void	Matrix::MakeUnitMatrix( void )
{
	if( m_nRowNum == m_nColNum )
	{
		for( int i=0; i<m_nRowNum; i++ )
			for( int j=0; j<m_nColNum; j++ )
				if( i == j )
					m_pdElem[i][j] = 1;
				else
					m_pdElem[i][j] = 0;
	}
}

void Matrix::MakeRandomElements( double dMax, double dMin )
{
	int			i,j;
	double	transf;

	srand( (unsigned)time( NULL ) );
	transf = (dMax - dMin) / RAND_MAX;

	for( i=0; i<m_nRowNum; i++ )
		for( j=0; j<m_nColNum; j++ )
			m_pdElem[i][j] = rand() * transf + dMin;
}

void	Matrix::Clear( void )
{
	for( int i=0; i<m_nRowNum; i++ )
		for(  int j=0; j<m_nColNum; j++ )
			m_pdElem[i][j] = 0;
}

/*******************		MATRI»NE FUNKCIJE		**********************/


Matrix	Transp( const Matrix &T )
{
	Matrix		Ret(T.m_nColNum, T.m_nRowNum);
	int	i, j;

	for( i=0; i<Ret.m_nRowNum; i++ )
		for( j=0; j<Ret.m_nColNum; j++ )
			Ret.m_pdElem[i][j] = T.m_pdElem[j][i];
	return Ret;
}

Matrix  Invert( const Matrix &T )
{
	Matrix	Ret(T.m_nRowNum,T.m_nRowNum), a(T.m_nRowNum, T.m_nColNum);
	int			i,j,k, m, n, i1, l;
	double	a1, s, help;
	
	m = T.m_nRowNum;
	n = T.m_nColNum;

	a = T;

	if( m == n )
	{
		//matricu Ret izjednaËiti sa jediniËnom matricom
		Ret.MakeUnitMatrix();

		for( k=0; k<n; k++ )
		{
//			if( k != n-1 )
			{
				i1 = k;
				a1 = fabs(a.m_pdElem[k][k]);

				// u k-tom stupcu nai najveÊi element po apsolutnoj vrijednosti
				for( i=k+1; i<n; i++ )
				{
					if( a1 < fabs(a.m_pdElem[i][k] ) )
					{
						i1 = i;
						a1 = fabs(a.m_pdElem[i][k]);
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
				s = a.m_pdElem[k][k];
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

void	RightInverse( Matrix &a, Matrix *matInv )
{
	Matrix		V(a.m_nColNum, a.m_nColNum), B(a.m_nRowNum,1), X(a.m_nColNum,1);
	Vector		vecW(a.m_nColNum);
	int				i, j;

	if( a.m_nRowNum == matInv->m_nColNum && a.m_nColNum == matInv->m_nRowNum )
	{
		SingValDecomp( &a, &vecW, &V );

		for( i=0; i<a.m_nRowNum; i++ )
		{
			B.Clear();
			B[i][0] = 1;

			SVDBackSubst( a, &vecW, V, B, &X );
			
			for( j=0; j<a.m_nColNum; j++ )
				(*matInv)[j][i] = X[j][0];
		}
	}
	else
	{
		printf("Krive dimenzije matrice u racunanju desnog inverza !!!");
		assert(0);
	}
}

void	LeftInverse( Matrix &a, Matrix *matInv )
{
	Matrix		aTransp(a.m_nColNum, a.m_nRowNum);
	Vector		vecW(aTransp.m_nColNum);
	int				i, j;

	if( a.m_nRowNum == matInv->m_nColNum && a.m_nColNum == matInv->m_nRowNum )
	{
		Matrix		V(aTransp.m_nColNum, aTransp.m_nColNum), B(aTransp.m_nRowNum,1);
		Matrix		X(aTransp.m_nColNum,1), retTransp(aTransp.m_nColNum, aTransp.m_nRowNum);

		aTransp = Transp(a);

		SingValDecomp( &aTransp, &vecW, &V );

		for( i=0; i<aTransp.m_nRowNum; i++ )
		{
			B.Clear();
			B[i][0] = 1;

			SVDBackSubst( aTransp, &vecW, V, B, &X );
			
			for( j=0; j<aTransp.m_nColNum; j++ )
				retTransp[j][i] = X[j][0];
		}
		*matInv = Transp(retTransp);
	}
	else
	{
		printf("Krive dimenzije matrice u racunanju lijevog inverza !!!");
		assert(0);
	}
}

/**************************************************************/
//			LU DEKOMPOZICIJA

int LUDecomp( Matrix &a, int *indx, double *d )
{
	int i, j, k, imax;
	int			ret = 1;
	double	big, dum , sum, temp;
	double	*vv;

	assert( a.m_nRowNum == a.m_nColNum );

	vv = (double *) calloc( a.m_nRowNum, sizeof(double) );
	*d = 1.0;
	for( i=0; i<a.m_nRowNum; i++ )
	{
		big = 0.0;
		for( j=0; j<a.m_nRowNum; j++ )
			if( (temp=fabs(a.m_pdElem[i][j])) > big )
				big = temp;
		if( big == 0.0)
		{
			printf("\n Singularna matrica u LUDecomp !!!");
			free(vv);

			return LU_SING;
		}
		vv[i] = 1.0 / big;
	}
	for( j=0; j<a.m_nRowNum; j++ )
	{
		for( i=0; i<j; i++ )
		{
			sum = a.m_pdElem[i][j];
			for( k=0; k<i; k++ )
				sum -= a.m_pdElem[i][k] * a.m_pdElem[k][j];
			a.m_pdElem[i][j] = sum;
		}
		big = 0.0;
		for( i=j; i<a.m_nRowNum; i++ )
		{
			sum = a.m_pdElem[i][j];
			for( k=0; k<j; k++ )
				sum -= a.m_pdElem[i][k] * a.m_pdElem[k][j];
			a.m_pdElem[i][j] = sum;
			if( (dum=vv[i] * fabs(sum)) >= big )
			{
				big = dum;
				imax = i;
			}
		}
		if( j != imax )
		{
			for( k=0; k<a.m_nRowNum; k++ )
			{
				dum = a.m_pdElem[imax][k];
				a.m_pdElem[imax][k] = a.m_pdElem[j][k];
				a.m_pdElem[j][k] = dum;
			}
			*d = -(*d);
			vv[imax] = vv[j];
		}
		indx[j] = imax;
		if( a.m_pdElem[j][j] == 0.0 )
		{
			a.m_pdElem[j][j] = 1e-20;
			
			printf("\n Singularna matrica u LUDecomp !!!!!");
			ret = LU_SING;
		}
		if( j != a.m_nRowNum-1 )
		{
			dum = 1.0 / a.m_pdElem[j][j];
			for( i=j+1; i<a.m_nRowNum; i++ )
				a.m_pdElem[i][j] *= dum;
		}
	}
	free(vv);
	return ret;
}

void LUBackSubst( Matrix &a, int *indx, Matrix &b )
{
	int		ii=-1, ip;
	int		i, j;
	double	sum;

	assert( a.m_nRowNum == b.m_nRowNum );

	for( i=0; i<a.m_nRowNum; i++ )
	{
		ip = indx[i];
		sum = b.m_pdElem[ip][0];
		b.m_pdElem[ip][0] = b.m_pdElem[i][0];
		if( ii != -1 )
			for( j=ii; j<=i-1; j++ )
				sum -= a.m_pdElem[i][j] * b.m_pdElem[j][0];
		else if (sum)
			ii = i;
		b.m_pdElem[i][0] = sum;
	}
	for( i=a.m_nRowNum-1; i>=0; i-- )
	{
		sum = b.m_pdElem[i][0];
		for( j=i+1; j<a.m_nRowNum; j++ )
			sum -= a.m_pdElem[i][j] * b.m_pdElem[j][0];
		b.m_pdElem[i][0] = sum / a.m_pdElem[i][i];
	}
}

void LUBackSubst( Matrix &a, int *indx, Vector &b )
{
	int		ii=-1, ip;
	int		i, j;
	double	sum;

	assert( a.m_nRowNum == b.GetDim() );

	for( i=0; i<a.m_nRowNum; i++ )
	{
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if( ii != -1 )
			for( j=ii; j<=i-1; j++ )
				sum -= a.m_pdElem[i][j] * b[j];
		else if (sum)
			ii = i;
		b[i] = sum;
	}
	for( i=a.m_nRowNum-1; i>=0; i-- )
	{
		sum = b[i];
		for( j=i+1; j<a.m_nRowNum; j++ )
			sum -= a.m_pdElem[i][j] * b[j];
		b[i] = sum / a.m_pdElem[i][i];
	}
}

void LUBackSubst( Matrix &a, int *indx, double b[] )
{
	int		ii=-1, ip;
	int		i, j;
	double	sum;

//	assert( a.m_nRowNum == b.GetDim() );

	for( i=0; i<a.m_nRowNum; i++ )
	{
		ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		if( ii != -1 )
			for( j=ii; j<=i-1; j++ )
				sum -= a.m_pdElem[i][j] * b[j];
		else if (sum)
			ii = i;
		b[i] = sum;
	}
	for( i=a.m_nRowNum-1; i>=0; i-- )
	{
		sum = b[i];
		for( j=i+1; j<a.m_nRowNum; j++ )
			sum -= a.m_pdElem[i][j] * b[j];
		b[i] = sum / a.m_pdElem[i][i];
	}
}

/**************************************************************/
//			QR DEKOMPOZICIJA

void	QRDecomp( Matrix &a, Vector &c, Vector &d, int *sing )
{
	//	Vröi QR dekompoziciju
	//		Gornja trokutna matrica R se vraÊa u gornjem trokutu matrice 'a'
	//		osim dijagonalnih elemenata, koji se vraÊaju u vektoru 'd'
	//		Ortogonalan matrica Q se vraÊa u faktoriziranoj formi
	int			i, j, k, n;
	double	scale, sigma, sum, tau;

	n = RowNum(a);
	*sing = 0;
	for( k=1; k<=n; k++ )
	{
		scale = 0.0;
		for( i=k; i<=n; i++ )
			scale = DMAX(scale, fabs(a[i-1][k-1]));
		if( scale == 0.0 )
		{
			*sing = 1;
			c[k-1] = d[k-1] = 0.0;
		}
		else
		{
			for( i=k; i<=n; i++ )
				a[i-1][k-1] /= scale;
			for( sum=0.0, i=k; i<=n; i++ )
				sum += DSQR(a[i-1][k-1]);
			sigma = SIGN(sqrt(sum), a[k-1][k-1]);
			a[k-1][k-1] += sigma;
			c[k-1] = sigma * a[k-1][k-1];
			d[k-1] = -scale*sigma;

			for( j=k+1; j<=n; j++ )
			{
				for( sum=0.0, i=k; i<=n; i++ )
					sum += a[i-1][k-1] * a[i-1][j-1];
				tau = sum / c[k-1];
				for( i=k; i<=n; i++ )
					a[i-1][j-1] -= tau* a[i-1][k-1];
			}
		}
	}
	d[n-1] = a[n-1][n-1];
	if( d[n-1] == 0.0 )
		*sing = 1;
}

void	QSolve( Matrix &a, Vector &c, Vector &d, Vector &b )
{
	//	Rijeöava sistem pomoÊu QR dekompozicije
	//		u 'b' se nalazi vektor desne strane, i u njemu se na kraju nalazi rijeöenje sistema
	int		i, j, n;
	double	sum, tau;

	n = RowNum(a);
	for( j=1; j<=n; j++ )
	{
		for( sum=0.0, i=j; i<=n; i++ )
			sum += a[i-1][j-1] * b[i-1];
		tau = sum / c[j-1];
		for( i=j; i<=n; i++ )
			b[i-1] -= tau*a[i-1][j-1];
	}
	RSolve(a,d,b);
}

void	RSolve( Matrix &a, Vector &d, Vector &b )
{
	//	Rijeöava R*x = b, gdje je R gornja trokutna matrica spremljena u 'a' i 'd'
	int			i, j, n;
	double	sum;

	n = RowNum(a);
	b[n-1] /= d[n-1];
	for( i=n-1; i>=1; i-- )
	{
		for( sum=0.0, j=i+1; j<=n; j++ )
			sum += a[i-1][j-1] * b[j-1];
		b[i-1] = (b[i-1] - sum) / d[i-1];
	}
}

void	QRUpdate( Matrix &r, Matrix &qt, Vector &u, Vector &v )
{
	int			i, j, k, n;

	n = RowNum(r);
	for( k=n; k>=1; k-- )
	{
		if( u[k-1] )
			break;
	}
	if( k < 1 )
		k=1;
	for( i=k-1; i>=1; i-- )
	{
		JacRotate(r,qt,i,u[i-1],-u[i+1-1]);
		if( u[i-1] == 0.0 )
			u[i-1] = fabs(u[i+1-1]);
		else if( fabs(u[i-1]) > fabs(u[i+1-1]) )
			u[i-1] = fabs(u[i-1]) * sqrt(1.0 + DSQR(u[i+1-1] / u[i-1]) );
		else
			u[i-1] = fabs(u[i+1-1]) * sqrt(1.0 + DSQR(u[i-1]/u[i+1-1]));
	}
	for( j=1; j<=n; j++ )
		r[1-1][j-1] += u[1-1] * v[j-1];
	for( i=1; i<k; i++ )
		JacRotate(r,qt,i,r[i-1][i-1],-r[i+1-1][i-1] );
}

void	JacRotate( Matrix &r, Matrix &qt, int i, double a, double b )
{
	int			j, n;
	double	c, fact, s, w, y;
	
	n = RowNum(r);

	if( a == 0 )
	{
		c = 0.0;
		s = ( b >= 0.0 ? 1.0 : -1.0 );
	}
	else if( fabs(a) > fabs(b) )
	{
		fact = b / a;
		c = SIGN(1.0/sqrt(1.0 + (fact*fact)), a);
		s = fact * c;
	}
	else
	{
		fact = a / b;
		s = SIGN(1.0 / sqrt(1.0 + (fact*fact)), b);
		c = fact * s;
	}

	for( j=i; j<=n; j++ )
	{
		y=r[i-1][j-1];
		w = r[i+1-1][j-1];
		r[i-1][j-1] = c*y - s*w;
		r[i+1-1][j-1] = s*y + c*w;
	}
	for( j=1; j<=n; j++ )
	{
		y = qt[i-1][j-1];
		w = qt[i+1-1][j-1];
		qt[i-1][j-1] = c*y -s*w;
		qt[i+1-1][j-1] = s*y + c*w;
	}
}

/*************************************************************/
//		SINGULAR VALUE DEKOMPOZICIJA

int	SingValDecomp(  Matrix *a, Vector *vecW, Matrix *v )
{
	int			m, n;
	int			flag, i, j, jj, k, l, nm, its;
	double	anorm, c, f, g, h, s, scale, x, y, z, *rv1;

	m = a->m_nRowNum;
	n = a->m_nColNum;

	if( vecW->GetDim() != n )
		return SVD_VECTOR_W_DIM;
	if( v->m_nRowNum != n || v->m_nColNum != n )
		return SVD_MATRIX_V_DIM;

	rv1 = (double *) calloc(n, sizeof(double) );
	g = scale = anorm = 0.0;			// Householderova redukcija na bidiagonalnu formu
	
	for ( i=0; i<n; i++ )
	{
		l=i+1;
		rv1[i] = scale * g;
		g = s = scale = 0.0;
		if( i < m )
		{
			for( k=i; k<m; k++ )
				scale += fabs((*a)[k][i]);
			if( scale )
			{
				for( k=i; k<m; k++ )
				{
					(*a)[k][i] /= scale;
					s += (*a)[k][i] * ((*a)[k][i]);
				}
				f = (*a)[i][i];
				g = -SIGN(sqrt(s),f);
				h = f*g - s;
				(*a)[i][i] = f - g;
				
				for( j=l; j<n; j++ )
				{
					for( s=0.0, k=i; k<m; k++ )
						s += (*a)[k][i] * ((*a)[k][j]);
				
					f = s/h;
					for( k=i; k<m; k++ )
						(*a)[k][j] += f * ((*a)[k][i]);
				}
				for( k=i; k<m; k++ )
					(*a)[k][i] *= scale;
			}
		}
		(*vecW)[i] = scale * g;
		g = s = scale = 0.0;
		if( i<m && i!=(n-1) )			// OPREZ
		{
			for( k=l; k<n; k++ )
				scale += fabs((*a)[i][k]);
			if( scale )
			{
				for(k=l; k<n; k++ )
				{
					(*a)[i][k] /= scale;
					s += (*a)[i][k] * ((*a)[i][k]);
				}
				f = (*a)[i][l];
				g = -SIGN(sqrt(s),f);
				h=f*g - s;
				(*a)[i][l] = f -g;
				for( k=l; k<n; k++ )
					rv1[k] = (*a)[i][k] / h;
				for( j=l; j<m; j++ )
				{
					for( s=0.0, k=l; k<n; k++ )
						s += (*a)[j][k] * ((*a)[i][k]);
					for( k=l; k<n; k++ )
						(*a)[j][k] += s*rv1[k];
				}
				for( k=l; k<n; k++ )
					(*a)[i][k] *= scale;
			}
		}
		anorm = DMAX(anorm, (fabs((*vecW)[i])+fabs(rv1[i])));
	}
	for(i=n-1;i>=0; i-- )			// sakupljanje transformacija desne strane
	{
		if( i < n-1 )
		{
			if( g )
			{
				for( j=l; j<n; j++ )
					(*v)[j][i] = ((*a)[i][j] / ((*a)[i][l])) / g;
				for( j=l; j<n; j++ )
				{
					for( s=0.0, k=l; k<n; k++ )
						s += (*a)[i][k] * ((*v)[k][j]);
					for( k=l; k<n; k++ )
						(*v)[k][j] += s * ((*v)[k][i]);
				}
			}
			for( j=l; j<n; j++ )
				(*v)[i][j] = (*v)[j][i] = 0.0;
		}
		(*v)[i][i] = 1.0;
		g = rv1[i];
		l=i;
	}
	
	for( i=IMIN(m,n)-1; i>=0; i-- )		// sakupljanje transformacija lijeve strane
	{
		l=i+1;
		g=(*vecW)[i];

		for(j=l; j<n; j++ )
			(*a)[i][j] = 0.0;
		
		if( g )
		{
			g = 1.0 / g;
			for( j=l; j<n; j++ )
			{
				for( s=0.0, k=l; k<m; k++ )
					s += (*a)[k][i] * ((*a)[k][j]);
		
				f = (s / (*a)[i][i]) * g;
				for( k=i; k<m; k++ )
					(*a)[k][j] += f * ((*a)[k][i]);
			}
			for( j=i; j<m; j++ )
				(*a)[j][i] *= g;
		}
		else
			for (j=i; j<m; j++ )
				(*a)[j][i] = 0.0;
			
		++(*a)[i][i];
	}

	for( k=n-1; k>=0; k--)			// dijagonalizacija bidiagonalne forme
	{
		for( its=1; its<=30; its++)
		{
			flag=1;
			for( l=k; l>=0; l--)
			{
				nm=l-1;
				if((fabs(rv1[l])+anorm) == anorm )
				{
					flag=0;
					break;
				}
				if((fabs((*vecW)[nm])+anorm) == anorm )
					break;
			}
			if(flag)
			{
				c=0.0;
				s=1.0;
				for( i=l; i<=k; i++)
				{
					f=s*rv1[i];
					rv1[i]=c*rv1[i];
					if( (fabs(f)+anorm) == anorm )
						break;
					g=(*vecW)[i];
					h=pythag(f,g);
					(*vecW)[i]=h;
					h=1.0/h;
					c=g*h;
					s=-f*h;
					for( j=0; j<m;j++)
					{
						y=(*a)[j][nm];
						z=(*a)[j][i];
						(*a)[j][nm]=y*c + z*s;
						(*a)[j][i]=z*c - y*s;
					}
				}
			}
			z=(*vecW)[k];
			if( l==k )
			{
				if(z<0.0)
				{
					(*vecW)[k]=-z;
					for( j=0; j<n; j++)
						(*v)[j][k]=-((*v)[j][k]);
				}
				break;
			}
			if(its==30)
			{
				printf("No converence in 30 SingValDecopm iteration!!!");
				assert(0);
				return	SVD_NO_CONVERG;
			}
			x=(*vecW)[l];
			nm=k-1;
			y=(*vecW)[nm];
			g=rv1[nm];
			h=rv1[k];

			f=( (y-z)*(y+z) + (g-h)*(g+h) ) / (2.0*h*y);
			g=pythag(f,1.0);
			f=( (x-z)*(x+z) + h*( (y/(f+SIGN(g,f))) - h ) )/x;
			c = s = 1.0;

			for(j=l;j<=nm;j++)
			{
				i=j+1;
				g=rv1[i];
				y=(*vecW)[i];
				h=s*g;
				g=c*g;
				z=pythag(f,h);
				rv1[j]=z;
				c=f/z;
				s=h/z;
				f=x*c + g*s;
				g=g*c - x*s;
				h=y*s;
				y *= c;
				for(jj=0; jj<n; jj++)
				{
					x=(*v)[jj][j];
					z=(*v)[jj][i];
					(*v)[jj][j]=x*c + z*s;
					(*v)[jj][i]=z*c - x*s;
				}
				z=pythag(f,h);
				(*vecW)[j]=z;
				if(z)
				{
					z=1.0/z;
					c=f*z;
					s=h*z;
				}
				f=c*g + s*y;
				x=c*y - s*g;
				for(jj=0; jj<m; jj++)
				{
					y=(*a)[jj][j];
					z=(*a)[jj][i];
					(*a)[jj][j]=y*c + z*s;
					(*a)[jj][i]=z*c - y*s;
				}
			}
			rv1[l]=0.0;
			rv1[k]=f;
			(*vecW)[k]=x;
		}
	}
	free(rv1);

	return 0;
}

void	SVDBackSubst( Matrix &u, Vector *vecW, Matrix &v, Matrix &b, Matrix *x )
{
	int			i, j, jj;
	double	s, *tmp;

	tmp = (double *) calloc(u.m_nColNum, sizeof(double) );
	
	for( j=0; j<u.m_nColNum; j++)
	{
		s=0.0;
		if( (*vecW)[j])
		{
			for(i=0; i<u.m_nRowNum; i++)
				s += u[i][j] * b[i][0];
			s /= (*vecW)[j];
		}
		tmp[j]=s;
	}
	for( j=0; j<u.m_nColNum; j++)
	{
		s=0.0;
		for( jj=0; jj<u.m_nColNum; jj++)
			s += v[j][jj] * tmp[jj];
		(*x)[j][0] = s;
	}
	
	free(tmp);
}

double pythag(double a, double b)
{
	double absa, absb;
	
	absa=fabs(a);
	absb=fabs(b);
	
	if( absa > absb )
		return absa*sqrt(1.0+DSQR(absb/absa));
	else
		return (absb==0.0 ? 0.0:absb*sqrt(1.0+DSQR(absa/absb)));
}

void	Print( const Matrix &a )
{
	int		i,j;
	Matrix	b(4,4);

	if( a.m_pdElem != NULL && a.m_nRowNum!=0 && a.m_nColNum!=0 )
  {
		for( i=0; i<a.m_nRowNum; i++ )
		{
			fprintf(stdout, "\n");
			for( j=0; j<a.m_nColNum; j++ )
				fprintf(stdout,"%7.2f ", a.m_pdElem[i][j] );
		}
	}                   
}


/***************************************************************************************/
/**********					EIGENVALUES									******************/
/***************************************************************************************/

#define RADIX 2.0

void Balance( Matrix &a )
{
	int			n, last, j, i;
	double	s, r, g, f, c, sqrdx;

	if( a.m_nRowNum != a.m_nColNum )
		return;
	else
		n = a.m_nRowNum;

	sqrdx = RADIX * RADIX;
	last = 0;
	while( last == 0 )
	{
		last = 1;
		for( i=0; i<n; i++ )
		{
			r = c = 0.0;
			for( j=0; j<n; j++ )
			{
				if( j != i )
				{
					c += fabs(a[j][i]);
					r += fabs(a[i][j]);
				}
				if( c && r )
				{
					g = r / RADIX;
					f = 1.0;
					s = c + r;
					while( c < g )
					{
						f *= RADIX;
						c *= sqrdx;
					}
					g = r* RADIX;
					while( c > g )
					{
						 f/= RADIX;
						 c /= sqrdx;
					}
					if( ( c+r) / f < 0.95 * s )
					{
						last = 0;
						g = 1.0 / f;
						for( j=0; j<n; j++ )
							a[i][j] *= g;
						for( j=0; j<n; j++ )
							a[j][i] *= f;
					}
				}
			}
		}
	}
}

#define SWAP(g,h) {y=(g); (g)=(h);(h)=y;}

Matrix&	ReductionToHess( Matrix	&matA )
{
	int			n, m, j, i;
	double	y, x;

	n=matA.m_nRowNum;

	for( m=2; m<n; m++ )
	{
		x = 0.0;
		i = m-1;
		for( j=m-1; j<n; j++ )
		{
			if( fabs(matA.m_pdElem[j][m-2]) > fabs(x))
			{
				x = matA.m_pdElem[j][m-2];
				i=j;
			}
		}
		if( i != m-1 )
		{
			for( j=m-2; j<n; j++ )
				SWAP(matA.m_pdElem[i][j],matA.m_pdElem[m-1][j]);
			
			for( j=0; j<n; j++ )
				SWAP(matA.m_pdElem[j][i], matA.m_pdElem[j][m-1]);
		}
		if( x )
		{
			for( i=m; i<n; i++ )
			{
				if( ( y = matA.m_pdElem[i][m-2] ) != 0.0 )
				{
					y /= x;
					matA.m_pdElem[i][m-2] = y;

					for( j=m-1; j<n; j++ )
						matA.m_pdElem[i][j] -= y * matA.m_pdElem[m-1][j];
					
					for( j=0; j<n; j++ )
						matA.m_pdElem[j][m-1] += y * matA.m_pdElem[j][i];
				}
			}
		}
	}
	return matA;
}

Matrix&	TransfToHess( Matrix	&matA )
{
	if( matA.IsSquare() && matA.m_nRowNum>2)
	{
		ReductionToHess(matA);
		for( int i=2; i<matA.m_nRowNum; i++ )
			for( int j=0; j<i-1; j++ )
				matA[i][j] = 0.0;
	}

	return matA;
}

void	HessEigenvalues( Matrix &matA, Vector *vectR, Vector *vectI )
{
	int			i, j, n, m, nn, l, k, its, mmin;
	double	x, y, z, w, v, u, t, s, r, q, p, anorm;

	n = matA.m_nRowNum;

	anorm = fabs(matA.m_pdElem[0][0]);
	for( i=1; i<n; i++ )
		for( j=(i-1); j<n; j++ )
			anorm += fabs(matA.m_pdElem[i][j]);

	nn = n;
	t = 0.0;
	while( nn >= 1 )
	{
		its = 0;
		do
		{
			for( l=nn; l>=2; l-- )
			{
				s = fabs(matA.m_pdElem[l-2][l-2]) + fabs(matA.m_pdElem[l-1][l-1]);
				if( s == 0.0 )
					s = anorm;
				if( (double) (fabs(matA.m_pdElem[l-1][l-2]) + s) == s )
					break;
			}
			x = matA.m_pdElem[nn-1][nn-1];
			if( l == nn )
			{
				(*vectR)[nn-1] = x+t;
				(*vectI)[nn-1] = 0.0;
				nn--;
			}
			else
			{
				y = matA.m_pdElem[nn-2][nn-2];
				w = matA.m_pdElem[nn-1][nn-2] * matA.m_pdElem[nn-2][nn-1];

				if( l == (nn-1) )
				{
					p = 0.5 * (y-x);
					q = p*p+w;
					z = sqrt(fabs(q));
					x += t;
					if( q >= 0.0 )
					{
						z = p+SIGN(z,p);
						(*vectR)[nn-2] = (*vectR)[nn-1] = x+z;
						if( z )
							(*vectR)[nn-1] = x - w/z;
						(*vectI)[nn-2]= (*vectI)[nn-1] = 0.0;
					}
					else
					{
						(*vectR)[nn-2] = (*vectR)[nn-1] = x + p;
						(*vectI)[nn-2] = -1*((*vectI)[nn-1]=z);
					}
					nn -= 2;
				}
				else
				{
					if( its == 30 )
					{
						fprintf(stderr, "To many iterations in HessEigenvalues!!!");
						assert(0);
					}
					if( its == 10 || its == 20 )
					{
						t += x;
						for( i=0; i<nn; i++ )
							matA.m_pdElem[i][i] -= x;
						
						s = fabs(matA.m_pdElem[nn-1][nn-2]) + fabs(matA.m_pdElem[nn-2][nn-1]);
						y = x = 0.75 * s;
						w = -0.4375 * s * s;
					}
					++its;
					for( m=(nn-2); m>=1; m-- )
					{
						z = matA.m_pdElem[m-1][m-1];
						r = x - z;
						s = y - z;
						p = (r*s-w)/matA.m_pdElem[m][m-1] + matA.m_pdElem[m-1][m];
						q = matA.m_pdElem[m][m] - z - r - s;
						r = matA.m_pdElem[m+1][m];
						s = fabs(p) + fabs(q) + fabs(r);
						p /= s;
						q /= s;
						r /= s;
						if( m == 1 )
							break;

						u = fabs(matA.m_pdElem[m-1][m-2]) * (fabs(q) + fabs(r) );
						v = fabs(p) * ( fabs(matA.m_pdElem[m-2][m-2]) + fabs(z) + fabs(matA.m_pdElem[m][m]) );
						if( (double) (u+v) == v )
							break;
					}
					for( i=m+2; i<=nn; i++ )
					{
						matA.m_pdElem[i-1][i-3] = 0.0;
						if( i != (m+2) )
							matA.m_pdElem[i-1][i-4] = 0.0;
					}

					for( k=m; k<=nn-1; k++ )
					{
						if( k != m )
						{
							p = matA.m_pdElem[k-1][k-2];
							q = matA.m_pdElem[k][k-2];
							r = 0.0;
							if( k != (nn-1) )
								r = matA.m_pdElem[k+1][k-2];
						
							if( (x=fabs(p) + fabs(q) + fabs(r) ) != 0.0 )
							{
								p /= x;
								q /= x;
								r /= x;
							}
						}
						if( ( s = SIGN(sqrt(p*p + q*q + r*r), p) ) != 0.0 )
						{
							if( k == m )
							{
								if( l != m )
									matA.m_pdElem[k-1][k-2] = - matA.m_pdElem[k-1][k-2];
							}
							else
								matA.m_pdElem[k-1][k-2] = - s*x;

							p += s;
							x = p/s;
							y = q/s;
							z = r/s;
							
							q /= p;
							r /= p;

							for( j=k; j<=nn; j++ )
							{
								p = matA.m_pdElem[k-1][j-1] + q*matA.m_pdElem[k][j-1];
								if( k != (nn-1) )
								{
									p += r * matA.m_pdElem[k+1][j-1];
									matA.m_pdElem[k+1][j-1] -= p*z;
								}
								matA.m_pdElem[k][j-1] -= p*y;
								matA.m_pdElem[k-1][j-1] -= p*x;
							}
							mmin = nn<k+3 ? nn : k+3;
							for( i=1; i<=mmin; i++ )
							{
								p = x*matA.m_pdElem[i-1][k-1] + y*matA.m_pdElem[i-1][k];
								if( k != (nn-1) )
								{
									p += z*matA.m_pdElem[i-1][k+1];
									matA.m_pdElem[i-1][k+1] -= p*r;
								}
								matA.m_pdElem[i-1][k] -= p*q;
								matA.m_pdElem[i-1][k-1] -= p;
							}
						}
					}
				}
			}
		} while (l < nn-1 );
	}
}

void	CalcEigenvalues( Matrix matA, Vector *vecR, Vector *vecI )
{
	ReductionToHess(matA);
	HessEigenvalues(matA, vecR, vecI);
}

void	CalcEigenvalues( Matrix *matA, Vector *vecR, Vector *vecI )
{
	ReductionToHess(*matA);
	HessEigenvalues(*matA, vecR, vecI );
}

void	CalcEigenvalues( Matrix matA, ComplexVector *cvecR )
{
	Vector		vecR(RowNum(matA)), vecI(RowNum(matA));

	ReductionToHess(matA);
	HessEigenvalues(matA, &vecR, &vecI );

	for( int i=0; i<RowNum(matA); i++ )
	{
		(*cvecR)[i].Set( vecR[i], vecI[i] );
	}
}

void	CalcEigenvalues( Matrix *matA, ComplexVector *cvecR )
{
	Vector		vecR(RowNum(*matA)), vecI(RowNum(*matA));

	ReductionToHess(*matA);
	HessEigenvalues(*matA, &vecR, &vecI );

	for( int i=0; i<RowNum(*matA); i++ )
	{
		(*cvecR)[i].Set( vecR[i], vecI[i] );
	}
}

int	CalcEigenvectors( Matrix &a, Vector &vecEigenval, Vector **vecEigenvec )
{
	int				i, j, n, ret;
	Vector		*vecW;
	Matrix		*matTemp, *matV;

	ret = 0;
	n = RowNum(a);

	vecW = new Vector(n);
	matTemp = new Matrix(n,n);
	matV = new Matrix(n,n);

	for( i=0; i<n; i++ )
	{
		*matTemp = a;
		for( j=0; j<n; j++ )
			(*matTemp)[j][j] -= vecEigenval[i];

		SingValDecomp(matTemp, vecW, matV);
		for( int k=0; k<n; k++ )
			if( fabs((*vecW)[k]) < 1.0e-10 )
			{
				for( j=0; j<n; j++ )
					(*(vecEigenvec[ret]))[j] = (*matV)[j][k];
		
				ret++;
				break;
			}
	}
	delete matTemp;
	delete matV;

	return ret;
}

