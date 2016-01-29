#if !defined(AFX_PLOTLYAPEXPDLG_H__BF95A623_567E_11D1_B663_C0398C198829__INCLUDED_)
#define AFX_PLOTLYAPEXPDLG_H__BF95A623_567E_11D1_B663_C0398C198829__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PlotLyapExpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlotLyapExpDlg dialog

class CPlotLyapExpDlg : public CDialog
{
// Construction
public:
	CPlotLyapExpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlotLyapExpDlg)
	enum { IDD = IDD_PLOT_LYAP_EXP };
	CString	m_szChangePar;
	CString	m_szFixedVal;
	int		m_nConnect;
	int		m_nMark;
	double	m_dStep;
	double	m_dX1;
	double	m_dX2;
	int		m_nCalcAll;
	int		m_nUseAlreadyCalc;
	double	m_dEps;
	int		m_nMaxIterNum;
	double	m_dMinStepSize;
	double	m_dT;
	double	m_dEpsAbs;
	double	m_dEpsRel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotLyapExpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlotLyapExpDlg)
	afx_msg void OnPlotLyapCalcAll();
	afx_msg void OnPlotLyapConnect();
	afx_msg void OnPlotLyapMarkPoints();
	afx_msg void OnPlotLyapUseAlreadyCalc();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTLYAPEXPDLG_H__BF95A623_567E_11D1_B663_C0398C198829__INCLUDED_)
