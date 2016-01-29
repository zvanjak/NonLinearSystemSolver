#if !defined(AFX_PLOTEQUSURFACEDLG_H__2A904311_4E9B_11D1_B64A_C1D41F448829__INCLUDED_)
#define AFX_PLOTEQUSURFACEDLG_H__2A904311_4E9B_11D1_B64A_C1D41F448829__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PlotEquSurfaceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlotEquSurfaceDlg dialog

class CPlotEquSurfaceDlg : public CDialog
{
// Construction
public:
	CPlotEquSurfaceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlotEquSurfaceDlg)
	enum { IDD = IDD_PLOT_EQU_SURFACE };
	int		m_nPlot2D;
	int		m_nPlot3D;
	double	m_dRange1From;
	double	m_dRange2From;
	double	m_dRange1To;
	double	m_dRange2To;
	CString	m_szVar1;
	CString	m_szVar2;
	CString	m_szVar3;
	BOOL	m_bShowBif;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotEquSurfaceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlotEquSurfaceDlg)
	afx_msg void OnPlotEqu2D();
	afx_msg void OnPlotEqu3D();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTEQUSURFACEDLG_H__2A904311_4E9B_11D1_B64A_C1D41F448829__INCLUDED_)
