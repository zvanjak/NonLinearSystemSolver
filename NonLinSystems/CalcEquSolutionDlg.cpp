// CalcEquSolutionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nonlinsystems.h"
#include "CalcEquSolutionDlg.h"
#include "NLSysDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalcEquSolutionDlg dialog


CCalcEquSolutionDlg::CCalcEquSolutionDlg(CNLSysDoc *pDocument, CWnd* pParent /*=NULL*/)
	: CDialog(CCalcEquSolutionDlg::IDD, pParent)
{
	pDoc = pDocument;

	Create(IDD_CALC_EQU_SOLUTION, pParent);
	
	//{{AFX_DATA_INIT(CCalcEquSolutionDlg)
	m_szEigenVal = _T("");
	m_szParVal = _T("");
	m_szSysVar = _T("");
	m_dEps = 0.0;
	m_nMaxIter = 0;
	//}}AFX_DATA_INIT
}


void CCalcEquSolutionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcEquSolutionDlg)
	DDX_Control(pDX, IDC_EQU_SOL_CALCULATE, m_ctrlCalculate);
	DDX_Control(pDX, IDC_EQU_SOL_SYS_VAR_FOUND, m_ctrlSysVar);
	DDX_Control(pDX, IDC_EQU_SOL_PAR_VAL_FOUND, m_ctrlParVal);
	DDX_Text(pDX, IDC_EQU_SOL_EIGENVAL, m_szEigenVal);
	DDX_Text(pDX, IDC_EQU_SOL_PAR_VAL, m_szParVal);
	DDX_Text(pDX, IDC_EQU_SOL_SYS_VAR, m_szSysVar);
	DDX_Text(pDX, IDC_EQU_SOL_EPS, m_dEps);
	DDV_MinMaxDouble(pDX, m_dEps, 1.e-008, 1.e-003);
	DDX_Text(pDX, IDC_EQU_SOL_MAX_ITER, m_nMaxIter);
	DDV_MinMaxInt(pDX, m_nMaxIter, 10, 2000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalcEquSolutionDlg, CDialog)
	//{{AFX_MSG_MAP(CCalcEquSolutionDlg)
	ON_BN_CLICKED(IDC_EQU_SOL_CALCULATE, OnEquSolCalculate)
	ON_LBN_SELCHANGE(IDC_EQU_SOL_PAR_VAL_FOUND, OnSelchangeEquSolParVal)
	ON_LBN_SELCHANGE(IDC_EQU_SOL_SYS_VAR_FOUND, OnSelchangeEquSolSysVar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalcEquSolutionDlg message handlers

void CCalcEquSolutionDlg::OnEquSolCalculate() 
{
	int			ret;
	char		StartVal[200], ParVal[200];
	Vector	X(pDoc->m_DES.GetDim()), P(pDoc->m_DES.GetParNum());

	UpdateData(TRUE);

	strcpy(StartVal, LPCSTR(m_szSysVar) );
	strcpy(ParVal, LPCSTR(m_szParVal) );

	if( (ret = pDoc->m_DES.FillVarValuesFromString(StartVal, X)) == 0 )
		ret = pDoc->m_DES.FillParValuesFromString(ParVal, P);

	if( ret < 0 )
		AfxMessageBox(pDoc->m_DES.ErrorText(ret));

	ret = pDoc->m_DES.CalcEqvSolution(&X, P);
	if( ret == 0 )
	{
		int		num, varnum, len, j;
		char	str[500];

		num = pDoc->PutInEquSolArray(X,P,&varnum);
		
		if( num >= 0 )
		{
			memset(str,0,500);
			len = 0;
			for( j=0; j<pDoc->m_DES.GetParNum(); j++ )
				len += sprintf(str + len, "%s = %10.6f, ", pDoc->m_DES.GetParVarName(j), (*pDoc->m_EquSolAnal.m_apvecParVal[num])[j] );
			
			CString		add(str);
			m_ctrlParVal.AddString(add);	
		}

		m_ctrlParVal.UpdateData();
	}
	else
		AfxMessageBox(IDS_COULDNT_FIND_EQU_SOL);
}

void CCalcEquSolutionDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();

	delete this;
}

void CCalcEquSolutionDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
	DestroyWindow();
}

void CCalcEquSolutionDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	DestroyWindow();
}

BOOL CCalcEquSolutionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int			i, j, len;
	char		str[500];

	m_ctrlSysVar.SetHorizontalExtent( 1000 );
	m_ctrlParVal.SetHorizontalExtent( 500 );

	for( i=0; i<pDoc->m_EquSolAnal.m_nEquSolNum; i++ )
	{
		memset(str,0,500);
		len = 0;
		for( j=0; j<pDoc->m_DES.GetParNum(); j++ )
			len += sprintf(str + len, "%s = %10.6f, ", pDoc->m_DES.GetParVarName(j), (*pDoc->m_EquSolAnal.m_apvecParVal[i])[j] );
		
		CString		add(str);
		m_ctrlParVal.AddString(add);	
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCalcEquSolutionDlg::OnSelchangeEquSolParVal() 
{
	// TODO: Add your control notification handler code here
	int		i, j, sel, len;
	char	str[500];

	// ukoliko odaberemo nove vrijednosit parametara
	sel = m_ctrlParVal.GetCurSel();
	
	// najprije æemo izbrisati vrijednosti iz liste varijabli sustava
	while( ( i=m_ctrlSysVar.GetCount() ) > 0 )
		m_ctrlSysVar.DeleteString(i-1);
	
	// a zatim treba ubaciti nove
	i = 0;
	while( pDoc->m_EquSolAnal.m_appvecSysVar[sel][i] != NULL )
	{
		memset(str,0,500);
		len = 0;
		for( j=0; j<pDoc->m_DES.GetDim(); j++ )
			len += sprintf(str + len, "%s = %10.6f, ", pDoc->m_DES.GetSysVarName(j), (*pDoc->m_EquSolAnal.m_appvecSysVar[sel][i])[j] );
		
		CString		add(str);
		m_ctrlSysVar.AddString(add);	
		
		i++;
	}

	UpdateData(FALSE);
}

void CCalcEquSolutionDlg::OnSelchangeEquSolSysVar() 
{
	// TODO: Add your control notification handler code here
	int		selPar, selVar, len, j;
	char	str[500];
	
	Vector		X(pDoc->m_DES.GetDim()), P(pDoc->m_DES.GetParNum());
	ComplexVector			cvec(pDoc->m_DES.GetDim());

	selVar = m_ctrlSysVar.GetCurSel();	
	selPar = m_ctrlParVal.GetCurSel();	

	memset(str,0,500);
	len = 0;
	
	P = *pDoc->m_EquSolAnal.m_apvecParVal[selPar];
	X = *pDoc->m_EquSolAnal.m_appvecSysVar[selPar][selVar];
													
	pDoc->m_DES.CalcEigenvalues(0, X, P, &cvec );
	len = 0;
	for( j=0; j<pDoc->m_DES.GetDim(); j++ )
	{
		len += sprintf(str + len, "%10.6f + i %10.6f", Re(cvec[j]), Im(cvec[j]) );
		str[len] = 13;
		str[len+1] = 10;
		len += 2;
	}
	CString		addEigen(str);
	m_szEigenVal = addEigen;

	UpdateData(FALSE);
}
