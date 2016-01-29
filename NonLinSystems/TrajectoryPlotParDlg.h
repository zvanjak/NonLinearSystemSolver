#if !defined(AFX_TRAJECTORYPLOTPARDLG_H__0ADE2553_44A3_11D1_B623_9BDF4FBDB829__INCLUDED_)
#define AFX_TRAJECTORYPLOTPARDLG_H__0ADE2553_44A3_11D1_B623_9BDF4FBDB829__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TrajectoryPlotParDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrajectoryPlotParDlg dialog

class CTrajectoryPlotParDlg : public CDialog
{
// Construction
public:
	CTrajectoryPlotParDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrajectoryPlotParDlg)
	enum { IDD = IDD_TRAJECTORY_PLOT_PARAM };
	int		m_nScaleYDifferent;
	int		m_nScaleYSame;
	double	m_dStepSize;
	double	m_dStartTime;
	double	m_dEndTime;
	CString	m_szChgParam;
	int		m_nPlotMultiPar;
	int		m_nPlotSinglePar;
	CString	m_szParValues;
	CString	m_szVar1;
	CString	m_szVar2;
	CString	m_szVar3;
	CString	m_szVar4;
	CString	m_szLegend1;
	CString	m_szLegend2;
	CString	m_szLegend3;
	CString	m_szLegend4;
	int		m_nSaveValNum;
	double	m_dEps;
	double	m_dStartPlotTime;
	double	m_dEndPlotTime;
	CString	m_szParamVal;
	CString	m_szStartVal;
	CString	m_szTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrajectoryPlotParDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrajectoryPlotParDlg)
	afx_msg void OnTPlotMultiParam();
	afx_msg void OnTPlotSingleParam();
	afx_msg void OnTPlotScaleYDifferent();
	afx_msg void OnTPlotScaleYSame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRAJECTORYPLOTPARDLG_H__0ADE2553_44A3_11D1_B623_9BDF4FBDB829__INCLUDED_)
