#if !defined(AFX_PHASEPLOTPARDLG_H__DE95F8E3_44D2_11D1_B624_D1E997BBA929__INCLUDED_)
#define AFX_PHASEPLOTPARDLG_H__DE95F8E3_44D2_11D1_B624_D1E997BBA929__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PhasePlotParDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPhasePlotParDlg dialog

class CPhasePlotParDlg : public CDialog
{
// Construction
public:
	CPhasePlotParDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPhasePlotParDlg)
	enum { IDD = IDD_PHASE_PLOT_PARAM };
	double	m_dStepSize;
	double	m_dStartTime;
	double	m_dEndTime;
	CString	m_szXAxis;
	CString	m_szYAxis;
	CString	m_szZAxis;
	CString	m_szParValues;
	int		m_nPlot2D;
	int		m_nPlot3D;
	int		m_nPlotMultiPar;
	int		m_nPlotSinglePar;
	CString	m_szChgParam;
	int		m_nScaleYDifferent;
	int		m_nScaleYSame;
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
	//{{AFX_VIRTUAL(CPhasePlotParDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPhasePlotParDlg)
	afx_msg void OnPhasePlotparam2d();
	afx_msg void OnPhasePlotparam3d();
	afx_msg void OnPhasePlotparamMultiParam2();
	afx_msg void OnPhasePlotparamSinglePar();
	afx_msg void OnPhaseScaleYDifferent();
	afx_msg void OnPhaseScaleYSame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHASEPLOTPARDLG_H__DE95F8E3_44D2_11D1_B624_D1E997BBA929__INCLUDED_)
