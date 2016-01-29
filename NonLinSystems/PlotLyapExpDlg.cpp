// PlotLyapExpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NonLinSystems.h"
#include "PlotLyapExpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotLyapExpDlg dialog


CPlotLyapExpDlg::CPlotLyapExpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotLyapExpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlotLyapExpDlg)
	m_szChangePar = _T("");
	m_szFixedVal = _T("");
	m_nConnect = -1;
	m_nMark = 0;
	m_dStep = 1.0;
	m_dX1 = 0.0;
	m_dX2 = 0.0;
	m_nCalcAll = -1;
	m_nUseAlreadyCalc = 0;
	m_dEps = 0.0;
	m_nMaxIterNum = 0;
	m_dMinStepSize = 0.0;
	m_dT = 0.0;
	m_dEpsAbs = 1.0e-5;
	m_dEpsRel = 1.0e-4;
	//}}AFX_DATA_INIT
}


void CPlotLyapExpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotLyapExpDlg)
	DDX_Text(pDX, IDC_PLOT_LYAP_CHANGE_PAR, m_szChangePar);
	DDX_Text(pDX, IDC_PLOT_LYAP_FIXED_VAL, m_szFixedVal);
	DDX_Radio(pDX, IDC_PLOT_LYAP_CONNECT, m_nConnect);
	DDX_Radio(pDX, IDC_PLOT_LYAP_MARK, m_nMark);
	DDX_Text(pDX, IDC_PLOT_LYAP_STEP, m_dStep);
	DDV_MinMaxDouble(pDX, m_dStep, 1.e-006, 1000000.);
	DDX_Text(pDX, IDC_PLOT_LYAP_X1, m_dX1);
	DDX_Text(pDX, IDC_PLOT_LYAP_X2, m_dX2);
	DDX_Radio(pDX, IDC_PLOT_LYAP_CALC_ALL, m_nCalcAll);
	DDX_Radio(pDX, IDC_PLOT_LYAP_USE_ALREADY_CALC, m_nUseAlreadyCalc);
	DDX_Text(pDX, IDC_PLOT_LYAP_EPS, m_dEps);
	DDX_Text(pDX, IDC_PLOT_LYAP_MAX_ITER_NUM, m_nMaxIterNum);
	DDX_Text(pDX, IDC_PLOT_LYAP_MIN_STEPSIZE, m_dMinStepSize);
	DDX_Text(pDX, IDC_PLOT_LYAP_T, m_dT);
	DDX_Text(pDX, IDC_PLOT_LYAP_EPS_ABS, m_dEpsAbs);
	DDV_MinMaxDouble(pDX, m_dEpsAbs, 0., 100.);
	DDX_Text(pDX, IDC_PLOT_LYAP_EPS_REL, m_dEpsRel);
	DDV_MinMaxDouble(pDX, m_dEpsRel, 1.e-006, 100.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotLyapExpDlg, CDialog)
	//{{AFX_MSG_MAP(CPlotLyapExpDlg)
	ON_BN_CLICKED(IDC_PLOT_LYAP_CALC_ALL, OnPlotLyapCalcAll)
	ON_BN_CLICKED(IDC_PLOT_LYAP_CONNECT, OnPlotLyapConnect)
	ON_BN_CLICKED(IDC_PLOT_LYAP_MARK, OnPlotLyapMarkPoints)
	ON_BN_CLICKED(IDC_PLOT_LYAP_USE_ALREADY_CALC, OnPlotLyapUseAlreadyCalc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotLyapExpDlg message handlers

void CPlotLyapExpDlg::OnPlotLyapCalcAll() 
{
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PLOT_LYAP_CALC_ALL);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PLOT_LYAP_USE_ALREADY_CALC);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nCalcAll = 0;
	m_nUseAlreadyCalc = -1;

	CEdit	*pEd1 = (CEdit *) GetDlgItem(IDC_PLOT_LYAP_STEP);
	pEd1->EnableWindow(TRUE);
}

void CPlotLyapExpDlg::OnPlotLyapUseAlreadyCalc() 
{
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PLOT_LYAP_CALC_ALL);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PLOT_LYAP_USE_ALREADY_CALC);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nCalcAll = -1;
	m_nUseAlreadyCalc = 0;

	CEdit	*pEd1 = (CEdit *) GetDlgItem(IDC_PLOT_LYAP_STEP);
	pEd1->EnableWindow(FALSE);
}

void CPlotLyapExpDlg::OnPlotLyapMarkPoints() 
{
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PLOT_LYAP_MARK);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PLOT_LYAP_CONNECT);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nConnect = -1;
	m_nMark = 0;
}

void CPlotLyapExpDlg::OnPlotLyapConnect() 
{
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PLOT_LYAP_MARK);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PLOT_LYAP_CONNECT);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nConnect = 0;
	m_nMark = -1;
}

BOOL CPlotLyapExpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CEdit	*pEd1 = (CEdit *) GetDlgItem(IDC_PLOT_LYAP_STEP);
	pEd1->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
