#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"



//	StartPoint3D - poèetna toèka od koje æe poèeti traženje
//  nDim - u koliko dimenzija se traži
//  func - pokazivaè na funkciju za koju se traži minimum, koja kao 
//	       parametar ima niz pokazivaèa, koji treba biti bar 'nDim' dugaèak
//  Min  - u ovu varijablu se sprema rezultat

void	FuncMinN( Vector StartPoint3D, int nDim, double (*func) (double *), Vector *Min )
{
	int			bLower, IterNum=0;
	double		FuncVal, TempFuncVal, sum;
	int			i, j, h, l;
	Vector		*Simplex, *Xc, *Xr, *Xe, *Xk;
	double		*pdInit;
	
	assert( nDim>0 );		// ukoliko je kriva dimenzija, prekini izvršavanje

	pdInit = new double[nDim];
	for( i=0; i<nDim; i++ )
		pdInit[i] = 0;

	Simplex = new Vector[nDim+1];
	for( i=0; i<=nDim; i++ )
		Simplex[i].Reinitialize(nDim);

	Xc = new Vector(nDim );
	Xr = new Vector(nDim );
	Xe = new Vector(nDim );
	Xk = new Vector(nDim );

	// za centar simpleksa æemo uzeti poèetnu toèku
	Simplex[0] = StartPoint3D;
	// a za ostale toèke æemo uzeti još na tu poèetnu toèku dodane jediniène
	// vektore u smjeru koordinatnih osi
	for( i=1; i<=nDim; i++ )
	{
		pdInit[i] = 1.0;
		Simplex[i].InitValues(pdInit);
		pdInit[i] = 0.0;
	}

	do
	{
		// odredjivanje tocke u kojoj je funkcija najveca
		h=0;
		FuncVal = (*func) (Simplex[h].GetPtr());
		for( i=1; i<nDim+1; i++ )
			if( (TempFuncVal=(*func)( Simplex[i].GetPtr() )) > FuncVal )
			{
				h=i;
				FuncVal = TempFuncVal;
			}

		// odredjivanje tocke u kojoj je funkcija najmanja
		l=0;
		FuncVal = (*func)( Simplex[l].GetPtr() );
		for( i=1; i<nDim+1; i++ )
			if( (TempFuncVal=(*func)( Simplex[i].GetPtr() )) < FuncVal )
			{
				l=i;
				FuncVal = TempFuncVal;
			}

 		// odredjivanje centroida
		Xc->InitValues(pdInit);
		for( i=0; i<nDim+1; i++ )
			if( i != h )
				*Xc = Simplex[i] + (*Xc);
		*Xc =  (1/ (double) nDim) * (*Xc);

		// refleksija
		*Xr = (1 + ALPHA) * (*Xc) - ALPHA * Simplex[h];

		if( (*func)( Xr->GetPtr() ) < (*func)( Simplex[l].GetPtr() ) )
		{
			// ekspanzija
			*Xe = (1 - GAMA) * (*Xc) + GAMA * (*Xr);
			if( (*func)(Xe->GetPtr()) < (*func)( Simplex[l].GetPtr() ) )
			{
				Simplex[h] = *Xe;
			}
			else
			{
				Simplex[h] = *Xr;
			}
		}
		else
		{
			bLower = 1;
			for( j=0; j<nDim+1; j++ )
				if( j!= h )
					if( (*func)( Xr->GetPtr() ) < (*func)( Simplex[j].GetPtr() ) )
						bLower = 0;
			
			if( bLower == 1 )
			{
				if( (*func)( Xr->GetPtr() ) < (*func)( Simplex[h].GetPtr() ) )
				{
					Simplex[h] = *Xr;
				}
				// kontrakcija
				*Xk = (1 - BETA) * (*Xc) + BETA * Simplex[h];
				if( (*func)( Xk->GetPtr() ) > (*func)( Simplex[h].GetPtr() ) )
				{
					for( j=0; j<nDim+1; j++ )
						if( j != l )
							Simplex[j] = 0.5 * (Simplex[j] + Simplex[l]);
				}
				else
				{
					Simplex[h] = *Xk;
				}
			}
			else
			{
				Simplex[h] = *Xr;
			}
		}
		
		// da li rješenje zadovoljava
		sum = 0;
		for( j=0; j<nDim+1; j++ )
			sum += pow( ( (*func)(Simplex[j].GetPtr()) - (*func)(Xc->GetPtr())), 2 );

		sum /= nDim;
		sum = sqrt(sum);
		IterNum++;
	} while( sum > EPS && IterNum < SIMPLEX_MAX_ITER_NUM );

	*Min = *Xc;

//	delete pdInit;

	delete []Simplex;
	delete Xc;
	delete Xe;
	delete Xr;
	delete Xk;
}


