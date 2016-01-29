// NLSysOptions.cpp : implementation file
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
// CNLSysOptions dialog


CNLSysOptionsDlg::CNLSysOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNLSysOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLSysOptions)
	m_nNewWindow = -1;
	m_nSameWindow = -1;
	//}}AFX_DATA_INIT
}


void CNLSysOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLSysOptions)
	DDX_Radio(pDX, IDC_OPTIONS_NEW_WINDOW, m_nNewWindow);
	DDX_Radio(pDX, IDC_OPTIONS_SAME_WINDOW, m_nSameWindow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLSysOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CNLSysOptionsDlg)
	ON_BN_CLICKED(IDC_OPTIONS_NEW_WINDOW, OnOptionsNewWindow)
	ON_BN_CLICKED(IDC_OPTIONS_SAME_WINDOW, OnOptionsSameWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLSysOptions message handlers

void CNLSysOptionsDlg::OnOptionsNewWindow() 
{
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_OPTIONS_NEW_WINDOW);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_OPTIONS_SAME_WINDOW);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nNewWindow = 0;
	m_nSameWindow = -1;	
}

void CNLSysOptionsDlg::OnOptionsSameWindow() 
{
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_OPTIONS_NEW_WINDOW);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_OPTIONS_SAME_WINDOW);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nNewWindow = -1;
	m_nSameWindow = 0;	
}
