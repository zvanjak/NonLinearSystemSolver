#include <stdio.h>
#include <complex>

template<class _T> void arth( const _T &z );

void main( void )
{
	complex	a(1,1),b;

	b = arsh(a);
}

template < class _T > complex<_T> arsh( const complex<_T> &z )
{
	complex	Tmp, i(0,1);

	Tmp = log(z + pow(1 + z * z, 0.5) );

	return Tmp;
}