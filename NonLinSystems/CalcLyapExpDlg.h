#if !defined(AFX_CALCLYAPEXPDLG_H__4AC6F8A3_54F8_11D1_B65E_85DAC8BA8929__INCLUDED_)
#define AFX_CALCLYAPEXPDLG_H__4AC6F8A3_54F8_11D1_B65E_85DAC8BA8929__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CalcLyapExpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalcLyapExpDlg dialog

class CCalcLyapExpDlg : public CDialog
{
// Construction
public:
	CCalcLyapExpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalcLyapExpDlg)
	enum { IDD = IDD_CALC_LYAP_EXP };
	CString	m_szStartParVal;
	CString	m_szStartVal;
	double	m_dEps;
	double	m_dMinStepSize;
	double	m_dT;
	double	m_dEpsAbs;
	double	m_dEpsRel;
	int		m_nMaxIterNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcLyapExpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcLyapExpDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCLYAPEXPDLG_H__4AC6F8A3_54F8_11D1_B65E_85DAC8BA8929__INCLUDED_)
