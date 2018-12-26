#pragma once

#include "ToolbarEh.h"

// CToolbarView view

class CToolbarView : public CView
{
	DECLARE_DYNCREATE(CToolbarView)

protected:
	CToolbarView();           // protected constructor used by dynamic creation
	virtual ~CToolbarView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    void SetOwner(CWnd* pOwner);

public:
	CToolbarEh* m_pToolbar;

private:
    CWnd* m_pOwner;

public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};


