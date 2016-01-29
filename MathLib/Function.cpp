#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>

#include	"mathlib.h"

#include <complex>

using namespace std;

template<class Type> Function<Type>::Function()
{
	m_pfFunc = NULL;
	m_pfncFunc = NULL;
	
	m_eType = NORMAL;
}

template<class Type> Function<Type>::Function( Type (*pfFunc)( Type ) )
{
	m_pfFunc = pfFunc;
	m_pfncFunc = NULL;

	m_eType = NORMAL;
}

template<class Type> Function<Type>::~Function()
{
}

template<class Type> Type Function<Type>::Evaluate( Type x )
{
	if( m_pfFunc != NULL )
	{
		Type		ret;
		ret = (*m_pfFunc)(x);
		return ret;
	}
	else
	{
		printf("Funkcija nje definirana !!!");
//		assert(0);
		return Type();
	}
}

template<class Type> Type Function<Type>::Derive( Type x )
{
	return Type();
}

template<class Type> Type Function<Type>::Integrate( Type a, Type b )
{
	return Type();
}

template<class Type> Function<Type> Function<Type>::Derive( void )
{
	Function	fncRet;

	fncRet.m_pfFunc = NULL;
	fncRet.m_pfncFunc = this;
	fncRet.m_eType = DERIVATIVE;

	return fncRet;
}


template	class		Function<float>;
template	class		Function<double>;
template	class		Function< complex<double> >;
//template	class		Function< Complex >; javlja INTERNAL COMPILER ERROR