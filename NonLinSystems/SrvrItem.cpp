// SrvrItem.cpp : implementation of the CNLSysSrvrItem class
//

#include "stdafx.h"
#include "NonLinSystems.h"

#include "NLSysDoc.h"
#include "NLSysView.h"
#include "SrvrItem.h"

#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLSysSrvrItem implementation

IMPLEMENT_DYNAMIC(CNLSysSrvrItem, COleServerItem)

CNLSysSrvrItem::CNLSysSrvrItem(CNLSysDoc* pContainerDoc)
	: COleServerItem(pContainerDoc, TRUE)
{
	// TODO: add one-time construction code here
	//  (eg, adding additional clipboard formats to the item's data source)
}

CNLSysSrvrItem::~CNLSysSrvrItem()
{
	// TODO: add cleanup code here
}

void CNLSysSrvrItem::Serialize(CArchive& ar)
{
	// CNLSysSrvrItem::Serialize will be called by the framework if
	//  the item is copied to the clipboard.  This can happen automatically
	//  through the OLE callback OnGetClipboardData.  A good default for
	//  the embedded item is simply to delegate to the document's Serialize
	//  function.  If you support links, then you will want to serialize
	//  just a portion of the document.

	if (!IsLinkedItem())
	{
		CNLSysDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->Serialize(ar);
	}
}

BOOL CNLSysSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// Most applications, like this one, only handle drawing the content
	//  aspect of the item.  If you wish to support other aspects, such
	//  as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
	//  implementation of OnGetExtent should be modified to handle the
	//  additional aspect(s).

	if (dwDrawAspect != DVASPECT_CONTENT)
		return COleServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CNLSysSrvrItem::OnGetExtent is called to get the extent in
	//  HIMETRIC units of the entire item.  The default implementation
	//  here simply returns a hard-coded number of units.

	CNLSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: replace this arbitrary size

	rSize = CSize(4000, 4000);   // 4000 x 4000 HIMETRIC units

	return TRUE;
}

BOOL CNLSysSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	// Remove this if you use rSize
	UNREFERENCED_PARAMETER(rSize);

	CNLSysDoc* pDoc = GetDocument();
	CNLSysView	*pView;
	ASSERT_VALID(pDoc);

	POSITION pos = pDoc->GetFirstViewPosition();
	if( pos != NULL )
		pView = (CNLSysView *) pDoc->GetNextView(pos);
	else
		ASSERT(0);			// znaèi da nije definiran ni jedan pogled


	// koristimo istu funkciju kao i za obièni OnDraw, s time da ipak predajemo ovu 1
	// kako bi funkcija znala odakle je pozvana(za ovaj poziv ne postavlja viewport extent)
	pView->InitpDC(pDC, 1);

	// nacratati æemo okvir oko objekta
	if( pView->m_nPlotType != CNLSysView::PHASE_PLOT_3D )
	{
		int			x0, y0, x1, y1;
		x0 = (int) (pView->m_PlotPar.m_dXLow * pView->m_PlotPar.m_dMulX);
		x1 = (int) (pView->m_PlotPar.m_dXUpp * pView->m_PlotPar.m_dMulX);
		y0 = (int) (pView->m_PlotPar.m_dYLow * pView->m_PlotPar.m_dMulY);
		y1 = (int) (pView->m_PlotPar.m_dYUpp * pView->m_PlotPar.m_dMulY);
		
		CPen		tmpPen, *pPrevPen;
		tmpPen.CreatePen(PS_SOLID, 10, RGB(0,0,0) );
		pPrevPen = pDC->SelectObject(&tmpPen);

		CRect		rectBorders(x0,y0,x1,y1);
		pDC->Rectangle( &rectBorders );
	
		pDC->SelectObject(pPrevPen);
	}
	else
	{
		CPen		tmpPen, *pPrevPen;
		tmpPen.CreatePen(PS_SOLID, 10, RGB(0,0,0) );
		pPrevPen = pDC->SelectObject(&tmpPen);

		CRect		rectBorders(-2000,-2000,2000,2000);
		pDC->Rectangle( &rectBorders );
	}

	pView->PlotAxis(pDC,1);							// nacrtaj koordinatni sistem

	CPoint		pntOrg(10,1);
	pDC->DPtoLP(&pntOrg,1);

	if( pView->m_bCalcDone == FALSE )
		pDC->TextOut( pntOrg.x, pntOrg.y, "Waiting for command !!!         " );
	if( pView->m_bCalcInProgress == TRUE )
		pDC->TextOut( pntOrg.x, pntOrg.y, "Calculating !!!                           " );
	if( !pDoc->IsDefined() )
		pDC->TextOut( pntOrg.x, pntOrg.y, "System is not defined !!!             ");

	if( pView->m_bCalcDone == TRUE && pView->m_bCalcInProgress == FALSE && pView->m_bSystemChanged == FALSE )
	{
		if( pView->m_nPlotType == CNLSysView::TRAJECTORY_PLOT )
			pView->DrawTrajectory(pDC);
		if( pView->m_nPlotType == CNLSysView::PHASE_PLOT_2D )
			pView->DrawPhase2D(pDC);
		if( pView->m_nPlotType == CNLSysView::PHASE_PLOT_3D )
			pView->DrawPhase3D(pDC);
		if( pView->m_nPlotType == CNLSysView::EQU_SURFACE_PLOT_2D )
			pView->DrawEquSurface2D(pDC);
		if( pView->m_nPlotType == CNLSysView::FOURIER_SPECTRUM_PLOT )
			pView->DrawSpectrum(pDC);
		if( pView->m_nPlotType == CNLSysView::LYAP_EXP_PLOT )
			pView->DrawLyapExp(pDC);
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CNLSysSrvrItem diagnostics

#ifdef _DEBUG
void CNLSysSrvrItem::AssertValid() const
{
	COleServerItem::AssertValid();
}

void CNLSysSrvrItem::Dump(CDumpContext& dc) const
{
	COleServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////

