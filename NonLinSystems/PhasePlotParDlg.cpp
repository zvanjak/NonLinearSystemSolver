// PhasePlotParDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NonLinSystems.h"
#include "PhasePlotParDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhasePlotParDlg dialog


CPhasePlotParDlg::CPhasePlotParDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPhasePlotParDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhasePlotParDlg)
	m_dStepSize = 0.0;
	m_dStartTime = 0.0;
	m_dEndTime = 0.0;
	m_szXAxis = _T("");
	m_szYAxis = _T("");
	m_szZAxis = _T("");
	m_szParValues = _T("");
	m_nPlot2D = -1;
	m_nPlot3D = -1;
	m_nPlotMultiPar = -1;
	m_nPlotSinglePar = -1;
	m_szChgParam = _T("");
	m_nScaleYDifferent = -1;
	m_nScaleYSame = -1;
	m_nSaveValNum = 0;
	m_dEps = 0.0;
	m_dStartPlotTime = 0.0;
	m_dEndPlotTime = 0.0;
	m_szParamVal = _T("");
	m_szStartVal = _T("");
	m_szTitle = _T("");
	//}}AFX_DATA_INIT
}


void CPhasePlotParDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhasePlotParDlg)
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_STEP, m_dStepSize);
	DDV_MinMaxDouble(pDX, m_dStepSize, 1.e-010, 10.);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_T1, m_dStartTime);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_T2, m_dEndTime);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_X, m_szXAxis);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_Y, m_szYAxis);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_Z, m_szZAxis);
	DDX_Text(pDX, IDC_PHASE_PAR_VALUES, m_szParValues);
	DDX_Radio(pDX, IDC_PHASE_PLOTPARAM_2D, m_nPlot2D);
	DDX_Radio(pDX, IDC_PHASE_PLOTPARAM_3D, m_nPlot3D);
	DDX_Radio(pDX, IDC_PHASE_PLOTPARAM_MULTI_PARAM2, m_nPlotMultiPar);
	DDX_Radio(pDX, IDC_PHASE_PLOTPARAM_SINGLE_PAR, m_nPlotSinglePar);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_CHANGING_PAR, m_szChgParam);
	DDX_Radio(pDX, IDC_PHASE_SCALE_Y_DIFFERENT, m_nScaleYDifferent);
	DDX_Radio(pDX, IDC_PHASE_SCALE_Y_SAME, m_nScaleYSame);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_SAVE_NUM, m_nSaveValNum);
	DDV_MinMaxInt(pDX, m_nSaveValNum, 10, 100000);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_EPS, m_dEps);
	DDV_MinMaxDouble(pDX, m_dEps, 1.e-008, 10.);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_PLOT_T1, m_dStartPlotTime);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_PLOT_T2, m_dEndPlotTime);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_PARAM_VAL, m_szParamVal);
	DDX_Text(pDX, IDC_PHASE_PLOTPARAM_START_VAL, m_szStartVal);
	DDX_Text(pDX, IDC_PHASE_PLOT_TITLE, m_szTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhasePlotParDlg, CDialog)
	//{{AFX_MSG_MAP(CPhasePlotParDlg)
	ON_BN_CLICKED(IDC_PHASE_PLOTPARAM_2D, OnPhasePlotparam2d)
	ON_BN_CLICKED(IDC_PHASE_PLOTPARAM_3D, OnPhasePlotparam3d)
	ON_BN_CLICKED(IDC_PHASE_PLOTPARAM_MULTI_PARAM2, OnPhasePlotparamMultiParam2)
	ON_BN_CLICKED(IDC_PHASE_PLOTPARAM_SINGLE_PAR, OnPhasePlotparamSinglePar)
	ON_BN_CLICKED(IDC_PHASE_SCALE_Y_DIFFERENT, OnPhaseScaleYDifferent)
	ON_BN_CLICKED(IDC_PHASE_SCALE_Y_SAME, OnPhaseScaleYSame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhasePlotParDlg message handlers

void CPhasePlotParDlg::OnPhasePlotparam2d() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PHASE_PLOTPARAM_2D);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PHASE_PLOTPARAM_3D);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nPlot2D = 0;
	m_nPlot3D = -1;			
}

void CPhasePlotParDlg::OnPhasePlotparam3d() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PHASE_PLOTPARAM_2D);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PHASE_PLOTPARAM_3D);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nPlot2D = -1;
	m_nPlot3D = 0;			
}

void CPhasePlotParDlg::OnPhasePlotparamMultiParam2() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PHASE_PLOTPARAM_MULTI_PARAM2);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PHASE_PLOTPARAM_SINGLE_PAR);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nPlotMultiPar = 0;
	m_nPlotSinglePar = -1;
}

void CPhasePlotParDlg::OnPhasePlotparamSinglePar() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PHASE_PLOTPARAM_MULTI_PARAM2);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PHASE_PLOTPARAM_SINGLE_PAR);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);	

	m_nPlotMultiPar = -1;
	m_nPlotSinglePar = 0;
}

void CPhasePlotParDlg::OnPhaseScaleYDifferent() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PHASE_SCALE_Y_SAME);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PHASE_SCALE_Y_DIFFERENT);

	pButt1->SetCheck(0);
	pButt2->SetCheck(1);

	m_nScaleYSame = -1;
	m_nScaleYDifferent = 0;		
}

void CPhasePlotParDlg::OnPhaseScaleYSame() 
{
	// TODO: Add your control notification handler code here
	CButton *pButt1 = (CButton *) GetDlgItem(IDC_PHASE_SCALE_Y_SAME);
	CButton *pButt2 = (CButton *) GetDlgItem(IDC_PHASE_SCALE_Y_DIFFERENT);

	pButt1->SetCheck(1);
	pButt2->SetCheck(0);

	m_nScaleYSame = 0;
	m_nScaleYDifferent = -1;			
}
