#pragma once

#include "Common.h"
#include "zt_mfc/zt_view.h"


class CToolbarEh;

class CLeftWnd : public ZTCScrollView
{
	DECLARE_DYNCREATE(CLeftWnd)

protected:
	CLeftWnd();           // protected constructor used by dynamic creation
	virtual ~CLeftWnd();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

 

protected:
    bool m_bInited;
   
	CWnd *m_pChildWnd;
public:
    
    CToolbarEh* m_pToolbar;

public:
	void AppendWnd(CWnd* pWnd);
	CWnd* GetChildWnd();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()
    afx_msg void OnSize(UINT nType, int cx, int cy);
};


