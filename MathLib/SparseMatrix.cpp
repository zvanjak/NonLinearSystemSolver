#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

SparseMatrix::SparseMatrix( unsigned long lMax )
{
	m_ulMax = lMax;
	m_ulElemNum = 0;

	m_plIJ = new unsigned long[lMax];
	m_pdElem = new double[lMax];
}

// Copy konstruktor
SparseMatrix::SparseMatrix( const SparseMatrix &Copy )
{
	m_ulElemNum = Copy.m_ulElemNum;
	m_ulMax = Copy.m_ulMax;

	m_plIJ = new unsigned long[m_ulMax];
	m_pdElem = new double[m_ulMax];

	memcpy(m_plIJ, Copy.m_plIJ, sizeof(unsigned long) * m_ulElemNum );
	memcpy(m_pdElem, Copy.m_pdElem, sizeof(double) * m_ulElemNum );
}

SparseMatrix::~SparseMatrix()
{
	if( m_plIJ != NULL )
		delete []m_plIJ;
	if( m_pdElem != NULL )
		delete [] m_pdElem;
}


// Transformira normalnu matricu u 'sparse' zapis
SparseMatrix::SparseMatrix( Matrix &a, double dThresh, unsigned long lMax )
//void sprsin( float **a, int n, float thresh, unsigned long nmax, float sa[], unsigned long ija[] )
{
	unsigned long k;
	int			i, j;
	int			n;

	if( RowNum(a) != ColNum(a) )
	{
		puts("Transformacija u 'sparse' matricu samo za kvadratne matrice");
		assert(0);
	}

	n = RowNum(a);

	// prije pridruzivanja trebamo alocirati prostor za 'sparse' matricu
	m_ulMax = lMax;
	m_plIJ = new unsigned long[lMax];
	m_pdElem = new double[lMax];

	for( j=0; j<n; j++ )
		m_pdElem[j] = a[j][j];
	
	m_plIJ[0] = n+1;
	k = n;
	for( i=0; i<n; i++ )
	{
		for( j=0; j<n; j++ )
		{
			if( fabs(a[i][j]) >= dThresh && i != j )
			{
				if( ++k > m_ulMax )
				{
					puts("nmax premali");
					assert(0);
				}
				m_pdElem[k] = a[i][j];
				m_plIJ[k] = j;
			}
		}
		m_plIJ[i+1] = k+1;
	}
	m_ulElemNum = m_plIJ[m_plIJ[0]-1];
}

void SparseMatrix::PrintArrays()
{
	unsigned long i;

	for( i=0; i<m_ulElemNum; i++ )
		printf("\n i = %ld  -  ija = %ld ,  sa = %lf ", i, m_plIJ[i], m_pdElem[i] );
}


// mnozi matricu [ sa, ija] sa vektorom x[], rezultat je u vektoru b[]
Vector		operator*( SparseMatrix &a, Vector &b )
{
	unsigned long i, k;
	Vector	 ret(b.GetDim());

	if( a.m_plIJ[0] != ((unsigned long) b.GetDim()+1) )
	{
		puts("vektor i matrica nisu odgovarajucih dimenzija");
		assert(0);
	}
	for( i=0; i<((unsigned long)b.GetDim()); i++ )
	{
		ret[i] = a.m_pdElem[i] * b[i];
		
		for( k = a.m_plIJ[i]; k <= a.m_plIJ[i+1]-1; k++ )
			ret[i] += a.m_pdElem[k] * b[a.m_plIJ[k]];
	}
	return ret;
}

// Mnozi matricu transponiranu matrici [ija, sa] sa vektorom x, rezultat b[]
Vector	MulTranspVec( SparseMatrix &a, Vector &b )
//void sprstx( float sa[], unsigned long ija[], float x[], float b[], unsigned long n )
{
	unsigned long  i, j, k, n;
	Vector	 ret(b.GetDim());

	n = (unsigned long) b.GetDim();

	if( a.m_plIJ[0] != n+1 )
	{
		puts("vektor i matrica nisu odgovarajucih dimenzija");
		assert(0);
	}

	for( i=0; i<n; i++ )
		ret[i] = a.m_pdElem[i] * b[i];
	for( i=0; i<n; i++ )
	{
		for( k = a.m_plIJ[i]; k <= a.m_plIJ[i+1]-1; k++ )
		{
			j = a.m_plIJ[k];
			ret[j] += a.m_pdElem[k] * b[i];
		}
	}
	return ret;
}

#define SWAP(a,b) itemp=(a);(a)=(b);(b)=itemp;
#define M 7
#define NSTACK	50

void iindexx( unsigned long n, long arr[], unsigned long indx[] )
{
	unsigned long i, indxt, ir=n, itemp, j, k, l=1;
	int			jstack=0, *istack;
	long		a;

	istack = (int *) calloc(NSTACK+1, sizeof(int) );
	for( j=1; j<=n; j++ )
		indx[j-1] = j-1;
	
	for(;;)
	{
		if( ir-l < M )
		{
			for( j=l+1; j<=ir; j++ )
			{
				indxt = indx[j-1];
				a = arr[indxt-1];
				for( i=j-1; i>=1; i-- )
				{
					if( arr[indx[i-1]-1] <= a )
						break;
					indx[i+1-1] = indx[i-1];
				}
				indx[i+1-1] = indxt;
			}
			if( jstack == 0 )
				break;
			ir = istack[jstack---1];
			l = istack[jstack---1];
		}
		else
		{
			k =(l+ir) >> 1;
			SWAP(indx[k-1], indx[l+1-1]);
			
			if( arr[indx[l+1-1]-1] > arr[indx[ir-1]-1] )
			{
				SWAP(indx[l+1-1], indx[ir-1]);
			}
			if( arr[indx[l-1]-1] > arr[indx[ir-1]-1] )
			{
				SWAP(indx[l-1], indx[ir-1]);
			}
			if( arr[indx[l+1-1]-1] > arr[indx[l-1]-1])
			{
				SWAP(indx[l+1-1], indx[l-1]);
			}
			i = l+1;
			j = ir;
			indxt = indx[l-1];
			a = arr[indxt-1];
			for(;;)
			{
				do i++;
				while (arr[indx[i-1]-1] < a );
				do j--;
				while (arr[indx[j-1]-1] > a );
				if( j < i )
					break;
				SWAP(indx[i-1], indx[j-1]);
			}
			indx[l-1] = indx[j-1];
			indx[j-1] = indxt;
			jstack += 2;
			if( jstack > NSTACK)
			{
				puts("NSTACK premali u iindexx");
				assert(0);
			}
			if( ir-i+1 >= j-l )
			{
				istack[jstack-1] = ir;
				istack[jstack-1-1] = i;
				ir = j-1;
			}
			else
			{
				istack[jstack-1] = j-1;
				istack[jstack-1-1] = l;
				l = i;
			}
		}
	}
	free(istack);
}

// Konstruira transponiranu matricu
SparseMatrix	Transp( const SparseMatrix &a )
//void sprstp( float sa[], unsigned long ija[], float sb[], unsigned long ijb[] )
{
	SparseMatrix	b( a.m_ulMax );
	unsigned long j, jl, jm, jp, ju, k, m, n2, noff, inc, iv;
	double		v;

	b.m_ulElemNum = a.m_ulElemNum;

	n2 = a.m_plIJ[0] + 1;
	for( j=0; j<n2-2; j++ )
		b.m_pdElem[j] = a.m_pdElem[j];

//	for( int l=0; l<11; l++ )
//		printf("\nA_ij = %ld    ,    B_ij = %ld", a.m_plIJ[l], b.m_plIJ[l] );
	
	iindexx(a.m_plIJ[n2-1-1] - a.m_plIJ[1-1], (long *)&a.m_plIJ[n2-1-1], &b.m_plIJ[n2-1] );
//	printf("\nPoslije");
//	for( l=0; l<11; l++ )
//		printf("\n%d  A_ij = %3ld    ,    B_ij = %3ld", l, a.m_plIJ[l], b.m_plIJ[l] );

	jp = 0;
	for( k=a.m_plIJ[1-1]; k<=a.m_plIJ[n2-1-1]-1; k++ )
	{
		m = b.m_plIJ[k] + (1) + n2 - 1;
		b.m_pdElem[k] = a.m_pdElem[m - (1)];
		
		for( j=jp+1; j<=a.m_plIJ[m - (1)]+1; j++ )
			b.m_plIJ[j-1] = k;
		
		jp = a.m_plIJ[m - (1)] + 1;
		jl = 1;
		ju = n2-1;
		while( ju - jl > 1 )
		{
			jm = (ju + jl) / 2;
			if( a.m_plIJ[jm-1] + (1) > m )
				ju = jm;
			else
				jl = jm;
		}
		b.m_plIJ[k] = jl - 1;
	}
	for( j=jp+1; j<n2; j++ )
		b.m_plIJ[j-1] = a.m_plIJ[n2-1-1];
	
	for( j=1; j<=n2-2; j++ )
	{
		jl = b.m_plIJ[j+1-1] - b.m_plIJ[j-1];
		noff = b.m_plIJ[j-1] - 1 + (1);
		inc = 1;
		do
		{
			inc *= 3;
			inc++;
		}	while( inc <= jl);
		do
		{
			inc /= 3;
			for( k = noff + inc + 1; k <= noff + jl; k++ )
			{
				iv = b.m_plIJ[k-1] + (1);
				v = b.m_pdElem[k-1];
				m = k;
				while( b.m_plIJ[m-inc-1] + (1) > iv )
				{
					b.m_plIJ[m-1] = b.m_plIJ[m-inc-1];
					b.m_pdElem[m-1] = b.m_pdElem[m-inc-1];
					m -= inc;
					if( m-noff <= inc )
						break;
				}
				b.m_plIJ[m-1] = iv - (1);
				b.m_pdElem[m-1] = v;
			}
		} while( inc > 1);
	}
	return b;
}

// Mnozi A sa transp(B)
void sprstm( float sa[], unsigned long ija[], float sb[], unsigned long ijb[],
						float tresh, unsigned long nmax, float sc[], unsigned long ijc[] )
{
	unsigned long i, ijma, ijmb, j, k, ma, mb, mbb;
	float		sum;

	if( ija[1] != ijb[1] )
	{
		puts("Matrice krivih dimenzija");
		assert(0);
	}
	ijc[1]= k = ija[1];
	for( i=1; i<=ija[1]-2; i++ )
	{
		for( j=1; j<=ijb[1]-2; j++ )
		{
			if( i == j )
				sum = sa[i] * sb[j];
			else
				sum = 0.0e0;
			mb = ijb[j];
			for( ma =ija[i]; ma<=ija[i+1]-1; ma++ )
			{
				ijma = ija[ma];
				if( ijma == j )
					sum += sa[ma] * sb[j];
				else
				{
					while( mb < ijb[j+1] )
					{
						ijmb = ijb[mb];
						if( ijmb == i )
						{
							sum += sa[i] * sb[mb++];
							continue;
						}
						else if( ijmb < ijma )
						{
							mb++;
							continue;
						}
						else if( ijmb == ijma )
						{
							sum += sa[ma] * sb[mb++];
							continue;
						}
						break;
					}
				}
			}
			for( mbb = mb; mbb<=ijb[j+1]-1; mbb++)
			{
				if ( ijb[mbb] == i);
					sum += sa[i] * sb[mbb];
			}
			if( i == j )
				sc[i] = sum;
			else if( fabs(sum) > tresh )
			{
				if ( k > nmax )
				{
					puts("spprstm : nmax premali ");
					assert(0);
				}
				sc[k] = sum;
				ijc[k++] = j;
			}
		}
		ijc[i+1] = k;
	}
}