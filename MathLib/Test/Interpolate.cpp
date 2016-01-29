#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mathlib.h>
/*
struct stupnjevi
{
	int	stup;
	int	min;
};

void transform( struct stupnjevi stup[], int n, double rad[] )
{
	int		i;

	for( i=0; i<n; i++ )
	{
		rad[i] = stup[i].stup + stup[i].min / 60.0;
	}
}

void main( void )
{
	struct stupnjevi ziva[8] = { 40, 52, 41, 13, 41, 33, 41, 49, 42, 23, 42, 26, 43, 24, 44, 6 };
	struct stupnjevi kadmij[6] = { 41, 3, 41, 7, 42, 11, 42, 35, 42, 46, 43, 14 };
	struct stupnjevi neon[6] = { 40, 49, 40, 58, 41, 6, 41, 27, 41, 53, 42, 45 };

	double	ziva_rad[8];
	double	kadmij_rad[6];
	double	neon_rad[6];

	double val_dulj[8] = { 690, 624, 578, 548, 496, 492, 435, 408 };

	transform( ziva, 8, ziva_rad );
	transform( kadmij, 6, kadmij_rad );
	transform( neon, 6, neon_rad );

	double	val, err;

	printf("Element   Otklon      Valna duzina    Num.greska\n");
	puts("------------------------------------------------");

	for( int i=0; i<6; i++ )
	{
		polint(ziva_rad, val_dulj, 8, kadmij_rad[i], &val, &err );
		printf(" Kadmij   %2d %2d'   -   %7.1f    ,    %6.2f\n", kadmij[i].stup, kadmij[i].min, val, err );
	}

	printf("\n");

	for( i=0; i<6; i++ )
	{
		polint(ziva_rad, val_dulj, 8, neon_rad[i], &val, &err );
		printf("  Neon    %2d %2d'   -   %7.1f    ,    %6.2f\n", neon[i].stup, neon[i].min, val, err );
	}
}
*/