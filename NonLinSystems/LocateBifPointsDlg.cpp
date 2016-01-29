// LocateBifPointsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NonLinSystems.h"
#include "NLSysDoc.h"
#include "LocateBifPointsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLocateBifPointsDlg dialog

CLocateBifPointsDlg::CLocateBifPointsDlg(CNLSysDoc *pDocument /*=NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CLocateBifPointsDlg::IDD, pParent)
{
	pDoc = pDocument;

	//{{AFX_DATA_INIT(CLocateBifPointsDlg)
	m_nHopf = -1;
	m_nSaddleNode = -1;
	m_szStartVal = _T("");
	m_szStartParVal = _T("");
	m_nSearchNum = 1;
	m_dP1 = 0.0;
	m_dP2 = 0.0;
	//}}AFX_DATA_INIT
}

void CLocateBifPointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLocateBifPointsDlg)
	DDX_Control(pDX, IDC_LOC_BIF_EQU_SOL_STATE_VAR, m_buttEquSolution);
	DDX_Control(pDX, IDC_LOC_BIF_EIGEN_START, m_ctrlEigenStart);
	DDX_Control(pDX, IDC_LOC_BIF_RANDOMIZE_EIGEN, m_buttRandomize);
	DDX_Radio(pDX, IDC_LOC_BIF_HOPF, m_nHopf);
	DDX_Radio(pDX, IDC_LOC_BIF_SNB, m_nSaddleNode);
	DDX_Text(pDX, IDC_LOC_BIF_VAR_START, m_szStartVal);
	DDX_Text(pDX, IDC_LOC_BIF_PAR_START, m_szStartParVal);
	DDX_Text(pDX, IDC_LOC_BIF_SEARCH_NUM, m_nSearchNum);
	DDV_MinMaxInt(pDX, m_nSearchNum, 1, 100);
	DDX_Text(pDX, IDC_LOC_BIF_P1, m_dP1);
	DDX_Text(pDX, IDC_LOC_BIF_P2, m_dP2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLocateBifPointsDlg, CDialog)
	//{{AFX_MSG_MAP(CLocateBifPointsDlg)
	ON_BN_CLICKED(IDC_LOC_BIF_HOPF, OnLocateBifHopf)
	ON_BN_CLICKED(IDC_LOC_BIF_SNB, OnLocateBifSNB)
	ON_BN_CLICKED(IDC_LOC_BIF_RANDOMIZE_EIGEN, OnLocateBifRandomizeEigen)
	ON_BN_CLICKED(IDC_LOC_BIF_EQU_SOL_STATE_VAR, OnLocateBifEquSolStateVar)
	ON_BN_CLICKED(IDC_LOC_BIF_AUTOMATIC, OnLocBifAutomatic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLocateBifPointsDlg message handlers

void CLocateBifPointsDlg::OnLocateBifHopf() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_LOC_BIF_HOPF);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_LOC_BIF_SNB);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nHopf = 0;
	m_nSaddleNode = -1;		
}

void CLocateBifPointsDlg::OnLocateBifSNB() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_LOC_BIF_HOPF);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_LOC_BIF_SNB);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nHopf = -1;
	m_nSaddleNode = 0;		
}

void CLocateBifPointsDlg::OnLocateBifRandomizeEigen() 
{
	char		str[500];
	double	valReal, valImag;
	int			i, len=0;
	CString	buff('\0',500);

	memset( str, 0, 500);
	srand( (unsigned) time( NULL ) );
	if( m_nHopf == 0 )
	{
		for( i=0; i<m_nDim; i++ )
		{
			valReal = ((double) rand()) / RAND_MAX * 2 - 1;
			valImag = ((double) rand()) / RAND_MAX * 2 - 1;
			
			sprintf(&str[len], "%10.5f+i%10.5f", valReal, valImag );
			
			str[len+22] = 13;
			str[len+23] = 10;
			len += 24;
		}
	}
	else
	{
		for( i=0; i<m_nDim; i++ )
		{
			valReal = ((double) rand()) / RAND_MAX * 2 - 1;
			sprintf(&str[len], "%10.5f", valReal );
			
			str[len+10] = 13;
			str[len+11] = 10;
			len += 12;
		}
	}
	LPTSTR	p = buff.GetBuffer(100);    strcpy(p, str);    buff.ReleaseBuffer();

	m_ctrlEigenStart.SetWindowText(buff);
	m_ctrlEigenStart.UpdateData(FALSE);

	m_ctrlEigenStart.GetWindowText( m_szEigenStart );
}

void CLocateBifPointsDlg::OnLocateBifEquSolStateVar() 
{
	// TODO: Add your control notification handler code here
	CString	buff('\0',500);
	char		StartVal[200], ParVal[200], str[500];
	int			i, ret, len;
	Vector	X(pDoc->m_DES.GetDim()), P(pDoc->m_DES.GetParNum());

	UpdateData(TRUE);

	strcpy(StartVal, LPCSTR(m_szStartVal) );
	strcpy(ParVal, LPCSTR(m_szStartParVal) );

	if( (ret = pDoc->m_DES.FillVarValuesFromString(StartVal, X)) == 0 )
		ret = pDoc->m_DES.FillParValuesFromString(ParVal, P);

	if( ret < 0 )
		AfxMessageBox(pDoc->m_DES.ErrorText(ret));
	else
	{
		if( pDoc->m_DES.CalcEqvSolution(&X, P) == 0 )
		{
			memset(str,0,500);
			len = 0;
			for( i=0; i<m_nDim; i++ )
			{
				len += sprintf(&str[len], "%s=%10.5f", pDoc->m_DES.GetSysVarName(i), X[i] );
				
				str[len] = 13;
				str[len+1] = 10;
				len += 2;
			}
			LPTSTR	p = m_szStartVal.GetBuffer(200);    strcpy(p, str);    m_szStartVal.ReleaseBuffer();
			UpdateData(FALSE);
		}
		else
			AfxMessageBox(IDS_COULDNT_FIND_EQU_SOL);
	}
}

void CLocateBifPointsDlg::OnLocBifAutomatic() 
{
	UpdateData(TRUE);
	EndDialog( ID_AUTOMATIC );	
}
