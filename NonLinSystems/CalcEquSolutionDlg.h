#if !defined(AFX_CALCEQUSOLUTIONDLG_H__F914F4D3_6631_11D1_B694_9DF0AEE7142F__INCLUDED_)
#define AFX_CALCEQUSOLUTIONDLG_H__F914F4D3_6631_11D1_B694_9DF0AEE7142F__INCLUDED_

class		CNLSysDoc;

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CalcEquSolutionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcEquSolutionDlg dialog

class CCalcEquSolutionDlg : public CDialog
{
public:
	CNLSysDoc		*pDoc;

// Construction
public:
	CCalcEquSolutionDlg(CNLSysDoc *pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcEquSolutionDlg)
	enum { IDD = IDD_CALC_EQU_SOLUTION };
	CButton	m_ctrlCalculate;
	CListBox	m_ctrlSysVar;
	CListBox	m_ctrlParVal;
	CString	m_szEigenVal;
	CString	m_szParVal;
	CString	m_szSysVar;
	double	m_dEps;
	int		m_nMaxIter;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcEquSolutionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcEquSolutionDlg)
	afx_msg void OnEquSolCalculate();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEquSolParVal();
	afx_msg void OnSelchangeEquSolSysVar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCEQUSOLUTIONDLG_H__F914F4D3_6631_11D1_B694_9DF0AEE7142F__INCLUDED_)
