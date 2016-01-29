// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "NonLinSystems.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CToolBarCtrl   *ctrl;

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	ctrl = &m_wndToolBar.GetToolBarCtrl();
//	ctrl->RestoreState(HKEY_CURRENT_USER, "Software\\MyProducts\\NonLinSystems", "NLSysStandardToolBar");

	// TODO: Add your specialized creation code here
	if (!m_wndWindSizeToolBar.Create(this) ||
		!m_wndWindSizeToolBar.LoadToolBar(IDR_WINDOW_SIZE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndWindSizeToolBar.SetBarStyle(m_wndWindSizeToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndWindSizeToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndWindSizeToolBar);
	
	ctrl = &m_wndWindSizeToolBar.GetToolBarCtrl();
//	ctrl->RestoreState(HKEY_CURRENT_USER, "Software\\MyProducts\\NonLinSystems", "NLSysWindowToolBar");

	// TODO: Add your specialized creation code here
	if (!m_wndCommandsToolBar.Create(this) ||
		!m_wndCommandsToolBar.LoadToolBar(IDR_COMMANDS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndCommandsToolBar.SetBarStyle(m_wndCommandsToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndCommandsToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_TOP);
	DockControlBar(&m_wndCommandsToolBar);

	ctrl = &m_wndCommandsToolBar.GetToolBarCtrl();
//	ctrl->RestoreState(HKEY_CURRENT_USER, "Software\\MyProducts\\NonLinSystems", "NLSysCommandsToolBar");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;

	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CToolBarCtrl   *ctrl;
	
	ctrl = &m_wndToolBar.GetToolBarCtrl();
	ctrl->SaveState(HKEY_CURRENT_USER, "Software\\MyProducts\\NonLinSystems", "NLSysStandardToolBar");
	
	ctrl = &m_wndWindSizeToolBar.GetToolBarCtrl();
	ctrl->SaveState(HKEY_CURRENT_USER, "Software\\MyProducts\\NonLinSystems", "NLSysWindowsToolBar");

	ctrl = &m_wndCommandsToolBar.GetToolBarCtrl();
	ctrl->SaveState(HKEY_CURRENT_USER, "Software\\MyProducts\\NonLinSystems", "NLSysCommandsToolBar");
	
	CMDIFrameWnd::OnClose();
}
