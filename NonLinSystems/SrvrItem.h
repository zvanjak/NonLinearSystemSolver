// SrvrItem.h : interface of the CNLSysSrvrItem class
//

#if !defined(AFX_SRVRITEM_H__72A8E5C5_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_)
#define AFX_SRVRITEM_H__72A8E5C5_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CNLSysSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CNLSysSrvrItem)

// Constructors
public:
	CNLSysSrvrItem(CNLSysDoc* pContainerDoc);

// Attributes
	CNLSysDoc* GetDocument() const
		{ return (CNLSysDoc*)COleServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLSysSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CNLSysSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
private:
	void InitpDC( CDC *pDC, CNLSysView *pView);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVRITEM_H__72A8E5C5_43EB_11D1_B621_FFBA54FBBB29__INCLUDED_)
