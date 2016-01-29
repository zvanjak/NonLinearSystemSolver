#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>
#include	<ctype.h>
#include	<stdlib.h>
#include	<assert.h>
#include	<conio.h>
#include	<time.h>

#include	"mathlib.h"

/********************************************************************/
/*						KALKULATOR						*/
/********************************************************************/

 
MathSpace::MathSpace()
{
	int	i;
	
	for(i=0; i<VAR_NUM; i++)
		Vars[i].bFree = TRUE;

	for(i=0; i<USER_FUNC_NUM; i++)
	{
		UserFuncs[i].bFree = TRUE;
		UserFuncs[i].nVarsInFunc = 0;
		UserFuncs[i].Rpn = NULL;
	}

	Opers[0].cOperName = '+';
	Opers[0].nPriority = ADD_PRIOR;
	Opers[0].nOperType = BIN_OPER;
	Opers[1].cOperName = '-';
	Opers[1].nPriority = ADD_PRIOR;
	Opers[1].nOperType = BIN_OPER;
	Opers[2].cOperName = '*';
	Opers[2].nPriority = MUL_PRIOR;
	Opers[2].nOperType = BIN_OPER;
	Opers[3].cOperName = '/';
	Opers[3].nPriority = MUL_PRIOR;
	Opers[3].nOperType = BIN_OPER;
	Opers[4].cOperName = '^';
	Opers[4].nPriority = POW_PRIOR;
	Opers[4].nOperType = BIN_OPER;
	// tilda nam predstavlja unarni minus (tilda zato da se razlikuje od obiènog operatora oduzimanja)
	Opers[5].cOperName = '~';
	Opers[5].nPriority = UNARY_MINUS_PRIOR;
	Opers[5].nOperType = UNARY_OPER;

	strcpy(Funcs[0].strFuncName, "exp");
	Funcs[0].nPriority = FUNC_PRIOR;
	Funcs[0].nParNum = 1;
	strcpy(Funcs[1].strFuncName, "ln");
	Funcs[1].nPriority = FUNC_PRIOR;
	Funcs[1].nParNum = 1;
	strcpy(Funcs[2].strFuncName, "log");
	Funcs[2].nPriority = FUNC_PRIOR;
	Funcs[2].nParNum = 1;

	strcpy(Funcs[3].strFuncName, "sin");
	Funcs[3].nPriority = FUNC_PRIOR;
	Funcs[3].nParNum = 1;
	strcpy(Funcs[4].strFuncName, "cos");
	Funcs[4].nPriority = FUNC_PRIOR;
	Funcs[4].nParNum = 1;
	strcpy(Funcs[5].strFuncName, "tg");
	Funcs[5].nPriority = FUNC_PRIOR;
	Funcs[5].nParNum = 1;
	strcpy(Funcs[6].strFuncName, "ctg");
	Funcs[6].nPriority = FUNC_PRIOR;
	Funcs[6].nParNum = 1;
	
	strcpy(Funcs[7].strFuncName, "arcsin");
	Funcs[7].nPriority = FUNC_PRIOR;
	Funcs[7].nParNum = 1;
	strcpy(Funcs[8].strFuncName, "arccos");
	Funcs[8].nPriority = FUNC_PRIOR;
	Funcs[8].nParNum = 1;
	strcpy(Funcs[9].strFuncName, "arctg");
	Funcs[9].nPriority = FUNC_PRIOR;
	Funcs[9].nParNum = 1;
	strcpy(Funcs[10].strFuncName, "arcctg");
	Funcs[10].nPriority = FUNC_PRIOR;
	Funcs[10].nParNum = 1;

	strcpy(Funcs[11].strFuncName, "sh");
	Funcs[11].nPriority = FUNC_PRIOR;
	Funcs[11].nParNum = 1;
	strcpy(Funcs[12].strFuncName, "ch");
	Funcs[12].nPriority = FUNC_PRIOR;
	Funcs[12].nParNum = 1;
	strcpy(Funcs[13].strFuncName, "th");
	Funcs[13].nPriority = FUNC_PRIOR;
	Funcs[13].nParNum = 1;
	strcpy(Funcs[14].strFuncName, "cth");
	Funcs[14].nPriority = FUNC_PRIOR;
	Funcs[14].nParNum = 1;

	strcpy(Funcs[15].strFuncName, "arsh");
	Funcs[15].nPriority = FUNC_PRIOR;
	Funcs[15].nParNum = 1;
	strcpy(Funcs[16].strFuncName, "arch");
	Funcs[16].nPriority = FUNC_PRIOR;
	Funcs[16].nParNum = 1;
	strcpy(Funcs[17].strFuncName, "arth");
	Funcs[17].nPriority = FUNC_PRIOR;
	Funcs[17].nParNum = 1;
	strcpy(Funcs[18].strFuncName, "arcth");
	Funcs[18].nPriority = FUNC_PRIOR;
	Funcs[18].nParNum = 1;

	strcpy(m_szError[0], "Krivi broj greške !!!");
	strcpy(m_szError[1], "Broj otvorenih i zatvorenih zagrada nije u ravnoteži");
	strcpy(m_szError[2], "Numerièka konstanta nije dobro napisana");
	strcpy(m_szError[3], "Nema više mjesta za handle funkcija");
	strcpy(m_szError[4], "Nema više mjesta za handle varijable");
	strcpy(m_szError[5], "Pozvano izraèunavanje funkcije sa handleom za koji ne postoji funkcija");
	strcpy(m_szError[6], "Dva operatora jedan do drugoga");
	strcpy(m_szError[7], "Operator odmah poslije imena funkcije");
	strcpy(m_szError[8], "Nema više memorije !!!");
	strcpy(m_szError[9], "Varijabla odmah poslije konstante");
	strcpy(m_szError[10], "Predznak na krivom mjestu");

}

MathSpace::~MathSpace()
{
}

// Funkcija definira varijablu, i vraca pokazivac na mjesto u memoriji gdje je 
//		alociran prostor za njenu vrijednost
double* MathSpace::DefineVar( char *Name )
{
	int			i;
	double	*ret=NULL;

	//	najprije treba provjeriti da li vec postoji varijabla s tim imenom
	for( i=0; i<VAR_NUM; i++ )
		if( strcmp(Vars[i].strName, Name) == 0 )
			return &(Vars[i].dValue);

	for( i=0; i<VAR_NUM; i++ )
	{
		if( Vars[i].bFree != FALSE )
		{
			Vars[i].bFree = FALSE;
			Vars[i].dValue = 0;
			strcpy( Vars[i].strName, Name );

			ret = &(Vars[i].dValue);
			break;
		}
	}

	// ako se vrati NULL, znaèi da više nema mjesta za definiranje varijabli
	return ret;
}

// Funkcija takodjer definira varijablu, ali s time da dodatno u
//		'pnVarNum' vraca na kojoj poziciji u nizu definiranih varijabli se
//		nalazi novodefinirana varijabla
double* MathSpace::DefineVar( char *Name, int *pnVarNum )
{
	int		i;
	double	*ret=NULL;

	//	najprije treba provjeriti da li vec postoji varijabla s tim imenom
	for( i=0; i<VAR_NUM; i++ )
		if( strcmp(Vars[i].strName, Name) == 0 )
		{
			*pnVarNum = i;
			return &(Vars[i].dValue);
		}

	for( i=0; i<VAR_NUM; i++ )
	{
		if( Vars[i].bFree != FALSE )
		{
			Vars[i].bFree = FALSE;
			Vars[i].dValue = 0;
			strcpy( Vars[i].strName, Name );

			ret = &(Vars[i].dValue);
			*pnVarNum = i;
			break;
		}
	}
	
	return ret;
}

int	MathSpace::DefineFunction( char *Expr )
{
	int			i, j, k, inc, last, help, nVar, bFind=FALSE, nOperNum, RpnPtr=0, StackPtr=0;
	int			nPar=0, nBalance=0, nLastElem=0;
	char		*str1, str2[50];
	char		Rpn[500], Stack[100];
	int			retHandle, nLen=0;
	double	dHelp;

	// nadji slobodno mjesto za funkciju u nizu definiranih handle-ova
	retHandle = -1;
	for( i=0; i<USER_FUNC_NUM; i++ )
		if( UserFuncs[i].bFree == TRUE )
		{
			UserFuncs[retHandle].nVarsInFunc = 0;
			retHandle = i;
			break;
		}
	
	if( retHandle < 0 )
		return CERR_NO_MORE_FUNCHANDLE;

	memset(Rpn, 0, 500);
	memset(Stack, 0, 100);
	
	i=0;
	while( Expr[i] != 0 )		// sve dok ne dodjemo do kraja stringa
	{
		last = inc = 0;

		if( isalpha(Expr[i]) )
		{
			last = GetString(Expr,i);				// nadji koji je zadnji znak u ovo stringu
			str1 = Expr + i;
			strncpy(str2, str1, last-i );		// i izdvoji od pocetka do kraja
			str2[last-i] = 0;								// postavi terminator stringa
//			printf("\nString konst = %s", str2 );
			inc = last - i;
			
			// sada treba pronaci varijablu s tim imenom
			bFind = FALSE;
			for( j=0; j<VAR_NUM; j++ )
			{
				if( Vars[j].bFree == FALSE )
					if( strcmp(str2, Vars[j].strName) == 0 )
					{
						// ako jest varijabla, prije nje ne smije biti num.konstanta
						if( nLastElem == CONST_ELEM )
							return CERR_CONST_VAR;

						bFind = TRUE;
						Rpn[RpnPtr++] = VAR;		// stavi varijablu u izlazni string
						Rpn[RpnPtr++] = j;
						nBalance++;

						// sada zapiši da je funkcija ovisna o toj varijabli
						int		out=FALSE;
						for( k=0; k<UserFuncs[retHandle].nVarsInFunc; k++ )
							if( UserFuncs[retHandle].anFuncOfVars[k] == j )
								out = TRUE;

						if( out == FALSE )
						{
							// znaèi da nismo veæ unijeli da je funkcija ovisna o toj varijabli
							help = UserFuncs[retHandle].nVarsInFunc++;
							UserFuncs[retHandle].anFuncOfVars[help] = j;
						}
						
						nLastElem = VAR;
						break;
					}
			}
			// ako nismo našli varijablu s tim imenom, znaèi da nije definirana
			// pa æemo pogledati da li možda postoji funkcija s time imenom
			if( bFind != TRUE )
			{
				for( j=0; j<FUNC_NUM; j++ )
					if( strcmp( Funcs[j].strFuncName, str2) == 0 )
					{
						bFind = TRUE;
						
						while( StackPtr > 0 && Stack[StackPtr-1] != '(' && GetPriority(Stack,StackPtr-1) >= Funcs[j].nPriority )
						{
							Rpn[RpnPtr++] = Stack[--StackPtr];	// ovaj æe prebaciti "opisni" dio (FUNC ili OPER)
							Rpn[RpnPtr++] = Stack[--StackPtr];	// a ovo æe prebaciti broj operatora ili funkcije
						}
						// treba skinuti i lijevu zagradu sa steka
						if( Stack[StackPtr-1] == '(' )
							Stack[--StackPtr] = 0;

						Stack[StackPtr++] = j;
						Stack[StackPtr++] = FUNC;

						nLastElem = FUNC;
						break;
					}
				if( bFind != TRUE )
				{
					// ovo znaèi da imamo varijablu u funkciji koja još nije definirana
					// pa æemo je ovdje definirati
					DefineVar(str2, &nVar);

					// a ovdje je ubacujemo u popis varijabli funkcije
					help = UserFuncs[retHandle].nVarsInFunc++;
					UserFuncs[retHandle].anFuncOfVars[help] = nVar;

					Rpn[RpnPtr++] = VAR;		// stavi varijablu u izlazni string
					Rpn[RpnPtr++] = nVar;
					nBalance++;

					nLastElem = VAR;
				}
				else
					bFind = FALSE;
			}
			else
				bFind = FALSE;
		}

		// ako je slijedeæi znak znamenka, znaèi da imamo numerièku konstantu
		if( isdigit(Expr[i]) )
		{
			last = GetNumConst(Expr,i);
			if( last < 1 )
				return	CERR_NUMCONST;

			str1 = Expr + i;
			strncpy(str2, str1, last-i );
			str2[last-i] = 0;
//			printf("\nNum konst = %s", str2 );
			inc = last - i;

			dHelp = atof(str2);
			Rpn[RpnPtr++] = CONST_ELEM;
			
			// konstantu æemo staviti na stek
			memmove(Rpn+RpnPtr, &dHelp, sizeof(double) );
			RpnPtr += sizeof(double);

			nLastElem = CONST_ELEM;
			nBalance++;
		}
		
//	ovdje treba ubaciti slucaj kada se radi o unarnom minusu
//	tj., kada je prvi znak -, ili kada se - nalazi poslije otvorene zagrade
		if( Expr[i] == '-' && (i==0 || Expr[i-1]=='(') )
		{
			Expr[i] = '~';
		}

		// sada provjeravamo da li je slijedeæi znak možda operator
		if( (nOperNum = IsOper(Expr[i])) >= 0 )
		{
			// provjeriti æemo šte je bilo u prethodnom znaku
			if( nLastElem == OPER )			// znaèi da je i prethodni znak bio operator, š	to je greška
				return CERR_TWO_OPER;
			if( nLastElem == FUNC )
				return CERR_FUNC_OPER;

			while( StackPtr > 0 && Stack[StackPtr-1] != '(' && GetPriority(Stack,StackPtr-1) >= Opers[nOperNum].nPriority )
			{
				// za svaki binarni operator što se skine, smanjiti nBalance za 1
				if( Stack[StackPtr-1] == OPER && Opers[Stack[StackPtr-2]].nOperType == BIN_OPER )
					nBalance--;

				Rpn[RpnPtr++] = Stack[--StackPtr];	// ovaj æe prebaciti "opisni" dio (FUNC ili OPER)
				Rpn[RpnPtr++] = Stack[--StackPtr];	// a ovo æe prebaciti broj operatora ili funkcije
			}

			Stack[StackPtr++] = nOperNum;
			Stack[StackPtr++] = OPER;
			inc = 1;

			nLastElem = OPER;
		}

		if( Expr[i] == '(' )
		{
			Stack[StackPtr++] = '(';

			nPar++;
			inc = 1;
			nLastElem = LEFT_BRACKET;
		}

		if( Expr[i] == ')' )
		{
			StackPtr--;
			while( Stack[StackPtr] != '(' && StackPtr >= 0 )
			{
				if( Stack[StackPtr] == OPER )
					nBalance--;

				Rpn[RpnPtr++] = Stack[StackPtr--];	// ovaj æe prebaciti "opisni" dio (FUNC ili OPER)
				Rpn[RpnPtr++] = Stack[StackPtr--];	// a ovo æe prebaciti broj operatora ili funkcije
			}
			if( StackPtr < 0 )
				StackPtr = 0;

			nPar--;
			inc = 1;
			nLastElem = RIGHT_BRACKET;
		}

		i += inc;
	}
	if( nPar != 0 )
	{
		printf("\n Broj otvorenih i zatvorenih zagrada nije jednak !!!");

		return CERR_PARNUM;;
	}

	while( StackPtr > 0 )
	{
		// za svaki binarni operator što se skine, smanjiti nBalance za 1
		if( Stack[StackPtr-1] == OPER && Opers[Stack[StackPtr-2]].nOperType == BIN_OPER )
			nBalance--;

		Rpn[RpnPtr++] = Stack[--StackPtr];	// ovaj æe prebaciti "opisni" dio (FUNC ili OPER)
		Rpn[RpnPtr++] = Stack[--StackPtr];	// a ovo æe prebaciti broj operatora ili funkcije
	}

	nLen = RpnPtr + 5;

	if( nBalance != 1 )
	{
		printf("\n Broj argumenata operatora i broj operatora nisu u ravnoteži !!!");

		return CERR_BALANCE;
	}
	

/*	i=0;
	puts(" ");
	while( i<RpnPtr )
	{
		switch( Rpn[i] )
		{
		case FUNC :
			i++;
			printf("\n Funkcija %s", Funcs[Rpn[i]].strFuncName );
			i++;
			break;
		case OPER:
			i++;
		printf("\n Operator %c ", Opers[Rpn[i]].cOperName );
			i++;
			break;
		case VAR:
			i++;
			printf("\n Varijabla %s ", Vars[Rpn[i]].strName );
			i++;
			break;
		case CONST_ELEM:
			i++;
			memmove(&dHelp, Rpn+i, sizeof(double) );
			i+=sizeof(double);
			printf("\n Konstanta %e ", dHelp );
			break;
		}
	}*/

	// sada, na kraju, kada je sigurno sve u redu, možemo zauzeti handle
	UserFuncs[retHandle].bFree = FALSE;
	
	UserFuncs[retHandle].Rpn = new char[nLen];
	memcpy( UserFuncs[retHandle].Rpn, Rpn, nLen );

	return retHandle;
}

int	GetString( char *str, int start )
{
	while( isalnum(str[start]) )
		start++;
	return start;
}

int GetNumConst( char *str, int start )
{
	int		i, bExp=FALSE, bPoint=FALSE, bSign=FALSE;
	
	i=start;
	while( isdigit(str[i]) || (str[i]=='.' && bPoint==FALSE)
					|| (tolower(str[i])=='e' && bExp==FALSE) 
					|| (str[i]=='-' && bExp==TRUE && bSign==FALSE) )
	{
		if( str[i] == '.' )
			bPoint = TRUE;
		if( tolower(str[i]) == 'e' )
			bExp = TRUE;
		if( str[i] == '-' )
		{
			if( tolower(str[i-1]) == 'e' )		// ako je to predznak eksponenta, onda prije toga mora biti 'e'
				bSign = TRUE;
			else
				return -1;			// a ako nije 'e', znaèi da je konstanta krivo napisana
		}
		i++;
	}
	if( str[i] == '.' )		// znaèi druga toèka u istoj konstanti
		return -1;					// znaèi, greška

	return i;
}

int MathSpace::IsOper( char ch )
{
	int	i, ret=-1;

	for( i=0; i<OPER_NUM; i++ )
		if( Opers[i].cOperName == ch )
			ret = i;

	return ret;
}

OPER_TYPE	MathSpace::OperType( char ch )
{
	int					i;
	OPER_TYPE		ret=UNKNOWN_OPER;

	for( i=0; i<OPER_NUM; i++ )
		if( Opers[i].cOperName == ch )
			ret = Opers[i].nOperType;

	if( ret == UNKNOWN_OPER )
	{
		fprintf(stdout, " %c  nepoznati operator u OperType !!!", ch);
	}

	return ret;
}

int	MathSpace::GetPriority( char *Stack, int nPos )
{
	if( Stack[nPos] == FUNC )
		return Funcs[Stack[nPos-1]].nPriority;
	else if( Stack[nPos] == OPER )
		return Opers[Stack[nPos-1]].nPriority;
	else
	{
		printf("\n Na steku se naslo nesto sto nije funkcija ili operator !!!");
		assert(0);
		return 0;
	}
}

void MathSpace::PrintRpn( void )
{

}


void MathSpace::UndefineAllFunctions()
{
	int		i;

	for(i=0; i<USER_FUNC_NUM; i++)
	{
		if( UserFuncs[i].bFree == FALSE && UserFuncs[i].Rpn != NULL )
			delete []UserFuncs[i].Rpn;
	
		UserFuncs[i].bFree = TRUE;
		UserFuncs[i].nVarsInFunc = 0;
		UserFuncs[i].Rpn = NULL;
	}
}

void	MathSpace::UndefineFunction( int hFunc )
{
	if( hFunc >= 0 && hFunc < USER_FUNC_NUM )
	{
		if( UserFuncs[hFunc].bFree == FALSE && UserFuncs[hFunc].Rpn != NULL )
			delete []UserFuncs[hFunc].Rpn;

		UserFuncs[hFunc].bFree = TRUE;
		UserFuncs[hFunc].nVarsInFunc = 0;
		UserFuncs[hFunc].Rpn = NULL;
	}
}

void	MathSpace::UndefineAllVars()
{
	for( int i=0; i<VAR_NUM; i++ )
	{
		Vars[i].bFree = TRUE;
		memset( Vars[i].strName, 0, MAX_VAR_NAME_LEN );
	}
}

double MathSpace::Evaluate( int FuncHandle )
{
	int			i, StackPtr = 0;
	char		*Rpn;
	double	Stack[100], dHelp=0, ret, x, y;

	if( UserFuncs[FuncHandle].bFree != FALSE )
	{
		fprintf(stdout, "Pozvano je izracunavanje funkcije sa nevazecim handle-om !!!");
		assert(0);
	}
	
	Rpn = UserFuncs[FuncHandle].Rpn;

	i=0;
	while( Rpn[i] != 0 )
	{
		if( Rpn[i] == VAR )
		{
			Stack[StackPtr++] = Vars[Rpn[i+1]].dValue;
			i+=2;
		}
		else if( Rpn[i] == CONST_ELEM )
		{
			i++;
			memmove(&dHelp, Rpn+i, sizeof(double) );
			Stack[StackPtr++] = dHelp;
			i += (sizeof(double) );
		}
		else if( Rpn[i] == OPER )
		{
			i++;
			y = Stack[StackPtr-1];
			x = Stack[StackPtr-2];
			switch( Rpn[i] )
			{
			case 0 :
				dHelp = x + y;
				Stack[StackPtr-2] = dHelp;
				StackPtr--;
				break;
			case 1 :
				dHelp = x - y;
				Stack[StackPtr-2] = dHelp;
				StackPtr--;
				break;
			case 2 :
				dHelp = x * y;
				Stack[StackPtr-2] = dHelp;
				StackPtr--;
				break;
			case 3 :
				if( y == 0.0 )
					throw MathLibException(DIV_ZERO);
				else
				{
					dHelp = x / y;
					Stack[StackPtr-2] = dHelp;
					StackPtr--;
				}
				break;
			case 4 :
				dHelp = pow( x , y );
				Stack[StackPtr-2] = dHelp;
				StackPtr--;
				break;
			case 5:
				Stack[StackPtr-1] *= -1;
				break;
			}
			i++;
		}
		else if( Rpn[i] == FUNC )
		{
			i++;
			x = Stack[StackPtr-1];
			switch( Rpn[i] )
			{
			case FUNC_EXP:
				Stack[StackPtr-1] = exp(x);
				break;
			case FUNC_LN:
				Stack[StackPtr-1] = log(x);
				break;
			case FUNC_LOG:
				Stack[StackPtr-1] = log10(x);
				break;
			case FUNC_SIN:
				Stack[StackPtr-1] = sin(x);
				break;
			case FUNC_COS:
				Stack[StackPtr-1] = cos(x);
				break;
			case FUNC_TG:
				if( cos (x) == 0.0 )
				{
					fprintf(stdout, "Tg(90) = nije definiran !!");
					assert(0);
				}
				else
					Stack[StackPtr-1] = sin(x) / cos(x);
				break;
			case FUNC_CTG:
				if( sin (x) == 0.0 )
				{
					fprintf(stdout, "Ctg(0) = nije definiran !!");
					assert(0);
				}
				else
					Stack[StackPtr-1] = cos(x) / sin(x);
				break;
			case FUNC_ARCSIN:
				Stack[StackPtr-1] = asin(x);
				break;
			case FUNC_ARCCOS:
				Stack[StackPtr-1] = acos(x);
				break;
			case FUNC_ARCTG:
				Stack[StackPtr-1] = atan(x);
				break;
			case FUNC_ARCCTG:
				Stack[StackPtr-1] = 1 / atan(x);
				break;
			case FUNC_SH:
				Stack[StackPtr-1] = sinh(x);
				break;
			case FUNC_CH:
				Stack[StackPtr-1] = cosh(x);
				break;
			case FUNC_TH:
				Stack[StackPtr-1] = tanh(x);
				break;
			case FUNC_CTH:
				Stack[StackPtr-1] = 1 / tanh(x);
				break;
			case FUNC_ARSH:
				Stack[StackPtr-1] = arsh(x);
				break;
			case FUNC_ARCH:
				if( x >= 1.0 )
					Stack[StackPtr-1] = arch(x);
				else
				{
					fprintf(stdout, "Nije definiran !!");
				}
				break;
			case FUNC_ARTH:
				if( x > -1.0 && x < 1.0 )
					Stack[StackPtr-1] = arth(x);
				else
				{
				}
				break;
			case FUNC_ARCTH:
				if( x >= -1.0 && x <= 1.0 )
				{
				}
				else
					Stack[StackPtr-1] = arcth(x);
				break;
			}
			i++;
		}
		else
		{
			printf("\n E nesto nije u redu !!!");
			fprintf(stdout, "Greska kod izracunavanja !!!");
			assert(0);
		}
	}
	ret = Stack[0];

	return ret;
}

int MathSpace::GetNumOfVars( void )
{
	int		num = 0;

	for( int i=0; i<VAR_NUM; i++ )
		if( Vars[i].bFree == FALSE )
			num++;
	
	return num;
}

int	MathSpace::GetVarNum( const char *szVarName )
{
	for( int i=0; i<VAR_NUM; i++ )
		if( strcmp( szVarName, Vars[i].strName ) == 0 )
			return	i;

	return -1;
}

double*	MathSpace::GetVar( char *szVarName )
{
	for( int i=0; i<VAR_NUM; i++ )
		if( strcmp( szVarName, Vars[i].strName ) == 0 )
			return	&(Vars[i].dValue);

	return NULL;
}

double	*MathSpace::GetVar( int nVarNum )
{
	if( nVarNum < VAR_NUM && nVarNum >= 0 )
		if( Vars[nVarNum].bFree == FALSE )
			return &(Vars[nVarNum].dValue);
	
	return NULL;
}

const char *MathSpace::GetVarName( int i )
{
	if( i >= 0 && i < VAR_NUM )
		return Vars[i].strName;
	else
		return NULL;
}

const char *MathSpace::GetVarName( double *pdVar )
{
	for( int i=0; i<VAR_NUM; i++ )
		if( &(Vars[i].dValue) == pdVar )
			return Vars[i].strName;
	
	return NULL;
}

char*	MathSpace::ErrorText( int nErr )
{
	if( nErr < 0 && nErr >= -CERR_ERROR_NUM )
		return	m_szError[-nErr];
	else
		return  m_szError[0];
}

int MathSpace::FuncMinN( Vector StartPoint3D, int nDim, int hFunc, Vector *Min )
{
	int			bLower, IterNum=0;
	double	FuncVal, TempFuncVal, sum;
	int			i, j, h, l, ret=-1;
	Vector	*Simplex, *Xc, *Xr, *Xe, *Xk;
	double	*pdInit;
	
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
	for( i=0; i<nDim; i++ )
	{
		pdInit[i] = 1.0;
		Simplex[i+1].InitValues(pdInit);
		pdInit[i] = 0.0;
	}

	do
	{
		// odredjivanje tocke u kojoj je funkcija najveca
		h=0;
/*		for( i=0; i<nDim; i++ )
		{
			nVarNum = UserFuncs[hFunc].anFuncOfVars[i];
			Vars[nVarNum].dValue = Simplex[h].Get(i);
		}*/
		FuncVal = FuncValInVector(hFunc, &Simplex[h]);

		for( i=1; i<nDim+1; i++ )
			if( (TempFuncVal=FuncValInVector(hFunc, &Simplex[i])) > FuncVal )
			{
				h=i;
				FuncVal = TempFuncVal;
			}

		// odredjivanje tocke u kojoj je funkcija najmanja
		l=0;
		FuncVal = FuncValInVector(hFunc, &Simplex[l]);
		for( i=1; i<nDim+1; i++ )
			if( ( TempFuncVal=FuncValInVector(hFunc, &Simplex[i]) ) < FuncVal )
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

		if( FuncValInVector(hFunc, Xr) < FuncValInVector(hFunc, &Simplex[l]) )
		{
			// ekspanzija
			*Xe = (1 - GAMA) * (*Xc) + GAMA * (*Xr);
			if( FuncValInVector(hFunc, Xe) < FuncValInVector(hFunc, &Simplex[l]) )
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
					if( FuncValInVector(hFunc, Xr) < FuncValInVector(hFunc, &Simplex[j]) )
						bLower = 0;
			
			if( bLower == 1 )
			{
				if( FuncValInVector(hFunc, Xr) < FuncValInVector(hFunc, &Simplex[h]) )
				{
					Simplex[h] = *Xr;
				}
				// kontrakcija
				*Xk = (1 - BETA) * (*Xc) + BETA * Simplex[h];
				if( FuncValInVector(hFunc, Xk) > FuncValInVector(hFunc, &Simplex[h]) )
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
			sum += pow( ( FuncValInVector(hFunc, &Simplex[j]) - FuncValInVector(hFunc, Xc)), 2 );

		sum /= nDim;
		sum = sqrt(sum);
		IterNum++;
	} while( sum > EPS && IterNum < SIMPLEX_MAX_ITER_NUM );

	if( IterNum < SIMPLEX_MAX_ITER_NUM )
		ret = 0;

	*Min = *Xc;

//	delete pdInit;

	delete []Simplex;
	delete Xc;
	delete Xe;
	delete Xr;
	delete Xk;

	return ret;
}

//	Izracunava vrijednost funkcije dane sa 'hFunc' u tocki danoj sa
//	'Vert'
double MathSpace::FuncValInVector(int hFunc, Vector *Vert )
{
	int		nVarNum;
	int		i;
	double	FuncVal;

	// prekopiraj vrijednosti iz Vector u varijable u kojima ovisi funkcija	
	for( i=0; i<Vert->GetDim(); i++ )
	{
		nVarNum = UserFuncs[hFunc].anFuncOfVars[i];
		Vars[nVarNum].dValue = Vert->Get(i);
	}
	// i izraèunaj vrijednost funkcije za te varijable
	FuncVal = Evaluate(hFunc);

	return FuncVal;
}

void MathSpace::DiffSysIntegrate( struct DiffEqSystem *DES, double t1, double t2, int nSteps,
																 double *x, double **y )
{
	int			i, k, n;
	double	t, h;

	static double a2=0.2, a3=0.3, a4=0.6, a5=1, a6=0.875, 
		b21=0.2, b31=3.0/40.0, b32=9.0/40.0, b41=0.3, b42=-0.9, b43=1.2,
		b51=-11.0/54.0, b52=2.5, b53=-70.0/27.0, b54=35.0/27.0,
		b61=1631.0/55296.0, b62=175.0/512.0, b63=575.0/13824.0,
		b64=44275.0/110592.0, b65=253.0/4096.0, 
		c1=37.0/378.0, c3=250.0/621.0, c4=125.0/594.0, c6=512.0/1771.0,
		dc5=-277.00/14336.0;
	
	double	dc1=c1-2825.0/27648.0, dc3=c3-18575.0/48384.0,
					dc4=c4-13525.0/55296.0, dc6=c6-0.25;

	double	*ak2, *ak3, *ak4, *ak5, *ak6, *ytemp, *yout, *yvar, *dydx;

	n = DES->m_nDim;

	ak2 = new double[n];
	ak3 = new double[n];
	ak4 = new double[n];
	ak5 = new double[n];
	ak6 = new double[n];

	ytemp = new double[n];
	yout  = new double[n];
	yvar  = new double[n];
	dydx  = new double[n];

	h = (t2-t1)/nSteps;
	x[0] = t1;
	t	= t1;

	//	postavi varijable na pocetne vrijednosti
	*(DES->m_pdTime) = t1;
	for( i=0; i<n; i++ )
		y[i][0] = yvar[i] = *(DES->m_pdVars[i]) = DES->m_adStartValues[i];

	for( k=0; k<nSteps; k++ )
	{
		for( i=0; i<n; i++ )
			dydx[i] = Evaluate(DES->m_anFuncHandle[i]);
		for( i=0; i<n; i++ )
			ytemp[i] = yvar[i] + b21*h*dydx[i];

//	2. korak
		// najprije odredjivanje derivacije
		*(DES->m_pdTime) = t+a2*h;
		for( i=0; i<n; i++ )
			*(DES->m_pdVars[i]) = ytemp[i];
		for( i=0; i<n; i++ )
			ak2[i] = Evaluate(DES->m_anFuncHandle[i]);
		// a zatim racunanje koraka
		for( i=0; i<n; i++ )
			ytemp[i] = yvar[i] + h*(b31*dydx[i] + b32*ak2[i]);

//	3. korak
		*(DES->m_pdTime) = t+a3*h;
		for( i=0; i<n; i++ )
			*(DES->m_pdVars[i]) = ytemp[i];
		for( i=0; i<n; i++ )
			ak3[i] = Evaluate(DES->m_anFuncHandle[i]);

		for( i=0; i<n; i++ )
			ytemp[i] = yvar[i] + h*(b41*dydx[i] + b42*ak2[i] + b43*ak3[i]);

//	4. korak
		*(DES->m_pdTime) = t+a4*h;
		for( i=0; i<n; i++ )
			*(DES->m_pdVars[i]) = ytemp[i];
		for( i=0; i<n; i++ )
			ak4[i] = Evaluate(DES->m_anFuncHandle[i]);

		for( i=0; i<n; i++ )
			ytemp[i] = yvar[i] + h*(b51*dydx[i] + b52*ak2[i] + b53*ak3[i] + b54*ak4[i]);

//	5. korak
		*(DES->m_pdTime) = t+a5*h;
		for( i=0; i<n; i++ )
			*(DES->m_pdVars[i]) = ytemp[i];
		for( i=0; i<n; i++ )
			ak5[i] = Evaluate(DES->m_anFuncHandle[i]);

		for( i=0; i<n; i++ )
			ytemp[i] = yvar[i] + h*(b61*dydx[i] + b62*ak2[i] + b63*ak3[i] + b64*ak4[i] + b65*ak5[i]);

//	6. korak
		*(DES->m_pdTime) = t+a6*h;
		for( i=0; i<n; i++ )
			*(DES->m_pdVars[i]) = ytemp[i];
		for( i=0; i<n; i++ )
			ak6[i] = Evaluate(DES->m_anFuncHandle[i]);

		for( i=0; i<n; i++ )
			yout[i] = yvar[i] + h*(c1*dydx[i] + c3*ak3[i] + c4*ak4[i] + c6*ak6[i]);

//		for( i=0; i<n; i++ )
//			yerr[i] = h*(dc1*dydx[i] + dc3*ak3[i] + dc4*ak4[i] + dc5*ak5[i] + dc6*ak6[i]);


		if( (double)(*(DES->m_pdTime) + h) == *(DES->m_pdTime) )
		{
			fprintf(stderr, "Stepsize to small !!!");
			assert(0);
		}

		t += h;
		*(DES->m_pdTime) += h;
		x[k+1] = *(DES->m_pdTime);
		
		
		for( i=0; i<n; i++ )
		{
			y[i][k+1] = yvar[i] = yout[i];
		
			// yvar[i] su zavrsne vrijednosti varijabli y[i] iz prethodnog intervala
			// i sada ih koristimo za pocetak slijedeceg intervala
			*(DES->m_pdVars[i]) = yvar[i];
		}
	}
	delete []ak2;
	delete []ak3;
	delete []ak4;
	delete []ak5;
	delete []ak6;

	delete []ytemp;
	delete []yout;
	delete []yvar;
	delete []dydx;
}


/*************************************************************************************/
/*************************************************************************************/
/*************************************************************************************/

DiffEqvSystem::DiffEqvSystem()
{
	m_nN = 0;
	m_nP = 0;

	for( int i=0; i<DES_MAXDIM; i++ )
	{
		m_lpszEq[i] = NULL;
		m_lpszStartVal[i] = NULL;
		m_lpszParamVal[i] = NULL;

		m_anFuncHandle[i] = -1;

		m_pdVar[i] = NULL;
		m_pdParam[i] = NULL;
	}

	strcpy( m_szErrorDES[0], "Krivi broj greške !!!");
	strcpy( m_szErrorDES[1], "U jednadžbi mora postojati znak = !!!");
	strcpy( m_szErrorDES[2], "Ne mogu definirati varijablu !!!");
	strcpy( m_szErrorDES[3], "Greška kod postavljanja poèetno uvjeta !!!");
	strcpy( m_szErrorDES[4], "Greška kod postavljanja vrijednosti parametra !!!");
	strcpy( m_szErrorDES[5], "Pogrešno ime varijable kod zadavanja poèetnog uvjeta !!!");
	strcpy( m_szErrorDES[6], "Pogrešno ime varijable kod zadavanja parametra !!!");
	strcpy( m_szErrorDES[7], "Nema više memorije !!!");
	strcpy( m_szErrorDES[8], "Korak postupka premali !!!");
	strcpy( m_szErrorDES[9], "Previše koraka integracije !!!");
	strcpy( m_szErrorDES[10], "Nije zadan dovoljan broj poèetnih uvjeta !!!");
	strcpy( m_szErrorDES[11], "Greška u zadavanju intervala integracije !!!");
	strcpy( m_szErrorDES[12], "Nije definirana vrijednost svih parametara !!!");
	strcpy( m_szErrorDES[13], "Nije postignuta konvergencija kod raèunanja Lyapunovljevih eksponenata !!!");
	strcpy( m_szErrorDES[14], "Pogrešno ime varijable (nije niti sistemska var. niti parametar) !!!");
}

DiffEqvSystem::~DiffEqvSystem()
{
}

int	DiffEqvSystem::InputDES( char *lpszEquations, char *lpszIndepVar, char *lpszStartVal, char *lpszParam )
{
	int		ret=0;

	ret = ParseDES(lpszEquations, lpszIndepVar, lpszStartVal, lpszParam);
	if( ret >= 0 )
	{
		ret = GetData();
		return ret;
	}
	else
		return	ret;
}

int	DiffEqvSystem::ParseDES( char *lpszEquations, char *lpszIndepVar, char *lpszStartVal, char *lpszParam )
{
	int		i, j, k;
	char	sTemp[500];

	strcpy(m_szIndepVar, lpszIndepVar);

	//	ovdje cemo pokupiti dif. jednadzbe kako su unesene
	i = j = k =0;
	while( lpszEquations[i] != 0 )
	{
		sTemp[k++] = lpszEquations[i++];
		if( lpszEquations[i] == 13 || lpszEquations[i] == '\n' || lpszEquations[i] == 0 )
		{
			sTemp[k] = 0;		// terminator za kraj stringa
			// sada æemo j-tu jednadžbu prebaciti u èlanski string
			m_lpszEq[j] = new char[k+1];
			strcpy( m_lpszEq[j], sTemp );

			k = 0;
			j++;		// slijedeæa jednadžba
//			if( lpszEquations[i] != 0 )
//				i++;
			while( lpszEquations[i] != 0 && isalpha(lpszEquations[i]) == 0 )
				i++;
		}
	}
	m_nN = j;
	
	//  a ovdje cemo skupiti pocetne vrijednosti
	i = j = k =0;
	while( lpszStartVal[i] != 0 )
	{
		sTemp[k++] = lpszStartVal[i++];
		if( lpszStartVal[i] == 13 || lpszStartVal[i] == '\n' || lpszStartVal[i] == 0 )
		{
			sTemp[k] = 0;
			// sada æemo j-tu poèetnu vrijednost prebaciti u èlanski string
			m_lpszStartVal[j] = new char[k+1];
			strcpy( m_lpszStartVal[j], sTemp );

			k = 0;
			j++;
			if( lpszStartVal[i] != 0 )
				i++;			// da preskocimo ENTER
			while( lpszStartVal[i] != 0 && isalpha(lpszStartVal[i]) == 0 )
				i++;
		}
	}
	if( j < m_nN )
	{
		// treba izvesti èišæenje prije povratka
		for( i=0; i<m_nN; i++ )
		{
			delete m_lpszEq[i];
			m_lpszEq[i] = NULL;
		}
		for( i=0; i<j; i++ )
		{
			delete m_lpszStartVal[i];
			m_lpszStartVal[i] = NULL;
		}

		return DESERR_NOT_ENOUGH_START_COND;
	}

	//  i još moramo skupiti parametre
	i = j = k =0;
	while( lpszParam[i] != 0 )
	{
		sTemp[k++] = lpszParam[i++];
		if( lpszParam[i] == 13 || lpszParam[i] == '\n' || lpszParam[i] == 0 )
		{
			sTemp[k] = 0;
			// sada æemo j-tu poèetnu vrijednost prebaciti u èlanski string
			m_lpszParamVal[j] = new char[k+1];
			strcpy( m_lpszParamVal[j], sTemp );

			k = 0;
			j++;
			if( lpszParam[i] != 0 )
				i++;
			while( lpszParam[i] != 0 && isalpha(lpszParam[i]) == 0 )
				i++;
		}
	}
	m_nP = j;

	return	TRUE;
}

int	DiffEqvSystem::GetData( void )
{
	int			i, j, k, ret=1;
	char		sTemp[100];
	char		*lpszTemp[DES_MAXDIM];		// ovi ce pokazivati gdje pocinje funkcija (znaci pokazuju na znak nakon znaka = )
	double	*dTmp;

	UndefineAllFunctions();			// najprije æemo oèistiti Math objekt
	UndefineAllVars();
	
	for( i=0; i<m_nN; i++ )
	{
		//	u str cemo prebaciti sve znakove do znaka =
		//	to ce nam biti ime varijable
		j = k = 0;
		while( m_lpszEq[i][j] != '=' && m_lpszEq[i][j] != 0 )
			sTemp[k++] = m_lpszEq[i][j++];
		sTemp[k++] = 0;

		// zapamti gdje pocinje funkcija jer ce nam trebati kasnije
		lpszTemp[i] = m_lpszEq[i] + k;

		if( m_lpszEq[i][j] == 0 )		// znaci da operacija nije uspjela, tj. da nema znaka =
			return DESERR_NO_EQ_CHAR;
		else
		{
			m_pdVar[i] = DefineVar(sTemp);
			
			if( m_pdVar[i] == NULL )
				return DESERR_VAR_DEF_UNABLE;		// nismo uspjeli definirati varijablu

			// A sada æemo odmah ubaciti i pripadnu funkciju
			// prekopiraj tijelo funkcije u sTemp
			strcpy(sTemp, lpszTemp[i]);
			if( ( m_anFuncHandle[i] = DefineFunction(sTemp) ) < 0 )
				return m_anFuncHandle[i];	// nismo uspjeli definirati funkciju pa æemo vratiti dobijeni broj greške
		}
	}

	// Ovdje postavljamo poèetne uvjete
	for( i=0; i<m_nN; i++ )
	{
		j = k = 0;
		while( m_lpszStartVal[i][j] != '=' && m_lpszStartVal[i][j] != 0 )
			sTemp[k++] = m_lpszStartVal[i][j++];
		sTemp[k++] = 0;

		// zapamti gdje pocinje dio stringa koji sadrži num.konstantu
		lpszTemp[i] = m_lpszStartVal[i] + k;

		if( m_lpszStartVal[i][j] == 0 )		// znaci da operacija nije uspjela, tj. da nema znaka =
			return DESERR_SYN_ERR_START_VAL;
		else
		{
			dTmp = GetVar(sTemp);			// najprije trebamo pokazivaè na 'dValue'
			if( dTmp == NULL )
				return	DESERR_START_VAR_NAME_WRONG;
			else if ( IsSystemVar(sTemp) == 0 )
			{
				// moguæe je da zadamo poèetni uvjet s imenom varijable koja nije sistemska
				return	DESERR_START_VAR_NAME_WRONG;
			}
			else
			{
				strcpy(sTemp, lpszTemp[i]);
				// u sTemp nam je sada numerièka vrijednost varijable
				// i trebamo naæi koja je to po redu varijabla u našem sistemu
				// odnosno, kojom je ona jednadžbom zadana

				for( i=0; i<DES_MAXDIM; i++ )
					if( dTmp == m_pdVar[i] )
					{
						m_adStartValues[i] = atof(sTemp);
						break;
					}
			}
		}
	}

	// a ovdje postavljamo vrijednosti parametara
	for( i=0; i<m_nP; i++ )
	{
		j = k = 0;
		while( m_lpszParamVal[i][j] != '=' && m_lpszParamVal[i][j] != 0 )
			sTemp[k++] = m_lpszParamVal[i][j++];
		sTemp[k++] = 0;

		// zapamti gdje pocinje dio stringa koji sadrži num.konstantu
		lpszTemp[i] = m_lpszParamVal[i] + k;

		if( m_lpszParamVal[i][j] == 0 )		// znaci da operacija nije uspjela, tj. da nema znaka =
			return DESERR_SYN_ERR_PAR_VAL;
		else
		{
			dTmp = GetVar(sTemp);			// najprije trebamo pokazivaè na 'dValue'
			if( dTmp == NULL || IsSystemVar(sTemp)==1 )
				return	DESERR_PAR_VAR_NAME_WRONG;
			else
			{
				m_pdParam[i] = dTmp;

				strcpy(sTemp, lpszTemp[i]);
				*dTmp = m_adParValues[i] = atof(sTemp);
			}
		}
	}

	if( GetNumOfVars() < m_nN + m_nP )
	{
		// znaèi da nismo definirali dovoljno parametara
		return DESERR_NOT_ENOUGH_PAR_VAL;
	}
	m_pdTime = DefineVar(m_szIndepVar);			// definiramo nezavisnu varijablu

	return TRUE;
}



int	DiffEqvSystem::FillVarValuesFromString( const char *szEq, Vector &VarVal )
{
	int			i, j, k;
	char		lpszEq[DES_MAXDIM][30];
	char		*lpszTemp[DES_MAXDIM];		// ovi ce pokazivati gdje pocinje funkcija (znaci pokazuju na znak nakon znaka = )
	char		sTemp[100];

	i = j = k =0;
	while( szEq[i] != 0 )
	{
		sTemp[k++] = szEq[i++];
		if( szEq[i] == 13 || szEq[i] == '\n' || szEq[i] == 0 )
		{
			sTemp[k] = 0;
			// sada æemo j-tu poèetnu vrijednost prebaciti u string
			strcpy( lpszEq[j], sTemp );

			k = 0;
			j++;
			if( szEq[i] != 0 )
				i++;			// da preskocimo ENTER
			while( szEq[i] != 0 && isalpha(szEq[i]) == 0 )
				i++;
		}
	}
	if( j < m_nN )
		return DESERR_NOT_ENOUGH_START_COND;

	// Ovdje postavljamo poèetne uvjete
	for( i=0; i<m_nN; i++ )
	{
		j = k = 0;
		while( lpszEq[i][j] != '=' && lpszEq[i][j] != 0 )
			sTemp[k++] = lpszEq[i][j++];
		sTemp[k++] = 0;

		// zapamti gdje pocinje dio stringa koji sadrži num.konstantu
		lpszTemp[i] = lpszEq[i] + k;

		if( lpszEq[i][j] == 0 )		// znaci da operacija nije uspjela, tj. da nema znaka =
			return DESERR_SYN_ERR_START_VAL;
		else
		{
			if( IsSystemVar(sTemp) == 0 )
			{
				// moguæe je da zadamo poèetni uvjet s imenom varijable koja nije sistemska
				return	DESERR_START_VAR_NAME_WRONG;
			}
			else
			{
				j = GetOrderOfSysVar(sTemp);

				strcpy(sTemp, lpszTemp[i]);
				// u sTemp nam je sada numerièki iznos varijable
				// i trebamo naæi koja je to po redu varijabla u našem sistemu
				// odnosno, kojom je ona jednadžbom zadana
				VarVal[j] = atof(sTemp);
			}
		}
	}
	return 0;
}

int	DiffEqvSystem::FillParValuesFromString( const char *szEq, Vector &ParVal )
{
	int			i, j, k;
	char		lpszEq[DES_MAXDIM][30];
	char		*lpszTemp[DES_MAXDIM];		// ovi ce pokazivati gdje pocinje funkcija (znaci pokazuju na znak nakon znaka = )
	char		sTemp[100];

	i = j = k =0;
	while( szEq[i] != 0 )
	{
		sTemp[k++] = szEq[i++];
		if( szEq[i] == 13 || szEq[i] == '\n' || szEq[i] == 0 )
		{
			sTemp[k] = 0;
			// sada æemo j-tu poèetnu vrijednost prebaciti u string
			strcpy( lpszEq[j], sTemp );

			k = 0;
			j++;
			if( szEq[i] != 0 )
				i++;			// da preskocimo ENTER
			while( szEq[i] != 0 && isalpha(szEq[i]) == 0 )
				i++;
		}
	}
	if( j < m_nP )
		return DESERR_NOT_ENOUGH_PAR_VAL;

	for( i=0; i<m_nP; i++ )
	{
		j = k = 0;
		while( lpszEq[i][j] != '=' && lpszEq[i][j] != 0 )
			sTemp[k++] = lpszEq[i][j++];
		sTemp[k++] = 0;

		// zapamti gdje pocinje dio stringa koji sadrži num.konstantu
		lpszTemp[i] = lpszEq[i] + k;

		if( lpszEq[i][j] == 0 )		// znaci da operacija nije uspjela, tj. da nema znaka =
			return DESERR_SYN_ERR_PAR_VAL;
		else
		{
			if( IsParameterVar(sTemp) == 0 )
			{
				return	DESERR_PAR_VAR_NAME_WRONG;
			}
			else
			{
				j = GetOrderOfParVar(sTemp);

				strcpy(sTemp, lpszTemp[i]);
				// u sTemp nam je sada numerièki iznos varijable
				// i trebamo naæi koja je to po redu varijabla u našem sistemu
				// odnosno, kojom je ona jednadžbom zadana
				ParVal[j] = atof(sTemp);
			}
		}
	}
	return 0;
}

int	DiffEqvSystem::FillMixedValuesFromString( const char *szEq, Vector &SysVal, Vector &ParVal )
{
	int			i, j, k, num;
	char		lpszEq[DES_MAXDIM+DES_MAXPAR][30];
	char		*lpszTemp[DES_MAXDIM+DES_MAXPAR];		// ovi ce pokazivati gdje pocinje funkcija (znaci pokazuju na znak nakon znaka = )
	char		sTemp[100];

	i = j = k =0;
	while( szEq[i] != 0 )
	{
		sTemp[k++] = szEq[i++];
		if( szEq[i] == 13 || szEq[i] == '\n' || szEq[i] == 0 )
		{
			sTemp[k] = 0;
			// sada æemo ime j-te varijable prebaciti u string
			strcpy( lpszEq[j], sTemp );

			k = 0;
			j++;
			if( szEq[i] != 0 )
				i++;			// da preskocimo ENTER
			while( szEq[i] != 0 && isalpha(szEq[i]) == 0 )
				i++;
		}
	}
	num = j;		// da znamo koliko varijabli pridružujemo

	for( i=0; i<num; i++ )
	{
		j = k = 0;
		while( lpszEq[i][j] != '=' && lpszEq[i][j] != 0 )
			sTemp[k++] = lpszEq[i][j++];
		sTemp[k++] = 0;

		// zapamti gdje pocinje dio stringa koji sadrži num.konstantu
		lpszTemp[i] = lpszEq[i] + k;

		if( lpszEq[i][j] == 0 )		// znaci da operacija nije uspjela, tj. da nema znaka =
			return DESERR_SYN_ERR_PAR_VAL;
		else
		{
			if( IsParameterVar(sTemp) )
			{
				j = GetOrderOfParVar(sTemp);

				strcpy(sTemp, lpszTemp[i]);
				// u sTemp nam je sada numerièki iznos varijable
				ParVal[j] = atof(sTemp);
			}
			else if( IsSystemVar(sTemp) )
			{
				j = GetOrderOfSysVar(sTemp);
				
				strcpy(sTemp, lpszTemp[i]);
				SysVal[j] = atof(sTemp);
			}
			else
			{
				return	DESERR_VAR_NAME_WRONG;
			}
		}
	}
	return 0;
}

double	DiffEqvSystem::GetStartValue( int i )
{
	if( i > 0 && i < GetDim() )
		return m_adStartValues[i];
	else
		return 0;
}

double	DiffEqvSystem::GetParamValue( int i )
{
	if( i>0 && i<GetParNum() )
		return m_adParValues[i];
	else
		return 0;
}

int		DiffEqvSystem::SetInitialCond( Vector &vecInitCond )
{
	if( vecInitCond.GetDim() == m_nN )
	{
		for( int i=0; i<m_nN; i++ )
			m_adStartValues[i] = vecInitCond[i];
		
		return 0;
	}
	else
		return -1;
}

int		DiffEqvSystem::SetParamValues( Vector &vecParamVal )
{
	if( vecParamVal.GetDim() == m_nP )
	{
		for( int i=0; i<m_nP; i++ )
			m_adParValues[i] = vecParamVal[i];
		
		return 0;
	}
	else
		return -1;
}

const char *DiffEqvSystem::GetSysVarName( int i )
{
	return GetVarName(m_pdVar[i]);
}

const char *DiffEqvSystem::GetParVarName( int i )
{
	return GetVarName(m_pdParam[i]);
}

// funkcija vraæa true ukoliko se radi o varijabli sustava
int	DiffEqvSystem::IsSystemVar( char *szVarName )
{
	int			i, ret=0;
	double	*pd;

	pd = GetVar(szVarName);		// pokazivaè na varijablu
	
	// a sada æemo provjeriti da li u našem sustavu ima sistemska varijabla
	// s takvim pokazivaèem
	if( pd != NULL )
		for( i=0; i<DES_MAXDIM; i++ )
			if( m_pdVar[i] == pd )
				ret = 1;							// ako ima, onda se radi o varijabli sustava

	return ret;
}

int	DiffEqvSystem::IsSystemVar( int nVarNum )
{
	int			i, ret=0;
	double	*pd;

	if( nVarNum < VAR_NUM && nVarNum >= 0 )
	{
		pd = &(Vars[nVarNum].dValue);		// pokazivaè na varijablu
	
		// a sada æemo provjeriti da li u našem sustavu ima sistemska varijabla
		// s takvim pokazivaèem
		if( pd != NULL )
			for( i=0; i<DES_MAXDIM; i++ )
				if( m_pdVar[i] == pd )
					ret = 1;							// ako ima, onda se radi o varijabli sustava
	}

	return ret;
}

// funkcija vraæa true ukoliko se radi o parametru sustava
int	DiffEqvSystem::IsParameterVar( char *szVarName )
{
	int			i, ret=0;
	double	*pd;

	pd = GetVar(szVarName);		// pokazivaè na varijablu
	
	// a sada æemo provjeriti da li u našem sustavu ima sistemska varijabla
	// s takvim pokazivaèem
	if( pd != NULL )
		for( i=0; i<DES_MAXDIM; i++ )
			if( m_pdParam[i] == pd )
				ret = 1;							// ako ima, onda se radi o varijabli sustava

	return ret;
}

int	DiffEqvSystem::IsParameterVar( int nVarNum )
{
	int			i, ret=0;
	double	*pd;

	if( nVarNum < VAR_NUM && nVarNum >= 0 )
	{
		pd = &(Vars[nVarNum].dValue);		// pokazivaè na varijablu
		
		// a sada æemo provjeriti da li u našem sustavu ima sistemska varijabla
		// s takvim pokazivaèem
		if( pd != NULL )
			for( i=0; i<DES_MAXDIM; i++ )
				if( m_pdParam[i] == pd )
					ret = 1;							// ako ima, onda se radi o varijabli sustava
	}
	return ret;
}

// funkcija vraæa redni broj varijable u nizu varijabli sustava
//	parametar je ime varijable
//	ukoliko dana varijabla nije varijabla sustava, vraæa -1
int	DiffEqvSystem::GetOrderOfSysVar( char *szVarName )
{
	int			i;
	double	*pd;

	pd = GetVar(szVarName);

	if( pd != NULL )
		for( i=0; i<m_nN; i++ )
			if( m_pdVar[i] == pd )
				return i;

	return -1;
}

// funkcija vraæa redni broj varijable u nizu varijabli sustava
//	parametar je redni broj varijable u nizu svih definiranih varijabli
//	ukoliko dana varijabla nije varijabla sustava, vraæa -1
int	DiffEqvSystem::GetOrderOfSysVar( int nVarNum )
{
	int			i;
	double	*pd;

	pd = GetVar(nVarNum);

	if( pd != NULL )
		for( i=0; i<m_nN; i++ )
			if( m_pdVar[i] == pd )
				return i;

	return -1;
}

// funkcija vraæa redni broj varijable parametra u nizu varijabli sustava
//	parametar funkcije je ime varijable parametra
//	ukoliko dani parametar ne postoji u sustavu, vraæa -1
int	DiffEqvSystem::GetOrderOfParVar( char *szParName )
{
	int			i;
	double	*pd;

	pd = GetVar(szParName);

	if( pd != NULL )
		for( i=0; i<m_nP; i++ )
			if( m_pdParam[i] == pd )
				return i;

	return -1;
}

int	DiffEqvSystem::GetOrderOfParVar( int nParNum )
{
	int			i;
	double	*pd;

	pd = GetVar(nParNum);

	if( pd != NULL )
		for( i=0; i<m_nP; i++ )
			if( m_pdParam[i] == pd )
				return i;

	return -1;
}

char*	DiffEqvSystem::ErrorText( int nErrorNum )
{
	if( nErrorNum > -100 )		// znaèi da je to greška iz bazne klase
		return MathSpace::ErrorText(nErrorNum);
	else
		return m_szErrorDES[-nErrorNum-100];

}

//	Stepper rutina za RK-metodu
//  odredjuje korak postupka
int DiffEqvSystem::RKStep( Vector &y, Vector &dydx, double *x, double htry, 
					 double eps, Vector &yscal, double *hdid, double *hnext )
{
	int		i, ret=0;
	double	errmax, h, htemp, xnew;
	Vector	*yerr, *ytemp;

	yerr = new Vector(m_nN);
	ytemp = new Vector(m_nN);
	h = htry;

	for(;;)
	{
		RKAlg(y,dydx,*x,h,*ytemp,*yerr);
		errmax = 0.0;
		for( i=0; i<m_nN; i++ )
			errmax = DMAX(errmax, fabs((*yerr)[i]/yscal[i]));
		errmax /= eps;

		if( errmax <= 1.0 )
			break;

		htemp = SAFETY * h * pow(errmax,PSHRNK);
		h = (h >= 0.0 ? DMAX(htemp, 0.1*h) : DMIN(htemp, 0.1*h));

		xnew = (*x) + h;
		if( xnew == *x )
		{
//			fprintf(stdout, "\n stepsize underflow in rkqs!!!");
			ret = 1;
//			assert(0);
		}
	}
	if( errmax > ERRCON )
		*hnext = SAFETY * h * pow(errmax, PGROW);
	else
		*hnext = 5.0 * h;

	*x += (*hdid=h);
	for( i=0; i<m_nN; i++ )
		y[i] = (*ytemp)[i];

	delete	yerr;
	delete	ytemp;

	return ret;
}

//	Algoritam rutina na RK-metodu
//	implementira osnovnu formulu metodu i pocinjuci od zavisnih varijabli y[i] u x
//	racuna y u x+h
void DiffEqvSystem::RKAlg( Vector  &y, Vector &dydx, double x, double h, Vector &yout, Vector &yerr )
/*
	Dobivsi vrijednosti za n varijabli y[1..n] i njihovih derivacija 
	dydx[1..n] poznatih u x, koristi cash-Karpovu RK metodu da napreduje 
	s rijesenjem preko intervala h i vraca varijable u yout[1..n]
	Takodjer vraca lokalnu gresku zaokruzivanja u yerr
*/
{
	int		i;
	static double a2=0.2, a3=0.3, a4=0.6, a5=1, a6=0.875, 
		b21=0.2, b31=3.0/40.0, b32=9.0/40.0, b41=0.3, b42=-0.9, b43=1.2,
		b51=-11.0/54.0, b52=2.5, b53=-70.0/27.0, b54=35.0/27.0,
		b61=1631.0/55296.0, b62=175.0/512.0, b63=575.0/13824.0,
		b64=44275.0/110592.0, b65=253.0/4096.0, 
		c1=37.0/378.0, c3=250.0/621.0, c4=125.0/594.0, c6=512.0/1771.0,
		dc5=-277.00/14336.0;
	double	dc1=c1-2825.0/27648.0, dc3=c3-18575.0/48384.0,
		dc4=c4-13525.0/55296.0, dc6=c6-0.25;
	Vector	*ak2, *ak3, *ak4, *ak5, *ak6, *ytemp;

	ak2 = new Vector( m_nN );
	ak3 = new Vector( m_nN );
	ak4 = new Vector( m_nN );
	ak5 = new Vector( m_nN );
	ak6 = new Vector( m_nN );
	ytemp = new Vector( m_nN );

	for( i=0; i<m_nN; i++ )
		(*ytemp)[i] = y[i] + b21*h*dydx[i];

//	2. korak
	// najprije odredjivanje derivacije
	*m_pdTime = x+a2*h;
	for( i=0; i<m_nN; i++ )
		*m_pdVar[i] = (*ytemp)[i];
	for( i=0; i<m_nN; i++ )
		(*ak2)[i] = Evaluate(m_anFuncHandle[i]);
	// a zatim racunanje koraka
	for( i=0; i<m_nN; i++ )
		(*ytemp)[i] = y[i] + h*(b31*dydx[i] + b32*(*ak2)[i]);

//	3. korak
	*m_pdTime = x+a3*h;
	for( i=0; i<m_nN; i++ )
		*m_pdVar[i] = (*ytemp)[i];
	for( i=0; i<m_nN; i++ )
		(*ak3)[i] = Evaluate(m_anFuncHandle[i]);

	for( i=0; i<m_nN; i++ )
		(*ytemp)[i] = y[i] + h*(b41*dydx[i] + b42*(*ak2)[i] + b43*(*ak3)[i]);

//	4. korak
	*m_pdTime = x+a4*h;
	for( i=0; i<m_nN; i++ )
		*m_pdVar[i] = (*ytemp)[i];
	for( i=0; i<m_nN; i++ )
		(*ak4)[i] = Evaluate(m_anFuncHandle[i]);

	for( i=0; i<m_nN; i++ )
		(*ytemp)[i] = y[i] + h*(b51*dydx[i] + b52*(*ak2)[i] + b53*(*ak3)[i] + b54*(*ak4)[i]);

//	5. korak
	*m_pdTime = x+a5*h;
	for( i=0; i<m_nN; i++ )
		*m_pdVar[i] = (*ytemp)[i];
	for( i=0; i<m_nN; i++ )
		(*ak5)[i] = Evaluate(m_anFuncHandle[i]);

	for( i=0; i<m_nN; i++ )
		(*ytemp)[i] = y[i] + h*(b61*dydx[i] + b62*(*ak2)[i] + b63*(*ak3)[i] + b64*(*ak4)[i] + b65*(*ak5)[i]);

//	6. korak
	*m_pdTime = x+a6*h;
	for( i=0; i<m_nN; i++ )
		*m_pdVar[i] = (*ytemp)[i];
	for( i=0; i<m_nN; i++ )
		(*ak6)[i] = Evaluate(m_anFuncHandle[i]);

	for( i=0; i<m_nN; i++ )
		yout[i] = y[i] + h*(c1*dydx[i] + c3*(*ak3)[i] + c4*(*ak4)[i] + c6*(*ak6)[i]);

	for( i=0; i<m_nN; i++ )
		yerr[i] = h*(dc1*dydx[i] + dc3*(*ak3)[i] + dc4*(*ak4)[i] + dc5*(*ak5)[i] + dc6*(*ak6)[i]);

	delete ak2;
	delete ak3;
	delete ak4;
	delete ak5;
	delete ak6;
	delete ytemp;
}

#define MAXSTP_CALC		200000

int DiffEqvSystem::Integrate( double x1, double x2, double eps, 
						double h1, double hmin, 
						int *nok, int *nbad, int *kount, double dxsav,
						Vector *vecX, Matrix *matY )
//  nok, nbad - broj dobrih i loših poduzetih koraka
//  h1 - poèetna duljina koraka
//	eps - tražena preciznost
//  hmin - minimalna dozvoljena duljina koraka ( može biti 0 )
//  dxsav - razmak izmeðu dvije vrijednosti koje su zapamæene
{
	int			nstp, i, kmax;
	double	xsav, x, hnext, hdid, h;
	Vector	*yscal, *y, *dydx;

	if( x1 == x2 || x2 < x1 )
		return DESERR_INTERVAL_WRONG;

	yscal = new Vector(m_nN);
	dydx	= new Vector(m_nN);
	y			= new Vector(m_nN);

	kmax = vecX->GetDim();
	x=x1;
	h = SIGN(h1, x2-x1);
	*nok = (*nbad) = *kount = 0;
	for( i=0; i<m_nN; i++ )
	{
		(*y)[i] = m_adStartValues[i];
		*m_pdVar[i] = m_adStartValues[i];		// da bi mogli izraèunavati funkcije, potrebno je inicijalizirati vrijednosti
	}
	for( i=0; i<m_nP; i++ )
		*m_pdParam[i] = m_adParValues[i];
	
//	printf("\n y[]    =  %12.5f    %12.5f    %12.5f   %12.5f", (*y)[0], (*y)[1], (*y)[2], (*y)[3] );

	if( kmax > 0 ) 
		xsav = x - dxsav*2.0;

	for( nstp=1; nstp<=MAXSTP_CALC; nstp++ )
	{
		for( i=0; i<m_nN; i++ )
			(*dydx)[i] = Evaluate(m_anFuncHandle[i]);

		for( i=0; i<m_nN; i++ )
		{
			(*yscal)[i] = fabs((*y)[i]) + fabs((*dydx)[i]*h) + TINY;
/*			if( (*yscal)[i] < 1e-15 )
				(*yscal)[i] = 1;*/
		}
//		printf("\n (*yscal)  =  %12.5f    %12.5f    %12.5f   %12.5f", (*yscal)[0], (*yscal)[1], (*yscal)[2], (*yscal)[3] );
		
		if( kmax > 0 && *kount < kmax-1 && fabs(x-xsav) > fabs(dxsav) )
		{
			(*vecX)[(*kount)] = x;
			for( i=0; i<m_nN; i++ )
				(*matY)[i][*kount] = (*y)[i];
			(*kount)++;
			xsav = x;
		}
		if( (x+h-x2)*(x+h-x1) > 0.0 ) 
			h=x2 - x;

		if( RKStep(*y,*dydx,&x,h,eps,*yscal,&hdid,&hnext) != 0 )
		{
			delete	dydx;
			delete	y;
			delete	yscal;

//			fprintf(stdout, "\n Step size too small in odeint!!!");
			return	DESERR_STEPSIZE_SMALL;
		}

		if( hdid == h)
		{
			++(*nok);
//			printf("\n y[]    =  %12.5f    %12.5f    %12.5f   %12.5f", (*y)[0], (*y)[1], (*y)[2], (*y)[3] );
//	printf("\n h      =  %12.8e", h );
//			getch();
		}
		else
			++(*nbad);

		if( (x-x2)*(x2-x1) >= 0.0 )
		{
//			for( i=0; i<m_nN; i++ )
//				m_adStartValues[i] = (*y)[i];
			if( kmax )
			{
				(*vecX)[(*kount)++] = x;
				for( i=0; i<m_nN; i++ )
					(*matY)[i][*kount] = (*y)[i];
			}

			delete	dydx;
			delete	y;
			delete	yscal;

			return TRUE;
		}
		if( fabs(hnext) <= hmin )
		{
			delete	dydx;
			delete	y;
			delete	yscal;

//			fprintf(stdout, "\n Step size too small in odeint!!!");
			return	DESERR_STEPSIZE_SMALL;
		}
		h = hnext;
	}
	delete	dydx;
	delete	y;
	delete	yscal;

//	fprintf(stdout, "\nToo many steps in odeint!!!");
	return	DESERR_TO_MANY_STEPS;
}


void	DiffEqvSystem::CalcJacobian( double t, Vector &vecX, Vector &vecPar, Matrix *Jacobian )
{
	int			i, j, nRel=0;
	double	h, temp;
	Vector	*f=NULL, *vecFuncVal;//, *vecX;

	f = new Vector(m_nN);
	vecFuncVal = new Vector(m_nN);
	SystemFunc(0, vecX, vecPar, vecFuncVal);

	for( j=0; j<m_nN; j++ )
	{
/*		printf("\nX na pocetku petlje!!");
		Print(*vecX);*/
		temp = vecX[j];
		h = 1e-5 * fabs(temp);
		if( h == 0.0 )
			h = 1e-5;

		vecX[j] = temp + h;
		h = vecX[j] - temp;

		SystemFunc(0, vecX, vecPar, f);
		vecX[j] = temp;

		for( i=0; i<vecFuncVal->GetDim(); i++ )
			Jacobian->SetElem(i,j, ((*f)[i] - (*vecFuncVal)[i]) / h );
	}

	delete vecFuncVal;
	delete f;
}

void DiffEqvSystem::CalcParameterJacobian( double t, Vector &X, Vector &P, Matrix *matParJac )
{
	// Raèuna jakobijan sustava s obzirom na vektor parametara
	//	X - toèka u kojoj se raèuna
	//	P - vrijednost parametara za koje se raèuna
	//	Jacobian - pointer na matricu u koju æe se smjestiti rezultat
	int			i, j;
	double	h, temp;
	Vector	*f1, *f2;

	f1 = new Vector(m_nN);		//	ovdje æemo smjestiti vrijednosti u x1
	f2 = new Vector(m_nN);		//  a ovdje u x2
	
	SystemFunc(t,X,P,f1);		// raèunamo vrijednost funkcije u x1

	for( j=0; j<m_nP; j++ )
	{
		temp = P[j];
		h = 1e-5 * fabs(temp);		// ovdje odredjujemo najmanji h uz koji
		if( h == 0.0 )						// æemo zadržati toènost numerièki odreðene
			h = 1e-5;								// derivacije

		P[j] = temp + h;					// sada raèunamo koordinate toèke x2
		h = P[j] - temp;

		SystemFunc(t,X,P,f2);		// raèunamo vrijednost funkcije u x2
		P[j] = temp;

		for( i=0; i<m_nN; i++ )
			(*matParJac)[i][j] = ((*f2)[i] - (*f1)[i]) / h;
	}
	
	delete f1;
	delete f2;
}

void	DiffEqvSystem::CalcEigenvalues( double t, Vector &X, Vector &P, Vector *EigenReal, Vector *EigenImag )
{
	Matrix		*mJac, *mHess;

	mJac = new Matrix(m_nN, m_nN);
	mHess = new Matrix(m_nN, m_nN);
	
	CalcJacobian(t,X,P,mJac);

	(*mHess) = ReductionToHess(*mJac);
	HessEigenvalues(*mHess, EigenReal, EigenImag);

	mJac->~Matrix();
	mHess->~Matrix();
}

void	DiffEqvSystem::CalcEigenvalues( double t, Vector &X, Vector &P, ComplexVector *Eigen  )
{
	Vector	EReal(m_nN), EImag(m_nN);

	CalcEigenvalues(t, X, P, &EReal, &EImag);

	ComplexVector	compl(EReal, EImag );
	*Eigen = compl;
}


void DiffEqvSystem::FindJacobian( int nDim, Vector &vecX, Vector *vecFuncVal, Matrix *Jacobian, 
			void (DiffEqvSystem::*pfVecFunc)(double, Vector&, Vector*) )
{
	int			i, j;
	double	h, temp;
	Vector	*f=NULL;

	f = new Vector(nDim);

	// ukoliko nemamo izraèunate derivacije u jednoj toèci, moramo ih izraèunati 
	if( vecFuncVal == NULL )
		(this->*pfVecFunc)(0,vecX, vecFuncVal);

	for( j=0; j<nDim; j++ )
	{
		temp = vecX[j];
		h = 1e-5 * fabs(temp);
		if( h == 0.0 )
			h = 1e-5;

		vecX[j] = temp + h;
		h = vecX[j] - temp;

		(this->*pfVecFunc)(0,vecX,f);
		vecX[j] = temp;

		for( i=0; i<vecFuncVal->GetDim(); i++ )
			Jacobian->SetElem(i,j, ((*f)[i] - (*vecFuncVal)[i]) / h );
	}

	delete f;
}
							
#define MAXITS_NEWT		250
#define	TOLF_NEWT			1.0E-5
#define	TOLMIN_NEWT		1.0E-6
#define	TOLX_NEWT			1.0E-9
#define	STPMX_NEWT			100.0

#define	ALF_LNSRCH		1.0e-4

void DiffEqvSystem::LineSearch( int n, Vector &xold, double fold, Vector &g, Vector &p, Vector &x, 
						double *f, double stpmax, int *check, void (DiffEqvSystem::*vecfunc)(double, Vector&, Vector*) )
{
	int			i;
	double	a, alam, alam2, alamin, b, disc, f2, fold2, rhs1, rhs2, slope;
	double	sum, temp, test, tmplam;
	Vector	*fvec;

	fvec = new Vector(n);

	*check = 0;
	for( sum=0.0, i=1; i<=n; i++ )
		sum += p[i-1] * p[i-1];
	sum = sqrt(sum);
	if( sum > stpmax )
		for( i=1; i<=n; i++ )
			p[i-1] *= stpmax / sum;

	for( slope=0.0, i=1; i<=n; i++ )
		slope += g[i-1] * p[i-1];

	test = 0.0;

	for( i=1; i<=n; i++ )
	{
		temp = fabs(p[i-1]) / DMAX(fabs(xold[i-1]),1.0);
		if( temp > test )
			test = temp;
	}
	alamin = TOLX_NEWT / test;
	alam = 1.0;
	for( ; ; )
	{
		for( i=1; i<=n; i++ )
			x[i-1] = xold[i-1] + alam*p[i-1];

//		*f = fmin(x);
		(this->*vecfunc)(0,x,fvec);
		for( sum=0.0, i=0; i<n; i++ )
			sum += DSQR((*fvec)[i]);

		*f = 0.5*sum;

		if( alam < alamin )
		{
			for( i=1; i<=n; i++ )
				x[i-1] = xold[i-1];
			*check = 1;
			return;
		}
		else if( *f <= fold+ALF_LNSRCH*alam*slope) return;
		else
		{
			if( alam == 1.0 )
				tmplam = -slope / (2.0*(*f - fold - slope));
			else
			{
				rhs1 = *f - fold - alam*slope;
				rhs2 = f2 - fold - alam2*slope;

				a = (rhs1/(alam*alam) - rhs2/(alam2*alam2))/(alam - alam2);
				b = (-alam2*rhs1 / (alam*alam) + alam*rhs2/(alam2*alam2))/(alam - alam2);

				if( a == 0.0 )
					tmplam = -slope/(2.0*b);
				else
				{
					disc = b*b - 3.0*a*slope;
					if( disc < 0.0 )
					{
						printf("Roundoff problem in lnsrch !!!");
						*check = 1;
						return;
//						assert(0);
					}
					else
						tmplam = (-b+sqrt(disc)) / (3.0*a);
				}
				if( tmplam > 0.5*alam)
					tmplam = 0.5*alam;
			}
		}
		alam2 = alam;
		f2 = *f;
		fold2 = fold;
		alam = DMAX(tmplam, 0.1*alam);
	}

	delete fvec;
}

int DiffEqvSystem::Newton( Vector &x, int n, int *check, void (DiffEqvSystem::*vecfunc)(double, Vector&, Vector*) )
{
	int			i, its, j, *indx;
	double	d, den, f, fold, stpmax, sum, temp, test;
	Matrix	*fjac=NULL;
	Vector	*g=NULL, *p=NULL, *xold=NULL, *fvec=NULL;

	indx = new int[n];
	fjac = new Matrix(n,n);
	g = new Vector(n);
	p = new Vector(n);
	xold = new Vector(n);
	fvec = new Vector(n);

//	f = fmin(x);
	(this->*vecfunc)(0,x,fvec);
	for( sum=0.0, i=0; i<n; i++ )
		sum += DSQR((*fvec)[i]);

	f = 0.5*sum;

	test = 0.0;
	for( i=1; i<=n; i++ )
		if( fabs((*fvec)[i-1]) > test )
			test = fabs((*fvec)[i-1]);

	if( test< 0.01*TOLF_NEWT )
	{
		*check = 0;
		delete fvec; delete xold; delete p; delete g;
		delete fjac; delete []indx; return 0;
	}
	for( sum=0.0, i=1; i<=n; i++ )
		sum += DSQR(x[i-1]);
	stpmax = STPMX_NEWT * DMAX(sqrt(sum),(double)n);

	for( its=1; its<=MAXITS_NEWT; its++ )
	{
		FindJacobian(n,x,fvec,fjac,vecfunc);
		
		for( i=1; i<=n; i++ )
		{
			for( sum=0.0, j=1; j<=n; j++ )
				sum += (*fjac)[j-1][i-1] * (*fvec)[j-1];
			(*g)[i-1] = sum;
		}
		for( i=1; i<=n; i++ )
			(*xold)[i-1] = x[i-1];
		fold = f;
		for( i=1; i<=n; i++ )
			(*p)[i-1] = -(*fvec)[i-1];
		
		LUDecomp(*fjac,indx,&d);
		LUBackSubst(*fjac,indx,*p);
		LineSearch(n,*xold,fold,*g,*p,x,&f,stpmax,check,vecfunc);

		(this->*vecfunc)(0, x, fvec );

		test = 0.0;
		for( i=1; i<=n; i++ )
			if( fabs((*fvec)[i-1]) > test )
				test = fabs((*fvec)[i-1]);

		if( test < TOLF_NEWT )
		{
			*check = 0;
			delete fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; return 0;
		}
		if( *check )
		{
			test = 0.0;
			den = DMAX(f, 0.5*n);
			for( i=1; i<=n; i++ )
			{
				temp = fabs((*g)[i-1]) * DMAX(fabs(x[i-1]), 1.0) / den;
				if( temp > test )
					test = temp;
			}
			*check = (test < TOLMIN_NEWT ? 1 : 0 );
			delete fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; return 1;
		}
		test = 0.0;
		for( i=1; i<=n; i++ )
		{
			temp = (fabs(x[i-1] - (*xold)[i-1])) / DMAX(fabs(x[i-1]), 1.0);
			if( temp > test )
				test = temp;
		}
		if( test < TOLX_NEWT )
		{
			*check = 1;
			delete fvec; delete xold; delete p; delete g;
			delete fjac; delete []indx; return 1;
		}
	}
	*check = 1;
	printf("MAXITS exceeded in newt ");
	delete fvec; delete xold; delete p; delete g;
	delete fjac; delete []indx;
	return 2;
}


void	DiffEqvSystem::SystemFunc( double x, Vector &X, Vector &Par, Vector *dydx )
{
	int		i;	

	*m_pdTime = x;							// postavi vrijednost nezavisne varijable
	for( i=0; i<m_nN; i++ )			// postavi vrijednost ostalih varijabli
		*m_pdVar[i] = X[i];
	
	for( i=0; i<m_nP; i++ )			// postavi vrijednost parametara
		*m_pdParam[i] = Par[i];

	for( i=0; i<m_nN; i++ )			// i sada izraèunaj
		(*dydx)[i] = Evaluate(m_anFuncHandle[i]);
}

int		DiffEqvSystem::CalcLyapExp( Vector &vecInitCond, Vector &vecPar, Vector &vecLyapExp, double T,
				double hmin, double eps, int nIterNumMax, double Er, double Ea)
{
	// - raèuna sve Lyapunovljeve eksponente zadanog sustava, za vrijednost poèetnih uvjeta zadanih
	//   sa vecInitCond i za vrijednosti parametara vecPar
	Matrix		*matU, *matX, *matV;
	Vector		*vecSum, *vecLambda, *vecLambdaOld, *vecStartVal;
	int				nDim, i, j, k, l, nNumOK, nNumBad, nKount, nIterNum, ret=0;
	double		h, t1, t2, scalProd, norm, *pdXp, **pdYp;

	nDim = GetDim();

	pdXp = (double *) calloc(20, sizeof(double) );
	pdYp = (double **) calloc(nDim+nDim*nDim, sizeof(double *) );
	for( i=0; i<nDim+nDim*nDim; i++ )
		pdYp[i] = (double *) calloc(20, sizeof(double) );

	matU			= new Matrix(nDim, nDim);
	matX			= new Matrix(nDim, nDim);
	matV			= new Matrix(nDim, nDim);
	vecSum		= new Vector(nDim);
	vecLambda = new Vector(nDim);
	vecLambdaOld = new Vector(nDim);
	// u vecStartVal prvi 'nDim' su poèetne vrijednosti za jednadžbe sistema
	// a ostalih nDim*nDim za poè. vrijed. varijacijske jednadžbe
	vecStartVal	 = new Vector(nDim + nDim*nDim);
	
	matU->MakeUnitMatrix();				// u[][] mora imati ortonormalne stupce
	matX->Clear();
	matV->Clear();
	for( i=0; i<nDim; i++ )
	{
		(*vecSum)[i] = 0;
		(*vecLambda)[i] = 0;
		(*vecStartVal)[i] = vecInitCond[i];	// ovo su poèetne vrijednosti za integraciju jednadžbe sistema
		
		// a ovdje popunjavamo poèetne vrijednosti za integraciju varijacijske jednadžbe
		for( j=0; j<nDim; j++ )
			(*vecStartVal)[nDim + nDim*i + j] = (*matU)[i][j];
	}
	
	nIterNum = 0;
	t1 = 0;
	h = 1e-5;
	do
	{
		nIterNum++;
		if( nIterNumMax == nIterNum )
		{
//			printf("Nije postignuta konvergencija !!!");
			ret = DESERR_CALC_LYAP_NO_CONVERGENCE;
			break;
		}

		*vecLambdaOld = *vecLambda;

		t2 = t1 + T;
/*		Print(*vecStartVal);
		Print(vecPar);*/
		ret = ODEIntegrate( *vecStartVal, &vecPar, nDim+nDim*nDim, t1, t2, eps,
			h, hmin, &nNumOK, &nNumBad, &DiffEqvSystem::VarFunc,
			2, &nKount, pdXp, pdYp, T/2 );
		
		if( ret != 0 )
			break;

		// stavljamo rezultat u matricu X
		for( i=0; i<nDim; i++ )
			for( j=0; j<nDim; j++ )
				(*matX)[i][j] = pdYp[nDim + nDim*i + j][nKount];

		for( i=0; i<nDim; i++ )
		{
			for( j=0; j<nDim; j++ )
				(*matV)[j][i] = (*matX)[j][i];		// v[][i] = x[][i]

			for( j=0; j<i; j++ )
			{
				scalProd = 0;
				for( l=0; l<nDim; l++ )
					scalProd += (*matV)[l][i] * (*matU)[l][j];

				for( k=0; k<nDim; k++ )
					(*matV)[k][i] -= scalProd * (*matU)[k][j];
			}

			norm=0;
			for( j=0; j<nDim; j++ )
				norm += (*matV)[j][i] * (*matV)[j][i];
			norm = sqrt(norm);

			for( j=0; j<nDim; j++ )
				(*matU)[j][i] = (*matV)[j][i] / norm;
			
			(*vecSum)[i] += log(norm);
			(*vecLambda)[i] = (*vecSum)[i] / (nIterNum*T);
		}

		// treba ponovo popuniti poèetne vrijednosti za sljedeæi korak integracije
		for( i=0; i<nDim; i++ )
		{
			(*vecStartVal)[i] = pdYp[i][nKount];	// ovo su poèetne vrijednosti za integraciju jednadžbe sistema
			// a ovdje popunjavamo poèetne vrijednosti za integraciju varijacijske jednadžbe
			for( j=0; j<nDim; j++ )
				(*vecStartVal)[nDim + nDim*i + j] = (*matU)[i][j];
		}
	} while( nIterNum<5 || Norm(*vecLambdaOld - *vecLambda) > Er * Norm(*vecLambda) + Ea );

	vecLyapExp = *vecLambda;

	free(pdXp);
	for(i=0; i<nDim; i++ )
		free(pdYp[i]);
	free(pdYp);

	delete matU;
	delete matV;
	delete matX;
	delete vecSum;
	delete vecLambda;
	delete vecLambdaOld;

	return ret;
}

void	DiffEqvSystem::VarFunc( double t, Vector &X, Vector &P, Vector *dydx )
{
	// najprije treba izraèunati derivacije funkcija sistema
	SystemFunc(t,X,P,dydx);

	int				i, j;
	Matrix		matJac(m_nN,m_nN), matX(m_nN,m_nN), matRez(m_nN,m_nN);
	
	// stavljamo pripadne vrijednosti varijabli varijacijske jednadžbe u matricu
	for( i=0; i<m_nN; i++ )
		for( j=0; j<m_nN; j++ )
			matX[i][j] = X[m_nN + m_nN*i + j];

		// izraèunamo derivacije varijacijske jednadžbe
	CalcJacobian(t,X,P,&matJac);
/*	puts("Jacobijan u VaFunc");
	Print(matJac);

	puts("matX u VaFunc");
	Print(matX);*/

	matRez = matJac * matX;
/*	puts("matRez u VarFunc");
	Print(matRez);*/

	// a onda ih treba prebaciti u niz dydx
	for( i=0; i<m_nN; i++ )
		for( j=0; j<m_nN; j++ )
			(*dydx)[m_nN + m_nN*i + j] = matRez[i][j];
}


int	DiffEqvSystem::LocateBifurcationPoints( Vector *vecStart, Vector *vecParamOperPoint, Vector *vecLeftEigen )
{
	// - raèuna toèke u kojima se pojavljuje saddle-node bifurkacija, ali samo za 
	//   jednoparametarski sustav
	int			i, ret=-1, check;
	Vector	*val;

	if( m_nP == 1 )
	{
		val = new Vector(2*m_nN+1);

		for( i=0; i<2*m_nN+1; i++ )
			(*val)[i] = (*vecStart)[i];

		ret = Newton(*val, 2*m_nN+1, &check, &DiffEqvSystem::BifFunc );

 		if( ret == 0 )
		{
			for( i=0; i<2*m_nN+1; i++ )
				(*vecStart)[i] = (*val)[i];
			(*vecStart)[m_nN] = (*val)[2*m_nN];

			if( vecLeftEigen != NULL )
				for( i=0; i<m_nN; i++ )
					(*vecLeftEigen)[i] = (*val)[i+m_nN];
		}

		delete val;
	}
	else if( m_nP > 1 )
	{
		if( m_vecStartPoint != NULL )
			delete m_vecStartPoint;

		val = new Vector(2*m_nN+m_nP+1);

		// poèetne vrijednosti za varijable sustava
		for( i=0; i<2*m_nN+m_nP; i++ )
			(*val)[i] = (*vecStart)[i];

		// poèetna vrijednost za koeficijent k
		(*val)[2*m_nN+m_nP] = 1;

		// toèka za koju tražimo najbližu bifurkacijsku toèku
		m_vecStartPoint = new Vector(m_nP);
		if( vecParamOperPoint != NULL )
			*m_vecStartPoint = *vecParamOperPoint;
		else
			// ukoliko nije zadana, onda tražimo bifurkaciju najbližu ishodištu
			m_vecStartPoint->Clear();

		ret = Newton(*val, 2*m_nN+m_nP+1, &check, &DiffEqvSystem::MultiParBifFunc );

		if( ret == 0 )
		{
			for( i=0; i<m_nN; i++ )
				(*vecStart)[i] = (*val)[i];
			for( i=0; i<m_nP; i++ )
				(*vecStart)[m_nN+i] = (*val)[2*m_nN+i];

			if( vecLeftEigen != NULL )
				for( i=0; i<m_nN; i++ )
					(*vecLeftEigen)[i] = (*val)[i+m_nN];
		}
		delete val;
	}
	else
	{
		// sustav ne ovisi o parametrima, pa prema tome nema ni bifurkacije
		ret = -1;
	}

	return ret;

}

void	DiffEqvSystem::BifFunc( double x, Vector &X, Vector *dydx )
{
	Vector	*myX=NULL, *myP=NULL, *myV=NULL, *myRes=NULL;
	Matrix	*matJac=NULL;
	int			i, j;

	myX = new Vector(m_nN);
	myV = new Vector(m_nN);
	myRes = new Vector(m_nN);
	myP = new Vector(m_nP);
	matJac = new Matrix(m_nN,m_nN);

	for( i=0; i<m_nN; i++ )
		(*myX)[i] = X[i];
	for( i=m_nN; i<2*m_nN; i++ )
		(*myV)[i-m_nN] = X[i];
	for( i=2*m_nN; i<2*m_nN+m_nP; i++ )
		(*myP)[i-2*m_nN] = X[i];

	SystemFunc(x, *myX, *myP, dydx);
	CalcJacobian(x, *myX, *myP, matJac);

	for( i=0; i<m_nN; i++ )
	{
		(*dydx)[i+m_nN] = 0;
		for( j=0; j<m_nN; j++ )
			(*dydx)[i+m_nN] += (*matJac)[i][j] * (*myV)[j];
	}
	
	(*dydx)[2*m_nN] = -1;
	for( j=0; j<m_nN; j++ )
		(*dydx)[2*m_nN] += (*myV)[j] * (*myV)[j];

	myX->~Vector();
	myV->~Vector();
	myP->~Vector();
	myRes->~Vector();
	matJac->~Matrix();
}

void	DiffEqvSystem::MultiParBifFunc( double x, Vector &X, Vector *dydx )
{
	// Raèuna jednadžbe sustava za saddle-node bifurkaciju (višeparametarski sluèaj)
	//		X[0..n-1] - varijable sustava
	//		X[n..2n-1] - komponente lijevog svojstvenog vektora
	//		X[2n..2n+p-1] - 'p' komponenata parametara sustava
	//		X[2n+p] - koeficijent k
	int			i, j;
	double	dCoef;

	Vector	myX(m_nN), myP(m_nP), myW(m_nN), myRes(m_nP);
	Matrix	matJac(m_nN, m_nN), matParJac(m_nN, m_nP);

	for( i=0; i<m_nN; i++ )
		myX[i] = X[i];
	for( i=m_nN; i<2*m_nN; i++ )
		myW[i-m_nN] = X[i];
	for( i=2*m_nN; i<2*m_nN+m_nP; i++ )
		myP[i-2*m_nN] = X[i];
	
	dCoef = X[2*m_nN+m_nP];

	SystemFunc(x, myX, myP, dydx);
	CalcJacobian(x, myX, myP, &matJac );
	CalcParameterJacobian(x, myX, myP, &matParJac );

	for( i=0; i<m_nN; i++ )
	{
		(*dydx)[i+m_nN] = 0;
		for( j=0; j<m_nN; j++ )
			(*dydx)[i+m_nN] += myW[j] * matJac[j][i];
	}
	
	for( i=0; i<m_nP; i++ )
	{
		(*dydx)[i+2*m_nN] = dCoef * ( myP[i] - (*m_vecStartPoint)[i] );
		
		// na kraju ovog izraèunavanja myRs sadrži vektor normale na bifurkacijsku hiperplohu
		myRes[i] = 0;
		for( j=0; j<m_nN; j++ )
			myRes[i] += myW[j] * matParJac[j][i];

		(*dydx)[i+2*m_nN] += myRes[i];
	}

//	printf("\nMyRes u MultiBifFunc");
//	Print(myRes);
	
	(*dydx)[2*m_nN+m_nP] = -1;
	for( j=0; j<m_nP; j++ )
		(*dydx)[2*m_nN+m_nP] += myRes[j] * myRes[j];

/*	printf("\ndydx U MultiBifFunc");
	Print(*dydx);
	getch();*/
}

int	DiffEqvSystem::LocateHopfBifurcationPoints( Vector &StartVal, double *omega  )
{
	int			i, ret, check;
	Vector	*val;

	// ovaj 1 je za parametar omega (iznos imaginarne komponente svojstvene vrijednosti
	val = new Vector(3*m_nN + m_nP + 1);

	for( i=0; i<3*m_nN+2; i++ )
		(*val)[i] = StartVal[i];

	ret = Newton(*val, 3*m_nN+m_nP+1, &check, &DiffEqvSystem::HopfBifFunc );

	if( ret == 0 )
	{
		for( i=0; i<3*m_nN+1; i++ )
			StartVal[i] = (*val)[i];
		
		*omega = (*val)[3*m_nN+1];
	}
	delete val;

	return ret;
}

void	DiffEqvSystem::HopfBifFunc( double x, Vector &X, Vector *dydx )
{
	int			i, j;
	double	omega;

	Vector	*myX=NULL, *myP=NULL, *myVReal=NULL, *myVImag=NULL;
	Matrix	*matJac=NULL;

	myX = new Vector(m_nN);
	myVReal = new Vector(m_nN);
	myVImag = new Vector(m_nN);
	myP = new Vector(m_nP);
	matJac = new Matrix(m_nN,m_nN);

	for( i=0; i<m_nN; i++ )
		(*myX)[i] = X[i];
	for( i=m_nN; i<2*m_nN; i++ )
		(*myVReal)[i-m_nN] = X[i];
	for( i=2*m_nN; i<3*m_nN; i++ )
		(*myVImag)[i-2*m_nN] = X[i];
	
	for( i=3*m_nN; i<3*m_nN+m_nP; i++ )
		(*myP)[i-3*m_nN] = X[i];

	omega = X[3*m_nN+m_nP];

	SystemFunc(x, *myX, *myP, dydx);
	CalcJacobian(x, *myX, *myP, matJac);
	// sada se u prvih 'n' èlanova vektora 'dydx' nalaze vrijednosti funkcija sistema u zadanoj toèki
	
	// a sada treba naæi i vrijednosti ostalih funkcija sustava
	for( i=0; i<m_nN; i++ )
	{
		(*dydx)[i+m_nN] = 0;
		// množimo J * Re(v)
		for( j=0; j<m_nN; j++ )
			(*dydx)[i+m_nN] += (*matJac)[i][j] * (*myVReal)[j];
		
		// pošto množimo jediniènu matricu sa vektorom Im(v), isto je kao da samo 
		// zbrojimo odgovarajuæu komponentu tog vektora
		(*dydx)[i+m_nN] += omega * (*myVImag)[i];
	}
	
	for( i=0; i<m_nN; i++ )
	{
		(*dydx)[i+2*m_nN] = 0;
		// množimo J * Im(v)
		for( j=0; j<m_nN; j++ )
			(*dydx)[i+2*m_nN] += (*matJac)[i][j] * (*myVImag)[j];
		
		// pošto množimo jediniènu matricu sa vektorom Im(v), isto je kao da samo 
		// zbrojimo odgovarajuæu komponentu tog vektora
		(*dydx)[i+2*m_nN] -= omega * (*myVReal)[i];
	}

	(*dydx)[3*m_nN] = -1;
	for( j=0; j<m_nN; j++ )
		(*dydx)[3*m_nN] += (*myVReal)[j]*(*myVReal)[j] + (*myVImag)[j]*(*myVImag)[j];
	
	(*dydx)[3*m_nN+1] = 0;
	for( j=0; j<m_nN; j++ )
		(*dydx)[3*m_nN+1] += atan2((*myVImag)[j], (*myVReal)[j]);

	delete myX;
	delete myVReal;
	delete myVImag;
	delete myP;
	delete matJac;
}

int	DiffEqvSystem::CalcEqvSolution( Vector *vecX, Vector &vecP )
{
	int			i, ret, check;
	Vector	*val;

	val = new Vector( m_nN+m_nP );
	for( i=0; i<m_nN; i++ )
		(*val)[i] = (*vecX)[i];
	for( i=0; i<m_nP; i++ )
		(*val)[i+m_nN] = vecP[i];

	ret = Newton(*val, m_nN, &check, &DiffEqvSystem::EqvFunc );
	
	if( ret == 0 )
	{
		for( i=0; i<m_nN; i++ )
			(*vecX)[i] = (*val)[i];
	}
	delete val;

	return ret;
}

void DiffEqvSystem::EqvFunc( double t, Vector &X, Vector *dydx )
{
	Vector	myX(m_nN), myP(m_nP);
	int			i;

	for( i=0; i<m_nN; i++ )
		myX[i] = X[i];
	for( i=0; i<m_nP; i++ )
		myP[i] = X[i+m_nN];

	t = 0;
	SystemFunc(t, myX, myP, dydx );
}


//	Stepper rutina za RK-metodu
//  odredjuje korak postupka
int DiffEqvSystem::RKStep( Vector *y, Vector *vecPar, Vector *dydx, int n, double *x, double htry, 
			double eps, Vector *yscal, double *hdid, double *hnext,
			void (DiffEqvSystem::*pfFunc)(double, Vector &, Vector &, Vector *) )

/*	RK 5-og reda sa pracenjem lokalne greske zaokruzivanja da bi se osigurala tocnost i 
prilagodila duljina koraka.
ULAZ:
	vektor zavisnih varijabli  -  y[1..n]
	i njegove derivacije   -  dydx[1..n],   i to u pocetnoj tocki nezavisne varijable x
	duljina koraka(pocetna za probu)  -  htry
	zahtijevana tocnost	 -  eps
	i vektor sa kojim se skalira greska  -  yscal[1..n]
IZLAZ:
	y i x -  zamijenjeni sa novim vrijednostima
	hdid  -  postignuta duljina koraka
	hnext -  predvidjena duljina koraka u slijedecem koraku
*/
{
	int		i, ret=0;
	double	errmax, h, htemp, xnew;
	Vector	*yerr, *ytemp;

	yerr = new Vector(n);
	ytemp = new Vector(n);

	h = htry;

	for(;;)
	{
		RKAlg(y,vecPar,dydx,n,*x,h,ytemp,yerr,pfFunc);
		errmax = 0.0;
		for( i=0; i<n; i++ )
			errmax = DMAX(errmax, fabs((*yerr)[i]/(*yscal)[i]));
		errmax /= eps;

		if( errmax <= 1.0 )
			break;

		htemp = SAFETY * h * pow(errmax,PSHRNK);
		h = (h >= 0.0 ? DMAX(htemp, 0.1*h) : DMIN(htemp, 0.1*h));

		xnew = (*x) + h;
		if( xnew == *x )
		{
//			fprintf(stderr, "\n stepsize underflow in rkqs!!!");
//			assert(0);
			ret = 1;
		}
	}
	if( errmax > ERRCON )
		*hnext = SAFETY * h * pow(errmax, PGROW);
	else
		*hnext = 5.0 * h;

	*x += (*hdid=h);
//	printf(" x = %30.20e,    h = %30.20e", *x, h );
//	puts(" ");
	for( i=0; i<n; i++ )
		(*y)[i] = (*ytemp)[i];

	delete yerr;
	delete ytemp;

	return ret;
}

//	Algoritam rutina na RK-metodu
//	implementira osnovnu formulu metodu i pocinjuci od zavisnih varijabli y[i] u x
//	racuna y(x+h)
void	DiffEqvSystem::RKAlg( Vector *y, Vector *vecSystParam, Vector *dydx, int n, double x, double h, 
			Vector *yout, Vector *yerr,
			void (DiffEqvSystem::*pfFunc)(double, Vector &, Vector &, Vector *) )
/*
	Dobivsi vrijednosti za n varijabli y[1..n] i njihovih derivacija 
	dydx[1..n] poznatih u x, koristi Cash-Karpovu RK metodu da napreduje 
	s rijesenjem preko intervala h i vraca varijable u yout[1..n]
	Takodjer vraca lokalnu gresku zaokruzivanja u yerr[]
*/
{
	int		i;
	static double a2=0.2, a3=0.3, a4=0.6, a5=1, a6=0.875, 
		b21=0.2, b31=3.0/40.0, b32=9.0/40.0, b41=0.3, b42=-0.9, b43=1.2,
		b51=-11.0/54.0, b52=2.5, b53=-70.0/27.0, b54=35.0/27.0,
		b61=1631.0/55296.0, b62=175.0/512.0, b63=575.0/13824.0,
		b64=44275.0/110592.0, b65=253.0/4096.0, 
		c1=37.0/378.0, c3=250.0/621.0, c4=125.0/594.0, c6=512.0/1771.0,
		dc5=-277.00/14336.0;
	double	dc1=c1-2825.0/27648.0, dc3=c3-18575.0/48384.0,
		dc4=c4-13525.0/55296.0, dc6=c6-0.25;
	Vector	ak2(n), ak3(n), ak4(n), ak5(n), ak6(n), ytemp(n);
 
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + b21*h*(*dydx)[i];

	(this->*pfFunc)(x+a2*h, ytemp, *vecSystParam, &ak2);
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + h*(b31*(*dydx)[i] + b32*ak2[i]);

	(this->*pfFunc)(x+a3*h, ytemp, *vecSystParam, &ak3);
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + h*(b41*(*dydx)[i] + b42*ak2[i] + b43*ak3[i]);

	(this->*pfFunc)(x+a4*h, ytemp, *vecSystParam, &ak4);
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + h*(b51*(*dydx)[i] + b52*ak2[i] + b53*ak3[i] + b54*ak4[i]);

	(this->*pfFunc)(x+a5*h, ytemp, *vecSystParam, &ak5);
	for( i=0; i<n; i++ )
		ytemp[i] = (*y)[i] + h*(b61*(*dydx)[i] + b62*ak2[i] + b63*ak3[i] + b64*ak4[i] + b65*ak5[i]);

	(this->*pfFunc)(x+a6*h, ytemp, *vecSystParam, &ak6);
	for( i=0; i<n; i++ )
		(*yout)[i] = (*y)[i] + h*(c1*(*dydx)[i] + c3*ak3[i] + c4*ak4[i] + c6*ak6[i]);

	for( i=0; i<n; i++ )
		(*yerr)[i] = h*(dc1*(*dydx)[i] + dc3*ak3[i] + dc4*ak4[i] + dc5*ak5[i] + dc6*ak6[i]);

}

int DiffEqvSystem::ODEIntegrate( Vector ystart, Vector *vecPar, int nVar, double x1, double x2,
			double eps, double h1, double hmin, int *nok, int *nbad,
			void (DiffEqvSystem::*pfFunc)(double, Vector &, Vector &, Vector *),
			int kmax, int *kount, double *xp, double **yp, double dxsav )

//	ystart[] - vektor poèetnih vrijednosti
//  nVar  - broj dif. jednadžbi
//  nok, nbad - broj dobrih i loših poduzetih koraka
//  h1 - poèetna duljina koraka
//  hmin - minimalna dozvoljena duljina koraka ( može biti 0 )
//  m_pfSysFunc - korisnikova rutina koja raèuna derivacije u danoj toèki
//  rkqs - stepper rutina
//  kmax - koliko maksimalno izraèunatih vrijednosti y da se zapatmi
//  dxsav - razmak izmeðu dvije vrijednosti koje su zapamæene
//		POVRATNE VRIJEDNOSTI
//  xp[1..kmax] - u kojim toèkama su izraèunate vrijednosti y
//  yp[1..nvar][1..kmax] - vrijednosti varijabli
{
	int		nstp, i;
	double	xsav, x, hnext, hdid, h;
	Vector	*yscal, *y, *dydx;

	yscal = new Vector(nVar);
	y			= new Vector(nVar);
	dydx	= new Vector(nVar);

	x=x1;
	h = SIGN(h1, x2-x1);
	*nok = (*nbad) = 0;
	*kount = -1;
	for( i=0; i<nVar; i++ )
		(*y)[i] = ystart[i];
	if( kmax > 0 ) xsav = x - dxsav*2.0;

	for( nstp=1; nstp<=MAXSTP; nstp++ )
	{
		(this->*pfFunc)(x, *y, *vecPar, dydx);
//		Print(*dydx);
		for( i=0; i<nVar; i++ )
		{
			(*yscal)[i] = fabs((*y)[i]) + fabs((*dydx)[i]*h) + TINY;
			if( (*yscal)[i] == 0 )
				(*yscal)[i] = 1;
		}
		if( kmax > 0 && *kount < kmax-1 && fabs(x-xsav) > fabs(dxsav) )
		{
			xp[++(*kount)] = x;
			for( i=0; i<nVar; i++ )
				yp[i][*kount] = (*y)[i];
			xsav = x;
		}
		if( (x+h-x2)*(x+h-x1) > 0.0 ) 
			h=x2 - x;

		if( RKStep(y,vecPar,dydx,nVar,&x,h,eps,yscal,&hdid,&hnext,pfFunc) != 0 )
		{
			delete yscal;
			delete y;
			delete dydx;

			return DESERR_STEPSIZE_SMALL;
		}

		if( hdid == h)
			++(*nok);
		else
			++(*nbad);

		if( (x-x2)*(x2-x1) >= 0.0 )
		{
			for( i=0; i<nVar; i++ )
				ystart[i] = (*y)[i];
			if( kmax )
			{
				xp[++(*kount)] = x;
				for( i=0; i<nVar; i++ )
					yp[i][*kount] = (*y)[i];
			}

			delete yscal;
			delete y;
			delete dydx;

			return 0;
		}
		if( fabs(hnext) <= hmin )
		{
//			fprintf(stderr, "\n Step size too small in odeint!!!");
//			assert(0);

			delete yscal;
			delete y;
			delete dydx;

			return DESERR_STEPSIZE_SMALL;
		}
		h = hnext;
	}
//	fprintf(stderr, "\nToo many steps in odeint!!!");
//	assert(0);

	delete yscal;
	delete y;
	delete dydx;

	return DESERR_TO_MANY_STEPS;
}
