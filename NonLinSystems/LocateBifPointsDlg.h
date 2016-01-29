#if !defined(AFX_LOCATEBIFPOINTSDLG_H__C2271373_4EE9_11D1_B64C_B2D8EF488B29__INCLUDED_)
#define AFX_LOCATEBIFPOINTSDLG_H__C2271373_4EE9_11D1_B64C_B2D8EF488B29__INCLUDED_

class		CNLSysDoc;

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LocateBifPointsDlg.h : header file
//

#define ID_AUTOMATIC		5

/////////////////////////////////////////////////////////////////////////////
// CLocateBifPointsDlg dialog

class CLocateBifPointsDlg : public CDialog
{
// Construction
public:
	CLocateBifPointsDlg(CNLSysDoc *pDocument = NULL, CWnd* pParent = NULL);

	int					m_nDim;
	CString			m_szEigenStart;
	
	CNLSysDoc		*pDoc;
// Dialog Data
	//{{AFX_DATA(CLocateBifPointsDlg)
	enum { IDD = IDD_LOCATE_BIF_POINTS };
	CButton	m_buttEquSolution;
	CEdit	m_ctrlEigenStart;
	CButton	m_buttRandomize;
	int		m_nHopf;
	int		m_nSaddleNode;
	CString	m_szStartVal;
	CString	m_szStartParVal;
	int		m_nSearchNum;
	double	m_dP1;
	double	m_dP2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLocateBifPointsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLocateBifPointsDlg)
	afx_msg void OnLocateBifHopf();
	afx_msg void OnLocateBifSNB();
	afx_msg void OnLocateBifRandomizeEigen();
	afx_msg void OnLocateBifEquSolStateVar();
	afx_msg void OnLocBifAutomatic();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCATEBIFPOINTSDLG_H__C2271373_4EE9_11D1_B64C_B2D8EF488B29__INCLUDED_)
