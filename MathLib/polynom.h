#include "MLibBasic.h"

class	Polynom
{
private:
	int			m_nDegree;
	double	*m_pdCoef;

public:
	Polynom();
	Polynom( int n );
	Polynom( const Polynom &Copy );
	~Polynom();

	friend	int			GetDegree( const Polynom &a );
	friend	int			GetRealDegree( const Polynom & a );
	friend	double	GetCoef( const Polynom &a, int CoefNum );
	friend	double	GetLeadingCoef( const Polynom &a );
	friend	void		ReducePolynom( Polynom *a );

	double		Val( double x );
	Polynom		operator+( const Polynom &b ) const;
	Polynom		operator-( const Polynom &a ) const;
	Polynom		operator*( const Polynom &b ) const;
	Polynom		operator/( const Polynom &b ) const;
	Polynom		operator%( const Polynom &b ) const;	// dijeljenje po modulu (ostatak)
	Polynom&	operator=( const Polynom &b );

	Polynom		operator+=( const Polynom &b );
	Polynom		operator-=( const Polynom &b );
	Polynom		operator*=( const Polynom &b );
	Polynom		operator*=( const double b );
	Polynom		operator/=( const double &b );
//	Polynom		operator/=( const Polynom &b );

	double&		operator[]( int i );

	friend	Polynom	operator*( double a, const Polynom &b );
	friend	Polynom	operator*( const Polynom &a, double b );

	friend	void	Input( Polynom *a );
	friend	void	Print( const Polynom &a );
};

