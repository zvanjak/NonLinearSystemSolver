// TrajectoryPlotParDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NonLinSystems.h"
#include "TrajectoryPlotParDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrajectoryPlotParDlg dialog


CTrajectoryPlotParDlg::CTrajectoryPlotParDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrajectoryPlotParDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrajectoryPlotParDlg)
	m_nScaleYDifferent = -1;
	m_nScaleYSame = -1;
	m_dStepSize = 0.0;
	m_dStartTime = 0.0;
	m_dEndTime = 0.0;
	m_szChgParam = _T("");
	m_nPlotMultiPar = -1;
	m_nPlotSinglePar = -1;
	m_szParValues = _T("");
	m_szVar1 = _T("");
	m_szVar2 = _T("");
	m_szVar3 = _T("");
	m_szVar4 = _T("");
	m_szLegend1 = _T("");
	m_szLegend2 = _T("");
	m_szLegend3 = _T("");
	m_szLegend4 = _T("");
	m_nSaveValNum = 0;
	m_dEps = 0.0;
	m_dStartPlotTime = 0.0;
	m_dEndPlotTime = 0.0;
	m_szParamVal = _T("");
	m_szStartVal = _T("");
	m_szTitle = _T("");
	//}}AFX_DATA_INIT
}


void CTrajectoryPlotParDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrajectoryPlotParDlg)
	DDX_Radio(pDX, IDC_SCALE_Y_DIFFERENT, m_nScaleYDifferent);
	DDX_Radio(pDX, IDC_SCALE_Y_SAME, m_nScaleYSame);
	DDX_Text(pDX, IDC_PLOTPARAM_STEP, m_dStepSize);
	DDV_MinMaxDouble(pDX, m_dStepSize, 1.e-010, 10.);
	DDX_Text(pDX, IDC_PLOTPARAM_T1, m_dStartTime);
	DDX_Text(pDX, IDC_PLOTPARAM_T2, m_dEndTime);
	DDX_Text(pDX, IDC_PLOTPARAM_CHANGING_PAR, m_szChgParam);
	DDX_Radio(pDX, IDC_TPLOTPARAM_MULTI_PARAM, m_nPlotMultiPar);
	DDX_Radio(pDX, IDC_TPLOTPARAM_SINGLE_PAR, m_nPlotSinglePar);
	DDX_Text(pDX, IDC_PAR_VALUES, m_szParValues);
	DDX_Text(pDX, IDC_PLOTPARAM_TIME_VAR1, m_szVar1);
	DDX_Text(pDX, IDC_PLOTPARAM_TIME_VAR2, m_szVar2);
	DDX_Text(pDX, IDC_PLOTPARAM_TIME_VAR3, m_szVar3);
	DDX_Text(pDX, IDC_PLOTPARAM_TIME_VAR4, m_szVar4);
	DDX_Text(pDX, IDC_PLOTPARAM_LEGEND_VAR1, m_szLegend1);
	DDX_Text(pDX, IDC_PLOTPARAM_LEGEND_VAR2, m_szLegend2);
	DDX_Text(pDX, IDC_PLOTPARAM_LEGEND_VAR3, m_szLegend3);
	DDX_Text(pDX, IDC_PLOTPARAM_LEGEND_VAR4, m_szLegend4);
	DDX_Text(pDX, IDC_PLOTPARAM_SAVE_NUM, m_nSaveValNum);
	DDV_MinMaxInt(pDX, m_nSaveValNum, 10, 100000);
	DDX_Text(pDX, IDC_PLOTPARAM_EPS, m_dEps);
	DDV_MinMaxDouble(pDX, m_dEps, 1.e-008, 10.);
	DDX_Text(pDX, IDC_PLOTPARAM_PLOT_T1, m_dStartPlotTime);
	DDX_Text(pDX, IDC_PLOTPARAM_PLOT_T2, m_dEndPlotTime);
	DDX_Text(pDX, IDC_PLOTPARAM_PARAM_VAL, m_szParamVal);
	DDX_Text(pDX, IDC_PLOTPARAM_START_VAL, m_szStartVal);
	DDX_Text(pDX, IDC_TRAJECTORY_PLOT_TITLE, m_szTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrajectoryPlotParDlg, CDialog)
	//{{AFX_MSG_MAP(CTrajectoryPlotParDlg)
	ON_BN_CLICKED(IDC_TPLOTPARAM_MULTI_PARAM, OnTPlotMultiParam)
	ON_BN_CLICKED(IDC_TPLOTPARAM_SINGLE_PAR, OnTPlotSingleParam)
	ON_BN_CLICKED(IDC_SCALE_Y_DIFFERENT, OnTPlotScaleYDifferent)
	ON_BN_CLICKED(IDC_SCALE_Y_SAME, OnTPlotScaleYSame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrajectoryPlotParDlg message handlers

void CTrajectoryPlotParDlg::OnTPlotMultiParam() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_TPLOTPARAM_MULTI_PARAM);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_TPLOTPARAM_SINGLE_PAR);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nPlotMultiPar = 0;
	m_nPlotSinglePar = -1;

	CEdit	*pEd1 = (CEdit *) GetDlgItem(IDC_PLOTPARAM_CHANGING_PAR);
	pEd1->EnableWindow(TRUE);
	CEdit	*pEd2 = (CEdit *) GetDlgItem(IDC_PAR_VALUES);
	pEd2->EnableWindow(TRUE);
}

void CTrajectoryPlotParDlg::OnTPlotSingleParam() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_TPLOTPARAM_MULTI_PARAM);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_TPLOTPARAM_SINGLE_PAR);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nPlotMultiPar = -1;
	m_nPlotSinglePar = 0;

	CEdit	*pEd1 = (CEdit *) GetDlgItem(IDC_PLOTPARAM_CHANGING_PAR);
	pEd1->EnableWindow(FALSE);
	CEdit	*pEd2 = (CEdit *) GetDlgItem(IDC_PAR_VALUES);
	pEd2->EnableWindow(FALSE);
}

void CTrajectoryPlotParDlg::OnTPlotScaleYDifferent() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_SCALE_Y_SAME);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_SCALE_Y_DIFFERENT);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nScaleYSame = -1;
	m_nScaleYDifferent = 0;		
}

void CTrajectoryPlotParDlg::OnTPlotScaleYSame() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_SCALE_Y_SAME);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_SCALE_Y_DIFFERENT);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nScaleYSame = 0;
	m_nScaleYDifferent = -1;		
}
