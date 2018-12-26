// LeftWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UI.h"
#include "LeftWnd.h"

#include "ZTUI.h"

// CLeftWnd

IMPLEMENT_DYNCREATE(CLeftWnd, ZTCScrollView)

CLeftWnd::CLeftWnd()
{
    m_bInited = false;
    m_pChildWnd = NULL;
}

CLeftWnd::~CLeftWnd()
{
    if (m_pChildWnd)
    {
		if(m_pChildWnd->m_hWnd)
		{
			m_pChildWnd->DestroyWindow();
		}
        delete m_pChildWnd;
       
    }
}


BEGIN_MESSAGE_MAP(CLeftWnd, ZTCScrollView)
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CLeftWnd drawing

void CLeftWnd::OnInitialUpdate()
{
    if (m_bInited)
    {
        return;
    }
    m_bInited = true;
    
    ZTCScrollView::OnInitialUpdate();


	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal, CSize(0, 0), CSize(80, 80));
}

void CLeftWnd::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CLeftWnd diagnostics

#ifdef _DEBUG
void CLeftWnd::AssertValid() const
{
	ZTCScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftWnd::Dump(CDumpContext& dc) const
{
	ZTCScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLeftWnd message handlers


BOOL CLeftWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default

    if (zDelta < 0)
    {
        OnVScroll(SB_LINEDOWN, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
    }
    else if (zDelta > 0)
    {
        OnVScroll(SB_LINEUP, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
    }

    return ZTCScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CLeftWnd::AppendWnd(CWnd* pWnd)
{
	pWnd->SetParent(this);
	m_pChildWnd = pWnd;
}

CWnd* CLeftWnd::GetChildWnd()
{
	return m_pChildWnd;
}

void CLeftWnd::OnSize(UINT nType, int cx, int cy)
{
    ZTCScrollView::OnSize(nType, cx, cy);

	if (m_pChildWnd->GetSafeHwnd())
    {
         MyMoveWindow(m_pChildWnd->m_hWnd, 0, 0, cx, cy, TRUE);

    }
}
