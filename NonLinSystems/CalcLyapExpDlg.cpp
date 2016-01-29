// CalcLyapExpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NonLinSystems.h"
#include "CalcLyapExpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcLyapExpDlg dialog


CCalcLyapExpDlg::CCalcLyapExpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalcLyapExpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcLyapExpDlg)
	m_szStartParVal = _T("");
	m_szStartVal = _T("");
	m_dEps = 0.0;
	m_dMinStepSize = 0.0;
	m_dT = 0.0;
	m_dEpsAbs = 1.0e-5;
	m_dEpsRel = 1.0e-4;
	m_nMaxIterNum = 0;
	//}}AFX_DATA_INIT
}


void CCalcLyapExpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcLyapExpDlg)
	DDX_Text(pDX, IDC_CALC_LYAP_PAR_VAL, m_szStartParVal);
	DDX_Text(pDX, IDC_CALC_LYAP_VAR_START, m_szStartVal);
	DDX_Text(pDX, IDC_CALC_LYAP_EPS, m_dEps);
	DDX_Text(pDX, IDC_calc_LYAP_MIN_STEPSIZE, m_dMinStepSize);
	DDX_Text(pDX, IDC_CALC_LYAP_T, m_dT);
	DDX_Text(pDX, IDC_CALC_LYAP_EPS_ABS, m_dEpsAbs);
	DDX_Text(pDX, IDC_CALC_LYAP_EPS_REL, m_dEpsRel);
	DDX_Text(pDX, IDC_calc_LYAP_MAX_ITER_NUM, m_nMaxIterNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalcLyapExpDlg, CDialog)
	//{{AFX_MSG_MAP(CCalcLyapExpDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcLyapExpDlg message handlers
