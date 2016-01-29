double GammaLn( double xx );
double GammaP( double a, double x );
double GammaQ( double a, double x );
double Beta( double z, double w );

void		GammaSer(double *gamser, double a, double x, double *gln );
void		GammaCFrac( double *gammcf, double a, double x , double *gln );

double	Errf( double x );
double	ErrfCompl( double x );

double	ExpInt( int n, double x);
double	Ei( double x);
double	BetaCFrac( double a, double b, double x );

double	BesselJ0( double x );
double	BesselY0( double x );
void		BesselJY( double x, double xnu, double *rj, double *ry, double *rjp, double *ryp );
void		beschb( double x, double *gam1, double *gam2, double *gampl, double *gammi );
double	chebev( double a, double b, double c[], int m, double x );

double	LegandrePoly( int l, int m, double x);
void		SphericalBessel( int n, double x, double *sj, double *sy, double *sjp, double *syp );

void		CiSi( double x, double *ci, double *si );