// NLSysView.h : interface of the CNLSysView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NLSYSVIEW_H__72A8E5C2_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_)
#define AFX_NLSYSVIEW_H__72A8E5C2_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_

#include "D:\Projects\Library\MathLib\mathlib.h"


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class		CPhasePlotParDlg;
class		CTrajectoryPlotParDlg;
class		CPlotEquSurfaceDlg;
class		CPlotLyapExpDlg;

#define	CENTER_WITHIN_WINDOW		1
#define	X_AXIS_WITHIN_WINDOW		2
#define	Y_AXIS_WITHIN_WINDOW		3
#define	NO_AXIS_WITHIN_WINDOW		-1

#define MAX_PLOTTING_VARS		4

#define MAX_CHG_PAR			8

struct VERTEX
{
	double	x;
	double	y;
	double	z;
	double	h;
};

struct CPointDouble
{
	double x, y;
};

CPointDouble	TransfToDoublePoint( double *point, Matrix *T );
CPoint				TransfToPoint( double *point, Matrix *T );
CPointDouble	TransfVertexToPoint( VERTEX point, Matrix *T );

typedef struct
{
	double	m_dStep;
	double	m_dEps;
	double	m_dStart, m_dEnd;

	int			m_nSaveValNum;
	// u m_nIntSteps se stavlja u koliko je stvarno toèaka izraèunat integral sustava
	// ukoliko je bilo jednoparametarsko iscrtavanje uzima se [0] èlan niza, a za višeparametarsko
	// iscrtavanje, za svaku vrijednost parametra u pripadni element niza
	int			m_nIntSteps[MAX_CHG_PAR];

	// poèetni uvjeti za integraciju u danom prozoru
	char		m_szParamVal[100];
	char		m_szInitCond[100];
	Vector	*m_vecInitCond;
	Vector	*m_vecParamVal;

	char		m_szChgPar[20];
	char		m_szChgParValues[100];
	int			m_nVarNum;
	enum		{ SYS_VAR, PAR_VAR } m_enChgType;
	int			m_nNumOfChgPar;
	Vector	*m_vecChgParList;

	BOOL		m_bParametersChanged;

	HWND		m_hWnd;
} IntegrateParameters;

typedef struct
{
	int			m_nWinExtX, m_nWinExtY;
	// m_nRealCalcNum se koristi kod crtanja Fourierovog spektra i kod crtanja Lyapunovljevih eksponenata
	int			m_nRealCalcNum;

	double	m_dStartPlot, m_dEndPlot;

	double	m_dXUpp, m_dXLow;
	double	m_dYUpp, m_dYLow;
	double	m_dStartXUpp, m_dStartXLow, m_dStartYUpp, m_dStartYLow;
	double	m_dXOrg, m_dYOrg;
	double	m_dMulX, m_dMulY, m_dStartMulX, m_dStartMulY;
	double	m_dPrintMulX, m_dPrintMulY;

	// pojedini èlan niza govori za unesenu varijablu, gdje se ta
	// varijabla nalazi u nizu SVIH definiranih varijabli
	int			m_anPlottingVars[MAX_PLOTTING_VARS];
	int			m_anOldPlottingVars[MAX_PLOTTING_VARS];
	// èlan niza za unesenu varijablu odgovara REDNOM BROJU te varijable
	// u nizu VARIJABLI SUSTAVA
	int			m_anSysVarsOrder[MAX_PLOTTING_VARS];
	
	// da li æe se susjedne izraèunate vrijednosti L.exp  spajati linijom, ili æemo ih 
	// iscrtavati kao toèke
	BOOL		m_bConnectLyap;		
	
	BOOL		m_bShowLegend, m_bSetLegendPos;

	enum		{ SCALE_Y_SAME, SCALE_Y_DIFFERENT }	m_nScaleY;
	enum		{ MULTI_PAR, SINGLE_PAR } m_nParNumPlot;
	enum		{	PLOT_2D, PLOT_3D } m_nPhasePlotDim;

	BOOL		m_bViewTypeChanged;				// TRUE kada se promjeni vrsta iscrtavanja u prozoru, ili kada se promjene varijable koje se iscrtavaju
	BOOL		m_bPlotParametersChanged;		// TRUE kod reskaliranja prozora (expand ili condense x- ili y-osi )
	BOOL		m_bXAxisIntervalChanged;		// TRUE kada se promjeni interval iscrtavanja na X-osi

	double	m_dAlpha, m_dTheta, m_dRho;

	Point3D	*pointLookTo;
	Matrix	*m_matT;

	BOOL			m_bShowBifInEqu, m_bSetBifLegendPos;
} PlotParameters;

typedef struct
{
	double	m_adMagY[10];
	double	m_adMinY[10], m_adMaxY[10];

	int		m_nMaxVar;
	int		m_nMinVar;
} AnalyseParameters;

typedef struct
{
	double	m_adMaxEqu[3], m_adMinEqu[3];		// max. i min. vrijednosti

	BOOL		m_bEquSurfaceDataAvailable;
	int			m_nEquSurfaceDataNum;
} EquSurfaceAnalyseParameter;

typedef struct
{
	int			m_nMaxVar, m_nMinVar;

	double	m_adMaxY[MAX_PLOTTING_VARS], m_adMinY[MAX_PLOTTING_VARS];
} SpectrumAnalyseParameters;

typedef struct
{
	int			m_nMaxVar, m_nMinVar;

	double	m_adMaxY[10], m_adMinY[10];
} LyapExpAnalyseParameters;

class CNLSysView : public CView
{
protected: // create from serialization only
	CNLSysView();
	DECLARE_DYNCREATE(CNLSysView)

// Attributes
public:
	CNLSysDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLSysView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNLSysView();
	void		Set3DData();
	void		SetCalcDone( BOOL val);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNLSysView)
	afx_msg void OnCancelEditSrvr();
	afx_msg void OnPlotTrajectory();
	afx_msg void OnPlotPhaseCurve();
	afx_msg void OnPlotGetCloser();
	afx_msg void OnPlotGetFarther();
	afx_msg void OnPlotXCondense();
	afx_msg void OnPlotXExpand();
	afx_msg void OnPlotXLeft();
	afx_msg void OnPlotXRight();
	afx_msg void OnPlotYCondense();
	afx_msg void OnPlotYDown();
	afx_msg void OnPlotYExpand();
	afx_msg void OnPlotYUp();
	afx_msg void OnRotateDown();
	afx_msg void OnRotateLeft();
	afx_msg void OnRotateRight();
	afx_msg void OnRotateUp();
	afx_msg void OnResetMul();
	afx_msg void OnPlotExpandBoth();
	afx_msg void OnPlotCondenseBoth();
	afx_msg void OnPlotEquilibriumSolutions();
	afx_msg void OnSetLegendPosition();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPlotLyapunovExp();
	afx_msg void OnUpdateShowLegend(CCmdUI* pCmdUI);
	afx_msg void OnShowLegend();
	afx_msg void OnPlotSpectrum();
	afx_msg void OnOptionsShowBifInEquSurf();
	afx_msg void OnUpdateOptionsShowBifInEquSurf(CCmdUI* pCmdUI);
	afx_msg void OnOptionsSetBifLegendPos();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	enum PlotType {	 PLOT_NOTHING, 
									 TRAJECTORY_PLOT, 
									 PHASE_PLOT_2D, PHASE_PLOT_3D, 
									 EQU_SURFACE_PLOT_2D, EQU_SURFACE_PLOT_3D,
									 FOURIER_SPECTRUM_PLOT,
									 LYAP_EXP_PLOT } m_nPlotType;

	Vector	*m_vecX;		// vrijednosti u kojima je izraèunat integral sistema
	Matrix	*m_matY;		// izraèunate vrijednosti integrala

	// ova matrica je ukoliko u istom prozoru prikazujemo trajektorije za razlièite vrijednosti parametara
	Vector	*m_apvecX[MAX_CHG_PAR];
	Matrix	*m_apmatY[MAX_CHG_PAR];	

	Matrix	*m_matEquSol;		// matrica sa izraèunatim ravnotežnim toèkama sustava

	Vector	*m_vecFreq;			// vektor sa frekvencijama u kojima je izraèunat spektar
	Matrix	*m_matSpectrum;	// matrica sa izraèunatim spektrom varijabli sustava

	Vector	*m_vecParVal;		// vrijednosti parametra za koje je izraèunat L. exp.
	Matrix	*m_matLyapExp;	// iznosi L. eksponenata

	IntegrateParameters		m_IntPar;						// parametri za integraciju
	IntegrateParameters		m_LastSuccIntPar;		// parametri zadnje uspješne integracije
	
	PlotParameters	m_PlotPar;			// parametri za iscrtavanje
	
	CString			m_aszLegend[MAX_PLOTTING_VARS];
	CPoint			m_pntLegendPos;
	CPoint			m_pntBifLegendPos;
	CString			m_szTitle;					// ime prozora

	AnalyseParameters						m_AnalPar;
	EquSurfaceAnalyseParameter	m_EquAnalPar;
	SpectrumAnalyseParameters		m_SpecAnal;
	LyapExpAnalyseParameters		m_LyapAnal;

	CPen		m_aPens[MAX_CHG_PAR], m_aPrintPens[MAX_CHG_PAR];

	BOOL		m_bCalcDone, m_bCalcInProgress;
	BOOL		m_bSystemChanged;
private:
	
	void InitpDC( CDC *pDC, int nCalledBy );
	void GetInterval( void );
	void PlotAxis( CDC *pDC, int nCalledBy );
	int  CenterWithinWindow( CPoint &pntOrigin );

	void DrawPhase3D( CDC *pDC );
	void DrawPhase2D( CDC *pDC);
	void DrawTrajectory( CDC *pDC);
	void DrawEquSurface2D( CDC *pDC);
	void DrawSpectrum( CDC *pDC );
	void DrawLyapExp( CDC *pDC );

	int	 DifferentIntPar();

	// funkcije SetDialogValues postavljaju SAMO vrijednosti èlanskih varijabli dijaloga
	// funkcije GetDialogValues, ukoliko je unos ispravan popunjavaju èlanske varijable CView klase
	int	 GetDialogValues( CPhasePlotParDlg &dlg );
	void SetDialogValues( CPhasePlotParDlg &dlg );
	int  GetDialogValues( CTrajectoryPlotParDlg &dlg );
	void SetDialogValues( CTrajectoryPlotParDlg &dlg );
	int  GetDialogValues( CPlotEquSurfaceDlg &dlg );
	void SetDialogValues( CPlotEquSurfaceDlg &dlg );
	int	 GetDialogValues( CPlotLyapExpDlg &dlg, LyapCalcParameters *pLCP );
	void SetDialogValues( CPlotLyapExpDlg &dlg );

	int		NumOfPlottingVars( void );

	CFrameWnd*	NewWindow( );
	BOOL				IsPlottingVar( int k );
	void				SystemChanged( BOOL val );

	friend	UINT		Integrate( LPVOID lParam);
	friend	UINT		CalcSpectrum( LPVOID lParam);
	friend	UINT		CalcEqvSolution( LPVOID lParam );
	friend	UINT		CalcLyapMany( LPVOID lParam );

	friend	class		CNLSysSrvrItem;
	friend	class		CNLSysDoc;
};

#ifndef _DEBUG  // debug version in NonLinSystemsView.cpp
inline CNLSysDoc* CNLSysView::GetDocument()
   { return (CNLSysDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLSYSVIEW_H__72A8E5C2_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_)

