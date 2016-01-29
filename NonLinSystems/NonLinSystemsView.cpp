// NonLinSystemsView.cpp : implementation of the CNLSysView class
//
  
#include "stdafx.h"
#include "NonLinSystems.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#include "NLSysDoc.h"
#include "NLSysView.h"

#include "TrajectoryPlotParDlg.h"
#include "PhasePlotParDlg.h"
#include "PlotEquSurfaceDlg.h"
#include "PlotLyapExpDlg.h"

#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLSysView

IMPLEMENT_DYNCREATE(CNLSysView, CView)

BEGIN_MESSAGE_MAP(CNLSysView, CView)
	//{{AFX_MSG_MAP(CNLSysView)
	ON_COMMAND(ID_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	ON_COMMAND(ID_PLOT_TRAJECTORY, OnPlotTrajectory)
	ON_COMMAND(ID_PLOT_PHASE_CURVE, OnPlotPhaseCurve)
	ON_COMMAND(ID_PLOT_GET_CLOSER, OnPlotGetCloser)
	ON_COMMAND(ID_PLOT_GET_FARTHER, OnPlotGetFarther)
	ON_COMMAND(ID_PLOT_X_CONDENSE, OnPlotXCondense)
	ON_COMMAND(ID_PLOT_X_EXPAND, OnPlotXExpand)
	ON_COMMAND(ID_PLOT_X_LEFT, OnPlotXLeft)
	ON_COMMAND(ID_PLOT_X_RIGHT, OnPlotXRight)
	ON_COMMAND(ID_PLOT_Y_CONDENSE_WINDOW, OnPlotYCondense)
	ON_COMMAND(ID_PLOT_Y_DOWN, OnPlotYDown)
	ON_COMMAND(ID_PLOT_Y_EXPAND_WINDOW, OnPlotYExpand)
	ON_COMMAND(ID_PLOT_Y_UP, OnPlotYUp)
	ON_COMMAND(ID_ROTATE_DOWN, OnRotateDown)
	ON_COMMAND(ID_ROTATE_LEFT, OnRotateLeft)
	ON_COMMAND(ID_ROTATE_RIGHT, OnRotateRight)
	ON_COMMAND(ID_ROTATE_UP, OnRotateUp)
	ON_COMMAND(ID_RESET_MUL, OnResetMul)
	ON_COMMAND(ID_PLOT_EXPAND_BOTH, OnPlotExpandBoth)
	ON_COMMAND(ID_PLOT_CONDENSE_BOTH, OnPlotCondenseBoth)
	ON_COMMAND(ID_PLOT_EQUIL_SOLUTIONS, OnPlotEquilibriumSolutions)
	ON_COMMAND(ID_PLOT_SET_LEGEND_POS, OnSetLegendPosition)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_PLOT_LIAPUNOV_EXP, OnPlotLyapunovExp)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOW_LEGEND, OnUpdateShowLegend)
	ON_COMMAND(ID_OPTIONS_SHOW_LEGEND, OnShowLegend)
	ON_COMMAND(ID_FOURIER_DRAW, OnPlotSpectrum)
	ON_COMMAND(ID_OPTIONS_SHOW_BIF_IN_EQU_SURF, OnOptionsShowBifInEquSurf)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_SHOW_BIF_IN_EQU_SURF, OnUpdateOptionsShowBifInEquSurf)
	ON_COMMAND(ID_OPTIONS_SET_BIF_LEGEND_POS, OnOptionsSetBifLegendPos)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLSysView construction/destruction

CNLSysView::CNLSysView()
{
	int			i;

	m_vecX = NULL;
	m_matY = NULL;

	m_vecFreq = NULL;
	m_matSpectrum = NULL;

	m_vecParVal = NULL;
	m_matLyapExp = NULL;

	for( i=0; i<MAX_CHG_PAR; i++ )
	{
		m_apvecX[i] = NULL;
		m_apmatY[i] = NULL;
	}

	m_matEquSol = new Matrix(3, 200);
	m_EquAnalPar.m_bEquSurfaceDataAvailable = FALSE;
	m_EquAnalPar.m_nEquSurfaceDataNum = 0;

	m_nPlotType = PLOT_NOTHING;

	m_bCalcDone = FALSE;
	m_bCalcInProgress = FALSE;
	m_bSystemChanged = FALSE;

	m_pntBifLegendPos = CPoint(0,0);
	m_pntLegendPos = CPoint(0,0);
	m_szTitle = "";

	m_IntPar.m_hWnd = m_hWnd;

	m_IntPar.m_vecInitCond = NULL;
	m_IntPar.m_vecParamVal = NULL;

	m_IntPar.m_nSaveValNum = 1000;
	m_IntPar.m_bParametersChanged = TRUE;
	
	m_IntPar.m_dStart = 0;
	m_IntPar.m_dEnd = 1.0;
	m_IntPar.m_dStep = 0.005;
	m_IntPar.m_dEps = 1e-5;

	m_IntPar.m_vecChgParList = NULL;
	memset(m_IntPar.m_szChgPar, 0, 20);
	memset(m_IntPar.m_szChgParValues, 0, 100 );

	m_LastSuccIntPar.m_dStart = -1;
	m_LastSuccIntPar.m_dEnd = -1;
	m_LastSuccIntPar.m_dStep = -1;
	m_LastSuccIntPar.m_dEps = -1;
	m_LastSuccIntPar.m_vecInitCond = NULL;
	m_LastSuccIntPar.m_vecParamVal = NULL;
	m_LastSuccIntPar.m_vecChgParList = NULL;

	m_PlotPar.m_bViewTypeChanged = FALSE;
	m_PlotPar.m_bPlotParametersChanged = FALSE;
	
	m_PlotPar.m_nScaleY = PlotParameters::SCALE_Y_DIFFERENT;
	m_PlotPar.m_nParNumPlot = PlotParameters::SINGLE_PAR;
	m_PlotPar.m_nPhasePlotDim = PlotParameters::PLOT_2D;

	m_PlotPar.m_bShowLegend = FALSE;
	m_PlotPar.m_bSetLegendPos = FALSE;

	m_PlotPar.m_bShowBifInEqu = FALSE;
	m_PlotPar.m_bSetBifLegendPos = FALSE;

	m_PlotPar.m_nWinExtX = 4000;
	m_PlotPar.m_nWinExtY = 4000;

	m_PlotPar.m_dXUpp = 1;
	m_PlotPar.m_dXLow = 0;
	m_PlotPar.m_dYUpp = 1;
	m_PlotPar.m_dYLow = -1;
	
	m_PlotPar.m_dStartPlot = 0;
	m_PlotPar.m_dEndPlot = 1.0;

	m_PlotPar.m_matT = new Matrix(4,4);
	m_PlotPar.pointLookTo = new Point3D(0,0,0);
	
	m_PlotPar.m_dRho = 20;
	m_PlotPar.m_dTheta = 45;
	m_PlotPar.m_dAlpha = 45;

	Set3DData();

	m_PlotPar.m_anPlottingVars[0] = 0;
	m_aszLegend[0] = CString("");
	for( i=1; i<MAX_PLOTTING_VARS; i++ )
	{
		m_PlotPar.m_anPlottingVars[i] = m_PlotPar.m_anOldPlottingVars[i] = -1;
		m_PlotPar.m_anSysVarsOrder[i] = -1;

		m_aszLegend[i] = CString("");
	}

	for( i=0; i<10; i++ )
		m_AnalPar.m_adMagY[i] = 1.0;
	
	m_AnalPar.m_nMaxVar = m_AnalPar.m_nMinVar = 0;
	m_LyapAnal.m_nMaxVar = m_LyapAnal.m_nMinVar = 0;
	m_SpecAnal.m_nMaxVar = m_SpecAnal.m_nMinVar = 0;

	m_aPens[0].CreatePen(PS_SOLID, 1, RGB(0,0,0) );
	m_aPens[1].CreatePen(PS_SOLID, 1, RGB(255,0,0) );
	m_aPens[2].CreatePen(PS_SOLID, 1, RGB(0,0,255) );
	m_aPens[3].CreatePen(PS_SOLID, 1, RGB(0,200,70) );
	m_aPens[4].CreatePen(PS_DASH, 1, RGB(0,0,0) );
	m_aPens[5].CreatePen(PS_DASH, 1, RGB(255,0,0) );
	m_aPens[6].CreatePen(PS_DASH, 1, RGB(0,0,255) );
	m_aPens[7].CreatePen(PS_DASH, 1, RGB(0,255,0) );

	m_aPrintPens[0].CreatePen(PS_SOLID, 1, RGB(0,0,0) );
	m_aPrintPens[1].CreatePen(PS_DASH, 1, RGB(0,0,0) );
	m_aPrintPens[2].CreatePen(PS_DOT, 1, RGB(0,0,0) );
	m_aPrintPens[3].CreatePen(PS_DASHDOT, 1, RGB(0,0,0) );
	m_aPrintPens[4].CreatePen(PS_SOLID, 1, RGB(0,0,0) );
	m_aPrintPens[5].CreatePen(PS_DASH, 1, RGB(0,0,0) );
	m_aPrintPens[6].CreatePen(PS_DOT, 1, RGB(0,0,0) );
	m_aPrintPens[7].CreatePen(PS_DASHDOT, 1, RGB(0,0,0) );
}

CNLSysView::~CNLSysView()
{
	if( m_vecX != NULL )
		delete m_vecX;
	if( m_matY != NULL )
		delete m_matY;
	
	if( m_matEquSol != NULL )
		delete m_matEquSol;
	
	if( m_vecFreq != NULL )
		delete m_vecFreq;
	if( m_matSpectrum != NULL )
		delete m_matSpectrum;

	if( m_vecParVal != NULL )
		delete m_vecParVal;
	if( m_matLyapExp != NULL )
		delete m_matLyapExp;

//	delete m_PlotPar.m_matT;
}

BOOL CNLSysView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNLSysView drawing

void CNLSysView::OnDraw(CDC* pDC)
{
	CNLSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	InitpDC(pDC,0);				// inicijaliziraj device-context (postavljanje origina-a, extent-a, poveæanja)

	PlotAxis(pDC,0);			// nacrtaj koordinatni sistem

	// ovdje æemo malo srediti toolbar
	CMainFrame		*pFrame = (CMainFrame *) AfxGetMainWnd();
	CChildFrame		*pChild = (CChildFrame *) pFrame->MDIGetActive();
	
	CNLSysView		*pView;
	if( pChild != NULL )
		pView = (CNLSysView *) pChild->GetActiveView();
	else
		pView = NULL;

	if( pView != NULL  )
	{
		CToolBarCtrl   *ctrl = &(pFrame->m_wndWindSizeToolBar.GetToolBarCtrl());
		if( pView->m_nPlotType == PHASE_PLOT_3D )
		{
			ctrl->HideButton( ID_ROTATE_LEFT, FALSE );
			ctrl->HideButton( ID_ROTATE_RIGHT, FALSE );
			ctrl->HideButton( ID_ROTATE_UP, FALSE );
			ctrl->HideButton( ID_ROTATE_DOWN, FALSE );
			ctrl->HideButton( ID_PLOT_GET_CLOSER, FALSE );
			ctrl->HideButton( ID_PLOT_GET_FARTHER, FALSE );
			ctrl->HideButton( ID_MOVE_CENTER_UP, FALSE );
			ctrl->HideButton( ID_MOVE_CENTER_DOWN, FALSE );
			ctrl->HideButton( ID_MOVE_CENTER_RIGHT, FALSE );
			ctrl->HideButton( ID_MOVE_CENTER_LEFT, FALSE );
			ctrl->HideButton( ID_MOVE_CENTER_CLOSER, FALSE );
			ctrl->HideButton( ID_MOVE_CENTER_FURTHER, FALSE );
			ctrl->HideButton( ID_PLOT_X_LEFT, TRUE );
			ctrl->HideButton( ID_PLOT_X_RIGHT, TRUE );
			ctrl->HideButton( ID_PLOT_Y_UP, TRUE );
			ctrl->HideButton( ID_PLOT_Y_DOWN, TRUE );
			ctrl->HideButton( ID_PLOT_Y_EXPAND_WINDOW, TRUE );
			ctrl->HideButton( ID_PLOT_Y_CONDENSE_WINDOW, TRUE );
			ctrl->HideButton( ID_PLOT_X_EXPAND, TRUE );
			ctrl->HideButton( ID_PLOT_X_CONDENSE, TRUE );
			ctrl->HideButton( ID_PLOT_EXPAND_BOTH, TRUE );
			ctrl->HideButton( ID_PLOT_CONDENSE_BOTH, TRUE );
		}
		else
		{
			ctrl->HideButton( ID_ROTATE_LEFT, TRUE );
			ctrl->HideButton( ID_ROTATE_RIGHT, TRUE );
			ctrl->HideButton( ID_ROTATE_UP, TRUE );
			ctrl->HideButton( ID_ROTATE_DOWN, TRUE );
			ctrl->HideButton( ID_PLOT_GET_CLOSER, TRUE );
			ctrl->HideButton( ID_PLOT_GET_FARTHER, TRUE );
			ctrl->HideButton( ID_MOVE_CENTER_UP, TRUE );
			ctrl->HideButton( ID_MOVE_CENTER_DOWN, TRUE );
			ctrl->HideButton( ID_MOVE_CENTER_RIGHT, TRUE );
			ctrl->HideButton( ID_MOVE_CENTER_LEFT, TRUE );
			ctrl->HideButton( ID_MOVE_CENTER_CLOSER, TRUE );
			ctrl->HideButton( ID_MOVE_CENTER_FURTHER, TRUE );
			ctrl->HideButton( ID_PLOT_X_LEFT, FALSE );
			ctrl->HideButton( ID_PLOT_X_RIGHT, FALSE );
			ctrl->HideButton( ID_PLOT_Y_UP, FALSE );
			ctrl->HideButton( ID_PLOT_Y_DOWN, FALSE );
			ctrl->HideButton( ID_PLOT_Y_EXPAND_WINDOW, FALSE );
			ctrl->HideButton( ID_PLOT_Y_CONDENSE_WINDOW, FALSE );
			ctrl->HideButton( ID_PLOT_X_EXPAND, FALSE );
			ctrl->HideButton( ID_PLOT_X_CONDENSE, FALSE );
			ctrl->HideButton( ID_PLOT_EXPAND_BOTH, FALSE );
			ctrl->HideButton( ID_PLOT_CONDENSE_BOTH, FALSE );
		}
//		ctrl->UpdateData(FALSE);
		ctrl->AutoSize();
		ctrl->UpdateWindow();
		ctrl->Invalidate();
	}
	
	CChildFrame  *pFrm = (CChildFrame *) GetParentFrame();
	if( pFrm != NULL )
	{
		if( !m_szTitle.IsEmpty() )
			pFrm->SetWindowText(pDoc->GetTitle() + " - " + m_szTitle);
	}
	
	CPoint		pntOrg(10,1);
	pDC->DPtoLP(&pntOrg,1);

	if( m_bCalcDone == FALSE )
		pDC->TextOut( pntOrg.x, pntOrg.y, "Waiting for command !!!         " );
	if( m_bCalcInProgress == TRUE )
		pDC->TextOut( pntOrg.x, pntOrg.y, "Calculating !!!                           " );
	if( !pDoc->IsDefined() )
		pDC->TextOut( pntOrg.x, pntOrg.y, "System is not defined !!!             ");

	if( m_bCalcDone == TRUE && m_bCalcInProgress == FALSE && m_bSystemChanged == FALSE )
	{
		if( m_nPlotType == TRAJECTORY_PLOT )
			DrawTrajectory(pDC);
		if( m_nPlotType == PHASE_PLOT_2D )
			DrawPhase2D(pDC);
		if( m_nPlotType == PHASE_PLOT_3D )
			DrawPhase3D(pDC);
		if( m_nPlotType == EQU_SURFACE_PLOT_2D )
			DrawEquSurface2D(pDC);
		if( m_nPlotType == FOURIER_SPECTRUM_PLOT )
			DrawSpectrum(pDC);
		if( m_nPlotType == LYAP_EXP_PLOT )
			DrawLyapExp(pDC);
	}
}

void CNLSysView::InitpDC(CDC * pDC, int nCalledBy )
{
	CNLSysDoc* pDoc = GetDocument();

	CPen	tmpPen;
	int		nMaxVar=0;

	if( ( m_IntPar.m_bParametersChanged == TRUE || m_PlotPar.m_bViewTypeChanged == TRUE || 
				m_PlotPar.m_bPlotParametersChanged == TRUE || m_PlotPar.m_bXAxisIntervalChanged == TRUE )
			&& m_bCalcDone == TRUE && pDoc->IsDefined() )
	{
		if( m_PlotPar.m_bXAxisIntervalChanged == TRUE )
		{
			GetInterval();
			m_PlotPar.m_bXAxisIntervalChanged = FALSE;
		}

		m_PlotPar.m_dMulY = 1;
	
		m_IntPar.m_bParametersChanged = FALSE;
		m_PlotPar.m_bViewTypeChanged = FALSE;
		m_PlotPar.m_bPlotParametersChanged = FALSE;

		if( m_nPlotType == TRAJECTORY_PLOT  || m_nPlotType == FOURIER_SPECTRUM_PLOT || m_nPlotType == LYAP_EXP_PLOT )
		{
			m_PlotPar.m_dMulX = m_PlotPar.m_nWinExtX / (m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow);
			m_PlotPar.m_dMulY = m_PlotPar.m_nWinExtY / (m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow);
		}
		else if( m_nPlotType == PHASE_PLOT_2D )
		{
			m_PlotPar.m_dMulX = m_PlotPar.m_nWinExtX / (m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow);
			m_PlotPar.m_dMulY = m_PlotPar.m_nWinExtY / (m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow);
		}
		else if( m_nPlotType == PHASE_PLOT_3D )
		{
			CPointDouble	trP;
			double	p[8][4], xMax, yMax, x, y, z;
			int			i, nXvar, nYvar, nZvar;

			nXvar = m_PlotPar.m_anSysVarsOrder[0];
			nYvar = m_PlotPar.m_anSysVarsOrder[1];
			nZvar = m_PlotPar.m_anSysVarsOrder[2];

			x = m_AnalPar.m_adMaxY[nXvar] - GetX(*m_PlotPar.pointLookTo);
			y = m_AnalPar.m_adMaxY[nYvar] - GetY(*m_PlotPar.pointLookTo);
			z = m_AnalPar.m_adMaxY[nZvar] - GetZ(*m_PlotPar.pointLookTo);
			m_PlotPar.m_dRho = 1.6 * sqrt(x*x + y*y + z*z);

			x = GetX(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * cos(m_PlotPar.m_dAlpha * 3.141/180);
			y = GetY(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * sin(m_PlotPar.m_dAlpha * 3.141/180);
			z = GetZ(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * sin(m_PlotPar.m_dTheta * 3.141/180);
			
			Point3D		pointEye(x,y,z);
			*m_PlotPar.m_matT = GetProjectionMatrix(pointEye, *m_PlotPar.pointLookTo);

			for( i=0; i<8; i++ )
			{
				if( i<2 || i==4 || i==5 )
					p[i][0] = m_AnalPar.m_adMaxY[nXvar];		// x-koord
				else
					p[i][0] = m_AnalPar.m_adMinY[nXvar];		// x-koord
				
				if( i==1 || i==2 || i==5 || i==6 )
					p[i][1] = m_AnalPar.m_adMaxY[nYvar];		// y-koord
				else
					p[i][1] = m_AnalPar.m_adMinY[nYvar];		// y-koord
				
				if( i<4 )
					p[i][2] = m_AnalPar.m_adMaxY[nZvar];		// z-koord
				else
					p[i][2] = m_AnalPar.m_adMinY[nZvar];		// z-koord

				p[i][3] = 1.0;
			}

			xMax = 0;
			yMax = 0;
			
			for( i=0; i<8; i++ )
			{
				trP = TransfToDoublePoint( p[i], m_PlotPar.m_matT );
				if( fabs(trP.x) > xMax )
					xMax = fabs(trP.x);
				if( fabs(trP.y) > yMax )
					yMax = fabs(trP.y);
			}
//			m_PlotPar.m_dXLow = 1;
			m_PlotPar.m_dMulX = m_PlotPar.m_nWinExtX / 1.5 / xMax;
			m_PlotPar.m_dMulY = m_PlotPar.m_nWinExtY / 1.5 / yMax;

			m_PlotPar.m_dStartMulX = m_PlotPar.m_dMulX;
			m_PlotPar.m_dStartMulY = m_PlotPar.m_dMulY;
		}
		else if( m_nPlotType ==	EQU_SURFACE_PLOT_2D )
		{
			m_PlotPar.m_dMulX = m_PlotPar.m_nWinExtX / (m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow);
			m_PlotPar.m_dMulY = m_PlotPar.m_nWinExtY / (m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow);
		}
	}
	
	if( !pDC->IsPrinting() )
	{
		m_PlotPar.m_dPrintMulX = 1;
		m_PlotPar.m_dPrintMulY = 1;
	}
	else
	{
		m_PlotPar.m_dPrintMulX = 2.5;
		m_PlotPar.m_dPrintMulY = 2.5;
	}

	CRect rect;
	GetClientRect(&rect);  // Get size of viewport

	pDC->SetMapMode(MM_ANISOTROPIC);

	pDC->SetWindowExt( m_PlotPar.m_nWinExtX, -m_PlotPar.m_nWinExtY );
	// ako je funkcija pozvana od ServerItem, tada ne postavljaj extent od viewporta
	if( nCalledBy != 1 )
		pDC->SetViewportExt( rect.Width(), rect.Height() );
	
	if( m_nPlotType == PHASE_PLOT_3D )
	{
		m_PlotPar.m_dXOrg =  m_PlotPar.m_nWinExtX/2;
		m_PlotPar.m_dYOrg =  m_PlotPar.m_nWinExtY/2;
		pDC->SetWindowOrg((int) (-m_PlotPar.m_dXOrg * m_PlotPar.m_dPrintMulX), (int) (m_PlotPar.m_dYOrg * m_PlotPar.m_dPrintMulY) );
	}
	else if( m_nPlotType == TRAJECTORY_PLOT || m_nPlotType == PHASE_PLOT_2D ||
			m_nPlotType == FOURIER_SPECTRUM_PLOT || m_nPlotType == LYAP_EXP_PLOT || m_nPlotType == EQU_SURFACE_PLOT_2D )
	{
		// postavi ishodiste
		m_PlotPar.m_dXOrg =  m_PlotPar.m_nWinExtX/2 - (m_PlotPar.m_dXUpp + m_PlotPar.m_dXLow)/2 * m_PlotPar.m_nWinExtX / (m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow);
		m_PlotPar.m_dYOrg =  m_PlotPar.m_nWinExtY/2 + (m_PlotPar.m_dYUpp + m_PlotPar.m_dYLow)/2 * m_PlotPar.m_nWinExtY / (m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow);

		pDC->SetWindowOrg((int) (-m_PlotPar.m_dXOrg * m_PlotPar.m_dPrintMulX), (int) (m_PlotPar.m_dYOrg * m_PlotPar.m_dPrintMulY) );
	}
}

void CNLSysView::PlotAxis( CDC *pDC, int nCalledBy )
{
	CNLSysDoc* pDoc = GetDocument();

	CPen		*pPrevPen, tmpPen;
	CPoint	pntCenter;
	double	x,y, mulx, muly;
	char		str[100];
	int			i, retCenter;
	int			X0, Y0;				// koordinate centra koordinatnog sustav (tj. sjecišta osi)

	if( m_nPlotType == TRAJECTORY_PLOT || m_nPlotType == FOURIER_SPECTRUM_PLOT ||
			m_nPlotType == PHASE_PLOT_2D || m_nPlotType == EQU_SURFACE_PLOT_2D ||
			m_nPlotType == LYAP_EXP_PLOT )
	{
		int			x0, y0, x1, y1, px, py;
		x0 = (int) (m_PlotPar.m_dXLow * m_PlotPar.m_dMulX);
		x1 = (int) (m_PlotPar.m_dXUpp * m_PlotPar.m_dMulX);
		y0 = (int) (m_PlotPar.m_dYLow * m_PlotPar.m_dMulY);
		y1 = (int) (m_PlotPar.m_dYUpp * m_PlotPar.m_dMulY);
		
		tmpPen.CreatePen(PS_SOLID, 10, RGB(0,0,0) );
		pPrevPen = pDC->SelectObject(&tmpPen);

		retCenter = CenterWithinWindow(pntCenter);
		if( retCenter == CENTER_WITHIN_WINDOW)
		{
			x = m_PlotPar.m_dXLow;
			y = m_PlotPar.m_dYLow;
			X0 = 0;
			Y0 = 0;
			mulx = m_PlotPar.m_dMulX;
			muly = m_PlotPar.m_dMulY;

			pDC->MoveTo(0, (int) (m_PlotPar.m_dYLow * m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY) );
			pDC->LineTo(0, (int) (m_PlotPar.m_dYUpp * m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY) );

			pDC->MoveTo((int) (m_PlotPar.m_dXLow * m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX), 0);
			pDC->LineTo((int) (m_PlotPar.m_dXUpp * m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX), 0);

		}
		else if( retCenter == X_AXIS_WITHIN_WINDOW )
		{
			x = m_PlotPar.m_dXLow * m_PlotPar.m_dMulX + 8;
			y = 0;
			X0 = (int) x;
			Y0 = 0;

			pDC->MoveTo((int) x, (int) (m_PlotPar.m_dYLow * m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY) );
			pDC->LineTo((int) x, (int) (m_PlotPar.m_dYUpp * m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY) );
	
			pDC->MoveTo((int) (m_PlotPar.m_dXLow * m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX), 0);
			pDC->LineTo((int) (m_PlotPar.m_dXUpp * m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX), 0);
		}
		else if( retCenter == Y_AXIS_WITHIN_WINDOW )
		{
			x = 0;
			y = m_PlotPar.m_dYUpp * m_PlotPar.m_dMulY - 8;
			X0 = 0;
			Y0 = (int) y;

			pDC->MoveTo((int) (m_PlotPar.m_dXLow * m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX), (int) y);
			pDC->LineTo((int) (m_PlotPar.m_dXUpp * m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX), (int) y);

			pDC->MoveTo(0, (int) (m_PlotPar.m_dYLow * m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY) );
			pDC->LineTo(0, (int) (m_PlotPar.m_dYUpp * m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY) );
		}
		else
		{
			x = m_PlotPar.m_dXLow * m_PlotPar.m_dMulX + 8;
			y = m_PlotPar.m_dYUpp * m_PlotPar.m_dMulY - 8;
			X0 = (int) x;
			Y0 = (int) y;

			pDC->MoveTo((int) x, (int) (m_PlotPar.m_dYLow * m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY) );
			pDC->LineTo((int) x, (int) (m_PlotPar.m_dYUpp * m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY) );
			
			pDC->MoveTo((int) (m_PlotPar.m_dXLow * m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX), (int) y);
			pDC->LineTo((int) (m_PlotPar.m_dXUpp * m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX), (int) y);
		}

		mulx = m_PlotPar.m_dMulX;
		muly = m_PlotPar.m_dMulY;

		double	dx, dy, SS, SSy, Start, Starty, dist, disty, val;
		char		szFormat[20];
		int			num, addmulx, order;

		CFont		tmpFont, *pPrevFont;
		
		if( nCalledBy == 0 )
		{
			tmpFont.CreateFont(100, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
							OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
			addmulx = 1;
		}
		else
		{
			tmpFont.CreateFont(200, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
							OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
			addmulx = 2;
		}
	
		dx = m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow;
		SS = pow( 10, floor(log10(fabs(dx))) );
		Start = SS * floor( ( m_PlotPar.m_dXLow + SS * 0.95 ) / SS );

		if( 2*SS > dx )
		{
			dist = SS / 4;
			num = 10;
			Start = SS * floor((m_PlotPar.m_dXLow + dist) / SS );
		}
		else
		{
			dist = SS;
			num = (int) (floor(dx/SS)) + 1;
		}

		for( i=0; i<num; i++ )
		{
			val = Start + i*dist;
			if( val > m_PlotPar.m_dXLow && val < m_PlotPar.m_dXUpp )
			{
				order = (int) floor(log10(fabs(val)));
				memset( szFormat, 0, 20 );
				if( order > 2 || order < -2 )
					sprintf(szFormat, "%%e" );
				else
					sprintf(szFormat, "%%4.%df", -order+2 );
		
				px = (int) ((val) * mulx);
				py = (int) Y0;
				pDC->MoveTo( px, py-25 );
				pDC->LineTo( px, py+25 );
				sprintf(str, szFormat, (float) val );

				if( val == 0  && (retCenter == Y_AXIS_WITHIN_WINDOW || retCenter == CENTER_WITHIN_WINDOW) )
					;
				else
				{
					pPrevFont = pDC->SelectObject(&tmpFont);
					pDC->TextOut( px-50*addmulx, py-40, str );
					pDC->SelectObject(pPrevFont);
				}
			}
		}

		dy = m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow;
		SSy = pow( 10, floor(log10(fabs(dy))) );
		Starty = SSy * floor( ( m_PlotPar.m_dYLow + SSy * 0.95 ) / SSy );

		if( 2*SSy > dy )
		{
			disty = SSy / 4;
			num = 10;
			Starty = SSy * floor((m_PlotPar.m_dYLow + disty) / SSy );
		}
		else
		{
			disty = SSy;
			num = (int) (floor(dy/SSy)) + 1;
		}

		for( i=0; i<num; i++ )
		{
			val = Starty + i*disty;
			if( val > m_PlotPar.m_dYLow && val < m_PlotPar.m_dYUpp )
			{
				order = (int) floor(log10(fabs(val)));
				memset( szFormat, 0, 20 );
				if( order > 2 || order < -2 )
					sprintf(szFormat, "%%e" );
				else
					sprintf(szFormat, "%%4.%df", -order+2 );

				px = (int) X0;
				py = (int) ((val) * muly);
				pDC->MoveTo( px-20, py );
				pDC->LineTo( px+20, py );
				sprintf(str, szFormat, (float) val );

				if( val == 0  && (retCenter == X_AXIS_WITHIN_WINDOW || retCenter == CENTER_WITHIN_WINDOW) )
					;
				else
				{
					pPrevFont = pDC->SelectObject(&tmpFont);
					pDC->TextOut( px+50, py + 100*addmulx/2, str );
					pDC->SelectObject(pPrevFont);
				}
			}
		}

		pDC->SelectObject(pPrevPen);

		if( m_nPlotType == EQU_SURFACE_PLOT_2D )
		{
			if( m_PlotPar.m_bShowBifInEqu == TRUE )
			{
				CPoint		print(0,0);
				
				print = m_pntBifLegendPos;
				if( !pDoc->IsEmbedded() )
					pDC->DPtoLP(&print);
				else if( pDoc->IsInPlaceActive() )
				{
//					char		str[500];
//					sprintf(str, "ja in-place (fizicke) %d , %d", print.x, print.y );
//					AfxMessageBox(str);
					pDC->DPtoLP(&print);
/*					sprintf(str, "ja in-place (logicke) %d , %d", print.x, print.y );
					AfxMessageBox(str);
					print.x = 18200;
					print.y = 5500;*/
				}
/*				else
				{
//					AfxMessageBox("ja embeded");
				}*/
			
				CFont	*pPrevFont, aFonts[2];
				CPen	*pPrevPen, tmpPen;
				int		nLegendEntriesNum=0, i;
				int		muly = 1;

				tmpPen.CreatePen(PS_SOLID, 8, RGB(0,0,0) );
				pPrevPen = pDC->SelectObject(&tmpPen);

				if( nCalledBy == 0 )
				{
					aFonts[0].CreateFont(100, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 
						OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
					aFonts[1].CreateFont(90, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
						OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
					
					muly = 1;
				}
				else
				{
					aFonts[0].CreateFont(200, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 
						OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
					aFonts[1].CreateFont(180, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
						OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
					
					muly = 2;
				}

				int		nX1, nY1, nX2, nY2, nLegendMaxLength;
				nLegendEntriesNum=2;
				nLegendMaxLength = 23;
				
				nX1 = (int) ((print.x-10*muly)*m_PlotPar.m_dPrintMulX);
				nY1 = (int) ((print.y+10*muly)*m_PlotPar.m_dPrintMulY);
				nX2 = nX1 + (int) ((200+nLegendMaxLength*27*muly) * m_PlotPar.m_dPrintMulX);
				nY2 = nY1 - (int) ((110*muly+100*nLegendEntriesNum*muly) * m_PlotPar.m_dPrintMulY);
				
				pDC->SelectObject(&tmpPen);
				pDC->Rectangle(nX1, nY1, nX2, nY2 );

				pPrevFont = pDC->SelectObject(&aFonts[0]);
				pDC->TextOut((int) ((print.x+20*muly)*m_PlotPar.m_dPrintMulX), (int) (print.y*m_PlotPar.m_dPrintMulY ), "Legend:");
				
				pDC->SelectObject(&aFonts[1]);

				CSize		WExt, VExt;
				int			sizeY;

				WExt = pDC->GetWindowExt();
				VExt = pDC->GetViewportExt();
				sizeY = (int) (floor(((double) WExt.cy * 15 *muly) / VExt.cy * VExt.cx / WExt.cx));

				i = 0;
				nX1 = (int) ((print.x+40*muly)*m_PlotPar.m_dPrintMulX);
				nY1 = (int) ((print.y-100*(i+1)*muly-42*muly)*m_PlotPar.m_dPrintMulY);
				
				CBrush		pBrush;
				pBrush.CreateSolidBrush(RGB(0,0,0));
				pDC->SelectObject(pBrush);

				pDC->Rectangle(nX1-15*muly, nY1-sizeY, nX1+15*muly, nY1+sizeY);
				pDC->TextOut((int) ((print.x+80*muly)*m_PlotPar.m_dPrintMulX), (int) ((print.y-100*(i+1)*muly)*m_PlotPar.m_dPrintMulY), "Hopf bifurcation" );

				i = 1;
				nX1 = (int) ((print.x+40*muly)*m_PlotPar.m_dPrintMulX);
				nY1 = (int) ((print.y-100*(i+1)*muly-42*muly)*m_PlotPar.m_dPrintMulY);
				
				pDC->Ellipse(nX1-15*muly, nY1-sizeY, nX1+15*muly, nY1+sizeY);
				pDC->TextOut((int) ((print.x+80*muly)*m_PlotPar.m_dPrintMulX), (int) ((print.y-100*(i+1)*muly)*m_PlotPar.m_dPrintMulY), "Saddle-node bifurcation" );
			}
		}
		else if( m_nPlotType == TRAJECTORY_PLOT || m_nPlotType == FOURIER_SPECTRUM_PLOT )
		{
			CPoint		print(0,0);
			
			print = m_pntLegendPos;
			pDC->DPtoLP(&print);
		
			CFont	*pPrevFont, aFonts[2];
			CPen	*pPrevPen, tmpPen;
			int		nLegendEntriesNum=0, i;

			tmpPen.CreatePen(PS_SOLID, 8, RGB(0,0,0) );
			pPrevPen = pDC->SelectObject(&tmpPen);

			if( nCalledBy == 0 )
			{
				aFonts[0].CreateFont(100, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 
					OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
				aFonts[1].CreateFont(90, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
					OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
			}
			else
			{
				aFonts[0].CreateFont(250, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, 
					OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
				aFonts[1].CreateFont(200, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
					OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
			}
			
			// ispisi legendu
			if( m_PlotPar.m_bShowLegend == TRUE )
			{
				nLegendEntriesNum=0;

				int		nX1, nY1, nX2, nY2, nLegendMaxLength, nTmp;
				
				nLegendMaxLength = 0;
				for( i=0; i<4; i++ )
				{
					if( !m_aszLegend[i].IsEmpty() )
					{
						nLegendEntriesNum++;
						if( (nTmp=m_aszLegend[i].GetLength()) > nLegendMaxLength )
							nLegendMaxLength = nTmp;

						if( nLegendMaxLength < 5 )
							// ovo je zato što se rijeè Legenda ispisuje dijelom 
							//izvan pravokutnika, kada su imena varijabli suviše kratka
							nLegendMaxLength = 5;
					}
				}
				if( nLegendEntriesNum > 0 )
				{
					nX1 = (int) ((print.x-10)*m_PlotPar.m_dPrintMulX);
					nY1 = (int) ((print.y+10)*m_PlotPar.m_dPrintMulY);
					nX2 = nX1 + (int) ((200+nLegendMaxLength*27) * m_PlotPar.m_dPrintMulX);
					nY2 = nY1 - (int) ((130+90*nLegendEntriesNum) * m_PlotPar.m_dPrintMulY);
					
					pDC->SelectObject(&tmpPen);
					pDC->Rectangle(nX1, nY1, nX2, nY2 );

					pPrevFont = pDC->SelectObject(&aFonts[0]);
					pDC->TextOut((int) ((print.x+20)*m_PlotPar.m_dPrintMulX), (int) (print.y*m_PlotPar.m_dPrintMulY), "Legenda :");
					
					pDC->SelectObject(&aFonts[1]);
					for( i=0; i<4; i++ )
					{
						if( !m_aszLegend[i].IsEmpty() )
						{
							if( pDoc->m_Options.m_bUseColor )
								pDC->SelectObject(&m_aPens[i]);
							else
								pDC->SelectObject(&m_aPrintPens[i]);

							pDC->MoveTo((int) ((print.x+20)*m_PlotPar.m_dPrintMulX),(int) ((print.y-90*(i+1)-45)*m_PlotPar.m_dPrintMulY) );
							pDC->LineTo((int) ((print.x+100)*m_PlotPar.m_dPrintMulX),(int) ((print.y-90*(i+1)-45)*m_PlotPar.m_dPrintMulY) );
							pDC->TextOut((int) ((print.x+155)*m_PlotPar.m_dPrintMulX), (int) ((print.y-90*(i+1))*m_PlotPar.m_dPrintMulY), m_aszLegend[i] );
						}
					}
					pDC->SelectObject(pPrevFont);
				}
			}
		}
	}
  else if( m_nPlotType == PHASE_PLOT_3D )
	{
		CPointDouble	CoordAxis[4];
		double	p[4], xMax;
		int			i;

		xMax = m_PlotPar.m_nWinExtX / 2 / m_PlotPar.m_dMulX;

		
		p[0]=GetX(*m_PlotPar.pointLookTo); p[1]=GetY(*m_PlotPar.pointLookTo); p[2]=GetZ(*m_PlotPar.pointLookTo); p[3]=1;
		CoordAxis[0] = TransfToDoublePoint( p, m_PlotPar.m_matT );
		
		p[0]=GetX(*m_PlotPar.pointLookTo)+xMax/2; p[1]=GetY(*m_PlotPar.pointLookTo); p[2]=GetZ(*m_PlotPar.pointLookTo); p[3]=1;
		CoordAxis[1] = TransfToDoublePoint( p, m_PlotPar.m_matT );
		
		p[0]=GetX(*m_PlotPar.pointLookTo); p[1]=GetY(*m_PlotPar.pointLookTo)+xMax/2; p[2]=GetZ(*m_PlotPar.pointLookTo); p[3]=1;
		CoordAxis[2] = TransfToDoublePoint( p, m_PlotPar.m_matT );

		p[0]=GetX(*m_PlotPar.pointLookTo); p[1]=GetY(*m_PlotPar.pointLookTo); p[2]=GetZ(*m_PlotPar.pointLookTo)+xMax/5; p[3]=1;
		CoordAxis[3] = TransfToDoublePoint( p, m_PlotPar.m_matT );

		tmpPen.CreatePen(PS_SOLID, 8, RGB(0,0,0) );
		pPrevPen = pDC->SelectObject(&tmpPen);

		for( i=1; i<4; i++ )
		{
			pDC->MoveTo( (int) CoordAxis[0].x, (int) CoordAxis[0].y );
			pDC->LineTo( (int) (CoordAxis[i].x * m_PlotPar.m_dMulX), (int) (CoordAxis[i].y * m_PlotPar.m_dMulY) );
		}

		const char *name;
		CFont		tmpFont, *pPrevFont;
		
		if( nCalledBy == 0 )
			tmpFont.CreateFont(100, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
							OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );
		else
			tmpFont.CreateFont(200, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
							OUT_TT_PRECIS, CLIP_LH_ANGLES, DEFAULT_QUALITY, DEFAULT_PITCH , "Ariel" );

		pPrevFont = pDC->SelectObject(&tmpFont);

		name = pDoc->m_DES.GetVarName(m_PlotPar.m_anPlottingVars[0]);
		pDC->TextOut( (int) (CoordAxis[1].x * m_PlotPar.m_dMulX)+10, (int) (CoordAxis[1].y * m_PlotPar.m_dMulY)+10, name );
		
		name = pDoc->m_DES.GetVarName(m_PlotPar.m_anPlottingVars[1]);
		pDC->TextOut( (int) (CoordAxis[2].x * m_PlotPar.m_dMulX)+10, (int) (CoordAxis[2].y * m_PlotPar.m_dMulY)+10, name );

		name = pDoc->m_DES.GetVarName(m_PlotPar.m_anPlottingVars[2]);
		pDC->TextOut( (int) (CoordAxis[3].x * m_PlotPar.m_dMulX)+10, (int) (CoordAxis[3].y * m_PlotPar.m_dMulY)+10, name );

		pDC->SelectObject(pPrevFont);

		// a sada definiramo koordinate za pravokutnik koji æe se iscrtavati u x-y ravnini
		p[0]=GetX(*m_PlotPar.pointLookTo)+xMax/2; p[1]=GetY(*m_PlotPar.pointLookTo)-xMax/2; p[2]=GetZ(*m_PlotPar.pointLookTo); p[3]=1;
		CoordAxis[0] = TransfToDoublePoint( p, m_PlotPar.m_matT );
		p[0]=GetX(*m_PlotPar.pointLookTo)+xMax/2; p[1]=GetY(*m_PlotPar.pointLookTo)+xMax/2; p[2]=GetZ(*m_PlotPar.pointLookTo); p[3]=1;
		CoordAxis[1] = TransfToDoublePoint( p, m_PlotPar.m_matT );
		p[0]=GetX(*m_PlotPar.pointLookTo)-xMax/2; p[1]=GetY(*m_PlotPar.pointLookTo)+xMax/2; p[2]=GetZ(*m_PlotPar.pointLookTo); p[3]=1;
		CoordAxis[2] = TransfToDoublePoint( p, m_PlotPar.m_matT );
		p[0]=GetX(*m_PlotPar.pointLookTo)-xMax/2; p[1]=GetY(*m_PlotPar.pointLookTo)-xMax/2; p[2]=GetZ(*m_PlotPar.pointLookTo); p[3]=1;
		CoordAxis[3] = TransfToDoublePoint( p, m_PlotPar.m_matT );

		pDC->MoveTo( (int) (CoordAxis[0].x * m_PlotPar.m_dMulX), (int) (CoordAxis[0].y * m_PlotPar.m_dMulY) );
		for( i=1; i<4; i++ )
			pDC->LineTo( (int) (CoordAxis[i].x * m_PlotPar.m_dMulX), (int) (CoordAxis[i].y * m_PlotPar.m_dMulY) );
		pDC->LineTo( (int) (CoordAxis[0].x * m_PlotPar.m_dMulX), (int) (CoordAxis[0].y * m_PlotPar.m_dMulY) );
		
		pDC->SelectObject(pPrevPen);
	}
}

// da li se ishodište nalazi unutar prozora ( da li se može iscrtati njegova pozicija na ekranu)
//	ukoliko se samo x (ili y ) os nalazi unutar prozora, tada vraæa koordinatu osi u pripadnoj koordinat pntOrigin
//	unutar prozora znaèi unutar 90 % unutrašnjosti prozora ( znaèi ukoliko je ishodište na samom 
//	rubu ekrana, uzima se kao da nije unutar prozora
int CNLSysView::CenterWithinWindow( CPoint &pntOrigin )
{
	BOOL		retX, retY;

	if( m_PlotPar.m_dXUpp > 0 && m_PlotPar.m_dXLow < 0 )
	{
		retY = TRUE;			// znaèi da je Y-os vidljiva u prozoru
		pntOrigin.x = 0;
	}
	else
		retY = FALSE;

	if( m_PlotPar.m_dYUpp > 0 && m_PlotPar.m_dYLow < 0 )
	{
		retX = TRUE;
		pntOrigin.y = 0;
	}
	else
		retX = FALSE;

	if( retX == TRUE && retY == TRUE )
		return CENTER_WITHIN_WINDOW;
	else if( retX == TRUE )
		return X_AXIS_WITHIN_WINDOW;
	else if( retY == TRUE )
		return Y_AXIS_WITHIN_WINDOW;
	else
		return NO_AXIS_WITHIN_WINDOW;
}

void CNLSysView::GetInterval( void )
{
	CNLSysDoc* pDoc = GetDocument();

	int			i, nSteps, k, nVar;
	double	dMaxVal, dMinVal, dMax=-1e30, dMin=1e30, div;

	// najprije analiziramo podatke dobivene integracijom sistema
	nSteps = m_PlotPar.m_nRealCalcNum-1;
	for( k=0; k<10; k++ )
		m_AnalPar.m_adMinY[k] = m_AnalPar.m_adMaxY[k] = 0;

	if( m_bCalcDone == TRUE )
	{
		// a sada æemo naæi maksimalne vrijednosti za y[i], kako bi
		// se znalo u kojim æe se granicama iscrtavati
		if( m_nPlotType == FOURIER_SPECTRUM_PLOT )
		{
			nSteps = m_PlotPar.m_nRealCalcNum;

			for( k=0; k<MAX_PLOTTING_VARS; k++ )
				if( m_PlotPar.m_anPlottingVars[k] != -1 )
				{
					nVar = m_PlotPar.m_anSysVarsOrder[k];

					dMinVal = 1e30;
					dMaxVal = -1e30;
					for( i=0; i<nSteps; i++ )
					{
						if( (*m_matSpectrum)[nVar][i] < dMinVal )
							dMinVal = (*m_matSpectrum)[nVar][i];
						if( (*m_matSpectrum)[nVar][i] > dMaxVal )
							dMaxVal = (*m_matSpectrum)[nVar][i];
					}
					m_SpecAnal.m_adMinY[nVar] = dMinVal;
					m_SpecAnal.m_adMaxY[nVar] = dMaxVal;

					if( dMinVal < dMin )
					{
						dMin = dMinVal;
						m_SpecAnal.m_nMinVar = nVar;
					}
					if( dMaxVal > dMax )
					{
						dMax = dMaxVal;
						m_SpecAnal.m_nMaxVar = nVar;
					}
				}
		}
		else if( m_nPlotType == EQU_SURFACE_PLOT_2D && m_EquAnalPar.m_bEquSurfaceDataAvailable == TRUE)
		{
			// a sada æemo analizirati eventualne parametre ravnotežne krivulje
			for( k=0; k<3; k++ )
			{
				dMaxVal = -1e30;
				dMinVal = 1e30;
				for( i=0; i<m_EquAnalPar.m_nEquSurfaceDataNum; i++ )
				{
					if( (*m_matEquSol)[k][i] < dMinVal )
						dMinVal = (*m_matEquSol)[k][i];
					if( (*m_matEquSol)[k][i] > dMaxVal )
						dMaxVal = (*m_matEquSol)[k][i];
				}
			
				m_EquAnalPar.m_adMinEqu[k] = dMinVal;
				m_EquAnalPar.m_adMaxEqu[k] = dMaxVal;
			}
		}
		else if( m_nPlotType == LYAP_EXP_PLOT )
		{
			m_LyapAnal.m_nMinVar = 0;
			m_LyapAnal.m_nMaxVar = 0;
			for( k=0; k<pDoc->m_DES.GetDim(); k++ )
			{
				dMinVal = 1e30;
				dMaxVal = -1e30;
				for( i=0; i<m_PlotPar.m_nRealCalcNum; i++ )
				{
					if( (*m_vecParVal)[i] > m_PlotPar.m_dStartPlot && (*m_vecParVal)[i] < m_PlotPar.m_dEndPlot )
					{
						if( (*m_matLyapExp)[k][i] < dMinVal )
							dMinVal = (*m_matLyapExp)[k][i];
						if( (*m_matLyapExp)[k][i] > dMaxVal )
							dMaxVal = (*m_matLyapExp)[k][i];
					}
				}
				m_LyapAnal.m_adMinY[k] = dMinVal;
				m_LyapAnal.m_adMaxY[k] = dMaxVal;

				if( dMinVal < dMin )
				{
					dMin = dMinVal;
					m_LyapAnal.m_nMinVar = k;
				}
				if( dMaxVal > dMax )
				{
					dMax = dMaxVal;
					m_LyapAnal.m_nMaxVar = k;
				}
			}
		}
		else if( m_nPlotType == TRAJECTORY_PLOT || m_nPlotType == PHASE_PLOT_2D || m_nPlotType == PHASE_PLOT_3D )
		{
			if( m_nPlotType == PHASE_PLOT_3D )
			{
				// ako crtamo 3D faznu krivulju, tada želimo da nam na poèetku pogled bude uperen toèno
				// u središte krivulje, što znaèi da treba naæi aritmetièku sredinu za sve tri varijable
				// i staviti je u pointLookTo
				{
					int			start_i, anInd[3];
					double	dSum, ar[3];

					anInd[0] = m_PlotPar.m_anSysVarsOrder[0];
					anInd[1] = m_PlotPar.m_anSysVarsOrder[1];
					anInd[2] = m_PlotPar.m_anSysVarsOrder[2];
					
					for( i=0; i<nSteps; i++ )
						if( (*m_vecX)[i] >= m_PlotPar.m_dStartPlot )
						{
							start_i = i;
							break;
						}

					for( k=0; k<3; k++ )
					{
						dSum = 0;
						for( i=start_i; i<nSteps; i++ )
							dSum += (*m_matY)[anInd[k]][i];
						
						ar[k] = dSum / (nSteps - start_i);
					}

					Point3D	temp( ar[0], ar[1], ar[2] );
					*(m_PlotPar.pointLookTo) = temp;
				}
			}

			if( m_PlotPar.m_nParNumPlot == PlotParameters::SINGLE_PAR )
			{
				if(  m_vecX != NULL && m_matY != NULL )
				{
					for( k=0; k<pDoc->m_DES.GetDim(); k++ )
					{
						dMinVal = 1e30;
						dMaxVal = -1e30;
						for( i=0; i<nSteps; i++ )
						{
							if( (*m_vecX)[i] > m_PlotPar.m_dStartPlot && (*m_vecX)[i] < m_PlotPar.m_dEndPlot)
							{
								if( (*m_matY)[k][i] < dMinVal )
									dMinVal = (*m_matY)[k][i];
								if( (*m_matY)[k][i] > dMaxVal )
									dMaxVal = (*m_matY)[k][i];
							}
						}
						m_AnalPar.m_adMinY[k] = dMinVal;
						m_AnalPar.m_adMaxY[k] = dMaxVal;

						div = dMaxVal - dMinVal;
						m_AnalPar.m_adMagY[k] = m_PlotPar.m_nWinExtY / div;

						if( IsPlottingVar(k) == TRUE )
						{
							if( dMinVal < dMin )
							{
								dMin = dMinVal;
								m_AnalPar.m_nMinVar = k;
							}
							if( dMaxVal > dMax )
							{
								dMax = dMaxVal;
								m_AnalPar.m_nMaxVar = k;
							}
						}
					}
				}
				else
				{
					// ovdje nismo smjeli doæi, tražimo GetInterval a vecX i matY nisu def.
					ASSERT(0);
				}
			}
			else
			{
				// ovdje je GetInterval za višeparametarski sluèaj
				dMinVal = 1e30;
				dMaxVal = -1e30;
				for( k=0; k<m_IntPar.m_nNumOfChgPar; k++ )
				{
					if( m_apvecX[k] != NULL && m_apmatY[k] != NULL )
					{
						int var = m_PlotPar.m_anSysVarsOrder[0];
						for( i=0; i<m_IntPar.m_nIntSteps[k]; i++ )
						{
							if( (*m_apvecX[k])[i] > m_PlotPar.m_dStartPlot && (*m_apvecX[k])[i] < m_PlotPar.m_dEndPlot)
							{
								if( (*m_apmatY[k])[var][i] < dMinVal )
									dMinVal = (*m_apmatY[k])[var][i];
								if( (*m_apmatY[k])[var][i] > dMaxVal )
									dMaxVal = (*m_apmatY[k])[var][i];
							}
						}
						m_AnalPar.m_adMinY[k] = dMinVal;
						m_AnalPar.m_adMaxY[k] = dMaxVal;

						if( IsPlottingVar(k) == TRUE )
						{
							if( dMinVal < dMin )
							{
								dMin = dMinVal;
								m_AnalPar.m_nMinVar = k;
							}
							if( dMaxVal > dMax )
							{
								dMax = dMaxVal;
								m_AnalPar.m_nMaxVar = k;
							}
						}
					}
				}
			}
		}
	}

	if( m_nPlotType == TRAJECTORY_PLOT  || m_nPlotType == FOURIER_SPECTRUM_PLOT || m_nPlotType == LYAP_EXP_PLOT )
	{
		m_PlotPar.m_dXUpp = m_PlotPar.m_dEndPlot;
		m_PlotPar.m_dXLow = m_PlotPar.m_dStartPlot;

		if( m_nPlotType == TRAJECTORY_PLOT )
		{
			m_PlotPar.m_dYUpp = m_AnalPar.m_adMaxY[m_AnalPar.m_nMaxVar];
			m_PlotPar.m_dYLow = m_AnalPar.m_adMinY[m_AnalPar.m_nMinVar];
		}
		else if( m_nPlotType == LYAP_EXP_PLOT )
		{
			m_PlotPar.m_dYUpp = m_LyapAnal.m_adMaxY[m_LyapAnal.m_nMaxVar];
			m_PlotPar.m_dYLow = m_LyapAnal.m_adMinY[m_LyapAnal.m_nMinVar];
		}
		else
		{
			m_PlotPar.m_dXUpp = (*m_vecFreq)[m_PlotPar.m_nRealCalcNum-1];
			m_PlotPar.m_dXLow = 0;
			m_PlotPar.m_dYUpp = m_SpecAnal.m_adMaxY[m_SpecAnal.m_nMaxVar];
			m_PlotPar.m_dYLow = m_SpecAnal.m_adMinY[m_SpecAnal.m_nMinVar];
		}

		m_PlotPar.m_dStartXUpp = m_PlotPar.m_dXUpp;
		m_PlotPar.m_dStartXLow = m_PlotPar.m_dXLow;
		m_PlotPar.m_dStartYUpp = m_PlotPar.m_dYUpp;
		m_PlotPar.m_dStartYLow = m_PlotPar.m_dYLow;

		m_PlotPar.m_dStartMulX = m_PlotPar.m_nWinExtX / (m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow);
		m_PlotPar.m_dStartMulY = m_PlotPar.m_nWinExtY / (m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow);
	}
	else if( m_nPlotType == PHASE_PLOT_2D )
	{
		int		Xvar, Yvar;
		Xvar = m_PlotPar.m_anSysVarsOrder[0];
		Yvar = m_PlotPar.m_anSysVarsOrder[1];
		
		m_PlotPar.m_dXUpp = m_AnalPar.m_adMaxY[Xvar];
		m_PlotPar.m_dXLow = m_AnalPar.m_adMinY[Xvar];

		m_PlotPar.m_dYUpp = m_AnalPar.m_adMaxY[Yvar];
		m_PlotPar.m_dYLow = m_AnalPar.m_adMinY[Yvar];

		m_PlotPar.m_dStartXUpp = m_PlotPar.m_dXUpp;
		m_PlotPar.m_dStartXLow = m_PlotPar.m_dXLow;
		m_PlotPar.m_dStartYUpp = m_PlotPar.m_dYUpp;
		m_PlotPar.m_dStartYLow = m_PlotPar.m_dYLow;

		m_PlotPar.m_dStartMulX = m_PlotPar.m_nWinExtX / (m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow);
		m_PlotPar.m_dStartMulY = m_PlotPar.m_nWinExtY / (m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow);
	}
	else if( m_nPlotType ==	EQU_SURFACE_PLOT_2D )
	{
		// malo æemo reskalirati maksimalne vrijednosti da se cijeli graf vidi na ekranu
		m_PlotPar.m_dXUpp = m_EquAnalPar.m_adMaxEqu[0] * 1.01;
		m_PlotPar.m_dXLow = m_EquAnalPar.m_adMinEqu[0] * 0.99;

		m_PlotPar.m_dYUpp = m_EquAnalPar.m_adMaxEqu[1] * 1.01;
		m_PlotPar.m_dYLow = m_EquAnalPar.m_adMinEqu[1] * 0.99;

		m_PlotPar.m_dStartXUpp = m_PlotPar.m_dXUpp;
		m_PlotPar.m_dStartXLow = m_PlotPar.m_dXLow;
		m_PlotPar.m_dStartYUpp = m_PlotPar.m_dYUpp;
		m_PlotPar.m_dStartYLow = m_PlotPar.m_dYLow;

		m_PlotPar.m_dStartMulX = m_PlotPar.m_nWinExtX / (m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow);
		m_PlotPar.m_dStartMulY = m_PlotPar.m_nWinExtY / (m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow);
	}
}

void CNLSysView::DrawTrajectory(CDC * pDC)
{
	CNLSysDoc	*pDoc = GetDocument();
	int			i, j, k;
	int			nInc, nSteps, nXvar, x, y, start_i;
	double	mulX, mulY;

	mulX = m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX;
	mulY = m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY;

	if( m_PlotPar.m_nParNumPlot == PlotParameters::SINGLE_PAR )
	{
		nInc = 1;
		nSteps = m_IntPar.m_nIntSteps[0]-1;
		
		if( nSteps > m_PlotPar.m_nWinExtX )
			nInc = 1 + (nSteps / m_PlotPar.m_nWinExtX);

		for( k=0; k<MAX_PLOTTING_VARS; k++ )
			if( (j = m_PlotPar.m_anPlottingVars[k]) != -1 )
			{
				if( pDC->IsPrinting() || pDoc->m_Options.m_bUseColor == FALSE)
					pDC->SelectObject(&m_aPrintPens[k]);
				else
					pDC->SelectObject(&m_aPens[k]);

				nXvar = m_PlotPar.m_anSysVarsOrder[k];
				
				for( i=0; i<nSteps; i++ )
					if( (*m_vecX)[i] >= m_PlotPar.m_dStartPlot )
					{
						start_i = i;
						break;
					}
			
				x = (int) ((*m_vecX)[start_i] * mulX);
				y = (int) ((*m_matY)[nXvar][start_i] * mulY);
				pDC->MoveTo(x,y);
				for( i=start_i+1; i<nSteps; i+=nInc )
				{
					x = (int) ((*m_vecX)[i] * mulX);
					y = (int) ((*m_matY)[nXvar][i] * mulY);
					
					if( (*m_vecX)[i] < m_PlotPar.m_dXUpp && (*m_vecX)[i] > m_PlotPar.m_dXLow )
						pDC->LineTo(x,y);
				}
			}
	}
	else
	{
		// izgleda da je odabrano višeparametarsko iscrtavanje u istom prozoru
		// pošto je u tom sluèaju dozvoljeno iscrtavanje samo jedne varijable, uzimamo [0]
		if( (j = m_PlotPar.m_anPlottingVars[0]) != -1 )
		{
			nXvar = m_PlotPar.m_anSysVarsOrder[0];
			for( k=0; k<m_IntPar.m_nNumOfChgPar; k++ )
			{
				nInc = 1;
				nSteps = m_IntPar.m_nIntSteps[k]-1;
				
				if( nSteps > m_PlotPar.m_nWinExtX )
					nInc = 1 + (nSteps / m_PlotPar.m_nWinExtX);

				if( pDC->IsPrinting() || pDoc->m_Options.m_bUseColor == FALSE)
					pDC->SelectObject(&m_aPrintPens[k]);
				else
					pDC->SelectObject(&m_aPens[k]);

				for( i=0; i<nSteps; i++ )
					if( (*m_apvecX[k])[i] >= m_PlotPar.m_dStartPlot )
					{
						start_i = i;
						break;
					}
				
				x = (int) ((*m_apvecX[k])[start_i] * mulX);
				y = (int) ((*m_apmatY[k])[nXvar][start_i] * mulY);
				pDC->MoveTo(x,y);
				for( i=start_i+1; i<nSteps; i+=nInc )
				{
					x = (int) ((*m_apvecX[k])[i] * mulX);
					y = (int) ((*m_apmatY[k])[nXvar][i] * mulY);
					
					if( (*m_apvecX[k])[i] < m_PlotPar.m_dXUpp && (*m_apvecX[k])[i] > m_PlotPar.m_dXLow )
						pDC->LineTo(x,y);
				}
			}
		}
		else
		{
			// ovdje nismo smjeli dospjeti, znaèi da je bila greška u dijalogu
			ASSERT(0);
		}
	}
}

void CNLSysView::DrawPhase2D(CDC * pDC)
{
	CNLSysDoc	*pDoc = GetDocument();
	double	mulX, mulY;
	int			i, nXvar, nYvar, nSteps, start_i;
	int			x, y;
	
	nSteps = m_PlotPar.m_nRealCalcNum-1;

	mulX = m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX;
	mulY = m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY;

	nXvar = m_PlotPar.m_anSysVarsOrder[0];
	nYvar = m_PlotPar.m_anSysVarsOrder[1];

	if( pDC->IsPrinting() || pDoc->m_Options.m_bUseColor == FALSE)
		pDC->SelectObject(&m_aPrintPens[0]);
	else
		pDC->SelectObject(&m_aPens[0]);

	for( i=0; i<nSteps; i++ )
		if( (*m_vecX)[i] >= m_PlotPar.m_dStartPlot )
		{
			start_i = i;
			break;
		}
	
	x = (int) ((*m_matY)[nXvar][start_i] * mulX);
	y = (int) ((*m_matY)[nYvar][start_i] * mulY);
	pDC->MoveTo(x,y);
	for( i=start_i+1; i<nSteps; i++ )
	{
		x = (int) ((*m_matY)[nXvar][i] * mulX);
		y = (int) ((*m_matY)[nYvar][i] * mulY);
		pDC->LineTo(x,y);
	}
}

void CNLSysView::DrawPhase3D(CDC * pDC)
{
	CNLSysDoc	*pDoc = GetDocument();
	CPointDouble	transfP;
	CPoint				point;

	double	mulX, mulY, p[4];
	int			i, nXvarInd, nYvarInd, nZvarInd, nSteps, start_i;
	
	nSteps = m_PlotPar.m_nRealCalcNum-1;

	mulX = m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX;
	mulY = m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY;

	nXvarInd = m_PlotPar.m_anSysVarsOrder[0];
	nYvarInd = m_PlotPar.m_anSysVarsOrder[1];
	nZvarInd = m_PlotPar.m_anSysVarsOrder[2];

	if( pDC->IsPrinting() || pDoc->m_Options.m_bUseColor == FALSE)
		pDC->SelectObject(&m_aPrintPens[0]);
	else
		pDC->SelectObject(&m_aPens[0]);

	for( i=0; i<nSteps; i++ )
		if( (*m_vecX)[i] >= m_PlotPar.m_dStartPlot )
		{
			start_i = i;
			break;
		}

	p[0] = (*m_matY)[nXvarInd][start_i];
	p[1] = (*m_matY)[nYvarInd][start_i];
	p[2] = (*m_matY)[nZvarInd][start_i];
	p[3] = 1.0;
	transfP = TransfToDoublePoint( p, m_PlotPar.m_matT );
	
	point.x = (long) (transfP.x * mulX);
	point.y = (long) (transfP.y * mulY);
	
	pDC->MoveTo(point);

	int			nType = 0;

	for( i=start_i+1; i<nSteps; i++ )
	{
		p[0] = (*m_matY)[nXvarInd][i];
		p[1] = (*m_matY)[nYvarInd][i];
		p[2] = (*m_matY)[nZvarInd][i];
		p[3] = 1.0;
		transfP = TransfToDoublePoint( p, m_PlotPar.m_matT );

		point.x = (long) (transfP.x * mulX);
		point.y = (long) (transfP.y * mulY);

		if( p[2] < GetZ(*m_PlotPar.pointLookTo) )			// ako je ispod z-ravnine
			pDC->SelectObject(&m_aPens[2]);		// onda ga crtamo iscrtkanom linijom
		else
			pDC->SelectObject(&m_aPens[0]);

		pDC->LineTo(point);
	}

}

void CNLSysView::DrawEquSurface2D( CDC *pDC )
{
	CNLSysDoc	*pDoc = GetDocument();
	double	mulX, mulY;
	int			i, nSteps, mulx;
	int			x, y;
	
	nSteps = m_EquAnalPar.m_nEquSurfaceDataNum;

	mulX = m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX;
	mulY = m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY;

	if( pDC->IsPrinting() || pDoc->m_Options.m_bUseColor == FALSE)
		pDC->SelectObject(&m_aPrintPens[0]);
	else
		pDC->SelectObject(&m_aPens[0]);

	x = (int) ((*m_matEquSol)[0][0] * mulX);
	y = (int) ((*m_matEquSol)[1][0] * mulY);
	pDC->MoveTo(x,y);
	for( i=0; i<nSteps; i++ )
	{
		x = (int) ((*m_matEquSol)[0][i] * mulX);
		y = (int) ((*m_matEquSol)[1][i] * mulY);
		pDC->LineTo(x,y);
	}

	// da li æemo iscrtati pozicije naðenih bifurkacijskih toèaka
	if( m_PlotPar.m_bShowBifInEqu == TRUE )
	{
		if( pDoc->IsEmbedded() )
			mulx = 3;
		else
			mulx = 1;

		double	val1, val2;
		int			x, y, nParNum, nVarNum;
		CBrush	pBrush;

		nParNum = pDoc->m_DES.GetOrderOfParVar(m_PlotPar.m_anPlottingVars[0]);
		nVarNum = pDoc->m_DES.GetOrderOfSysVar(m_PlotPar.m_anPlottingVars[1]);

		pBrush.CreateSolidBrush(RGB(0,0,0));
		CSize		WExt, VExt;
		int			sizeY;

		WExt = pDC->GetWindowExt();
		VExt = pDC->GetViewportExt();
		sizeY = (int) (floor(((double) WExt.cy * 15 * mulx) / VExt.cy * VExt.cx / WExt.cx));

		for( i=0; i<pDoc->m_BifAnal.m_nSNBPointsNum; i++ )
		{
			val1 = (*pDoc->m_BifAnal.m_avecSNBParValues[i])[nParNum];
			val2 = (*pDoc->m_BifAnal.m_avecSNBVarValues[i])[nVarNum];
			if( val1 > m_PlotPar.m_dXLow && val1 < m_PlotPar.m_dXUpp &&
					val2 > m_PlotPar.m_dYLow && val2 < m_PlotPar.m_dYUpp )
			{
				x = (int) (val1 * mulX);
				y = (int) (val2 * mulY);
				pDC->SelectObject(pBrush);
				pDC->Ellipse( x-15*mulx, y-sizeY, x+15*mulx, y+sizeY );
			}
		}

		sizeY = (int) (floor(((double) WExt.cy * 13 * mulx) / VExt.cy * VExt.cx / WExt.cx));

		for( i=0; i<pDoc->m_BifAnal.m_nHopfPointsNum; i++ )
		{
			val1 = (*pDoc->m_BifAnal.m_avecHopfParValues[i])[nParNum];
			val2 = (*pDoc->m_BifAnal.m_avecHopfVarValues[i])[nVarNum];
			if( val1 > m_PlotPar.m_dXLow && val1 < m_PlotPar.m_dXUpp &&
					val2 > m_PlotPar.m_dYLow && val2 < m_PlotPar.m_dYUpp )
			{
				x = (int) (val1 * mulX);
				y = (int) (val2 * mulY);
				pDC->SelectObject(pBrush);
				pDC->Rectangle( x-13*mulx, y-sizeY, x+13*mulx, y+sizeY );
			}
		}
	}
}

void CNLSysView::DrawSpectrum( CDC *pDC )
{
	CNLSysDoc	*pDoc = GetDocument();
	int			i, j, k;
	int			nInc, nSteps, nXvar, x, y;
	double	mulX, mulY;

	nInc = 1;
	nSteps = m_PlotPar.m_nRealCalcNum;
	
	if( nSteps > m_PlotPar.m_nWinExtX )
		nInc = 1 + (nSteps / m_PlotPar.m_nWinExtX);

	mulX = m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX;
	mulY = m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY;

	for( k=0; k<MAX_PLOTTING_VARS; k++ )
		if( (j = m_PlotPar.m_anPlottingVars[k]) != -1 )
		{
			if( pDC->IsPrinting() || pDoc->m_Options.m_bUseColor == FALSE)
				pDC->SelectObject(&m_aPrintPens[k]);
			else
				pDC->SelectObject(&m_aPens[k]);

			nXvar = m_PlotPar.m_anSysVarsOrder[k];
			
			x = (int) ((*m_vecFreq)[0] * mulX);
			y = (int) ((*m_matSpectrum)[nXvar][0] * mulY);
			pDC->MoveTo(x,y);
			for( i=1; i<nSteps; i+=nInc )
			{
				x = (int) ((*m_vecFreq)[i] * mulX);
				y = (int) ((*m_matSpectrum)[nXvar][i] * mulY);
				
				if( (*m_vecFreq)[i] < m_PlotPar.m_dXUpp && (*m_vecFreq)[i] > m_PlotPar.m_dXLow )
					pDC->LineTo(x,y);
			}
		}
}

void CNLSysView::DrawLyapExp( CDC *pDC )
{
	CNLSysDoc		*pDoc = GetDocument();
	int			i, j, k, x, y;
	double	mulX, mulY, temp;

	// najprije æemo ih poredati po redu
	for( i=0; i<m_PlotPar.m_nRealCalcNum; i++ )
		for( j=i+1; j<m_PlotPar.m_nRealCalcNum; j++ )
			if( (*m_vecParVal)[j] < (*m_vecParVal)[i] )
			{
				temp = (*m_vecParVal)[j];
				(*m_vecParVal)[j] = (*m_vecParVal)[i];
				(*m_vecParVal)[i] = temp;
				
				for( k=0; k<pDoc->m_DES.GetDim(); k++ )
				{
					temp = (*m_matLyapExp)[k][j];
					(*m_matLyapExp)[k][j] = (*m_matLyapExp)[k][i];
					(*m_matLyapExp)[k][i] = temp;
				}
			}

	mulX = m_PlotPar.m_dMulX * m_PlotPar.m_dPrintMulX;
	mulY = m_PlotPar.m_dMulY * m_PlotPar.m_dPrintMulY;

	for( j=0; j<pDoc->m_DES.GetDim(); j++ )
	{
		if( pDC->IsPrinting() || pDoc->m_Options.m_bUseColor == FALSE)
			pDC->SelectObject(&m_aPrintPens[j]);
		else
			pDC->SelectObject(&m_aPens[j]);

		x = (int) ((*m_vecParVal)[0] * mulX);
		y = (int) ((*m_matLyapExp)[j][0] * mulY);
		pDC->MoveTo(x,y);

		for( i=1; i<m_PlotPar.m_nRealCalcNum; i++ )
		{
			x = (int) ((*m_vecParVal)[i] * mulX);
			y = (int) ((*m_matLyapExp)[j][i] * mulY);
			
			if( m_PlotPar.m_bConnectLyap )
				pDC->LineTo( x, y );
			else
				pDC->Ellipse( x-10, y-10, x+10, y+10 );
		}

	}
}

CPointDouble	TransfToDoublePoint( double *point, Matrix *T )
{
	int		i, j;
	double	Transf[4];
	CPointDouble	ret; 

	for( i=0; i<4; i++ )
	{
		Transf[i] = 0;
		for( j=0; j<4; j++ )
			Transf[i] += point[j] * T->GetElem(j,i);
	}
	
	ret.x = (Transf[0] / Transf[3]);
	ret.y = (Transf[1] / Transf[3]);

	return	ret;
}

CPoint	TransfToPoint( double *point, Matrix *T )
{
	int		i, j;
	double	Transf[4];
	CPoint	ret; 

	for( i=0; i<4; i++ )
	{
		Transf[i] = 0;
		for( j=0; j<4; j++ )
			Transf[i] += point[j] * T->GetElem(j,i);
	}
	
	ret.x = (long) (Transf[0] / Transf[3]);
	ret.y = (long) (Transf[1] / Transf[3]);

	return	ret;
}

CPointDouble	TransfVertexToPoint( VERTEX point, Matrix *T )
{
	int		i;
	double	Transf[4];
	CPointDouble	ret; 

	for( i=0; i<4; i++ )
	{
		Transf[i] = 0;
		Transf[i] += point.x * T->GetElem(0,i);
		Transf[i] += point.y * T->GetElem(1,i);
		Transf[i] += point.z * T->GetElem(2,i);
		Transf[i] += point.h * T->GetElem(3,i);
	}
	
	ret.x = (Transf[0] / Transf[3]);
	ret.y = (Transf[1] / Transf[3]);

	return	ret;
}

/////////////////////////////////////////////////////////////////////////////
// CNLSysView printing

BOOL CNLSysView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNLSysView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNLSysView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.
void CNLSysView::OnCancelEditSrvr()
{
	GetDocument()->OnDeactivateUI(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CNLSysView diagnostics

#ifdef _DEBUG
void CNLSysView::AssertValid() const
{
	CView::AssertValid();
}

void CNLSysView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNLSysDoc* CNLSysView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNLSysDoc)));
	return (CNLSysDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNLSysView message handlers

/////////////////////////					TRAJEKTORIJE						///////////////////////////////////

void CNLSysView::OnPlotTrajectory() 
{
	CNLSysDoc		*pDoc = GetDocument();
	CTrajectoryPlotParDlg			dlg;
	
	SetDialogValues(dlg);	
	
again_trajectory:
	if( dlg.DoModal() == IDOK )
	{
		if( m_bCalcInProgress == TRUE )
		{
			AfxMessageBox(IDS_ALREADY_CALCULATING_FOR_THIS_WINDOW);
			return;
		}

 		if( GetDialogValues(dlg) )
		{
			if( m_PlotPar.m_nParNumPlot == PlotParameters::SINGLE_PAR )
			{
				m_IntPar.m_hWnd = m_hWnd;			// da kreirana nit zna kojem prozoru treba poslati poruku
				AfxBeginThread(Integrate, this);

				m_bCalcDone = FALSE;
			}
			else
			{
				if( pDoc->m_Options.m_nWindOpen == ProgramOptions::NEW_WIND )
				{
					int		i, j;
					CChildFrame *pFrame;
					CNLSysView	*pView;

					for( i=0; i<m_IntPar.m_nNumOfChgPar; i++ )
					{
						if( i != 0 )
						{
							// za num-1 vrijednosti moramo otvoriti nove prozore
							pFrame = (CChildFrame *) this->NewWindow();
							pView = (CNLSysView *)  pFrame->GetActiveView();
							
							// sada treba u novootvoreni prozor iskopirati sve potrebne podatke i pokrenuti integraciju
							pView->m_IntPar.m_dStep		= m_IntPar.m_dStep;
							pView->m_IntPar.m_dEps		= m_IntPar.m_dEps;
							pView->m_IntPar.m_dStart	= m_IntPar.m_dStart;
							pView->m_IntPar.m_dEnd		= m_IntPar.m_dEnd;
							pView->m_IntPar.m_nSaveValNum = m_IntPar.m_nSaveValNum;
							
							pView->m_IntPar.m_vecInitCond = new Vector(pDoc->m_DES.GetDim());
							pView->m_IntPar.m_vecParamVal = new Vector(pDoc->m_DES.GetParNum());
							*pView->m_IntPar.m_vecInitCond = *m_IntPar.m_vecInitCond;
							*pView->m_IntPar.m_vecParamVal = *m_IntPar.m_vecParamVal;

							for( j=0; j<MAX_PLOTTING_VARS; j++ )
							{
								pView->m_PlotPar.m_anPlottingVars[j] = m_PlotPar.m_anPlottingVars[j];
								pView->m_PlotPar.m_anSysVarsOrder[j] = m_PlotPar.m_anSysVarsOrder[j];
							}
							pView->m_PlotPar.m_dStartPlot = m_PlotPar.m_dStartPlot;
							pView->m_PlotPar.m_dEndPlot = m_PlotPar.m_dEndPlot;
							pView->m_PlotPar.m_bViewTypeChanged = TRUE;
							pView->m_nPlotType = TRAJECTORY_PLOT;
						}
						else
						{
							m_PlotPar.m_bViewTypeChanged = TRUE;
							m_nPlotType = TRAJECTORY_PLOT;
							
							pView = this;		// a prvu vrijedost æemo iscrtati u ovom prozoru
						}

						if( pView->m_szTitle.IsEmpty() )
						{
							const char	*str;
							char	tmp[100];
							if( m_IntPar.m_enChgType == IntegrateParameters::SYS_VAR )
								str = pDoc->m_DES.GetSysVarName(m_IntPar.m_nVarNum);
							else
								str = pDoc->m_DES.GetParVarName(m_IntPar.m_nVarNum);

							sprintf(tmp, " : %s = %f", str, (*m_IntPar.m_vecChgParList)[i] );
							
							CString	szTmp(tmp);
							pView->m_szTitle = szTmp;
						}

						if( m_IntPar.m_enChgType == IntegrateParameters::SYS_VAR )
							(*pView->m_IntPar.m_vecInitCond)[m_IntPar.m_nVarNum] = (*m_IntPar.m_vecChgParList)[i];
						else
							(*pView->m_IntPar.m_vecParamVal)[m_IntPar.m_nVarNum] = (*m_IntPar.m_vecChgParList)[i];

						pView->m_PlotPar.m_nParNumPlot = PlotParameters::SINGLE_PAR;

						// treba još popuniti znakovne nizove i IntegratePar sa ispravnim vrijednostima tako da
						// kada korisnik pozove dijalog za editiranje bude sve kako treba
						memset(pView->m_IntPar.m_szInitCond, 0, 100 );
 						memset(pView->m_IntPar.m_szParamVal, 0, 100 );
						
						const char *szName;
						int					len = 0;
						for( j=0; j<pDoc->m_DES.GetDim(); j++ )
						{
							szName = pDoc->m_DES.GetSysVarName(j);
							len += sprintf(pView->m_IntPar.m_szInitCond + len, "%s=%f\n\n", szName, (*pView->m_IntPar.m_vecInitCond)[j] );
							pView->m_IntPar.m_szInitCond[len-2] = 13;
							pView->m_IntPar.m_szInitCond[len-1] = 10;
						}
						len = 0;
						for( j=0; j<pDoc->m_DES.GetParNum(); j++ )
						{
							szName = pDoc->m_DES.GetParVarName(j);
							len += sprintf(pView->m_IntPar.m_szParamVal + len, "%s=%f\n\n", szName, (*pView->m_IntPar.m_vecParamVal)[j] );
							pView->m_IntPar.m_szParamVal[len-2] = 13;
							pView->m_IntPar.m_szParamVal[len-1] = 10;
						}
						
						pView->m_IntPar.m_hWnd = pView->m_hWnd;
						pView->m_bCalcDone = FALSE;

						AfxBeginThread(Integrate, pView);
					}
					// nakon što smo pootvarali sve prozore, treba još "poèistiti" èlanske varijable u ovom prozoru
					delete m_IntPar.m_vecChgParList;
					m_IntPar.m_vecChgParList = NULL;
					m_IntPar.m_nNumOfChgPar = 0;
				}
				else
				{
					m_IntPar.m_hWnd = m_hWnd;

					AfxBeginThread(Integrate, this);
					m_bCalcDone = FALSE;
				}
			}
		}
		else
			goto again_trajectory;
	
		Invalidate();
	}
}

void CNLSysView::SetDialogValues(CTrajectoryPlotParDlg & dlg)
{
	CNLSysDoc* pDoc = GetDocument();
	int			i, j;

	dlg.m_dStartTime = m_IntPar.m_dStart;
	dlg.m_dEndTime = m_IntPar.m_dEnd;
	dlg.m_dStepSize = m_IntPar.m_dStep;
	dlg.m_dEps = m_IntPar.m_dEps;
	dlg.m_nSaveValNum = m_IntPar.m_nSaveValNum;

	dlg.m_dStartPlotTime = m_PlotPar.m_dStartPlot;
	dlg.m_dEndPlotTime = m_PlotPar.m_dEndPlot;

	dlg.m_szStartVal  = m_IntPar.m_szInitCond;
	dlg.m_szParamVal	= m_IntPar.m_szParamVal;

	dlg.m_szChgParam = m_IntPar.m_szChgPar;
	dlg.m_szParValues = m_IntPar.m_szChgParValues;

	if( m_PlotPar.m_nScaleY == PlotParameters::SCALE_Y_SAME )
	{
		dlg.m_nScaleYSame = 0;
		dlg.m_nScaleYDifferent = -1;
	}
	else
	{
		dlg.m_nScaleYSame = -1;
		dlg.m_nScaleYDifferent = 0;
	}
	if( m_PlotPar.m_nParNumPlot == PlotParameters::MULTI_PAR )
	{
		dlg.m_nPlotMultiPar = 0;
		dlg.m_nPlotSinglePar = -1;
	}
	else
	{
		dlg.m_nPlotMultiPar = -1;
		dlg.m_nPlotSinglePar = 0;
	}

	dlg.m_szTitle = m_szTitle;

	for( i=0; i<MAX_PLOTTING_VARS; i++ )
		if( (j = m_PlotPar.m_anPlottingVars[i]) != -1 )
		{
			switch( i )
			{
			case 0:
				dlg.m_szVar1 = pDoc->m_DES.GetVarName(j);
				dlg.m_szLegend1 = m_aszLegend[0];
				break;
			case 1:
				dlg.m_szVar2 = pDoc->m_DES.GetVarName(j);
				dlg.m_szLegend2 = m_aszLegend[1];
				break;
			case 2:
				dlg.m_szVar3 = pDoc->m_DES.GetVarName(j);
				dlg.m_szLegend3 = m_aszLegend[2];
				break;
			case 3:
				dlg.m_szVar4 = pDoc->m_DES.GetVarName(j);
				dlg.m_szLegend4 = m_aszLegend[3];
				break;
			}
		}

}

int CNLSysView::GetDialogValues(CTrajectoryPlotParDlg & dlg)
{
	CNLSysDoc* pDoc = GetDocument();
	int			i, j, num, num2, ret=TRUE, OldSysVar[MAX_PLOTTING_VARS];
	const char *p = LPCSTR(dlg.m_szParValues);
	char				szTemp[100];
	double			adParVal[20];

	if( dlg.m_dStartPlotTime < dlg.m_dStartTime || dlg.m_dEndPlotTime > dlg.m_dEndTime )
	{
		AfxMessageBox(IDS_WRONG_PLOT_TIME);
		ret = FALSE;
	}

	for( i=0; i<MAX_PLOTTING_VARS; i++ )
	{
		// najprije zapamti koje smo varijable iscrtavali u prethodnom crtanju
		m_PlotPar.m_anOldPlottingVars[i] = m_PlotPar.m_anPlottingVars[i];
		OldSysVar[i] = m_PlotPar.m_anSysVarsOrder[i];

		m_PlotPar.m_anPlottingVars[i] = -1;
		m_PlotPar.m_anSysVarsOrder[i] = -1;
	}

	// ovdje æemo saznati koje su varijable odabrane za iscrtavanje
	num = 0;
	if( !dlg.m_szVar1.IsEmpty() && (j = pDoc->GetVarNum(dlg.m_szVar1)) >= 0 )
	{
		if( pDoc->m_DES.IsSystemVar(j) )
		{
			m_PlotPar.m_anPlottingVars[num] = j;
			m_PlotPar.m_anSysVarsOrder[num] = pDoc->m_DES.GetOrderOfSysVar(j);

			m_aszLegend[num++] = dlg.m_szLegend1;
		}
		else
		{
			AfxMessageBox(IDS_ERR_NOT_SYS_VAR);
			ret = FALSE;
		}
	}
	else if( !dlg.m_szVar1.IsEmpty() )		// ovdje obraditi gresku kada se ne unese ime varijable
	{
		AfxMessageBox(IDS_ERR_NOT_VAR_NAME);
		ret = FALSE;
	}

	if( !dlg.m_szVar2.IsEmpty() && (j = pDoc->GetVarNum(dlg.m_szVar2)) >= 0 )
	{
		if( pDoc->m_DES.IsSystemVar(j) )
		{
			m_PlotPar.m_anPlottingVars[num] = j;
			m_PlotPar.m_anSysVarsOrder[num] = pDoc->m_DES.GetOrderOfSysVar(j);

			m_aszLegend[num++] = dlg.m_szLegend2;
		}
		else
		{
			AfxMessageBox(IDS_ERR_NOT_SYS_VAR);
			ret = FALSE;
		}
	}
	else if( !dlg.m_szVar2.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_NOT_VAR_NAME);
		ret = FALSE;
	}

	if( !dlg.m_szVar3.IsEmpty() && (j = pDoc->GetVarNum(dlg.m_szVar3)) >= 0 )
	{
		if( pDoc->m_DES.IsSystemVar(j) )
		{
			m_PlotPar.m_anPlottingVars[num] = j;
			m_PlotPar.m_anSysVarsOrder[num] = pDoc->m_DES.GetOrderOfSysVar(j);

			m_aszLegend[num++] = dlg.m_szLegend3;
		}
		else
		{
			AfxMessageBox(IDS_ERR_NOT_SYS_VAR);
			ret = FALSE;
		}
	}
	else if( !dlg.m_szVar3.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_NOT_VAR_NAME);
		ret = FALSE;
	}

	if( !dlg.m_szVar4.IsEmpty() && (j = pDoc->GetVarNum(dlg.m_szVar4)) >= 0 )
	{
		if( pDoc->m_DES.IsSystemVar(j) )
		{
			m_PlotPar.m_anPlottingVars[num] = j;
			m_PlotPar.m_anSysVarsOrder[num] = pDoc->m_DES.GetOrderOfSysVar(j);

			m_aszLegend[num++] = dlg.m_szLegend4;
		}
		else
		{
			AfxMessageBox(IDS_ERR_NOT_SYS_VAR);
			ret = FALSE;
		}
	}
	else if( !dlg.m_szVar4.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_NOT_VAR_NAME);
		ret = FALSE;
	}

	if( dlg.m_nPlotMultiPar == 0 )
	{
		if( pDoc->m_Options.m_nWindOpen == ProgramOptions::SAME_WIND && dlg.m_szVar1.IsEmpty() )
		{
			AfxMessageBox(IDS_VAR1_FOR_MULTI_PAR);
			ret = FALSE;
		}
		else if( dlg.m_szChgParam.IsEmpty() )
		{
			AfxMessageBox(IDS_CHG_PAR_NOT_GIVEN);
			ret = FALSE;
		}
		else
		{
			// najprije provjeravamo da li je za mijenjajuæi parametar uneseno
			// ispravno ime varijable
			if( (j = pDoc->m_DES.GetVarNum(LPCSTR(dlg.m_szChgParam))) >= 0 )
			{
				if( pDoc->m_DES.IsSystemVar(j) )
				{
					m_IntPar.m_enChgType = IntegrateParameters::SYS_VAR;
					m_IntPar.m_nVarNum = pDoc->m_DES.GetOrderOfSysVar(j);
				}
				else
				{
					m_IntPar.m_enChgType = IntegrateParameters::PAR_VAR;
					m_IntPar.m_nVarNum = pDoc->m_DES.GetOrderOfParVar(j);
				}
				
				i = j = 0;
				num2 = 0;
				while( p[i] != 0 )
				{
					szTemp[j++] = p[i++];
					if( p[i] == ',' || p[i] == 0)
					{
						szTemp[j] = 0;
						adParVal[num2++] = atof(szTemp);
						j = 0;
						if( p[i] != 0 )
							i++;
					}
				}
				if( num2 <= 0 )
				{
					AfxMessageBox(IDS_EMPTY_PARAMETER_LIST);
					ret = FALSE;
				}
			}
			else
			{
				AfxMessageBox(IDS_ERR_NOT_VAR_NAME);
				ret = FALSE;
			}
		}
	}

	if( num == 0 && ret == TRUE )				// ako nismo unijeli ime niti jedne varijable
	{
		AfxMessageBox(IDS_ERR_NO_VAR_ENTERED);
		ret = FALSE;
	}

	Vector		InitCond(pDoc->m_DES.GetDim()), ParVal(pDoc->m_DES.GetParNum());

	if( pDoc->m_DES.FillVarValuesFromString( LPCSTR(dlg.m_szStartVal), InitCond ) != 0 
		  || pDoc->m_DES.FillParValuesFromString( LPCSTR(dlg.m_szParamVal), ParVal ) != 0 )
	{
			AfxMessageBox(IDS_WRONG_INIT_COND_OR_PAR_VAL);
			ret = FALSE;
	}

	if( ret == TRUE )
	{
		m_IntPar.m_dStart = dlg.m_dStartTime;
		m_IntPar.m_dEnd = dlg.m_dEndTime;
		m_IntPar.m_dStep = dlg.m_dStepSize;
		m_IntPar.m_dEps = dlg.m_dEps;
		m_IntPar.m_nSaveValNum = dlg.m_nSaveValNum;

		strcpy( m_IntPar.m_szInitCond, dlg.m_szStartVal );
		strcpy( m_IntPar.m_szParamVal, dlg.m_szParamVal );
		*m_IntPar.m_vecInitCond = InitCond;
		*m_IntPar.m_vecParamVal = ParVal;

		if( m_IntPar.m_vecChgParList != NULL )
		{
			delete m_IntPar.m_vecChgParList;
			m_IntPar.m_vecChgParList = NULL;
		}

		if( dlg.m_nPlotMultiPar == 0 )
		{
			if( m_PlotPar.m_nParNumPlot == PlotParameters::SINGLE_PAR )
			{
				m_IntPar.m_bParametersChanged = TRUE;
				m_bCalcDone = FALSE;
			}
			
			m_IntPar.m_vecChgParList = new Vector(num2);
			for( i=0; i<num2; i++ )
				(*m_IntPar.m_vecChgParList)[i] = adParVal[i];
			
			m_IntPar.m_nNumOfChgPar = num2;
			strcpy( m_IntPar.m_szChgPar, dlg.m_szChgParam );
			strcpy( m_IntPar.m_szChgParValues, dlg.m_szParValues );
		}

		if(	DifferentIntPar() )
		{
			m_IntPar.m_bParametersChanged = TRUE;
			pDoc->SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti
		}

		if( dlg.m_dStartPlotTime != m_PlotPar.m_dStartPlot || dlg.m_dEndPlotTime != m_PlotPar.m_dEndPlot )
			m_PlotPar.m_bXAxisIntervalChanged = TRUE;

		m_PlotPar.m_dStartPlot = dlg.m_dStartPlotTime;
		m_PlotPar.m_dEndPlot = dlg.m_dEndPlotTime;

		if( dlg.m_nPlotMultiPar == 0 )
			m_PlotPar.m_nParNumPlot = PlotParameters::MULTI_PAR;
		else
			m_PlotPar.m_nParNumPlot = PlotParameters::SINGLE_PAR;

		if( dlg.m_nScaleYSame == 0 )
			m_PlotPar.m_nScaleY = PlotParameters::SCALE_Y_SAME;
		else
			m_PlotPar.m_nScaleY = PlotParameters::SCALE_Y_DIFFERENT;

		if( m_nPlotType != TRAJECTORY_PLOT )
		{
			m_PlotPar.m_bViewTypeChanged = TRUE;
			if( m_nPlotType == FOURIER_SPECTRUM_PLOT )
				m_PlotPar.m_nRealCalcNum = m_IntPar.m_nIntSteps[0];
		}

		m_nPlotType = TRAJECTORY_PLOT;

		for( i=num; i<MAX_PLOTTING_VARS; i++ )
			m_aszLegend[i] = CString("");

		for( i=0; i<MAX_PLOTTING_VARS; i++ )
			if( m_PlotPar.m_anPlottingVars[i] != m_PlotPar.m_anOldPlottingVars[i] )
				m_PlotPar.m_bViewTypeChanged = TRUE;

		if( m_PlotPar.m_bViewTypeChanged == TRUE )
			pDoc->SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti

		m_szTitle = dlg.m_szTitle;
	}
	else
	{
		// pošto je unos u dijalog bio krivi, neæemo ništa mijenjati u CView klasi, pa moramo
		// vratiti stare vrijednosti
		for( i=0; i<MAX_PLOTTING_VARS; i++ )
		{
			// najprije zapamti koje smo varijable iscrtavali u prethodnom crtanju
			m_PlotPar.m_anPlottingVars[i] = m_PlotPar.m_anOldPlottingVars[i];
			m_PlotPar.m_anSysVarsOrder[i] = OldSysVar[i];
		}
	}

	return ret;
}

////////////////////////						FAZNE KRIVULJE					///////////////////////////////////

void CNLSysView::OnPlotPhaseCurve() 
{
	CPhasePlotParDlg			dlg;
	
	SetDialogValues(dlg);

again_phase:
	if( dlg.DoModal() == IDOK )
	{
		if( m_bCalcInProgress == TRUE )
		{
			AfxMessageBox(IDS_ALREADY_CALCULATING_FOR_THIS_WINDOW);
			return;
		}

		if( GetDialogValues(dlg) )
		{
			m_IntPar.m_hWnd = m_hWnd;
			AfxBeginThread(Integrate, this);
			m_bCalcDone = FALSE;
		}
		else
			goto again_phase;
		
		Invalidate();
	}
}

void CNLSysView::SetDialogValues(CPhasePlotParDlg & dlg)
{
	CNLSysDoc* pDoc = GetDocument();

	dlg.m_dStartTime = m_IntPar.m_dStart;
	dlg.m_dEndTime = m_IntPar.m_dEnd;
	dlg.m_dStepSize = m_IntPar.m_dStep;
	dlg.m_dEps = m_IntPar.m_dEps;
	dlg.m_nSaveValNum = m_IntPar.m_nSaveValNum;

	dlg.m_dStartPlotTime = m_PlotPar.m_dStartPlot;
	dlg.m_dEndPlotTime = m_PlotPar.m_dEndPlot;

	dlg.m_szStartVal  = m_IntPar.m_szInitCond;
	dlg.m_szParamVal	= m_IntPar.m_szParamVal;

	if( m_PlotPar.m_nScaleY == PlotParameters::SCALE_Y_SAME )
	{
		dlg.m_nScaleYSame = 0;
		dlg.m_nScaleYDifferent = -1;
	}
	else
	{
		dlg.m_nScaleYSame = -1;
		dlg.m_nScaleYDifferent = 0;
	}
	if( m_PlotPar.m_nParNumPlot == PlotParameters::MULTI_PAR )
	{
		dlg.m_nPlotMultiPar = 0;
		dlg.m_nPlotSinglePar = -1;
	}
	else
	{
		dlg.m_nPlotMultiPar = -1;
		dlg.m_nPlotSinglePar = 0;
	}
	if( m_PlotPar.m_nPhasePlotDim == PlotParameters::PLOT_2D )
	{
		dlg.m_nPlot2D = 0;
		dlg.m_nPlot3D = -1;
	}
	else
	{
		dlg.m_nPlot2D = -1;
		dlg.m_nPlot3D = 0;
		dlg.m_szZAxis = pDoc->m_DES.GetVarName(m_PlotPar.m_anPlottingVars[2]);
	}
	
	dlg.m_szXAxis = pDoc->m_DES.GetVarName(m_PlotPar.m_anPlottingVars[0]);
	dlg.m_szYAxis = pDoc->m_DES.GetVarName(m_PlotPar.m_anPlottingVars[1]);

	dlg.m_szTitle = m_szTitle;
}

int CNLSysView::GetDialogValues(CPhasePlotParDlg & dlg)
{
	CNLSysDoc* pDoc = GetDocument();
	int			i, j, num, ret=TRUE, OldSysVar[MAX_PLOTTING_VARS];

	if( dlg.m_dStartPlotTime < dlg.m_dStartTime || dlg.m_dEndPlotTime > dlg.m_dEndTime )
	{
		AfxMessageBox(IDS_WRONG_PLOT_TIME);
		ret = FALSE;
	}

	for( i=0; i<MAX_PLOTTING_VARS; i++ )
	{
		// najprije zapamti koje smo varijable iscrtavali u prethodnom crtanju
		m_PlotPar.m_anOldPlottingVars[i] = m_PlotPar.m_anPlottingVars[i];
		OldSysVar[i] = m_PlotPar.m_anSysVarsOrder[i];

		m_PlotPar.m_anPlottingVars[i] = -1;
		m_PlotPar.m_anSysVarsOrder[i] = -1;
	}

	num = 0;
	if( !dlg.m_szXAxis.IsEmpty() && (j = pDoc->GetVarNum(dlg.m_szXAxis)) >= 0 )
	{
		if( pDoc->m_DES.IsSystemVar(j) )
		{
			m_PlotPar.m_anPlottingVars[num] = j;
			m_PlotPar.m_anSysVarsOrder[num++] = pDoc->m_DES.GetOrderOfSysVar(j);
		}
		else
		{
			AfxMessageBox(IDS_ERR_NOT_SYS_VAR);
			return	FALSE;
		}
	}
	else if( !dlg.m_szXAxis.IsEmpty() )		// ovdje obraditi gresku kada se ne unese ime varijable
	{
		AfxMessageBox(IDS_ERR_NOT_VAR_NAME);
		ret=FALSE;
	}
	
	if( !dlg.m_szYAxis.IsEmpty() ) 
	{
		if( (j = pDoc->GetVarNum(dlg.m_szYAxis)) >= 0 &&pDoc->m_DES.IsSystemVar(j) )
		{
			m_PlotPar.m_anPlottingVars[num] = j;
			m_PlotPar.m_anSysVarsOrder[num++] = pDoc->m_DES.GetOrderOfSysVar(j);
		}
		else
		{
			AfxMessageBox(IDS_ERR_NOT_SYS_VAR);
			ret = FALSE;
		}
	}
	else if( dlg.m_szYAxis.IsEmpty() )
	{
		AfxMessageBox(IDS_ERR_NOT_GIVEN_VAR_NAME);
		ret=FALSE;
	}

	if( dlg.m_nPlot3D == 0 )
	{
		if( !dlg.m_szZAxis.IsEmpty() )
		{
			if( (j = pDoc->GetVarNum(dlg.m_szZAxis)) >= 0 && pDoc->m_DES.IsSystemVar(j) )
			{
				m_PlotPar.m_anPlottingVars[num] = j;
				m_PlotPar.m_anSysVarsOrder[num++] = pDoc->m_DES.GetOrderOfSysVar(j);
			}
			else
			{
				AfxMessageBox(IDS_ERR_NOT_SYS_VAR);
				ret = FALSE;
			}
		}
		else if( dlg.m_szZAxis.IsEmpty() )
		{
			AfxMessageBox(IDS_ERR_NOT_GIVEN_VAR_NAME);
			ret=FALSE;
		}
	}
	
	Vector		InitCond(pDoc->m_DES.GetDim()), ParVal(pDoc->m_DES.GetParNum());

	if( pDoc->m_DES.FillVarValuesFromString( LPCSTR(dlg.m_szStartVal), InitCond ) != 0 
		  || pDoc->m_DES.FillParValuesFromString( LPCSTR(dlg.m_szParamVal), ParVal ) != 0 )
	{
			AfxMessageBox(IDS_WRONG_INIT_COND_OR_PAR_VAL);
			ret = FALSE;
	}

	if( ret == TRUE )
	{
		m_IntPar.m_dStart = dlg.m_dStartTime;
		m_IntPar.m_dEnd = dlg.m_dEndTime;
		m_IntPar.m_dStep = dlg.m_dStepSize;
		m_IntPar.m_dEps = dlg.m_dEps;
		m_IntPar.m_nSaveValNum = dlg.m_nSaveValNum;
		
		strcpy( m_IntPar.m_szInitCond, dlg.m_szStartVal );
		strcpy( m_IntPar.m_szParamVal, dlg.m_szParamVal );
		*m_IntPar.m_vecInitCond = InitCond;
		*m_IntPar.m_vecParamVal = ParVal;

		if(	DifferentIntPar() )
		{
			m_IntPar.m_bParametersChanged = TRUE;
			pDoc->SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti
		}

		if( dlg.m_dStartPlotTime != m_PlotPar.m_dStartPlot || dlg.m_dEndPlotTime != m_PlotPar.m_dEndPlot )
			m_PlotPar.m_bXAxisIntervalChanged = TRUE;
		m_PlotPar.m_dStartPlot = dlg.m_dStartPlotTime;
		m_PlotPar.m_dEndPlot = dlg.m_dEndPlotTime;

		if( dlg.m_nPlotMultiPar == 0 )
			m_PlotPar.m_nParNumPlot = PlotParameters::MULTI_PAR;
		else
			m_PlotPar.m_nParNumPlot = PlotParameters::SINGLE_PAR;

		if( dlg.m_nScaleYSame == 0 )
			m_PlotPar.m_nScaleY = PlotParameters::SCALE_Y_SAME;
		else
			m_PlotPar.m_nScaleY = PlotParameters::SCALE_Y_DIFFERENT;

		if( dlg.m_nPlot2D == 0 )
		{
			m_PlotPar.m_nPhasePlotDim = PlotParameters::PLOT_2D;
			
			if( m_nPlotType != PHASE_PLOT_2D )
			{
				m_PlotPar.m_bViewTypeChanged = TRUE;
				if( m_nPlotType == FOURIER_SPECTRUM_PLOT )
					m_PlotPar.m_nRealCalcNum = m_IntPar.m_nIntSteps[0];
			}

			m_nPlotType = PHASE_PLOT_2D;
		}
		else
		{
			m_PlotPar.m_nPhasePlotDim = PlotParameters::PLOT_3D;
			
			if( m_nPlotType != PHASE_PLOT_3D )
			{
				m_PlotPar.m_bViewTypeChanged = TRUE;
				if( m_nPlotType == FOURIER_SPECTRUM_PLOT )
					m_PlotPar.m_nRealCalcNum = m_IntPar.m_nIntSteps[0];
			}

			m_nPlotType = PHASE_PLOT_3D;
		}

		for( i=0; i<MAX_PLOTTING_VARS; i++ )
			if( m_PlotPar.m_anPlottingVars[i] != m_PlotPar.m_anOldPlottingVars[i] )
				m_PlotPar.m_bViewTypeChanged = TRUE;

		if( m_PlotPar.m_bViewTypeChanged == TRUE )
			pDoc->SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti

		m_szTitle = dlg.m_szTitle;
	}
	else
	{
		for( i=0; i<MAX_PLOTTING_VARS; i++ )
		{
			m_PlotPar.m_anPlottingVars[i] = m_PlotPar.m_anOldPlottingVars[i];
			m_PlotPar.m_anSysVarsOrder[i] = OldSysVar[i];
		}
	}

	return ret;
}

//////////////////////					RAVNOTEŽNA RJEŠENJA							//////////////////////////////

void CNLSysView::OnPlotEquilibriumSolutions() 
{
	CPlotEquSurfaceDlg		dlg;
	CNLSysDoc		*pDoc = GetDocument();
	
	SetDialogValues(dlg);

again_equ_surface:
	if( dlg.DoModal() == IDOK )
	{
		if( m_bCalcInProgress == TRUE )
		{
			AfxMessageBox(IDS_ALREADY_CALCULATING_FOR_THIS_WINDOW);
			return;
		}

		if( GetDialogValues(dlg) )
		{
			EqvSolCalcParameters *pECP = new EqvSolCalcParameters;

			pECP->nType = EqvSolCalcParameters::EQU_SURFACE_PLOT_2D;

			pECP->x1 = m_PlotPar.m_dXLow;
			pECP->x2 = m_PlotPar.m_dXUpp;

			pECP->pDoc = pDoc;
			pECP->pView = this;

			pECP->nParVarNum = pDoc->m_DES.GetOrderOfParVar(m_PlotPar.m_anPlottingVars[0]);
			pECP->nSysVarNum = pDoc->m_DES.GetOrderOfSysVar(m_PlotPar.m_anPlottingVars[1]);

			m_IntPar.m_hWnd = m_hWnd;
			
			AfxBeginThread(CalcEqvSolution, pECP);
			m_bCalcDone = FALSE;
		}
		else
			goto again_equ_surface;

		Invalidate();
	}
}

void CNLSysView::SetDialogValues(CPlotEquSurfaceDlg & dlg)
{
	CNLSysDoc* pDoc = GetDocument();

	if( m_PlotPar.m_nPhasePlotDim == PlotParameters::PLOT_2D )
	{
		dlg.m_nPlot2D = 0;
		dlg.m_nPlot3D = -1;
	}
	else
	{
		dlg.m_nPlot2D = -1;
		dlg.m_nPlot3D = 0;
		dlg.m_szVar3 = pDoc->m_DES.GetVarName(m_PlotPar.m_anPlottingVars[2]);
	}

	// ako je i prethodno iscrtavanje bilo iscrtavanje ravnotežne površine
	if( m_nPlotType == EQU_SURFACE_PLOT_2D )
	{
		// onda ubaci prethodne vrijednosti raspona za iscrtavanje
		dlg.m_dRange1From = m_PlotPar.m_dXLow;
		dlg.m_dRange1To = m_PlotPar.m_dXUpp;
	}
	else if( m_nPlotType == EQU_SURFACE_PLOT_3D )
	{
		dlg.m_dRange1From = m_PlotPar.m_dXLow;
		dlg.m_dRange1To = m_PlotPar.m_dXUpp;
		dlg.m_dRange2From = m_PlotPar.m_dYLow;
		dlg.m_dRange2To = m_PlotPar.m_dYUpp;
	}

	dlg.m_szVar1 = pDoc->m_DES.GetVarName(m_PlotPar.m_anPlottingVars[0]);
	dlg.m_szVar2 = pDoc->m_DES.GetVarName(m_PlotPar.m_anPlottingVars[1]);
}

int CNLSysView::GetDialogValues(CPlotEquSurfaceDlg & dlg)
{
	CNLSysDoc* pDoc = GetDocument();
	int			i, j, num, ret=TRUE, OldSysVar[MAX_PLOTTING_VARS];

	for( i=0; i<MAX_PLOTTING_VARS; i++ )
	{
		// najprije zapamti koje smo varijable iscrtavali u prethodnom crtanju
		m_PlotPar.m_anOldPlottingVars[i] = m_PlotPar.m_anPlottingVars[i];
		OldSysVar[i] = m_PlotPar.m_anSysVarsOrder[i];

		m_PlotPar.m_anPlottingVars[i] = -1;
		m_PlotPar.m_anSysVarsOrder[i] = -1;
	}

	num = 0;
	if( !dlg.m_szVar1.IsEmpty() &&	(j = pDoc->GetVarNum(dlg.m_szVar1)) >= 0 )
	{
		// èim smo došli dovde znaèi da je uneseno ispravno ime varijable
		if( pDoc->m_DES.IsSystemVar(j) )
		{
			// ali, za prvi parametar obavezno mora biti uneseno ime parametra sustava
			AfxMessageBox(IDS_FIRST_EQU_SURF_MUST_BE_PAR);
			ret =	FALSE;
		}
		m_PlotPar.m_anPlottingVars[num++] = j;
	}
	else		// ovdje obraditi gresku kada se ne unese ime varijable
	{
		AfxMessageBox(IDS_ERR_NOT_VAR_NAME);
		ret = FALSE;
	}
	
	if( !dlg.m_szVar2.IsEmpty() && (j = pDoc->GetVarNum(dlg.m_szVar2)) >= 0 )
	{
		m_PlotPar.m_anPlottingVars[num] = j;

		if( pDoc->m_DES.IsSystemVar(j) )
			m_PlotPar.m_anSysVarsOrder[num++] = pDoc->m_DES.GetOrderOfSysVar(j);
	}
	else
	{
		AfxMessageBox(IDS_ERR_NOT_VAR_NAME);
		ret = FALSE;
	}
	
	// samo ukoliko smo dovde stigli bez greške, æemo promijeniti neke parametre u CView klasi
	if( ret != FALSE )
	{
		if( dlg.m_nPlot2D == 0 )
		{
			m_PlotPar.m_nPhasePlotDim = PlotParameters::PLOT_2D;
			
			if( m_nPlotType != EQU_SURFACE_PLOT_2D )
				m_PlotPar.m_bViewTypeChanged = TRUE;

			m_nPlotType = EQU_SURFACE_PLOT_2D;
		}
		else
		{
			m_PlotPar.m_nPhasePlotDim = PlotParameters::PLOT_3D;
			
			if( m_nPlotType != EQU_SURFACE_PLOT_3D )
				m_PlotPar.m_bViewTypeChanged = TRUE;

			m_nPlotType = EQU_SURFACE_PLOT_3D;
		}

		if( m_PlotPar.m_dXLow != dlg.m_dRange1From || m_PlotPar.m_dXUpp != dlg.m_dRange1To )
			m_PlotPar.m_bXAxisIntervalChanged = TRUE;

		m_PlotPar.m_dXLow = dlg.m_dRange1From;
		m_PlotPar.m_dXUpp = dlg.m_dRange1To;

		if( m_nPlotType == EQU_SURFACE_PLOT_3D )
		{
			m_PlotPar.m_dYLow = dlg.m_dRange2From;
			m_PlotPar.m_dYUpp = dlg.m_dRange2To;
		}
		for( i=0; i<MAX_PLOTTING_VARS; i++ )
			if( m_PlotPar.m_anPlottingVars[i] != m_PlotPar.m_anOldPlottingVars[i] )
				m_PlotPar.m_bViewTypeChanged = TRUE;

		if( m_PlotPar.m_bViewTypeChanged == TRUE )
			pDoc->SetModifiedFlag(TRUE);		// da upozori korisnika da se ima nešto novo za snimiti
	}
	else
	{
		for( i=0; i<MAX_PLOTTING_VARS; i++ )
		{
			m_PlotPar.m_anPlottingVars[i] = m_PlotPar.m_anOldPlottingVars[i];
			m_PlotPar.m_anSysVarsOrder[i] = OldSysVar[i];
		}
	}

	return ret;
}

/////////////////////////							FOURIEROV SPEKTAR								/////////////////////////

void CNLSysView::OnPlotSpectrum() 
{
	if( m_bCalcInProgress == TRUE )
	{
		AfxMessageBox(IDS_ALREADY_CALCULATING_FOR_THIS_WINDOW);
		return;
	}

	if( m_nPlotType == TRAJECTORY_PLOT || m_nPlotType == PHASE_PLOT_2D || m_nPlotType == PHASE_PLOT_3D )
	{
		m_nPlotType = FOURIER_SPECTRUM_PLOT;
		m_PlotPar.m_bViewTypeChanged = TRUE;

		m_bCalcDone = FALSE;
		m_IntPar.m_hWnd = m_hWnd;			// da kreirana nit zna kojem prozoru treba poslati poruku
		
		AfxBeginThread(CalcSpectrum, this);
		m_bCalcDone = FALSE;

		Invalidate();
	}
	else
		AfxMessageBox(IDS_FOURIER_PLOT_ONLY_FOR_TRAJECTORIES);
}

////////////////////				LYAPUNOVLJEVI EKSPONENTI						/////////////////////////////

void CNLSysView::OnPlotLyapunovExp() 
{
	CNLSysDoc		*pDoc = GetDocument();
	CPlotLyapExpDlg			dlg;
	BOOL		bSamePar;
	int			i, j, k, num;

	SetDialogValues(dlg);

again_plot_lyap:
	if( dlg.DoModal() == IDOK )
	{
		if( m_bCalcInProgress == TRUE )
		{
			AfxMessageBox(IDS_ALREADY_CALCULATING_FOR_THIS_WINDOW);
			return;
		}

		LyapCalcParameters	*pLCP = new LyapCalcParameters;
		pLCP->X = new Vector(pDoc->m_DES.GetDim());
		pLCP->P = new Vector(pDoc->m_DES.GetParNum());
		pLCP->pDocument = pDoc;
		pLCP->pView = this;
		pLCP->hWnd = m_hWnd;

		pLCP->dT = dlg.m_dT;
		pLCP->dMinH = dlg.m_dMinStepSize;
		pLCP->dEps = dlg.m_dEps;
		pLCP->nMaxIter = dlg.m_nMaxIterNum;
		pLCP->dEpsRel = dlg.m_dEpsRel;
		pLCP->dEpsAbs = dlg.m_dEpsAbs;

		if( GetDialogValues(dlg, pLCP) )
		{
			k = pLCP->nChangeValNum;

			if( dlg.m_nUseAlreadyCalc == 0 )
			{
				// ukoliko æemo samo iscrtati veæ izraèunate eksponente, onda ih samo treba posložiti i iscrtati
				// ali, najprije treba pobrojati koliko ih ima da znamo alocirati kolièinu memorije
				num = 0;
				for( i=0; i<pDoc->m_LyapAnal.m_nLyapExpNum; i++ )
				{
					bSamePar = TRUE;
					// sada treba provjeriti da li ovaj po preostalim parametrima odgovara
					for( j=0; j<pDoc->m_DES.GetDim(); j++ )
						if( (*pLCP->X)[j] != (*pDoc->m_LyapAnal.m_avecLyapStartVal[i])[j] )
						{
							// ukoliko se komponente vektora razliku, jedina pozitivna moguænost je da
							// je upravo to komponenta po kojoj mijenjamo
							if( !( pLCP->bChangePar == FALSE && k == j ) )
								bSamePar = FALSE;		// a ovdje izgleda da nije
						}
					for( j=0; j<pDoc->m_DES.GetParNum(); j++ )
						if( (*pLCP->P)[j] != (*pDoc->m_LyapAnal.m_avecLyapParVal[i])[j] )
						{
							if( !( pLCP->bChangePar == TRUE && k == j ) )
								bSamePar = FALSE;
						}
					if( bSamePar == TRUE )
							num++;
				}

				if( m_vecParVal != NULL )
				{
					delete m_vecParVal;
					m_vecParVal = NULL;
				}
				if( m_matLyapExp != NULL )
				{
					delete m_matLyapExp;
					m_matLyapExp = NULL;
				}

				m_vecParVal = new Vector(num);
				m_matLyapExp = new Matrix( pDoc->m_DES.GetDim(), num );

				num = 0;
				for( i=0; i<pDoc->m_LyapAnal.m_nLyapExpNum; i++ )
				{
					bSamePar = TRUE;
					// sada treba provjeriti da li ovaj po preostalim parametrima odgovara
					for( j=0; j<pDoc->m_DES.GetDim(); j++ )
						if( (*pLCP->X)[j] != (*pDoc->m_LyapAnal.m_avecLyapStartVal[i])[j] )
						{
							// ukoliko se komponente vektora razliku, jedina pozitivna moguænost je da
							// je upravo to komponenta po kojoj mijenjamo
							if( !( pLCP->bChangePar == FALSE && k == j ) )
								bSamePar = FALSE;		// a ovdje izgleda da nije
						}
					for( j=0; j<pDoc->m_DES.GetParNum(); j++ )
						if( (*pLCP->P)[j] != (*pDoc->m_LyapAnal.m_avecLyapParVal[i])[j] )
						{
							if( !( pLCP->bChangePar == TRUE && k == j ) )
								bSamePar = FALSE;
						}
					
					if( bSamePar == TRUE )
					{
						if( pLCP->bChangePar )
							// znaèi da smo za mijenjajuæi parametar odabrali parametar sustava
							(*m_vecParVal)[i] = (*pDoc->m_LyapAnal.m_avecLyapParVal[i])[k];
						else
							// a ovdje smo ako mijenjamo po varijabli sustava
							(*m_vecParVal)[i] = (*pDoc->m_LyapAnal.m_avecLyapStartVal[i])[k];

						for( j=0; j<pDoc->m_DES.GetDim(); j++ )
							(*m_matLyapExp)[j][num] = (*pDoc->m_LyapAnal.m_avecLyapExp[i])[j];
						
						num++;
					}
				}
				if( num == 0 )
					AfxMessageBox(IDS_NO_LYAP_EXP_CALC);
				
				m_PlotPar.m_nRealCalcNum = num;
				m_bCalcDone = TRUE;
			}
			else
			{
				// ukoliko æemo zahtijevati iscrtavanje za sve navedene vrijednosti, onda pokreæemo novu nit
				if( m_vecParVal != NULL )
				{
					delete m_vecParVal;
					m_vecParVal = NULL;
				}
				if( m_matLyapExp != NULL )
				{
					delete m_matLyapExp;
					m_matLyapExp = NULL;
				}

				num = (int) ((pLCP->dX2 - pLCP->dX1) / pLCP->dStep) + 1;
				m_vecParVal = new Vector(num);
				m_matLyapExp = new Matrix( pDoc->m_DES.GetDim(), num );

				AfxBeginThread(CalcLyapMany, pLCP);
				m_bCalcDone = FALSE;
			}
		}
		else
			goto again_plot_lyap;
	}
	Invalidate();
}

void CNLSysView::SetDialogValues( CPlotLyapExpDlg & dlg )
{
	CNLSysDoc		*pDoc = GetDocument();

	dlg.m_nConnect = -1;
	dlg.m_nMark = 0;

	dlg.m_nCalcAll = -1;
	dlg.m_nUseAlreadyCalc = 0;

	LPTSTR	p;

	p = dlg.m_szFixedVal.GetBuffer(500);
	strcpy(p, pDoc->m_szStartVal);
	strcat(p, "\r\n");
	strcat(p, pDoc->m_szParamVal);
	dlg.m_szFixedVal.ReleaseBuffer();

	dlg.m_dMinStepSize = 1e-20;
	dlg.m_dEps = 1e-4;
	dlg.m_nMaxIterNum = 10000;
	dlg.m_dT = 0.5;
}

int CNLSysView::GetDialogValues( CPlotLyapExpDlg & dlg, LyapCalcParameters *pLCP )
{
	CNLSysDoc		*pDoc = GetDocument();

	char		achChPar[20], achFixVal[500];

	strcpy(achChPar, dlg.m_szChangePar);
	// sada treba ustanoviti da li je uneseno ispravno ime, i kakva je to varijabla
	if( pDoc->m_DES.IsSystemVar(achChPar) )
	{
		pLCP->nChangeValNum = pDoc->m_DES.GetOrderOfSysVar(achChPar);
		pLCP->bChangePar = false;
	}
	else if ( pDoc->m_DES.IsParameterVar(achChPar) )
	{
		pLCP->nChangeValNum = pDoc->m_DES.GetOrderOfParVar(achChPar);
		pLCP->bChangePar = true;
	}
	else
	{
		AfxMessageBox(IDS_NOT_SYS_NOT_PAR);
		return 0;
	}
	
	m_nPlotType = LYAP_EXP_PLOT;
	m_PlotPar.m_bViewTypeChanged = TRUE;
	
	if( dlg.m_nConnect == 0 )
		m_PlotPar.m_bConnectLyap = TRUE;
	else
		m_PlotPar.m_bConnectLyap = FALSE;

	pLCP->dX1 = dlg.m_dX1;
	pLCP->dX2 = dlg.m_dX2;
	pLCP->dStep = dlg.m_dStep;

	strcpy(achFixVal, dlg.m_szFixedVal);
	pDoc->m_DES.FillMixedValuesFromString( achFixVal, *(pLCP->X), *(pLCP->P) );
	
	m_PlotPar.m_dStartPlot = pLCP->dX1;
	m_PlotPar.m_dEndPlot = pLCP->dX2;
	m_PlotPar.m_bXAxisIntervalChanged = TRUE;

	return 1;
}

//////////////////////////////////////////////////////////////////////
//  CNLSView  help functions

int	CNLSysView::DifferentIntPar()
{
	if( m_bSystemChanged == TRUE )
		return 1;

	if( m_IntPar.m_dStep != m_LastSuccIntPar.m_dStep )
		return 1;
	if( m_IntPar.m_dStart != m_LastSuccIntPar.m_dStart )
		return 1;
	if( m_IntPar.m_dEnd != m_LastSuccIntPar.m_dEnd )
		return 1;
	if( m_IntPar.m_nSaveValNum != m_LastSuccIntPar.m_nSaveValNum )
		return 1;
	if( m_IntPar.m_dEps != m_LastSuccIntPar.m_dEps )
		return 1;

	// što za višeparametarsko iscrtavanje - treba provjeriti da li je i lista parametara ista
	if( !(*m_IntPar.m_vecInitCond == *m_LastSuccIntPar.m_vecInitCond) )
		return 1;
	if( !(*m_IntPar.m_vecParamVal == *m_LastSuccIntPar.m_vecParamVal) )
		return 1;

	if( m_nPlotType == TRAJECTORY_PLOT && m_PlotPar.m_nParNumPlot == PlotParameters::MULTI_PAR )
	{
		if( !(*m_IntPar.m_vecChgParList == *m_LastSuccIntPar.m_vecChgParList ) )
			return 1;
	}
	
	// ako smo stigli ovdje, znaèi da su svi isti (odnosno nisu se promjenili)
	return 0;
}

// provjerava da li je k-ta sistemska varijabla, odreðena za iscrtavanje
BOOL CNLSysView::IsPlottingVar( int k )
{
	int		i;

	for( i=0; i<MAX_PLOTTING_VARS; i++ )
		if( m_PlotPar.m_anSysVarsOrder[i] == k )
			return TRUE;

	return FALSE;
}

void CNLSysView::SetCalcDone(BOOL val)
{
	m_bCalcDone = val;
}

int CNLSysView::NumOfPlottingVars( void )
{
	int			i, ret=0;

	switch( m_nPlotType )
	{
		case TRAJECTORY_PLOT :
			{
				for( i=0; i<MAX_PLOTTING_VARS; i++ )
					if( m_PlotPar.m_anPlottingVars[i] != -1 )
						ret++;
				break;
			}
		case PHASE_PLOT_2D :	ret = 2;  break;
		case PHASE_PLOT_3D :	ret = 3;  break;
	}

	return ret;
}

void CNLSysView::OnPlotGetCloser() 
{
	m_PlotPar.m_dRho *= 0.9;

	double		x, y, z;
	x = GetX(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * cos(m_PlotPar.m_dAlpha * 3.141/180);
	y = GetY(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * sin(m_PlotPar.m_dAlpha * 3.141/180);
	z = GetZ(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * sin(m_PlotPar.m_dTheta * 3.141/180);
			
	Point3D		pointEye(x,y,z);
	*m_PlotPar.m_matT = GetProjectionMatrix(pointEye, *m_PlotPar.pointLookTo);

	Invalidate();
}

void CNLSysView::OnPlotGetFarther() 
{
	m_PlotPar.m_dRho *= 1.1;

	double		x, y, z;
	x = GetX(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * cos(m_PlotPar.m_dAlpha * 3.141/180);
	y = GetY(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * sin(m_PlotPar.m_dAlpha * 3.141/180);
	z = GetZ(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * sin(m_PlotPar.m_dTheta * 3.141/180);

	Point3D		pointEye(x,y,z);
	*m_PlotPar.m_matT = GetProjectionMatrix(pointEye, *m_PlotPar.pointLookTo);

	Invalidate();
}

void CNLSysView::OnPlotXCondense() 
{
	if( m_nPlotType == PHASE_PLOT_3D )
		m_PlotPar.m_dMulX *= 0.9;
	else
	{
		double		x1, x2, as;

		as = (m_PlotPar.m_dXUpp + m_PlotPar.m_dXLow) / 2;
		x1 = as - (as - m_PlotPar.m_dXLow)/1.1;
		x2 = as + (m_PlotPar.m_dXUpp - as)/1.1;

		m_PlotPar.m_dXLow = x1;
		m_PlotPar.m_dXUpp = x2;

		m_PlotPar.m_bPlotParametersChanged = TRUE;
	}

	Invalidate();
}

void CNLSysView::OnPlotXExpand() 
{
	if( m_nPlotType == PHASE_PLOT_3D )
		m_PlotPar.m_dMulX *= 1.1;
	else
	{
		double		x1, x2, as;

		as = (m_PlotPar.m_dXUpp + m_PlotPar.m_dXLow)/2;
		x1 = as - (as - m_PlotPar.m_dXLow)*1.1;
		x2 = as + (m_PlotPar.m_dXUpp - as)*1.1;

		m_PlotPar.m_dXLow = x1;
		m_PlotPar.m_dXUpp = x2;

		m_PlotPar.m_bPlotParametersChanged = TRUE;
	}
	Invalidate();
}

void CNLSysView::OnPlotXRight() 
{
	double		x1, x2, dLen;

	dLen = m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow;

	x1 = m_PlotPar.m_dXLow - dLen*0.07;
	x2 = m_PlotPar.m_dXUpp - dLen*0.07;

	m_PlotPar.m_dXLow = x1;
	m_PlotPar.m_dXUpp = x2;

	Invalidate();	
}

void CNLSysView::OnPlotXLeft() 
{
	double		x1, x2, dLen;
	
	dLen = m_PlotPar.m_dXUpp - m_PlotPar.m_dXLow;

	x1 = m_PlotPar.m_dXLow + dLen*0.07;
	x2 = m_PlotPar.m_dXUpp + dLen*0.07;

	m_PlotPar.m_dXLow = x1;
	m_PlotPar.m_dXUpp = x2;

	Invalidate();	
}

void CNLSysView::OnPlotYCondense() 
{
	if( m_nPlotType == PHASE_PLOT_3D )
		m_PlotPar.m_dMulY *= 0.9;
	else
	{
		double		Y1, Y2, as;

		as = (m_PlotPar.m_dYUpp + m_PlotPar.m_dYLow) / 2;
		Y1 = as - (as - m_PlotPar.m_dYLow)/1.1;
		Y2 = as + (m_PlotPar.m_dYUpp - as)/1.1;

		m_PlotPar.m_dYLow = Y1;
		m_PlotPar.m_dYUpp = Y2;

		m_PlotPar.m_bPlotParametersChanged = TRUE;
	}

	Invalidate();
}

void CNLSysView::OnPlotYExpand() 
{
	if( m_nPlotType == PHASE_PLOT_3D )
		m_PlotPar.m_dMulY *= 1.1;
	else
	{
		double		Y1, Y2, as;

		as = (m_PlotPar.m_dYUpp + m_PlotPar.m_dYLow)/2;
		Y1 = as - (as - m_PlotPar.m_dYLow)*1.1;
		Y2 = as + (m_PlotPar.m_dYUpp - as)*1.1;

		m_PlotPar.m_dYLow = Y1;
		m_PlotPar.m_dYUpp = Y2;

		m_PlotPar.m_bPlotParametersChanged = TRUE;
	}
	Invalidate();
}

void CNLSysView::OnPlotYDown() 
{
	double		y1, y2, dLen;

	dLen = m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow;

	y1 = m_PlotPar.m_dYLow + dLen*0.07;
	y2 = m_PlotPar.m_dYUpp + dLen*0.07;

	m_PlotPar.m_dYLow = y1;
	m_PlotPar.m_dYUpp = y2;

	Invalidate();	
}

void CNLSysView::OnPlotYUp() 
{
	double		y1, y2, dLen;

	dLen = m_PlotPar.m_dYUpp - m_PlotPar.m_dYLow;

	y1 = m_PlotPar.m_dYLow - dLen*0.07;
	y2 = m_PlotPar.m_dYUpp - dLen*0.07;

	m_PlotPar.m_dYLow = y1;
	m_PlotPar.m_dYUpp = y2;

	Invalidate();
}

void CNLSysView::OnRotateDown() 
{
	m_PlotPar.m_dTheta += 10;		
	
	double		x, y, z;
	x = GetX(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * cos(m_PlotPar.m_dAlpha * 3.141/180);
	y = GetY(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * sin(m_PlotPar.m_dAlpha * 3.141/180);
	z = GetZ(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * sin(m_PlotPar.m_dTheta * 3.141/180);
			
	Point3D		pointEye(x,y,z);
	*m_PlotPar.m_matT = GetProjectionMatrix(pointEye, *m_PlotPar.pointLookTo);

	Invalidate();
}

void CNLSysView::OnRotateLeft() 
{
	m_PlotPar.m_dAlpha += 10;

	double		x, y, z;
	x = GetX(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * cos(m_PlotPar.m_dAlpha * 3.141/180);
	y = GetY(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * sin(m_PlotPar.m_dAlpha * 3.141/180);
	z = GetZ(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * sin(m_PlotPar.m_dTheta * 3.141/180);
			
	Point3D		pointEye(x,y,z);
	*m_PlotPar.m_matT = GetProjectionMatrix(pointEye, *m_PlotPar.pointLookTo);

	Invalidate();
}

void CNLSysView::OnRotateRight() 
{
	m_PlotPar.m_dAlpha -= 10;

	double		x, y, z;
	x = GetX(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * cos(m_PlotPar.m_dAlpha * 3.141/180);
	y = GetY(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * sin(m_PlotPar.m_dAlpha * 3.141/180);
	z = GetZ(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * sin(m_PlotPar.m_dTheta * 3.141/180);
			
	Point3D		pointEye(x,y,z);
	*m_PlotPar.m_matT = GetProjectionMatrix(pointEye, *m_PlotPar.pointLookTo);

	Invalidate();
}

void CNLSysView::OnRotateUp() 
{
	m_PlotPar.m_dTheta -= 10;		

	double		x, y, z;
	x = GetX(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * cos(m_PlotPar.m_dAlpha * 3.141/180);
	y = GetY(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * sin(m_PlotPar.m_dAlpha * 3.141/180);
	z = GetZ(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * sin(m_PlotPar.m_dTheta * 3.141/180);
			
	Point3D		pointEye(x,y,z);
	*m_PlotPar.m_matT = GetProjectionMatrix(pointEye, *m_PlotPar.pointLookTo);

	Invalidate();
}

void CNLSysView::OnResetMul() 
{
	m_PlotPar.m_dMulX = m_PlotPar.m_dStartMulX;	
	m_PlotPar.m_dMulY = m_PlotPar.m_dStartMulY;	

	m_PlotPar.m_dXUpp = m_PlotPar.m_dStartXUpp;
	m_PlotPar.m_dXLow = m_PlotPar.m_dStartXLow;
	m_PlotPar.m_dYUpp = m_PlotPar.m_dStartYUpp;
	m_PlotPar.m_dYLow = m_PlotPar.m_dStartYLow;

	Invalidate();
}

void CNLSysView::OnPlotExpandBoth() 
{
	m_PlotPar.m_dMulX *= 1.1;	
	m_PlotPar.m_dMulY *= 1.1;	

	Invalidate();
}

void CNLSysView::OnPlotCondenseBoth() 
{
	m_PlotPar.m_dMulX *= 0.9;	
	m_PlotPar.m_dMulY *= 0.9;	

	Invalidate();
}

CFrameWnd* CNLSysView::NewWindow() 
{
/*	CMainFrame *pMainFrame = (CMainFrame*)  AfxGetApp()->m_pMainWnd;
	// Get the active MDI child window.
	CChildFrame *pActiveChild = (CChildFrame *) pMainFrame->MDIGetActive();

	CNLSysDoc* pDocument;
	if (pActiveChild == NULL ||
	  (pDocument = (CNLSysDoc *)pActiveChild->GetActiveDocument()) == NULL)
	{
		TRACE0("Warning: No active document for WindowNew command.\n");
		AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
		return NULL;     // command failed
	}*/
	
	CNLSysDoc	*pDocument = GetDocument();
	// otherwise we have a new frame !
	CDocTemplate* pTemplate = pDocument->GetDocTemplate();
	ASSERT_VALID(pTemplate);
	CFrameWnd* pFrame = pTemplate->CreateNewFrame(pDocument, NULL/*pActiveChild*/);
	if (pFrame == NULL)
	{
		TRACE0("Warning: failed to create new frame.\n");
		return NULL;     // command failed
	}
	pTemplate->InitialUpdateFrame(pFrame, pDocument);
	pFrame->ActivateFrame();
	
	pFrame->SetWindowText(pDocument->GetTitle());
	
	return pFrame;
}

void CNLSysView::Set3DData()
{
	double		x, y, z;
	x = GetX(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * cos(m_PlotPar.m_dAlpha * 3.141/180);
	y = GetY(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * cos(m_PlotPar.m_dTheta * 3.141/180) * sin(m_PlotPar.m_dAlpha * 3.141/180);
	z = GetZ(*m_PlotPar.pointLookTo) + m_PlotPar.m_dRho * sin(m_PlotPar.m_dTheta * 3.141/180);
	Point3D		pointEye(x,y,z);

	*m_PlotPar.m_matT = GetProjectionMatrix(pointEye, *m_PlotPar.pointLookTo);
}

void CNLSysView::SystemChanged( BOOL val )
{
	m_bSystemChanged = val;
}

void CNLSysView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if( m_PlotPar.m_bSetLegendPos == TRUE )
	{
		m_pntLegendPos = point;
		m_PlotPar.m_bSetLegendPos = FALSE;
		Invalidate();
	}
	if( m_PlotPar.m_bSetBifLegendPos == TRUE )
	{
		m_pntBifLegendPos = point;
		m_PlotPar.m_bSetBifLegendPos = FALSE;
		Invalidate();
	}

	CView::OnLButtonDown(nFlags, point);
}

void CNLSysView::OnSetLegendPosition() 
{
	m_PlotPar.m_bSetLegendPos = TRUE;	
}

void CNLSysView::OnUpdateShowLegend(CCmdUI* pCmdUI) 
{
	if( m_PlotPar.m_bShowLegend == TRUE )
		pCmdUI->SetText("Hide legend");
	else
		pCmdUI->SetText("Show legend");
}

void CNLSysView::OnShowLegend() 
{
	// TODO: Add your command handler code here
	if( m_PlotPar.m_bShowLegend == TRUE )
		m_PlotPar.m_bShowLegend = FALSE;
	else
		m_PlotPar.m_bShowLegend = TRUE;

	Invalidate();
}

void CNLSysView::OnOptionsSetBifLegendPos() 
{
	m_PlotPar.m_bSetBifLegendPos = TRUE;	
}

void CNLSysView::OnOptionsShowBifInEquSurf() 
{
	if( m_PlotPar.m_bShowBifInEqu == TRUE )
		m_PlotPar.m_bShowBifInEqu = FALSE;
	else
		m_PlotPar.m_bShowBifInEqu = TRUE;

	Invalidate();
}

void CNLSysView::OnUpdateOptionsShowBifInEquSurf(CCmdUI* pCmdUI) 
{
	if( m_PlotPar.m_bShowBifInEqu == TRUE )
		pCmdUI->SetText("Hide bif.points in equ.surface plot");
	else
		pCmdUI->SetText("Show bif.points in equ.surface plot");
}

void CNLSysView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CNLSysDoc		*pDoc = GetDocument();
	
	if( pDoc->m_bSysDefined )
	{
		strcpy( m_IntPar.m_szParamVal, pDoc->m_szParamVal );
		strcpy( m_IntPar.m_szInitCond, pDoc->m_szStartVal );

		m_IntPar.m_vecInitCond = new Vector( pDoc->m_DES.GetDim() );
		m_IntPar.m_vecParamVal = new Vector( pDoc->m_DES.GetParNum() );
		pDoc->m_DES.FillVarValuesFromString( pDoc->m_szStartVal, *m_IntPar.m_vecInitCond );
		pDoc->m_DES.FillParValuesFromString( pDoc->m_szParamVal, *m_IntPar.m_vecParamVal );
	}	
}

