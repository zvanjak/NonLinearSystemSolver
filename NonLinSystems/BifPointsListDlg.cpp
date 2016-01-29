// BifPointsListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NonLinSystems.h"
#include "BifPointsListDlg.h"

#include "NLSysDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBifPointsListDlg dialog


CBifPointsListDlg::CBifPointsListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBifPointsListDlg::IDD, pParent)
{
	Create(IDD_SHOW_BIF_POINTS, pParent);
	
	//{{AFX_DATA_INIT(CBifPointsListDlg)
	m_szHopfEigenVal = _T("");
	m_szSNBEigenVal = _T("");
	//}}AFX_DATA_INIT

	m_nSNBPointsNum = 0;
	m_nHopfPointsNum = 0;
	m_pszSNBEigen = NULL;
	m_pszHopfEigen = NULL;
}

void CBifPointsListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBifPointsListDlg)
	DDX_Control(pDX, IDC_LIST_SNB_POINTS, m_ctrlSNBList);
	DDX_Control(pDX, IDC_LIST_HOPF_POINTS, m_ctrlHopfList);
	DDX_Text(pDX, IDC_HOPF_EIGENVAL, m_szHopfEigenVal);
	DDX_Text(pDX, IDC_SNB_EIGENVAL, m_szSNBEigenVal);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBifPointsListDlg, CDialog)
	//{{AFX_MSG_MAP(CBifPointsListDlg)
	ON_LBN_SELCHANGE(IDC_LIST_HOPF_POINTS, OnSelChangeHopfPoints)
	ON_LBN_SELCHANGE(IDC_LIST_SNB_POINTS, OnSelChangeSNBPoints)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBifPointsListDlg message handlers

void CBifPointsListDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();

	int			i;

	for( i=0; i<m_nSNBPointsNum; i++ )
		m_pszSNBEigen[i]->~CString();
	for( i=0; i<m_nHopfPointsNum; i++ )
		m_pszHopfEigen[i]->~CString();

	delete	m_pszSNBEigen;
	delete	m_pszHopfEigen;
	
	delete this;
}

void CBifPointsListDlg::OnOK() 
{
	CDialog::OnOK();
	DestroyWindow();
}

void CBifPointsListDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
	DestroyWindow();
}

void CBifPointsListDlg::OnSelChangeHopfPoints() 
{
	int		sel;

	sel = m_ctrlHopfList.GetCurSel();	
	m_szHopfEigenVal = *m_pszHopfEigen[sel];
	UpdateData(FALSE);
}

void CBifPointsListDlg::OnSelChangeSNBPoints() 
{
	int		sel;

	sel = m_ctrlSNBList.GetCurSel();
	m_szSNBEigenVal = *m_pszSNBEigen[sel];
	UpdateData(FALSE);
}
