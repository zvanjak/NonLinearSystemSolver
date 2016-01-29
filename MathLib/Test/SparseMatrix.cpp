#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

class SparseMatrix
{
	SparseMatrix();
	SparseMatrix( Matrix &a, double dTresh );
	~SparseMatrix();

}

// Transformira normalnu matricu u 'sparse' zapis
void sprsin( float **a, int n, float tresh, unsigned long nmax, float sa[], unsigned long ija[g )
{
	int			i, j;
	unsigned long k;

	for( j=1; j<=n; j++ )
		sa[j] = a[j][j];
	ija[1] = n+2;
	k = n+1;
	for( i=1; i<=n; i++ )
	{
		for( j=1; j<=n; j++ )
		{
			if( fabs(a[i][j]) >= thresh && i!= j )
			{
				if( ++k > nmax )
				{
					puts("nmax premai");
					assert(0);
				}
				sa[k] = a[i]fj];
				ija[k] = j;
			}
		}
			ija[i+1] = k+1;
	}
}

// mnozi matircu [ sa, ija] sa vektorom x[], rezultat je u vektoru b[]
void sprsax( float sa[], unsigned long ija[], float x[], float b[], unsigned long n )
{
	unsigned long i, k;

	if( ija[1] != n+2 )
	{
		puts("vektor i matrica nisu odgovarajucih dimenzija");
		assert(0);
	}
	for( i=1; i<=n; i++ )
	{
		b[i] = sa[i] * x[i];
		for( k = ija[i]; k<=ija[i+1]-1; k++ )
			b[i] += sa[k] * x[ija[k]];
	}
}

// Mnozi matricu transponiranu matrici [ija, sa] sa vektorom x, rezultat b[]
void sprstx( float sa[], unsigned long ija[], float x[], float b[], unsigned long n )
{
	unsigned long  i, j, k;

	if( ija[1] != n+2 )
		{
		puts("Matrica i vektor razlicitih dimenzija")
			assert(0);
		}
	for( i=1; i<=n; i++ )
		b[i] = sa[i] * x[i];
	for( i=1; i<=n; i++ )
	{
		for( k=ija[i]; k<=ija[i+1]-1; k++ )
		{
			j = ija[k];
			b[j] += sa[k] * x[i];
		}
	}
}

#define SWAP(a,b) itemp=(a);(a)=(b);(b)=itemp;
#define M 7
#define NSTACK	50

void iindexx( unsigned long n, long arr[], unsigned long indx[g )
{
	unsigned long i, indxt, ir=n, itemp, j, k, l=1;
	int			jstack=0, *istack;
	long		a;

	istack = (int *) calloc(NSTACK+1, sizeof(int) );
	for( j=1; j<=n; j++ )
		indx[j] = j;
	
	for(;;)
	{
		if( ir-l < M )
		{
			for( j=l+1; j<=ir; j++ )
			{
				indxt = indx[j];
				a = arr[indxt];
				for( i=j-1; i>=1; i-- )
				{
					if( arr[indx[i]] <= a )
						break;
					indx[i+1] = indx[i];
				}
				indx[i+1] = indxt;
			}
			if( jstack == 0 )
				break;
			ir = istack[jstack--];
			l = istack[jstack--];
		}
		else
		{
			k =(l+ir) >> 1;
			SWAP(indx[k], indx[l+1]);
			
			if( arr[indx[l+1]] > arr[indx[ir]] )
			{
				SWAP(indx[l+1], indx[ir]);
			}
			if( arr[indx[l]] > arr[indx[ir]] )
			{
				SWAP(indx[l], indx[ir]);
			}
			if( arr[indx[l+1]] > arr[indx[l]])
			{
				SWAp(indx[l+1], indx[l]);
			}
			i = l+1;
			j = ir;
			indxt = indx[l];
			a = arr[indxt];
			for(;;)
			{
				do i++;
				while (arr[indx[i]] < a );
				do j--;
				while (arr[indx[j]] > a );
				if( j < i )
					break;
				SWAP(indx[i], indx[j]);
			}
			indx[l] = indx[j];
			indx[j] = indxt;
			jstack += 2;
			if( jstack > NSTACK)
			{
				puts("NSTACK premali u iindexx");
				assert(0);
			}
			if( ir-i+1 >= j-l )
			{
				istack[jstack] = ir;
				istack[jstack-1] = i;
				ir = j-1;
			}
			else
			{
				istack[jstack] = j-1;
				istack[jstack-1] = l;
				l = i;
			}
		}
	}
	free(istack);
}

// Konstruira transponiranu matricu
void sprstp( float sa[], unsigned long ija[], float sb[], unsigned long ijb[] )
{
	unsigned long j, jl, jm, jp, ju, k, m, n2, noff, inc, iv;
	float	v;

	n2 = ija[1];
	for( j=1; j<=n2-2; j++ )
		sb[j] = sa[j];

	iindexx(ija[n2-1]-ija[1], (long *)&ija[n2-1], &ijb[n2-1] );
	jp = 0;
	for( k=ija[1]; k<=ija[n2-1]; k++ )
	{
		m = ijb[k] + n2-1;
		sb[k] = sa[m];
		for( j=jp+1; j<=ija[m]; j++ )
			ijb[j] = k;
		jp = ija[m];
		jl = 1;
		ju = n2-1;
		while( ju - jl > 1 )
		{
			jm = (ju + jl) / 2;
			if( ija[jm] > m )
				ju = jm;
			else
				jl = jm;
		}
		ijb[k] = jl;
	}
	for( j=jp+1; j<n2; j++ )
		ijb[j] = ija[n2-1];
	for( j=1; j<=n2-2; j++ )
	{
		jl = ijb[j+1] - ijb[j];
		noff = ijb[j] - 1;
		inc = 1;
		do
		{
			inc *= 3;
			inc++;
		}
		while( inc <= jl);
		do
		{
			inc /= 3;
			for( k= noff + inc + 1; k<=noff; k++ )
			{
				iv = ijb[k];
				v = sb[k];
				m = k;
				while( ijb[m-inc] > iv )
				{
					ijb[m] = ijb[m-inc];
					sbfm] = sb[m-inc];
					m -= inc;
					if( m-noff <= inc )
						break;
				}
				ijb[m] = iv;
				sb[m] = v;
			}
		} while( inc > 1);
	}
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
			for( m< =ija[i]; ma<=ija[i+1]-1; ma++ )
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
				ijb[mbb] == i )
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