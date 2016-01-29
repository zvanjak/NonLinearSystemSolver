#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <valarray>

#include <mathlib.h>

typedef	void (*VECFUNC)( std::valarray<double>, std::valarray<double> );

typedef  double (*pfFunc) (double);
typedef  void		(*pfVecFunc) (int , double [], double [] );
typedef  void		(*pfDAEFunc) (int , int, double [], double [], double [], double [] );

void func( int n, double a[], double b[] )
{
	a[0] = b[0];
}

double dosomething( int n, pfVecFunc pFunc )
{
	n++;
	return 0;
}

float func1( float x )
{
	return x;
}

double func2( double x )
{
	return x;
}

Complex func3( Complex z )
{
	return z*Sin(z)/(z - Complex(0.5,0.5) );
}
Complex	func4( Complex z )
{
	return Tan(z + Complex(0,1));
}

// globalna varijabla koja nam odreðuje koji koeficijent 
int		m;

float F1( float psi )
{
	float	a[6] = { 0.5, 0.24, 0.1, 0.09, 0.05, 0.01 };
	float	rez, x;

	rez = a[0];
	x = acos(psi / 3.141593);
	for( int i=1; i<6; i++ )
		rez += a[i] * pow(x, i);

	return (rez * cos(m * psi) );
}

float F2( float psi )
{
	float	a[6] = { 0.5, 0.24, 0.1, 0.09, 0.05, 0.01 };
	float	rez, x;

	rez = a[0];
	x = acos(psi / 3.141593);
	for( int i=1; i<6; i++ )
		rez += a[i] * pow(x, i);

	return (rez * sin(m * psi) );
}

float F3( float x )
{
	return x*x * cos(m * x);;
}

float F4( float x )
{
	return x*x * sin( m * x );
}
/*
void main ( void )
{
	float	a[10], b[10];

	m = 1;
	for( float x=-3.141; x<3.141; x+=0.2 )
		printf("%f   -   %f\n", F1(x), F2(x) );

	for( int i=0; i<10; i++ )
	{
		m = i;
		a[i] = 1/(2*3.14159) * qromb<float>(F3, -3.1415, 3.1415);
		b[i] = 1/(2*3.14159) * qromb<float>(F4, -3.1415, 3.1415);

		printf("a[%d] = %f  ,  b[%d] = %f\n", i, a[i], i, b[i] );
	}
}
*/
/*
void main( void )
{
	dosomething(10,func);
}
*/
/*
void main( void )
{
	Polynom a(5), b(3), c(5);

	a[0]=1.0;
	a[1] = 2.0;
	a[2] = 3.0;
	a[3] = -2.0;
	a[4] = -5.0;
	a[5] = -1.0;

	b[0] = -1.0;
	b[1] = 3.0;
	b[2] = -4.5;
	b[3] = 0.0;

//	ReducePolynom(&a);
//	ReducePolynom(&b);

	printf("\n a(1.0) = %f", a.Val(2.0));

	printf("\n a");
	Print(a);
	printf("\n b");
	Print(b);
	printf("\n a + b");
	Print(a+b);
	printf("\n a - b");
	Print(a-b);
	printf("\n a * b");
	Print(a*b);

	c=a/b;
	printf("\n a / b");
	Print(c);
	
	printf("\n a / b * b");
	Print(c*b);
	printf("\n");
	printf("\n");
}
*/
/*
void main( void )
{
	Matrix			a(5,5);
	Vector			vec(5);

	a.Clear();
	a[0][0] = 3.0;
	a[0][2] = 1.0;
	a[1][1] = 4.0;
	a[2][1] = 7.0;
	a[2][2] = 5.0;
	a[2][3] = 9.0;
	a[3][4] = 2.0;
	a[4][3] = 6.0;
	a[4][4] = 5.0;
	Print(a);
	vec.MakeRandomElements();

	// kreiramo 'sparse' matricu
	SparseMatrix	b(a, 1e-10 , 20);
	b.PrintArrays();
	
/*	printf("\nnormal");
	Print(Transp(a)*vec);
	printf("\nsparse");
	Print( MulTranspVec(b,vec) );
	printf("\nTransp(b)");
	Transp(b).PrintArrays();
}*/

/*
void main( void )
{
	clock_t	start, finish;
	time_t  start_time, finish_time;
	double	elapsed, elapsed_time;

	ComplexMatrix		a(300,300), h(300,300);
	ComplexVector		b(300);
	
	int			indx[300];
	double	dummy;

	printf("\nRandom ");

	a.MakeRandomElements();
	h = a;
//	printf("\nMatrica A ");
//	Print(a);
	b.MakeRandomElements();
//	printf("\nVektor B ");
//	Print(b);

	printf("\nStarted");
	start = clock();
	time( &start_time );
	
	LUDecomp(a, indx, &dummy );
//	printf("\nMatrica A ");
//	Print(a);
	LUBackSubst( a, indx, b );
//	printf("\nRjesenje - vektor B :");
//	Print(b);
	finish = clock();
	time( &finish_time );

	elapsed = (double) (finish - start);
	elapsed_time = difftime(finish_time, start_time);

  printf( "\nProgram complex LUDecomp takes %6.0f clocks.\n", elapsed );	
  printf( "\nProgram complex LUDecomp takes %6.0f seconds.\n", elapsed_time );	

//	printf("\nMnozenje A * B :");
//	Print(h*b);
}*/
/*
void main( void )
{
	Matrix	A(5,10);
	Vector	W(10);
	Matrix	matW(10,10);
	Matrix	V(10,10);

	A.MakeRandomElements();
	printf("\nMatrica A ");
	Print(A);
	SingValDecomp(&A, &W, &V);
	printf("\nMatrica U ");
	Print(A);
	printf("\nVektor W ");
	Print(W);
	printf("\nMatrica V ");
	Print(V);

	matW.Clear();
	for( int i=0; i<RowNum(matW); i++ )
		matW.SetElem(i,i,W[i]);

	printf("\nMatrica A ");
	Print(A * matW * Transp(V) );
}
*/
/*
#include <time.h>

void main( void )
{
	clock_t	start, finish;
	long		i;
	double	elapsed;

	float		x, y, w;
	x = 1.0;
	y = 5.0;
	start = clock();
	for( i=0; i<10000000; i++ )
		w = x * y;
	finish = clock();
	elapsed = (double) (finish - start);
  printf( "\nProgram FLOAT takes %6.0f clocks.\n", elapsed );	

	double	a, b, c;
	a = 1.0;
	b = 5.0;
	start = clock();
	for( i=0; i<10000000; i++ )
		c = a * b;
	finish = clock();
	elapsed = (double) (finish - start);
  printf( "\nProgram DOUBLE takes %6.0f clocks.\n", elapsed );	

	/*	y = 1.54;
	puts("Pocinje Microsoft");
	time( &start );
	for( i=0; i<500000; i++ )
		x = _j0(y);
	time( &finish );
	elapsed = difftime(finish, start);
  printf( "\nProgram takes %6.0f seconds.\n", elapsed );	


	puts("Pocinje Numerical");
	time( &start );
	for( i=0; i<500000; i++ )
		x = bessj0(y);
	time( &finish );
	elapsed = difftime(finish, start);
  printf( "\nProgram takes %6.0f seconds.\n", elapsed );	

	printf("Microsoft %20.10e\n", _j0(12.54) );
	printf("Numerical %20.10e\n", bessj0(12.54) );

	puts("Gotovo");

	x = 14;
	double rj, ry, rjp, ryp;
	bessjy(x, 1, &rj, &ry, &rjp, &ryp );

	printf("\nNumerical J1 = %20.10f,   Y1 = %20.10f", rj, ry );
	printf("\nMicrosoft J1 = %20.10f,   Y1 = %20.10f ", _j1(x), _y1(x) );
}*/