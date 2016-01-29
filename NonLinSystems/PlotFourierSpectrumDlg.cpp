// PlotFourierSpectrumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nonlinsystems.h"
#include "PlotFourierSpectrumDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlotFourierSpectrumDlg dialog


CPlotFourierSpectrumDlg::CPlotFourierSpectrumDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotFourierSpectrumDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlotFourierSpectrumDlg)
	m_szLegend1 = _T("");
	m_szLegend2 = _T("");
	m_szLegend3 = _T("");
	m_szLegend4 = _T("");
	m_szVar1 = _T("");
	m_szVar2 = _T("");
	m_szVar3 = _T("");
	m_szVar4 = _T("");
	//}}AFX_DATA_INIT
}


void CPlotFourierSpectrumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlotFourierSpectrumDlg)
	DDX_Text(pDX, IDC_PLOTSPECTRUM_LEGEND_VAR1, m_szLegend1);
	DDX_Text(pDX, IDC_PLOTSPECTRUM_LEGEND_VAR2, m_szLegend2);
	DDX_Text(pDX, IDC_PLOTSPECTRUM_LEGEND_VAR3, m_szLegend3);
	DDX_Text(pDX, IDC_PLOTSPECTRUM_LEGEND_VAR4, m_szLegend4);
	DDX_Text(pDX, IDC_PLOTSPECTRUM_VAR1, m_szVar1);
	DDX_Text(pDX, IDC_PLOTSPECTRUM_VAR2, m_szVar2);
	DDX_Text(pDX, IDC_PLOTSPECTRUM_VAR3, m_szVar3);
	DDX_Text(pDX, IDC_PLOTSPECTRUM_VAR4, m_szVar4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlotFourierSpectrumDlg, CDialog)
	//{{AFX_MSG_MAP(CPlotFourierSpectrumDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlotFourierSpectrumDlg message handlers
