#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"


//******************************************************************
//						COMPLEX
//******************************************************************

Complex::Complex()
{
	m_dReal = 0.0;
	m_dImag = 0.0;
}

Complex::~Complex()
{
}

Complex::Complex( double both )
{
	m_dReal = m_dImag = both;
}

Complex::Complex( double a, double b )
{
	m_dReal=a;
	m_dImag=b;
}

Complex::Complex( const Complex &Copy )
{
	m_dReal = Copy.m_dReal;
	m_dImag = Copy.m_dImag;
}

//*******************************************************************
//					Operatori
//*******************************************************************

Complex		Complex::operator+(   Complex &b ) 
{
	return Complex( m_dReal + b.m_dReal, m_dImag + b.m_dImag);
}

Complex		Complex::operator-(  Complex &b ) 
{
	return Complex( m_dReal - b.m_dReal, m_dImag - b.m_dImag);
}

Complex		Complex::operator*(  Complex &b ) 
{
	return Complex(m_dReal * b.m_dReal - m_dImag * b.m_dImag, m_dReal * b.m_dImag + m_dImag * b.m_dReal);
}

Complex		Complex::operator/(  Complex &b ) 
	{
		Complex		Ret(0,0);
		double 		d_c, c_d, H;
		
		if( fabs(b.m_dReal) >= fabs(b.m_dImag) )
		{
			d_c = b.m_dImag / b.m_dReal;
			H = b.m_dReal + b.m_dImag * d_c;

			if( H == 0.0 )
			{
				throw MathLibException(DIV_ZERO);
				printf("Beskonacan rezultat kod kompleksnog dijeljenja !!!");
//				assert(0);
			}

			Ret.m_dReal = (m_dReal + m_dImag * d_c) / H;
			Ret.m_dImag = (m_dImag - m_dReal * d_c) / H;
		}
		else
		{
			c_d = b.m_dReal / b.m_dImag;
			H = b.m_dReal  * c_d + b.m_dImag;
			
			if( H == 0.0 )
			{
				throw MathLibException(DIV_ZERO);
				printf("Beskonacan rezultat kod kompleksnog dijeljenja !!!");
//				assert(0);
			}

			Ret.m_dReal = (m_dReal * c_d + m_dImag) / H;
			Ret.m_dImag = (m_dImag * c_d - m_dReal) / H;
		}
		
		return	Ret;
}	

Complex	Complex::operator+=(  Complex &b )
{
	m_dReal += b.m_dReal;
	m_dImag += b.m_dImag;

	return	*this;
}

Complex	Complex::operator-=(  Complex &b )
{
	m_dReal -= b.m_dReal;
	m_dImag -= b.m_dImag;

	return	*this;
}

Complex	Complex::operator*=(  Complex &b )
{
	double	RealTmp, ImagTmp;

	RealTmp = m_dReal * b.m_dReal - m_dImag * b.m_dImag;
	ImagTmp = m_dReal * b.m_dImag + m_dImag * b.m_dReal;

	m_dReal = RealTmp;
	m_dImag = ImagTmp;

	return	*this;
}

Complex	Complex::operator/=(  Complex &b )
{
	Complex		Ret(0,0);
	double 		d_c, c_d, H;
	
	if( fabs(b.m_dReal) >= fabs(b.m_dImag) )
	{
		d_c = b.m_dImag / b.m_dReal;
		H = b.m_dReal + b.m_dImag * d_c;

		if( H == 0.0 )
		{
			throw MathLibException(DIV_ZERO);
			printf("Beskonacan rezultat kod kompleksnog dijeljenja !!!");
//			assert(0);
		}

		Ret.m_dReal = (m_dReal + m_dImag * d_c) / H;
		Ret.m_dImag = (m_dImag - m_dReal * d_c) / H;
	}
	else
	{
		c_d = b.m_dReal / b.m_dImag;
		H = b.m_dReal  * c_d + b.m_dImag;
		
		if( H == 0.0 )
		{
			throw MathLibException(DIV_ZERO);
			printf("Beskonacan rezultat kod kompleksnog dijeljenja !!!");
//			assert(0);
		}

		Ret.m_dReal = (m_dReal * c_d + m_dImag) / H;
		Ret.m_dImag = (m_dImag * c_d - m_dReal) / H;
	}
	m_dReal = Ret.m_dReal;
	m_dImag = Ret.m_dImag;
	
	return	*this;
}

Complex	operator+(  double a,  Complex &b )
{
	return	Complex(a + b.m_dReal, b.m_dImag );
}

Complex	operator-(  double a,  Complex &b )
{
	return Complex( a - b.m_dReal, -b.m_dImag );
}

Complex	operator*(  double a,  Complex &b )
{
	return Complex(a * b.m_dReal, a * b.m_dImag );
}

Complex	operator/(  double a,  Complex &b )
{
	Complex	Tmp(0,0);
	double	dTmp;

	if( b.m_dReal == 0 && b.m_dImag == 0 )
	{
		throw MathLibException(DIV_ZERO);
	}
	else
	{
		dTmp = pow(b.m_dReal,2) + pow(b.m_dImag,2);
		
		Tmp.m_dReal = a * b.m_dReal / dTmp;
		Tmp.m_dImag = -a * b.m_dImag / dTmp;
	}
	return	Tmp;
}

Complex	operator+(  Complex &a,  double b )
{
	return Complex( a.m_dReal + b, a.m_dImag );
}

Complex	operator-(  Complex &a,  double b )
{
	return Complex( a.m_dReal - b, a.m_dImag );
}

Complex operator*(  Complex &a,  double b )
{
	return Complex( a.m_dReal * b, a.m_dImag * b );
}

Complex	operator/(  Complex &a,  double b )
{
	if( b != 0 )
		return Complex( a.m_dReal / b, a.m_dImag / b );
	else
	{
		throw MathLibException(DIV_ZERO);
		printf("Dijeljenje kompleksnog broja s 0 !!!");
//		assert(0);
		return	Complex(0,0);	// beskonaèan rezultat
	}
}

Complex	Complex::operator+=(  double a )
{
	m_dReal += a;

	return	*this;
}

Complex	Complex::operator-=(  double a )
{
	m_dReal -= a;

	return	*this;
}

Complex	Complex::operator*=(  double a )
{
	m_dReal *= a;
	m_dImag *= a;

 	return *this;
}

Complex	Complex::operator/=(  double a )
{
	if( a != 0.0 )
	{
		m_dReal /= a; 
		m_dImag /= a;
	}
	else
	{
		throw MathLibException(DIV_ZERO);
	}

	return *this;
}

Complex&	Complex::operator=( Complex &b )
{
	m_dReal = b.m_dReal;
	m_dImag = b.m_dImag;
		
	return	*this;
}

Complex&	Complex::operator=( double b )
{
	m_dReal = b;
	m_dImag = 0;

	return	*this;
}

int	Complex::operator==(  Complex &b )
{
	if( m_dReal == b.m_dReal && m_dImag == b.m_dImag )
		return 1;
	else
		return 0;
}

int Complex::operator==( double b )
{
	if( m_dReal == b && m_dImag == 0.0 )
		return 1;
	else
		return 0;
}

//******************************************************************
//	              					Funkcije
//******************************************************************

void	Print( Complex &b )
{
	printf("\n%10.3f + i %-10.3f", b.m_dReal, b.m_dImag);
}

void	Print( Complex &b, char *szFormatString )
{
	printf(szFormatString, b.m_dReal, b.m_dImag);
}

void	Print( Complex &b, FILE *fp )
{
	fprintf(fp, "\n%10.3f + i %-10.3f", b.m_dReal, b.m_dImag);
}

void	Print( Complex &b, FILE *fp, char *szFormatString )
{
	fprintf(fp, szFormatString, b.m_dReal, b.m_dImag);
}

double	Arg(  Complex &b )
{
	double	dRet=0;

	if( b.m_dReal == 0 && b.m_dImag == 0 )
	{
		throw MathLibException(UNDEFINED_ARG);
	}
	else
		dRet = atan2( b.m_dImag, b.m_dReal);
	
	return	dRet;
}

double	Module(  Complex &A )
{
	double a, b;

	a = fabs(A.m_dReal);
	b = fabs(A.m_dImag);

	if( a >= b )
		return (a * sqrt(1 + (b/a)*(b/a)));
	else
		return (b * sqrt(1 + (a/b)*(a/b)));
}

double fabs( Complex &b )
{
	return Module(b);
}

double Complex::Re( void )
{
	return m_dReal;
}

double Complex::Im( void )
{
	return m_dImag;
}

double	Re(  Complex &b )
{
	return b.m_dReal;
}

double	Im(  Complex &b )
{
	return	b.m_dImag;
}

void Complex::Set( double dReal , double dImag )
{
	m_dReal = dReal;
	m_dImag = dImag;
}

int	GetComplexFromString( char *pszStr, double *dR, double *dI )
{
	char		sTemp[100];
	int			i, k;

	*dR = *dI = 0.0;
	i = k = 0;
	while( pszStr[i] != 0 )
	{
		memset(sTemp, 0, 100);

		while( pszStr[i] != '+' && pszStr[i] != 0 )
			sTemp[k++] = pszStr[i++];
		
		*dR = atof(sTemp);
		if( pszStr[i] == 0 )		// ako smo veæ stigli do kraja, a nismo imali imaginarni dio, to je u redu
		{
			*dI = 0;
			return 0;
		}
		
		while( pszStr[i] != 'i' && pszStr[i] != 0 )
			i++;
		if( pszStr[i] == 0 )		// znaèi da ima neèega poslije realnog dijela, a nije imaginarni dio
			return 1;
		else
		{
			// sada æemo izvaditi i imaginarni dio
			k = 0;
			i++;
			while( pszStr[i] != 0 )
				sTemp[k++] = pszStr[i++];
			*dI =atof(sTemp);
		}
	}
	return 0;
}

//**************      Eksponencijalne funkcije      **************************/

Complex	Exp(  Complex &b )
{
	Complex	Tmp(0,0);

	Tmp.m_dReal = exp(b.m_dReal) * cos(b.m_dImag);
	Tmp.m_dImag = exp(b.m_dReal) * sin(b.m_dImag);

	return Tmp;
}

Complex	Ln(  Complex &b )
{
	Complex	Tmp(0,0);

	if( Module(b) != 0 )
	{
		Tmp.m_dReal = log( Module( b ) );
		Tmp.m_dImag = Arg( b );
	}
	else		
	{
		// funkcija nije definirana [ln(0) = beskonacno]
		throw MathLibException(FUNC_UNDEFINED);
	}
	return	Tmp;
}

Complex	Pow(  Complex &a,  Complex &b )
{
	Complex	Tmp;
	double	dX, dY, dArg=0;

	if( Module(a) != 0 )
	{
		dArg = Arg(a);

		dX = b.m_dReal * log(Module(a)) - b.m_dImag * dArg;
		dY = b.m_dReal * dArg + b.m_dImag * log(Module(a));

		Tmp.m_dReal = exp(dX) * cos(dY);
		Tmp.m_dImag = exp(dX) * sin(dY);
	}
	else		
	{
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}

	return	Tmp;
}

Complex	Pow(  Complex &a,  double b )
{
	Complex	Tmp;
	double	dArg, dTmp;

	if( Module(a) != 0 )
	{
		dTmp = exp(b * log(Module(a)));
		dArg = Arg(a);
		
		Tmp.m_dReal = dTmp * cos(b * dArg);
		Tmp.m_dImag = dTmp * sin(b * dArg);
	}
	else		
	{
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}
	
	return	Tmp;
}

Complex	Pow(  double a,  Complex &b )
{
	Complex	Tmp;
	double	dX, dY, dArg=0;

	if( fabs(a) != 0 )
	{
		if( a < 0 )
			dArg = M_PI;

		dX = b.m_dReal * log(fabs(a)) - b.m_dImag * dArg;
		dY = b.m_dReal * dArg + b.m_dImag * log(fabs(a));

		Tmp.m_dReal = exp(dX) * cos(dY);
		Tmp.m_dImag = exp(dX) * sin(dY);
	}
	else		
	{
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}

	return	Tmp;
}


/****************			Trigonometrijske funkcije     *************************/

Complex	Sin(  Complex &b )
{
	double	dR, dI;

	dR=0.5 * sin(b.m_dReal) * ( exp(-b.m_dImag) + exp(b.m_dImag) );
	dI=0.5 * cos(b.m_dReal) * ( exp(b.m_dImag) - exp(-b.m_dImag) );

	return Complex(dR,dI);
}

Complex	Cos(  Complex &b )
{
	Complex	Tmp;

	Tmp.m_dReal = 0.5 * cos(b.m_dReal) * ( exp(-b.m_dImag) + exp(b.m_dImag) );
	Tmp.m_dImag = -0.5 * sin(b.m_dReal) * ( exp(-b.m_dImag) - exp(b.m_dImag) );

	return Tmp;
}

Complex	Tan(  Complex &z )
{
	Complex	Tmp, Ret;

	Tmp = Cos(z);
	if( Tmp.m_dReal == 0 && Tmp.m_dImag == 0 )
	{			
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}
	else
		Ret = Sin(z) / Tmp;

	return	Ret;
}

Complex	Ctg(  Complex &z )
{
	Complex Tmp, Ret;

	Tmp = Sin(z);
	if( Tmp.m_dReal == 0 && Tmp.m_dImag == 0 )
	{			
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}
	else
		Ret = Cos(z) / Tmp;

	return Ret;
}

Complex	Arcsin(  Complex &z )
{
	Complex	Tmp, i(0,1);

	Tmp = -1 * i * Ln(i * z + Pow(1 - z * z, 0.5) );

	return	Tmp;
}

Complex	Arccos(  Complex &z )
{
	Complex	Tmp, i(0,1);

	Tmp = -1 * i * Ln(z + Pow(z * z - 1, 0.5) );

	return Tmp;
}

Complex	Arctan(  Complex &z )
{
	Complex	Tmp, i(0,1);

	if( Re(z) == 0 && Im(z) == 1 )
	{				
		// funkcija nije definirana u (0,1)
		throw MathLibException(FUNC_UNDEFINED);
	}
	else
	{
		Tmp = 0.5 * i * Ln( (i + z) / (i - z) );
	}

	return Tmp;
}

Complex	Arcctg(  Complex &z )
{
	Complex	Tmp, i(0,1);

	if( Re(z) == 0 && Im(z) == -1 )
	{				
		// funkcija nije definirana u (0,-1)
		throw MathLibException(FUNC_UNDEFINED);
	}
	else
	{
		Tmp = 0.5 * i * Ln( (z - i) / (z + i) );
	}

	return Tmp;
}

/*******************				Hiperbolne funkcije					*************************/

Complex	Sh(  Complex &b )
{
	Complex	Tmp;

	Tmp.m_dReal = 0.5 * cos(b.m_dImag) * ( exp(b.m_dReal) - exp(-b.m_dReal) );
	Tmp.m_dImag = 0.5 * sin(b.m_dImag) * ( exp(b.m_dReal) + exp(-b.m_dReal) );

	return	Tmp;
}

Complex	Ch(  Complex &b )
{
	Complex	Tmp;

	Tmp.m_dReal = 0.5 * cos(b.m_dImag) * ( exp(b.m_dReal) + exp(-b.m_dReal) );
	Tmp.m_dImag = 0.5 * sin(b.m_dImag) * ( exp(b.m_dReal) - exp(-b.m_dReal) );

	return	Tmp;
}

Complex	Th(  Complex &z )
{
	Complex	Tmp, Ret;

	Tmp = Ch(z);
	if( Tmp.m_dReal == 0 && Tmp.m_dImag == 0 )
	{			
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}
	else
		Ret = Sh(z) / Tmp;

	return Ret;
}

Complex	Cth(  Complex &z )
{
	Complex	Tmp, Ret;

	Tmp = Sh(z);
	if( Tmp.m_dReal == 0 && Tmp.m_dImag == 0 )
	{			
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}
	else
		Ret = Ch(z) / Tmp;

	return Ret;
}

Complex	Arsh(  Complex &z )
{
	Complex	Tmp, i(0,1);

	Tmp = Ln(z + Pow(1 + z * z, 0.5) );

	return Tmp;
}

Complex	Arch(  Complex &z )
{
	Complex	Tmp;

	Tmp = Ln(z + Pow(z * z - 1, 0.5) );

	return Tmp;
}

Complex	Arth(  Complex &z )
{
	Complex	Tmp;

	if( z.m_dReal == 1 && z.m_dImag == 0 )
	{				
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}
	else
	{
		Tmp = 0.5 * Ln( (1 + z) / (1 - z) );
	}

	return Tmp;
}

Complex	Arcth(  Complex &z )
{
	Complex Tmp;

	if( z.m_dReal == 1 && z.m_dImag == 0 )
	{				
		// funkcija nije definirana
		throw MathLibException(FUNC_UNDEFINED);
	}
	else
		Tmp = 0.5 * Ln( (z + 1) / (z - 1) );

	return Tmp;
}

