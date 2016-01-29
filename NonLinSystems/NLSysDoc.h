// NLSysDoc.h : interface of the CNLSysDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NLSYSDOC_H__72A8E5C0_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_)
#define AFX_NLSYSDOC_H__72A8E5C0_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_

// ovaj nam treba radi sinkronizacijskih objekata
#include <afxmt.h>

#include "D:\Projects\Library\MathLib\mathlib.h"

#include "LocateBifPointsDlg.h"
#include "CalcEquSolutionDlg.h"
#include "CalcLyapExpDlg.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define	MAX_SNB_POINTS		100
#define	MAX_HOPF_POINTS		100
#define MAX_LYAP_EXP			1000

#define MAX_EQU_SOL				1000
#define MAX_DIFF_SYS_VAR_PER_EQU_SOL		20

#define EQU_SOL_TOL			1e-6
#define LYAP_EXP_TOL		1e-5
#define	BIF_POINT_TOL		1e-5

class CNLSysDoc;
class	CNLSysView;

typedef struct
{
	enum		MultiParWindowOpen { NEW_WIND, SAME_WIND } m_nWindOpen;

	double	m_dEps, m_dStepMin;
	BOOL		m_bUseColor;
} ProgramOptions;

typedef struct
{
	int			m_nSNBPointsNum;
	int			m_nHopfPointsNum;

	// polje pokazivaèa na vektore koji sadrže vrijednost varijabli sustava i parametara kod
	// kojih se pojavljuje bifurkacija
	Vector	*m_avecSNBVarValues[MAX_SNB_POINTS], *m_avecSNBParValues[MAX_SNB_POINTS];
	Vector	*m_avecHopfVarValues[MAX_HOPF_POINTS], *m_avecHopfParValues[MAX_HOPF_POINTS];

	// da li su analizirane pojave bifurkacija za zadani sustav, odnosno da li su dobijene vrijednosti jos
	// uvijek dobre (jer smo možda u meðuvremenu promjenili jednadžbe sustava)
//	BOOL		m_bSNBAnalysed, m_bHopfAnalysed;
} BifurcationAnalyse;

typedef struct
{
	enum			{ HOPF_BIF, SNB_BIF } nType;
	int				nSearchNum;
	double		dP1, dP2;

	CNLSysDoc	*pDocument;
} BifCalcParameters;

typedef struct
{
	int		m_nLyapExpNum;

	// polje pokazivaèa na vektore koji sadrže vrijednost varijabli sustava i parametara za
	// koje su izraèunati Lyapunovljevi eksponenti
	Vector	*m_avecLyapStartVal[MAX_LYAP_EXP];
	Vector	*m_avecLyapParVal[MAX_LYAP_EXP];
	
	Vector	*m_avecLyapExp[MAX_LYAP_EXP];
} LyapExpAnalyse;


typedef struct
{
	// ovo su parametri potrebni za konkretno izraèunavanje L. exp.
	Vector		*X, *P;
	double		dT, dMinH, dEps, dEpsRel, dEpsAbs;
	int				nMaxIter;
	
	// a ovo su parametri koji su potrebni kod crtanja grafa ovisnosti L. exp o nekoj varijabli,
	// pa ih stavljamo u ovu strukturu da ne moramo praviti novu
	int				nChangeValNum;
	bool			bChangePar;
	double		dX1, dX2, dStep;

	CNLSysDoc		*pDocument;
	CNLSysView	*pView;
	HWND				hWnd;
} LyapCalcParameters;

typedef struct
{
	enum		{	EQU_SURFACE_PLOT_2D, EQU_SURFACE_PLOT_3D } nType;

	int			nParVarNum, nSysVarNum;
	double	x1, x2;

	CNLSysView		*pView;
	CNLSysDoc			*pDoc;
} EqvSolCalcParameters;

typedef struct
{
	int				m_nEquSolNum;

	// niz pokazivaèa na vektore koji sadrže vrijednosti parametara za koje su izraèunate
	// ravnotežne toèke sustava
	Vector		*m_apvecParVal[MAX_EQU_SOL];
	// niz pokazivaèa na pokazivaèe na vektore varijabli sustava za koje je izraèunato rav. rješenje
	//	potrebno je stoga što za jedan skup vrijednosti parametara može biti više ravnotežnih 
	//	rješenja, s razlièitim vrijednostima varijabli sustava
	Vector		**m_appvecSysVar[MAX_EQU_SOL];
} EquSolAnalyse;

class CNLSysSrvrItem;

class CNLSysDoc : public COleServerDoc
{
protected: // create from serialization only
	CNLSysDoc();
	DECLARE_DYNCREATE(CNLSysDoc)

// Attributes
public:
	CNLSysSrvrItem* GetEmbeddedItem()
		{ return (CNLSysSrvrItem*)COleServerDoc::GetEmbeddedItem(); }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLSysDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	virtual void OnDeactivate();
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetVarNum( CString str );
	BOOL IsDefined();
	virtual ~CNLSysDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNLSysDoc)
	afx_msg void OnEditDiffEquations();
	afx_msg void OnLocateBifurcation();
	afx_msg void OnViewBifurcationPoints();
	afx_msg void OnCalcLyapExp();
	afx_msg void OnViewLyapExp();
	afx_msg void OnSetProgramOptions();
	afx_msg void OnCalcEquSol();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	DiffEqvSystem		m_DES;

	char		m_szParamVal[100];
	char		m_szStartVal[100];
	char		m_szIndVar[MAX_VAR_NAME_LEN];
	char		m_szEquations[500];

	BOOL		m_bSysDefined;			// da li je zadan sistem dif. jednadžbi

	CCriticalSection	*pCritIntegrate;
	
	ProgramOptions		m_Options;

	BifurcationAnalyse		m_BifAnal;
	LyapExpAnalyse				m_LyapAnal;
	EquSolAnalyse					m_EquSolAnal;

	int		GetDialogValues( CLocateBifPointsDlg &dlg, Vector &vecStart );
	int		GetDialogValues( CCalcLyapExpDlg &dlg, Vector &vVar, Vector &vPar );
	BOOL	LyapExpAlreadyCalc( Vector &X, Vector &P );

	// sve ove tri funkcije vraæaju 0 ukoliko smo tu bif. toèku veæ našli, i 1 ukoliko nismo
	int		PutInSNBBifArray( Vector &vecVal );
	int		PutInSNBBifArray( Vector &X, Vector &P );
	int		PutInHopfBifArray( Vector &vecVal, double omega );
	void	UndefAllBifPoints( void );

	int		PutInCalcLyapArray( Vector &X, Vector &P, Vector &vLyap );
	void	UndefAllLyapExp( void );

	int		PutInEquSolArray( Vector &X, Vector &P, int *nPosIntParList );
	void	UndefAllEquSol( void );

	friend	UINT		Integrate( LPVOID lParam );
	friend	UINT		CalcLyap( LPVOID lParam );
	friend	UINT		CalcLyapMany( LPVOID lParam );
	friend	UINT		CalcBif( LPVOID lParam );
	friend	UINT		CalcSpectrum( LPVOID lParam );
	friend	UINT		CalcEqvSolution( LPVOID lParam );

	friend	class		CNLSysView;
	friend	class		CNLSysSrvrItem;
	friend	class		CCalcEquSolutionDlg;
	friend	class		CLocateBifPointsDlg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLSYSDOC_H__72A8E5C0_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_)
