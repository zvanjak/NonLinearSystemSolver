#include "MLibBasic.h"

#if !defined	__VECTOR_H
#define __VECTOR_H

#define COPY    1
#define NO_COPY 2

class	Vector
{
private:
	int				m_nDim;
	double		*m_pdCoef;
	char			strName[10];

public:
	Vector();
	Vector( int nDim );
	Vector( int nDim, double *pdValues, int Copy=COPY );
	Vector( int nDim, char *strName );
 	Vector( Vector &Copy );
	~Vector();

	void	InitValues( double *aValues );
	void	InitValues( double dFirst, ... );
	void  MakeRandomElements( double dMax=1, double dMin=-1 );
	void	Reinitialize( int nNewDim );
	void	Realloc( int nNewDim );

	int				GetDim( void );
	double		Get( int i );
	double		*GetPtr( void );
	void			Clear( void );

	Vector	operator+(  Vector &a ) ;
	Vector	operator-(  Vector &a ) ;
	Vector	operator*(  double b ) ;

	Vector	operator+=(  Vector &a );
	Vector	operator-=(  Vector &a );
	Vector	operator*=(  double b );

	bool		operator==( Vector &b );

	Vector& operator=( const Vector &b );
	double&	operator[]( int i );

	friend Vector	operator+(  double a,  Vector &b );
	friend Vector	operator-(  double a,  Vector &b );
	friend Vector	operator*(  double a,  Vector &b );

	friend Vector	operator+(  Vector &a,  double b );
	friend Vector	operator-(  Vector &a,  double b );
	friend Vector	operator*(  Vector &a,  double b );

	friend double	Norm(  Vector &a );
	friend void		Normalize( Vector &a );

	friend double	FindMin( Vector &a );
	friend double	FindMax( Vector &a );

	friend void	Print( Vector &b );
};

class	ComplexVector
{
private:
	int				m_nDim;
	Complex		*m_pdCoef;
	char			strName[10];

public:
	ComplexVector();
	ComplexVector( int nDim );
 	ComplexVector( ComplexVector &Copy );
	ComplexVector( Vector &vecReal, Vector &vecImag );
	~ComplexVector();

	void	InitValues( Complex *aValues );
	void	InitValues( Complex dFirst, ... );
	void  MakeRandomElements( double dMax=1, double dMin=-1 );
	void	Reinitialize( int nNewDim );

	int				GetDim( void );
	Complex		Get( unsigned int i );
	Complex		*GetPtr( void );

	ComplexVector	operator+(  ComplexVector &a ) ;
	ComplexVector	operator-(  ComplexVector &a ) ;
	ComplexVector	operator*(  double b ) ;

	ComplexVector	operator+=(  ComplexVector &a );
	ComplexVector	operator-=(  ComplexVector &a );
	ComplexVector	operator*=(  double b );

	ComplexVector& operator=(  ComplexVector &b );
	Complex&	operator[]( int i );

	friend ComplexVector	operator+(  double a,  ComplexVector &b );
	friend ComplexVector	operator-(  double a,  ComplexVector &b );
	friend ComplexVector	operator*(  double a,  ComplexVector &b );

	friend ComplexVector	operator+(  ComplexVector &a,  double b );
	friend ComplexVector	operator-(  ComplexVector &a,  double b );
	friend ComplexVector	operator*(  ComplexVector &a,  double b );

	friend double	Norm(  ComplexVector &a );

	friend void	Print( ComplexVector &b );
};

#endif