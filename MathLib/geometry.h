#include "MLibBasic.h"

class	Point3D
{
private:
	double m_dX, m_dY, m_dZ;

public:
	Point3D();
	Point3D( Point3D &Copy );
	Point3D( double a, double b, double c );
	~Point3D();

	Point3D	operator+( const Point3D &b ) const;
	Point3D	operator-( const Point3D &b ) const;
	Point3D	operator+=( const Point3D &b );
	Point3D	operator-=( const Point3D &b );
	Point3D	operator*=( const double b );

	Point3D&	operator=( const Point3D &b );

	friend	Point3D	operator*( double a, const Point3D &b );
	friend	Point3D	operator*( const Point3D &a, double b );
	friend	Point3D	operator/( const Point3D &a, double b );

	friend	double	Distance( const Point3D &a, const Point3D &b );

	friend	double	GetX( const Point3D &a );
	friend	double	GetY( const Point3D &a );
	friend	double	GetZ( const Point3D &a );
	friend	void SetX( Point3D &a, double val );
	friend	void SetY( Point3D &a, double val );
	friend	void SetZ( Point3D &a, double val );
};

class	Vector3D
{
private:
	double	m_dX, m_dY, m_dZ;
public:
	Vector3D();
	Vector3D( Vector3D &Copy );
	Vector3D( Point3D &a, Point3D &b );
	Vector3D( double x, double y, double z );
	~Vector3D();

	friend	double	GetX( const	Vector3D &a );
	friend	double	GetY( const Vector3D &a );
	friend	double	GetZ( const Vector3D &a );

	friend	double		Module( const Vector3D &a );
	friend	double		Distance( const Vector3D &a, const Vector3D &b );
	friend	Vector3D	GetOrt( const Vector3D &a );

	void	Set( double x, double y, double z );

	Vector3D	operator+( const Vector3D &b ) const;
	Vector3D	operator-( const Vector3D &b ) const;
	Vector3D	operator*( const Vector3D &b ) const;			// vektorsko mnozenje
	double		operator&( const Vector3D &b ) const;			// skalarno mnozenje

	Vector3D	operator+=( const Vector3D &b );
	Vector3D	operator-=( const Vector3D &b );
	Vector3D	operator*=( const Vector3D &b );

	Vector3D	operator=( const Vector3D &b );
	double&		operator[]( int i );

	friend	Vector3D	operator*( double a, const Vector3D &b );
	friend	Vector3D	operator*( const Vector3D &a, double b );
	friend	Vector3D	operator/( const Vector3D &a, double b );
};