#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

#define ALPHA	1
#define BETA	0.5
#define GAMA	2
#define EPS		1e-6



//*************************************************************
//					VECTOR
//*************************************************************

Vector3D::Vector3D()
{
}

Vector3D::~Vector3D()
{
}

Vector3D::Vector3D( double a=0, double b=0, double c=0 )
{
	m_dX = a;
	m_dY = b;
	m_dZ = c;
}

Vector3D::Vector3D( Vector3D &Copy )
{
	m_dX = Copy.m_dX;
	m_dY = Copy.m_dY;
	m_dZ = Copy.m_dZ;
}

Vector3D::Vector3D( Point3D &a, Point3D &b )
{
	m_dX = GetX(a) - GetX(b);
	m_dY = GetY(a) - GetY(b);
	m_dZ = GetZ(a) - GetZ(b);
}

Vector3D	Vector3D::operator+( const Vector3D &b ) const
{
	return	Vector3D(m_dX + b.m_dX, m_dY + b.m_dY, m_dZ + b.m_dZ );
}

Vector3D	Vector3D::operator-( const Vector3D &b ) const
{
	return	Vector3D(m_dX - b.m_dX, m_dY - b.m_dY, m_dZ - b.m_dZ );
}

Vector3D	Vector3D::operator*( const Vector3D &b ) const
{
	Vector3D	Ret(0,0,0);

	Ret.m_dX = m_dY * b.m_dZ - m_dZ * b.m_dY;
	Ret.m_dY = -(m_dX * b.m_dZ - m_dZ * b.m_dX);
	Ret.m_dZ = m_dX * b.m_dY - m_dY * b.m_dX;

	return	Ret;
}

double	Vector3D::operator&( const Vector3D &b ) const
{
	return	m_dX * b.m_dX + m_dY * b.m_dY + m_dZ * b.m_dZ;
}

Vector3D	operator*( double a, const Vector3D &b )
{
	return	Vector3D( a * b.m_dX, a * b.m_dY, a * b.m_dZ );
}

Vector3D	operator*( const Vector3D &a, double b )
{
	return	Vector3D( b * a.m_dX, b * a.m_dY, b * a.m_dZ );
}

Vector3D	operator/( const Vector3D &a, double b )
{
	return	Vector3D(a.m_dX / b, a.m_dY / b, a.m_dZ / b );
}

Vector3D	Vector3D::operator+=( const Vector3D &b )
{
	m_dX += b.m_dX;
	m_dY += b.m_dY;
	m_dZ += b.m_dZ;

	return *this;
}

Vector3D	Vector3D::operator-=( const Vector3D &b )
{
	m_dX -= b.m_dX;
	m_dY -= b.m_dY;
	m_dZ -= b.m_dZ;

	return *this;
}

Vector3D	Vector3D::operator*=( const Vector3D &b )
{
	Vector3D	Ret(0,0,0);

	Ret.m_dX = m_dY * b.m_dZ - m_dZ * b.m_dY;
	Ret.m_dY = -(m_dX * b.m_dZ - m_dZ * b.m_dX);
	Ret.m_dZ = m_dX * b.m_dY - m_dY * b.m_dX;

	m_dX = Ret.m_dX;
	m_dY = Ret.m_dY;
	m_dZ = Ret.m_dZ;

	return	*this;
}

Vector3D	Vector3D::operator=( const Vector3D &a )
{
	m_dX = a.m_dX;
	m_dY = a.m_dY;
	m_dZ = a.m_dZ;

	return	*this;
}

double&	Vector3D::operator[]( int i )
{
	if( i == 0 )
		return m_dX;
	else if( i == 1 )
		return m_dY;
	else
		return m_dZ;
}

void Vector3D::Set( double x, double y, double z )
{
	m_dX = x;
	m_dY = y;
	m_dZ = z;
}

double	GetX( const Vector3D &a )
{
	return a.m_dX;
}

double	GetY( const Vector3D &a )
{
	return a.m_dY;
}

double	GetZ( const Vector3D &a )
{
	return a.m_dZ;
}

double	Module( const Vector3D &a )
{
	return sqrt( pow(a.m_dX,2) + pow(a.m_dY,2) + pow(a.m_dZ,2) );
}

Vector3D	GetOrt( const Vector3D &a )
{
	Vector3D	Ret(0,0,0);
	double	dMod;

	dMod = Module(a);
	if( dMod != 0 )
	{
		Ret.m_dX = a.m_dX / dMod;
		Ret.m_dY = a.m_dY / dMod;
		Ret.m_dZ = a.m_dZ / dMod;
	}
	
	return	Ret;
}

double	Distance( const Vector3D &a, const Vector3D &b )
{
	return sqrt( pow(a.m_dX - b.m_dX, 2) + pow(a.m_dY - b.m_dY, 2) + pow(a.m_dZ - b.m_dZ, 2) );
}

//**********************************************************
//						Point3D
//**********************************************************

Point3D::Point3D( double a=0, double b=0, double c=0 )
{
	m_dX = a;
	m_dY = b;
	m_dZ = c;
}

Point3D::~Point3D()
{
}

Point3D::Point3D( Point3D &Copy )
{
	m_dX = Copy.m_dX;
	m_dY = Copy.m_dY;
	m_dZ = Copy.m_dZ;
}

Point3D	Point3D::operator+( const Point3D &b ) const
{
	return Point3D(m_dX+b.m_dX, m_dY+b.m_dY, m_dZ+b.m_dZ);
}

Point3D	Point3D::operator-( const Point3D &b ) const
{
	return Point3D(m_dX-b.m_dX, m_dY-b.m_dY, m_dZ-b.m_dZ);
}

Point3D	Point3D::operator+=( const Point3D &b )
{
	m_dX += b.m_dX;
	m_dY += b.m_dY;
	m_dZ += b.m_dZ;

	return *this;
}

Point3D	Point3D::operator-=( const Point3D &b )
{
	m_dX -= b.m_dX;
	m_dY -= b.m_dY;
	m_dZ -= b.m_dZ;

	return *this;
}

Point3D	Point3D::operator*=( const double b )
{
	m_dX *= b;
	m_dY *= b;
	m_dZ *= b;

	return *this;
}

Point3D&	Point3D::operator=( const Point3D &b )
{
	m_dX = b.m_dX;
	m_dY = b.m_dY;
	m_dZ = b.m_dZ;

	return *this;
}

Point3D	operator*( double a, const Point3D &b )
{
	return Point3D(a * b.m_dX, a * b.m_dY, a * b.m_dZ );
}

Point3D	operator*( const Point3D &a, double b )
{
	return Point3D(b * a.m_dX, b * a.m_dY, b * a.m_dZ );
}

Point3D	operator/( const Point3D &a, double b )
{
	return Point3D(a.m_dX / b, a.m_dY / b, a.m_dZ / b);
}

double	GetX( const Point3D &a )
{
	return	a.m_dX;
}

double	GetY( const Point3D &a )
{
	return a.m_dY;
}

double	GetZ( const Point3D &a )
{
	return a.m_dZ;
}

void SetX( Point3D &a, double val )
{
	a.m_dX = val;
}

void SetY( Point3D &a, double val )
{
	a.m_dY = val;
}

void SetZ( Point3D &a, double val )
{
	a.m_dZ = val;
}

double	Distance( const Point3D &a, const Point3D &b )
{
	return sqrt( pow(a.m_dX - b.m_dX, 2) + pow(a.m_dY - b.m_dY, 2) + pow(a.m_dZ - b.m_dZ, 2) );
}

/****************************************************************/
/*******					3D							*********/
/****************************************************************/

Matrix	GetProjectionMatrix( Point3D O, Point3D G )
{
	double		x1, y1, z1, SinAlpha, CosAlpha, x2, y2, z2, SinBeta, CosBeta, h;
	Matrix		T(4,4), T1(4,4), T2(4,4);

	T1.MakeUnitMatrix();
	T2.MakeUnitMatrix();

// 1. transformacija - pomak ishodista u tocku ocista
	T1.SetElem(3, 0, -GetX(O) );
	T1.SetElem(3, 1, -GetY(O) );
	T1.SetElem(3, 2, -GetZ(O) );

	x1 = GetX(G) - GetX(O);
	y1 = GetY(G) - GetY(O);
	z1 = GetZ(G) - GetZ(O);
	
	SinAlpha = y1 / sqrt(x1*x1 + y1*y1);
	CosAlpha = x1 / sqrt(x1*x1 + y1*y1);

// 2. transformacija - rotacija oko osi z za kut alpha
	T2.SetElem(0, 0, CosAlpha);
	T2.SetElem(0, 1, -SinAlpha);
	T2.SetElem(1, 0, SinAlpha);
	T2.SetElem(1, 1, CosAlpha);

	T = T1 * T2;
	T1 = T;
	T2.Clear();

	x2 = sqrt(x1*x1 + y1*y1);
	y2 = 0;
	z2 = z1;

	SinBeta = x2 / sqrt(x2*x2 + z2*z2);
	CosBeta = z2 / sqrt(x2*x2 + z2*z2);

// 3. transformacija - rotacija oko osi y za kut beta
	T2.SetElem(0, 0, CosBeta);
	T2.SetElem(0, 2, SinBeta);
	T2.SetElem(1, 1, 1);
	T2.SetElem(2, 0, -SinBeta);
	T2.SetElem(2, 2, CosBeta);
	T2.SetElem(3, 3, 1);

	T = T1 * T2;
	T1 = T;
	T2.Clear();

// 4. transformacija - rotacija oko osi z za 90 stupnjeva
	T2.SetElem(0, 1, -1);
	T2.SetElem(1, 0, 1);
	T2.SetElem(2, 2, 1);
	T2.SetElem(3, 3, 1);

	T = T1 * T2;
	T1 = T;
	T2.Clear();

// 5. transformacija - promjena predznaka na x osi
	T2.SetElem(0, 0, -1);
	T2.SetElem(1, 1, 1);
	T2.SetElem(2, 2, 1);
	T2.SetElem(3, 3, 1);

	T = T1 * T2;
	T1 = T;

// perspektivna projekcija
	h = sqrt(x1*x1 + y1*y1 + z1*z1);

	T2.MakeUnitMatrix();
	T2.SetElem(2, 3, 1 / h );
	T2.SetElem(3, 3, 0);

	T = T1 * T2;

	return T;
}