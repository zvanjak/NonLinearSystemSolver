// PlotEquSurfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NonLinSystems.h"
#include "PlotEquSurfaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotEquSurfaceDlg dialog


CPlotEquSurfaceDlg::CPlotEquSurfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotEquSurfaceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlotEquSurfaceDlg)
	m_nPlot2D = -1;
	m_nPlot3D = -1;
	m_dRange1From = 0.0;
	m_dRange2From = 0.0;
	m_dRange1To = 0.0;
	m_dRange2To = 0.0;
	m_szVar1 = _T("");
	m_szVar2 = _T("");
	m_szVar3 = _T("");
	m_bShowBif = FALSE;
	//}}AFX_DATA_INIT
}


void CPlotEquSurfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotEquSurfaceDlg)
	DDX_Radio(pDX, IDC_PLOT_EQU_2D, m_nPlot2D);
	DDX_Radio(pDX, IDC_PLOT_EQU_3D, m_nPlot3D);
	DDX_Text(pDX, IDC_PLOT_EQU_RANGE1_FROM, m_dRange1From);
	DDX_Text(pDX, IDC_PLOT_EQU_RANGE2_FROM, m_dRange2From);
	DDX_Text(pDX, IDC_PLOT_EQU_RANGE1_TO, m_dRange1To);
	DDX_Text(pDX, IDC_PLOT_EQU_RANGE2_TO, m_dRange2To);
	DDX_Text(pDX, IDC_PLOT_EQU_X, m_szVar1);
	DDX_Text(pDX, IDC_PLOT_EQU_Y, m_szVar2);
	DDX_Text(pDX, IDC_PLOT_EQU_Z, m_szVar3);
	DDX_Check(pDX, IDC_PLOT_EQU_SHOW_BIF, m_bShowBif);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotEquSurfaceDlg, CDialog)
	//{{AFX_MSG_MAP(CPlotEquSurfaceDlg)
	ON_BN_CLICKED(IDC_PLOT_EQU_2D, OnPlotEqu2D)
	ON_BN_CLICKED(IDC_PLOT_EQU_3D, OnPlotEqu3D)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotEquSurfaceDlg message handlers

void CPlotEquSurfaceDlg::OnPlotEqu2D() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PLOT_EQU_2D);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PLOT_EQU_3D);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nPlot2D = 0;
	m_nPlot3D = -1;			
}

void CPlotEquSurfaceDlg::OnPlotEqu3D() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PLOT_EQU_2D);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PLOT_EQU_3D);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nPlot2D = -1;
	m_nPlot3D = 0;			
}
