#include "mathlib.h"

template < class T > T trapzd( T (*func)(T), T start, T end, int steps );
template < class T > T qtrap( T (*func)(T), T start, T end );
template < class T > T qsimp( T (*func)(T), T a, T b );
template < class T > T qromb( T (*func)(T), T a, T b );

template < class T > T midpnt( T (*func)(T), T a, T b, int n );
template < class T > T qromo( T (*func)(T), T a, T b, T (*choose)(T (*f)(T), T, T, int) );
