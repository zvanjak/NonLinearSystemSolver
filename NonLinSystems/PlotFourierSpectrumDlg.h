#if !defined(AFX_PLOTFOURIERSPECTRUMDLG_H__BDF8FA23_5ECB_11D1_B673_9000C0188A29__INCLUDED_)
#define AFX_PLOTFOURIERSPECTRUMDLG_H__BDF8FA23_5ECB_11D1_B673_9000C0188A29__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PlotFourierSpectrumDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlotFourierSpectrumDlg dialog

class CPlotFourierSpectrumDlg : public CDialog
{
// Construction
public:
	CPlotFourierSpectrumDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlotFourierSpectrumDlg)
	enum { IDD = IDD_PLOT_SPECTRUM };
	CString	m_szLegend1;
	CString	m_szLegend2;
	CString	m_szLegend3;
	CString	m_szLegend4;
	CString	m_szVar1;
	CString	m_szVar2;
	CString	m_szVar3;
	CString	m_szVar4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlotFourierSpectrumDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlotFourierSpectrumDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLOTFOURIERSPECTRUMDLG_H__BDF8FA23_5ECB_11D1_B673_9000C0188A29__INCLUDED_)
