#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

VectorFunction::VectorFunction( Vector (*pfFunc)( Vector &), int DomDim, int CodomDim )
{
	m_pfFunc = pfFunc;

	m_nCodomDim = CodomDim;
	m_nDomDim = DomDim;
}

VectorFunction::~VectorFunction()
{
}

Vector	VectorFunction::Evaluate( Vector &X )
{
	if( m_pfFunc != NULL )
		return (*m_pfFunc)(X);
	else
	{
		printf("Nije definirana funkcija");
		assert(0);

		Vector	a;
		return a;
	}
}

Matrix	VectorFunction::CalcJacobian( Vector &X )
{
	// numerièki izraèunava jacobian sistema u zadanoj toèci
	//	X - toèka u kojoj se raèuna
	//	P - vrijednost parametara za koje se raèuna
	//	Jacobian - pointer na matricu u koju æe se smjestiti rezultat
	int			i, j, nDim;
	double	h, temp;
	Vector	*f1, *f2;
	Matrix	Jacobian(m_nCodomDim, m_nDomDim);

	if( m_pfJac == NULL )
	{
		nDim = m_nDomDim;
		
		f1 = new Vector(m_nCodomDim);		//	ovdje æemo smjestiti vrijednosti u x1
		f2 = new Vector(m_nCodomDim);		//  a ovdje u x2
		
		*f1 = (*m_pfFunc)(X);		// raèunamo vrijednost funkcije u x1

		for( j=0; j<m_nDomDim; j++ )
		{
			temp = X[j];
			h = 1e-5 * fabs(temp);		// ovdje odredjujemo najmanji h uz koji
			if( h == 0.0 )						// cemo zadrzati tocnost numericki odredjene
				h = 1e-5;								// derivacije

			X[j] = temp + h;					// sada raèunamo koordinate toèke x2
			h = X[j] - temp;

			*f2 = (*m_pfFunc)(X);			// racunamo vrijednost funkcije u x2
			X[j] = temp;

			for( i=0; i<m_nCodomDim; i++ )
				Jacobian[i][j] = ((*f2)[i] - (*f1)[i]) / h;
		}
		
		delete f1;
		delete f2;

		return Jacobian;
	}
	else
		return (*m_pfJac)(X);
}
