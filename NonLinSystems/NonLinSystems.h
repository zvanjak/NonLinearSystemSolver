// NonLinSystems.h : main header file for the NONLINSYSTEMS application
//

#if !defined(AFX_NONLINSYSTEMS_H__72A8E5B8_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_)
#define AFX_NONLINSYSTEMS_H__72A8E5B8_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNonLinSystemsApp:
// See NonLinSystems.cpp for the implementation of this class
//

class CNonLinSystemsApp : public CWinApp
{
public:
	CNonLinSystemsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNonLinSystemsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	COleTemplateServer m_server;
		// Server object for document creation

	//{{AFX_MSG(CNonLinSystemsApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NONLINSYSTEMS_H__72A8E5B8_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_)
