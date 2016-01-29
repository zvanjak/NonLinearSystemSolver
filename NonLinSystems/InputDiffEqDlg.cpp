// InputDiffEqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NonLinSystems.h"
#include "InputDiffEqDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputDiffEqDlg dialog


CInputDiffEqDlg::CInputDiffEqDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDiffEqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDiffEqDlg)
	m_szEquations = _T("");
	m_szIndVar = _T("");
	m_szParamVal = _T("");
	m_szStartVal = _T("");
	//}}AFX_DATA_INIT
}


void CInputDiffEqDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDiffEqDlg)
	DDX_Text(pDX, IDC_IDES_DIFFEQ_SYS, m_szEquations);
	DDX_Text(pDX, IDC_IDES_IND_VAR, m_szIndVar);
	DDX_Text(pDX, IDC_IDES_PARAM, m_szParamVal);
	DDX_Text(pDX, IDC_IDES_START_VAL, m_szStartVal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDiffEqDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDiffEqDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDiffEqDlg message handlers
