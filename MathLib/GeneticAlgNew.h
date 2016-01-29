#ifndef __GENETIC_ALG_NEW
#define __GENETIC_ALG_NEW

#pragma warning ( disable : 4786 )

#include <vector>
#include <map>

using std::vector;
using std::map;

// forward declaration
class ResultsPool;

//##ModelId=39571AB10011
class GAErrException {
public:
	//##ModelId=39571AB10025
	GAErrException( int inErrorID ) { ErrorID = inErrorID; }

private:
	//##ModelId=39571AB1001B
	int ErrorID;
};

/***************************************************************************************/
/***************************************************************************************/

//##ModelId=39571AB003B3
class	CromResults {
public:
	//##ModelId=39571AB003C7
	float		CurrFuncVal;
	//##ModelId=39571AB003BE
	float		MaxFuncVal;
	//##ModelId=39571AB003BD
	long		IterNumForMaxVal;
};

//##ModelId=39571AB001BE
class	OptResults {
public:
	//##ModelId=39571AB00204
	float		BestFuncVal;
	//##ModelId=39571AB001FA
	int			BestCromInd;
	//##ModelId=39571AB001F1
	long		IterNum;

	//##ModelId=39571AB001E8
	vector<CromResults>		CRes; // rezultati pojedinog kromosoma
};

/***************************************************************************************/
/*******************				ABSTRACT INTERFACE CLASSES           ***********************/
/***************************************************************************************/

//##ModelId=39571AB00345
class	Cromosom {
public:
	//##ModelId=39571AB00369
	virtual	int			GenerateCromosom() = 0;

	// vraæa trenutnu vrijednost funkcije cilja
	//##ModelId=39571AB00368
	float		getFuncValue() { return Value; }
	
	// vraæa maksimalnu postignutu vrijednost funkcije cilja, i u kojoj je iteraciji postignuta
	//##ModelId=39571AB00365
	void		getMaxValue( float *Max, long *Iter ) { 
						*Max = MaxValue; 
						*Iter = MaxValInIterNum; 
					}

	//##ModelId=39571AB0035A
	void		setFuncValue( float Val ) { Value = Val; }
	//##ModelId=39571AB0035C
	void		setFuncValue( float Val, long IterNum ) { 	
						Value = Val; 
						if( Val > MaxValue ) {
							MaxValue = Val;
							MaxValInIterNum = IterNum;
						} 
					}

protected:
	//##ModelId=39571AB00359
	float		Value;						// vrijednost funkcije cilja za ovaj kromosom
	//##ModelId=39571AB00350
	float		MaxValue;					// max. postignuta vrijednost funkcije cilja za ovu jedinku
	//##ModelId=39571AB0034F
	long		MaxValInIterNum;	// u kojoj iteraciji je postignuta ta max. vrijednost
};

//##ModelId=39571AAF01A8
template<class _TCrom> class GeneticAlg {
public:
	///////////////////////////////////////////////////////////////////////////////////////
	//		konstruktor
	GeneticAlg( int inPopulationSize, int inIterNum, float inMutationProb );

	///////////////////////////////////////////////////////////////////////////////////////
	//		metode
	//##ModelId=39571AAF0221
	virtual	void		GenerateInitialPopulation() = 0;
	//##ModelId=39571AAF0218
	virtual	float		OptFunc( _TCrom & ) = 0;

	//##ModelId=39571AAF0217
	OptResults			PrepareResults();

	///////////////////////////////////////////////////////////////////////////////////////
	//		pristupne funkcije
	_TCrom&		getIndividual( int i );		// ukoliko je 'i' izvan raspona, throw exception

	///////////////////////////////////////////////////////////////////////////////////////
	//		get i set metode
	//##ModelId=39571AAF020F
	int			getPopulationSize() { return PopulationSize; }
	//##ModelId=39571AAF020D
	bool		setPopulationSize( int inPopSize ) 
					{ 
						if( inPopSize>0 ) {						// provjera da li je ispravan broj
							PopulationSize = inPopSize;
							
							if( CromList != NULL ) {		// ukoliko smo veæ alocirali nekakvu listu jedinki
								delete []CromList;
								CromList = new _TCrom[getPopulationSize()];
							}
							return true;
						} 
						else 
							return false;
					}
	
	//##ModelId=39571AAF0205
	long		getIterNum() { return IterNum; }
	//##ModelId=39571AAF0203
	bool		setIterNum( long inIterNum ) { IterNum = inIterNum; return true;}

	//##ModelId=39571AAF01FC
	float		getMutProb() { return MutationProb; }
	//##ModelId=39571AAF01FA
	bool		setMutProb( float inMutProb ) { MutationProb = inMutProb; return true; }

	///////////////////////////////////////////////////////////////////////////////////////
	//		Pool metode
	bool		addCurrentIteration();
	//##ModelId=39571AAF01EF
	bool		getIterationResults( int IterNum, vector<float> &Res ) 
					{ 
						return CromPool.getIterationResults(IterNum,Res); 
					}

protected:
	///////////////////////////////////////////////////////////////////////////////////////
	//		èlanske varijable
	//##ModelId=39571AAF01E6
	int			PopulationSize;
	//##ModelId=39571AAF01E5
	long		IterNum;
	//##ModelId=39571AAF01DC
	float		MutationProb;

	//##ModelId=39571AAF01DB
	long		CurrIterNum;

	//##ModelId=39571AAF01D1
	_TCrom				*CromList;
	//##ModelId=39571AAF01C7
	ResultsPool		CromPool;
};


/***************************************************************************************/
/******************				TOURNAMENT SELECTION ALGORITHM					**********************/
/***************************************************************************************/

//##ModelId=39571AB1007F
template<class _TCrom> class GenAlgTournamentSelection : public GeneticAlg<_TCrom> {
public:
	//##ModelId=39571AB100BC
	GenAlgTournamentSelection(int PopSize, int IterNum, float MutProb) : GeneticAlg<_TCrom>(PopSize,IterNum,MutProb) {}
	//##ModelId=39571AB100BB
	~GenAlgTournamentSelection() {} 

	virtual	void		GenerateInitialPopulation();

	virtual	void		PerformSelection(int *ToReplace, int ToSurvive[], int NumToSurvive);
	virtual	void		PerformCrossover(int Ind1, int Ind2, int IndRes);
	//##ModelId=39571AB100B4
	virtual	void		PerformMutation() = 0;

	//##ModelId=39571AB100B2
	virtual	float		OptFunc( _TCrom & ) = 0;
};

template<class _TCrom> int	CromosomCrossover( _TCrom &a, _TCrom &b, _TCrom &res );

/***************************************************************************************/
/*******************								POOL	CLASSES								 ***********************/
/***************************************************************************************/

//##ModelId=39571AB0009C
class ResultsPool {
public:
	//##ModelId=39571AB0010A
	ResultsPool( int inPopSize ) { PopSize = inPopSize; }

	//##ModelId=39571AB00101
	bool	addData( int IterNum, vector<float>& Res );

	// vraæa cjelokupni rezultat iteracije
	//##ModelId=39571AB000F8
	bool	getIterationResults( int IterNum, vector<float>& Res );

	// vraæaju i postavljaju rezultate za pojedinu jedinku u iteraciji
	//##ModelId=39571AB000EF
	bool	getIndividualResult( int IterNum, int CromNum, float *Res );
	//##ModelId=39571AB000E3
	bool	setIndividualResult( int IterNum, int CromNum, float Res );

	//##ModelId=39571AB000E2
	void	Print();

private:
	//##ModelId=39571AB000D9
	int		PopSize;
	//##ModelId=39571AB000CE
	map<int, vector<float> > mapCromProgress;
};

/***************************************************************************************/
/***********************				    IMPLEMENTACIJA				      ************************/
/***************************************************************************************/

template<class _TCrom>
GeneticAlg<_TCrom>::GeneticAlg( int inPopulationSize, int inIterNum, float inMutProb ) : CromPool(inPopulationSize)
{
	CromList = NULL;

	if( setPopulationSize(inPopulationSize) && setIterNum(inIterNum) && setMutProb(inMutProb) )
	{
		CurrIterNum = 0;
		CromList = new _TCrom[inPopulationSize];
	}
	else {
		throw(GAErrException(1));			// znaèi da nismo uspješno konstruirali objekt
	}
}

template<class _TCrom>
_TCrom& GeneticAlg<_TCrom>::getIndividual( int i)
{
	if( i>=0 && i<getPopulationSize() )
		return CromList[i];
	else {
		throw(GAErrException(2));
		return CromList[0];
	}
}

template<class _TCrom>
bool	GeneticAlg<_TCrom>::addCurrentIteration()
{
	vector<float>	Res;

	Res.resize(getPopulationSize());
	for( int i=0; i<getPopulationSize(); i++ )
		Res[i] = getIndividual(i).getFuncValue();

	CromPool.addData(CurrIterNum, Res);

	return true;
}

/***************************************************************************************/

template<class _TCrom> 
void	GenAlgTournamentSelection<_TCrom>::GenerateInitialPopulation()
{
	for( int i=0; i<PopulationSize; i++ ){
		CromList[i].GenerateCromosom();
		CromList[i].setFuncValue(OptFunc(CromList[i]),0);
	}
}

template<class _TCrom> 
void	GenAlgTournamentSelection<_TCrom>::PerformSelection(int *ToReplace, int ToSurvive[], int NumToSurvive)
{
	int			i, j;
	int			*Random = new int[NumToSurvive + 1];
	bool		AlreadyExist;
	float		MinValue;

	// izaberi tri jedinke
	for( i=0; i<NumToSurvive+1; i++ ) {
		do 
		{
			Random[i] = rand() % PopulationSize;
			
			// treba provjeriti da li smo tu jedinku veæ odabrali
			AlreadyExist = false;
			for( j=0; j<i; j++ )
				if( Random[i] == Random[j] )
				{
					AlreadyExist = true;
					break;
				}
		} while( AlreadyExist == true );
	}
	
	// odredi iznos funkcije cilja za svaku
	*ToReplace = Random[0];
	MinValue = CromList[Random[0]].getFuncValue();
	for( i=1; i<NumToSurvive+1; i++ )
		if( CromList[Random[i]].getFuncValue() < MinValue )
		{
			MinValue = CromList[Random[i]].getFuncValue();
			*ToReplace = Random[i];
		}

		// nakon što smo odabrali jedinku za zamjenu, treba preostale dvije pohraniti u polje ToSurvive
	j = 0;
	for( i=0; i<NumToSurvive+1; i++ )
		if( *ToReplace != Random[i] )		// ako to nije ona koja æe biti zamijenjena
			ToSurvive[j++] = Random[i];

	delete []Random;
}

template<class _TCrom> 
void	GenAlgTournamentSelection<_TCrom>::PerformCrossover(int Ind1, int Ind2, int IndRes)
{
	CromosomCrossover(CromList[Ind1], CromList[Ind2], CromList[IndRes]);
	CromList[IndRes].setFuncValue(OptFunc(CromList[IndRes]),CurrIterNum);
}

#endif
