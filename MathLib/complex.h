#include "MLibBasic.h"

class	Complex
{                      
public:
	Complex();
	Complex( double both );
	Complex( double re, double im );
	Complex( const Complex &Copy );
	~Complex();
	
	Complex		operator+(  Complex &b ) ;
	Complex		operator-(  Complex &b ) ;
	Complex		operator*(  Complex &b ) ;
	Complex		operator/(  Complex &b ) ;
	
	Complex		operator+=(  Complex &b );
	Complex		operator-=(  Complex &b );
	Complex		operator*=(  Complex &b );
	Complex		operator/=(  Complex &b );
	
	Complex		operator+=(  double b );
	Complex		operator-=(  double b );
	Complex		operator*=(  double b );
	Complex		operator/=(  double b );

	friend	Complex		operator+(  double a,  Complex &b );
	friend	Complex		operator-(  double a,  Complex &b );
	friend	Complex		operator*(  double a,  Complex &b );
	friend	Complex		operator/(  double a,  Complex &b );

	friend	Complex		operator+(  Complex &a,  double b );
	friend	Complex		operator-(  Complex &a,  double b );
	friend	Complex		operator*(  Complex &a,  double b );
	friend	Complex		operator/(  Complex &a,  double b );

	Complex&	operator=( Complex &b );
	Complex&	operator=( double b );
	
	int		operator==( Complex &b );
	int		operator==( double b );

	void		Set(double dReal, double dImag );
	double	Re( void );
	double	Im( void );
	
	friend	double		Arg(  Complex &b );
	friend	double		Module(  Complex &b );
	friend	double		fabs( Complex &b );

	friend	double		Re(  Complex &b );
	friend	double		Im(  Complex &b );
	
	friend	Complex		Sin(  Complex &b );
	friend	Complex		Cos(  Complex &b );
	friend	Complex		Tan(  Complex &b );
	friend	Complex		Ctg(  Complex &b );

	friend	Complex		Arcsin(  Complex &b );
	friend	Complex		Arccos(  Complex &b );
	friend	Complex		Arctan(  Complex &b );
	friend	Complex		Arcctg(  Complex &b );

	friend	Complex		Sh(  Complex &b );
	friend	Complex		Ch(  Complex &b );
	friend	Complex		Th(  Complex &b );
	friend	Complex		Cth(  Complex &b );

	friend	Complex		Arsh(  Complex &b );
	friend	Complex		Arch(  Complex &b );
	friend	Complex		Arth(  Complex &b );
	friend	Complex		Arcth(  Complex &b );

	friend	Complex		Exp(  Complex &b );
	friend	Complex		Ln(  Complex &b );

	friend	Complex		Pow( Complex &a,  Complex &b );
	friend	Complex	 	Pow( Complex &a,  double b );
	friend	Complex		Pow( double a,  Complex &b );

	friend	void		Print( Complex &b );
	friend	void		Print( Complex &b, char *szFormatString );
	friend	void		Print( Complex &b, FILE *fp );
	friend	void		Print( Complex &b, FILE *fp, char *szFormatString );
	
	friend	int			GetComplexFromString( char *lpszStr, double *dReal, double *dImag );

	friend	class		ComplexMatrix;
	friend	class		ComplexVector;

private:
	double	m_dReal, m_dImag;
};