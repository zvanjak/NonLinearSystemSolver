// NLSysOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nonlinsystems.h"
#include "NLSysOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLSysOptionsDlg dialog


CNLSysOptionsDlg::CNLSysOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNLSysOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLSysOptionsDlg)
	m_dEps = 0.0;
	m_nNewWindow = -1;
	m_nSameWindow = -1;
	m_dStepMin = 0.0;
	m_bUseColor = FALSE;
	//}}AFX_DATA_INIT
}


void CNLSysOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLSysOptionsDlg)
	DDX_Text(pDX, IDC_OPTIONS_EPS, m_dEps);
	DDX_Radio(pDX, IDC_OPTIONS_NEW_WINDOW, m_nNewWindow);
	DDX_Radio(pDX, IDC_OPTIONS_SAME_WINDOW, m_nSameWindow);
	DDX_Text(pDX, IDC_OPTIONS_STEPMIN, m_dStepMin);
	DDX_Check(pDX, IDC_OPTIONS_USE_COLOR, m_bUseColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLSysOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CNLSysOptionsDlg)
	ON_BN_CLICKED(IDC_OPTIONS_NEW_WINDOW, OnNewWindow)
	ON_BN_CLICKED(IDC_OPTIONS_SAME_WINDOW, OnSameWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLSysOptionsDlg message handlers

void CNLSysOptionsDlg::OnNewWindow() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_OPTIONS_NEW_WINDOW);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_OPTIONS_SAME_WINDOW);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nNewWindow = 0;
	m_nSameWindow = -1;	
}

void CNLSysOptionsDlg::OnSameWindow() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_OPTIONS_NEW_WINDOW);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_OPTIONS_SAME_WINDOW);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nNewWindow = -1;
	m_nSameWindow = 0;	
	
}
