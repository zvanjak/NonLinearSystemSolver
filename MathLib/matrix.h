#if !defined	__MATRIX_H
#define	__MATRIX_H

typedef unsigned int	UINT;

#define NO_CONT_ALLOCATION		1
#define	CONT_ALLOCATION			2

// Kodovi gresaka kod SVD dekompozicije
#define	SVD_VECTOR_W_DIM		1
#define SVD_MATRIX_V_DIM		2
#define SVD_NO_CONVERG			3

#define LU_SING			4

class	Matrix
{
private:
	int			m_nRowNum, m_nColNum;
	double	**m_pdElem;

public:
	Matrix();
	Matrix( int rows, int cols );
	Matrix( int rows, int cols, int nParam );
	Matrix( const Matrix &Copy );
	Matrix( double **Elem, int Row, int Col, int Copy=COPY );
	~Matrix();

	Matrix	operator+( const Matrix &b ) const;
	Matrix	operator-( const Matrix &b ) const;
	Matrix	operator*( const Matrix &b ) const;
	Vector	operator*( Vector &b );

	Matrix	operator+=( const Matrix &b );
	Matrix	operator-=( const Matrix &b );
	Matrix	operator*=( const Matrix &b );
	Matrix	operator*=( double b );
	double*	operator[]( int i );

	Matrix&	operator=( const Matrix &b );

	friend	Matrix	operator*( const double a,  const Matrix &b );
	friend	Matrix	operator*( const Matrix &a, const double b );

	void		MakeUnitMatrix( void );
	void    MakeRandomElements( double dMax=1, double dMin=-1 );

	void		Clear( void );
	double	GetElem( int i, int j );
	void		SetElem( int i, int j, double dValue );
	double	**GetPtr( void );

	int			IsSquare( void );
	int			IsSymetric( double dTiny = 1e-6 );
	int			IsOrthogonal( double dTiny = 1e-6 );
	
	friend	double	Elem( const Matrix &a, int i, int j );
	friend	int			RowNum( const Matrix &a );
	friend	int			ColNum( const Matrix &a );

	friend	Matrix	Transp( const Matrix &a );
	friend	Matrix	Invert( const Matrix &a );
	friend	void		RightInverse( Matrix &a, Matrix *matInv );
	friend	void		LeftInverse( Matrix &a, Matrix *matInv );
	friend	void		Print( const Matrix &a );

	friend	int		LUDecomp( Matrix &a, int *indx, double *d );
	friend	void	LUBackSubst( Matrix &a, int *indx, Matrix &b );
	friend	void	LUBackSubst( Matrix &a, int *indx, Vector &b );
	friend	void	LUBackSubst( Matrix &a, int *indx, double b[] );
	
	friend	void	QRDecomp( Matrix &a, Vector &c, Vector &d, int *sing );
	friend	void	QSolve( Matrix &a, Vector &c, Vector &d, Vector &b );
	friend	void	RSolve( Matrix &a, Vector &d, Vector &b );
	friend	void	QRUpdate( Matrix &a, Matrix &qt, Vector &u, Vector &v );
	friend	void	JacRotate( Matrix &r, Matrix &qt, int i, double a, double b );

	friend	int		SingValDecomp( Matrix *a, Vector *vecW, Matrix *v );
	friend	void	SVDBackSubst( Matrix &u, Vector *vecW, Matrix &v, Matrix &b, Matrix *x );

	friend  void		Balance( Matrix &mat );
	friend	Matrix&	ReductionToHess( Matrix	&matA );
	friend	Matrix& TransfToHess( Matrix &matA );
	friend	void		HessEigenvalues( Matrix &matA, Vector *vecR, Vector *vecI );

	friend	void		CalcEigenvalues( Matrix matA, Vector *vecR, Vector *vecI );
	friend	void		CalcEigenvalues( Matrix *matA, Vector *vecR, Vector *vecI );
	friend	void		CalcEigenvalues( Matrix matA, ComplexVector *cvecR );
	friend	void		CalcEigenvalues( Matrix *matA, ComplexVector *cvecEigen );

	friend	int			CalcEigenvectors( Matrix &matA, Vector &vecEigenvalues, Vector **vecEigenvectors );

	friend	Matrix	ToUpperTriang( const Matrix &a );
	friend	Matrix	ToLowerTriang( const Matrix &a );
};

double pythag(double a, double b);

class	ComplexMatrix
{
private:
	int				m_nRowNum, m_nColNum;
	Complex		**m_pdElem;

public:
	ComplexMatrix();
	ComplexMatrix( unsigned int rows, unsigned int cols );
	ComplexMatrix( const ComplexMatrix &Copy );
	~ComplexMatrix();

	ComplexMatrix	operator+( const ComplexMatrix &b ) const;
	ComplexMatrix	operator-( const ComplexMatrix &b ) const;
	ComplexMatrix	operator*( const ComplexMatrix &b ) const;
	ComplexVector	operator*( ComplexVector &b );
	ComplexVector	operator*( Vector &b );

	ComplexMatrix	operator+=( const ComplexMatrix &b );
	ComplexMatrix	operator-=( const ComplexMatrix &b );
	ComplexMatrix	operator*=( const ComplexMatrix &b );
	ComplexMatrix	operator*=( double b );

	ComplexMatrix&	operator=( const ComplexMatrix &b );

	friend	ComplexMatrix	operator*( const double a,  const ComplexMatrix &b );
	friend	ComplexMatrix	operator*( const ComplexMatrix &a, const double b );

	void		MakeUnitMatrix( void );
	void    MakeRandomElements( double dMax=1, double dMin=-1 );

	void		Clear( void );
	Complex	GetElem( int i, int j );
	void		SetElem( int i, int j, Complex dCValue );
	void		SetElem( int i, int j, double dReal, double dImag );
	Complex **GetPtr( void );
	
	friend	Complex	Elem( const ComplexMatrix &a, UINT i, UINT j );
	friend	UINT		RowNum( const ComplexMatrix &a );
	friend	UINT		ColNum( const ComplexMatrix &a );

	friend	ComplexMatrix	Transp( const ComplexMatrix &a );
	friend	ComplexMatrix	Invert( const ComplexMatrix &a );
	friend	ComplexMatrix	Solve( const ComplexMatrix &a );
	friend	void					Print( const ComplexMatrix &a );

	friend	void	LUDecomp( ComplexMatrix &a, int *indx, double *d );
	friend	void	LUBackSubst( ComplexMatrix &a, int *indx, ComplexMatrix &b );
	friend	void	LUBackSubst( ComplexMatrix &a, int *indx, ComplexVector &b );

/*	friend	ComplexMatrix	ToUpperTriang( const ComplexMatrix &a );
	friend	ComplexMatrix	ToLowerTriang( const ComplexMatrix &a );*/
};


Matrix	GetProjectionMatrix( Point3D O, Point3D G );

#endif