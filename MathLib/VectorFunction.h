class VectorFunction
{
	private :
		int			m_nDomDim, m_nCodomDim;

		Vector	(*m_pfFunc) ( Vector & );
		Matrix	(*m_pfJac)( Vector & );
	public:
		VectorFunction( Vector (*pfFunc)( Vector &), int DomDim, int CodomDim );
		~VectorFunction();

		Vector		Evaluate( Vector & );
		Matrix		CalcJacobian( Vector & );
};