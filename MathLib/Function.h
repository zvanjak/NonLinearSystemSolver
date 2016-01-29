template<class Type> class Function
{
	private:
		Type	(*m_pfFunc) ( Type );		// pokazivac na funkciju definiranu u programu
		Function	*m_pfncFunc;						// pokazivac na objekt klase Function ( ako se kace fnc2 = Derive(fnc1)

		enum { NORMAL, DERIVATIVE } m_eType;	// da li objekt sadrzi pokazivac na "pravu" funkciju
																					// ili pokazuje na drugi objekt klase Function ( ako je funkcija izvedena kao derivacija druge funkcije(odn. objekta klase Function
	public:
		Function();
		Function( Type (*pfFunc)(Type) );
		~Function();

		Type	Evaluate( Type x );
		Type	Derive( Type x );
		Type	Integrate( Type a, Type b );

		Function	Derive();
};
