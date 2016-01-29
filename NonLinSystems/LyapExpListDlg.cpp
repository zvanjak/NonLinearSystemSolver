// LyapExpListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NonLinSystems.h"
#include "LyapExpListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLyapExpListDlg dialog


CLyapExpListDlg::CLyapExpListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLyapExpListDlg::IDD, pParent)
{
	Create(IDD_SHOW_LYAP_EXP, pParent);

	//{{AFX_DATA_INIT(CLyapExpListDlg)
	m_szExpVal = _T("");
	//}}AFX_DATA_INIT

	m_nLyapExpNum = 0;
	m_pszLyapExpVal = NULL;
}


void CLyapExpListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLyapExpListDlg)
	DDX_Control(pDX, IDC_SHOW_LYAP_LIST_VAL, m_ctrlValList);
	DDX_Text(pDX, IDC_SHOW_LYAP_EXP_VAL, m_szExpVal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLyapExpListDlg, CDialog)
	//{{AFX_MSG_MAP(CLyapExpListDlg)
	ON_LBN_SELCHANGE(IDC_SHOW_LYAP_LIST_VAL, OnSelChangeLyapListVal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLyapExpListDlg message handlers

void CLyapExpListDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
	DestroyWindow();
}

void CLyapExpListDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	DestroyWindow();
}

void CLyapExpListDlg::OnSelChangeLyapListVal() 
{
	// TODO: Add your control notification handler code here
	int		sel;

	sel = m_ctrlValList.GetCurSel();	
	m_szExpVal = *m_pszLyapExpVal[sel];
	UpdateData(FALSE);
}

void CLyapExpListDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();

	for( int i=0; i<m_nLyapExpNum; i++ )
		m_pszLyapExpVal[i]->~CString();

	delete	m_pszLyapExpVal;
	
	delete this;
}
