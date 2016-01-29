#if !defined(AFX_NLSYSOPTIONSDLG_H__34259C03_5F21_11D1_B674_DA62097A8929__INCLUDED_)
#define AFX_NLSYSOPTIONSDLG_H__34259C03_5F21_11D1_B674_DA62097A8929__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NLSysOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNLSysOptionsDlg dialog

class CNLSysOptionsDlg : public CDialog
{
// Construction
public:
	CNLSysOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNLSysOptionsDlg)
	enum { IDD = IDD_OPTIONS };
	double	m_dEps;
	int		m_nNewWindow;
	int		m_nSameWindow;
	double	m_dStepMin;
	BOOL	m_bUseColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLSysOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNLSysOptionsDlg)
	afx_msg void OnNewWindow();
	afx_msg void OnSameWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLSYSOPTIONSDLG_H__34259C03_5F21_11D1_B674_DA62097A8929__INCLUDED_)
