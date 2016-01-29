// NLSysOptions.cpp : implementation file
//

#include "stdafx.h"
#include "nonlinsystems.h"
#include "NLSysOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLSysOptions dialog


CNLSysOptions::CNLSysOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CNLSysOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLSysOptions)
	m_nNewWindow = -1;
	m_nSameWindow = -1;
	m_dEps = 0.0;
	m_dStepMin = 0.0;
	//}}AFX_DATA_INIT
}


void CNLSysOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLSysOptions)
	DDX_Radio(pDX, IDC_OPTIONS_NEW_WINDOW, m_nNewWindow);
	DDX_Radio(pDX, IDC_OPTIONS_SAME_WINDOW, m_nSameWindow);
	DDX_Text(pDX, IDC_OPTIONS_EPS, m_dEps);
	DDV_MinMaxDouble(pDX, m_dEps, 1.e-009, 100.);
	DDX_Text(pDX, IDC_OPTIONS_STEPMIN, m_dStepMin);
	DDV_MinMaxDouble(pDX, m_dStepMin, 1.e-020, 100.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNLSysOptions, CDialog)
	//{{AFX_MSG_MAP(CNLSysOptions)
	ON_BN_CLICKED(IDC_OPTIONS_NEW_WINDOW, OnOptionsNewWindow)
	ON_BN_CLICKED(IDC_OPTIONS_SAME_WINDOW, OnOptionsSameWindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLSysOptions message handlers

void CNLSysOptions::OnOptionsNewWindow() 
{
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_OPTIONS_NEW_WINDOW);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_OPTIONS_SAME_WINDOW);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nNewWindow = 0;
	m_nSameWindow = -1;	
}

void CNLSysOptions::OnOptionsSameWindow() 
{
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_OPTIONS_NEW_WINDOW);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_OPTIONS_SAME_WINDOW);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nNewWindow = -1;
	m_nSameWindow = 0;	
}
