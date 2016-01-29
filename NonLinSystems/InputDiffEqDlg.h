#if !defined(AFX_INPUTDIFFEQDLG_H__18C75833_4463_11D1_B622_F4075B56BF29__INCLUDED_)
#define AFX_INPUTDIFFEQDLG_H__18C75833_4463_11D1_B622_F4075B56BF29__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InputDiffEqDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputDiffEqDlg dialog

class CInputDiffEqDlg : public CDialog
{
// Construction
public:
	CInputDiffEqDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputDiffEqDlg)
	enum { IDD = IDD_INPUT_DIFFEQ_SYSTEM };
	CString	m_szEquations;
	CString	m_szIndVar;
	CString	m_szParamVal;
	CString	m_szStartVal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputDiffEqDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputDiffEqDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTDIFFEQDLG_H__18C75833_4463_11D1_B622_F4075B56BF29__INCLUDED_)
