#ifndef __QAPNEW_H
#define	__QAPNEW_H

#include "GeneticAlgNew.h"
#include "Matrix.h"

#include <vector>
using std::vector;

/*****************************************************************************************/
// Simple Quadratic Assignement Problem
//	klasa koja opisuje problem kod kojeg 'n' kuglica treba rasporediti u 'n' slotova
class	QAPSimple {
public:
	QAPSimple(int inFacilityNum);

	int		N() { return FacilityNum; }

	virtual void	Optimize() = 0;
	float					OptFunc(std::vector<int> &Sol);

	void	SetMatrices( const Matrix<float> &a, const Matrix<float> &b, const Matrix<float> &c );
	void	SetA( const Matrix<float> &a );
	void	SetB( const Matrix<float> &b );
	void	SetC( const Matrix<float> &c );

protected:
	int		FacilityNum;

	Matrix<float>		A;		// matrica toka izmeðu kuglice 'i' i kuglice 'j'
	Matrix<float>		B;		// matrica udaljenosti meðu pojedinim slotovima
	Matrix<float>		C;		// trošak stavljanja kuglice 'i' na mjesto 'j'
};

/*****************************************************************************************/
/*****************************************************************************************/
//

class QAPCrom : public Cromosom {
public:
	QAPCrom();
	QAPCrom(int inFacNum);

	int		GenerateCromosom();
	void	Print();
	void	PrintHorizontal();

	int		N() { return FacilityNum; }
	void	SetFacilityNum( int N );

	int&	operator[]( int i );		// vraæa se po referenci -> može biti l-value

private:
	int	 FacilityNum;
	std::vector<int>	Sol;  // Sol[i] = slot u koji se stavlja i-ta kuglica

	// radi QAPSimpleGenetic::OptFunc() koja treba pristupiti Sol[i]
	friend	class QAPSimpleGenetic;	
};

/*****************************************************************************************/

class QAPSimpleGenetic : public QAPSimple, public GenAlgTournamentSelection<QAPCrom> {
public:
	QAPSimpleGenetic( int inFacNum, int inPopSize, int inIterNum, float inMutProb );

	void		PerformMutation();

	void		Optimize();
	float		OptFunc( QAPCrom &a ) { return QAPSimple::OptFunc(a.Sol); }
};

#endif