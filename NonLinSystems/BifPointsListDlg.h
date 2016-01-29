#if !defined(AFX_BIFPOINTSLISTDLG_H__B1802CB3_53B7_11D1_B65A_DBE794679829__INCLUDED_)
#define AFX_BIFPOINTSLISTDLG_H__B1802CB3_53B7_11D1_B65A_DBE794679829__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BifPointsListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBifPointsListDlg dialog

class CBifPointsListDlg : public CDialog
{
// Construction
public:
	CString		**m_pszSNBEigen;				// pokazivaèi na svojstvene vrijednosti u odgovarajuæim bif. toèkama
	CString		**m_pszHopfEigen;
	int				m_nSNBPointsNum, m_nHopfPointsNum;
public:
	CBifPointsListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBifPointsListDlg)
	enum { IDD = IDD_SHOW_BIF_POINTS };
	CListBox	m_ctrlSNBList;
	CListBox	m_ctrlHopfList;
	CString	m_szHopfEigenVal;
	CString	m_szSNBEigenVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBifPointsListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBifPointsListDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelChangeHopfPoints();
	afx_msg void OnSelChangeSNBPoints();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BIFPOINTSLISTDLG_H__B1802CB3_53B7_11D1_B65A_DBE794679829__INCLUDED_)
