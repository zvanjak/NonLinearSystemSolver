#if !defined(AFX_LYAPEXPLISTDLG_H__146638B3_550A_11D1_B65F_F6BA2BCA8829__INCLUDED_)
#define AFX_LYAPEXPLISTDLG_H__146638B3_550A_11D1_B65F_F6BA2BCA8829__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LyapExpListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLyapExpListDlg dialog

class CLyapExpListDlg : public CDialog
{
// Construction
public:
	CString		**m_pszLyapExpVal;				// pokazivaèi na svojstvene vrijednosti u odgovarajuæim bif. toèkama
	int				m_nLyapExpNum;
	CLyapExpListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLyapExpListDlg)
	enum { IDD = IDD_SHOW_LYAP_EXP };
	CListBox	m_ctrlValList;
	CString	m_szExpVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLyapExpListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLyapExpListDlg)
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSelChangeLyapListVal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LYAPEXPLISTDLG_H__146638B3_550A_11D1_B65F_F6BA2BCA8829__INCLUDED_)
