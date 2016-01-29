#if !defined(AFX_NLSYSOPTIONS_H__995A3543_5E5E_11D1_B671_BA24E93C8929__INCLUDED_)
#define AFX_NLSYSOPTIONS_H__995A3543_5E5E_11D1_B671_BA24E93C8929__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NLSysOptions.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNLSysOptions dialog

class CNLSysOptions : public CDialog
{
// Construction
public:
	CNLSysOptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNLSysOptions)
	enum { IDD = IDD_OPTIONS };
	int		m_nNewWindow;
	int		m_nSameWindow;
	double	m_dEps;
	double	m_dStepMin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLSysOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNLSysOptions)
	afx_msg void OnOptionsNewWindow();
	afx_msg void OnOptionsSameWindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLSYSOPTIONS_H__995A3543_5E5E_11D1_B671_BA24E93C8929__INCLUDED_)
