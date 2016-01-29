class SparseMatrix
{
private:
	double					*m_pdElem;			// elementi matrice
	unsigned long		*m_plIJ;				// polje indeksa
	unsigned long		m_ulMax;				// dimenzija alociranih polja
	unsigned long   m_ulElemNum;		// stvarni broj elemenata u poljima

public:
	SparseMatrix( unsigned long lNMax = 1000 );
	SparseMatrix( Matrix &a, double dTresh, unsigned long lMax = 1000 );
	SparseMatrix( const SparseMatrix &Copy );			// copy konstruktor
	~SparseMatrix();

	friend	Vector		operator*( SparseMatrix &a, Vector &b );

	friend	Vector				MulTranspVec( SparseMatrix &a, Vector &b );
	friend	SparseMatrix	Transp( const SparseMatrix &a );

	void	PrintArrays();
};


