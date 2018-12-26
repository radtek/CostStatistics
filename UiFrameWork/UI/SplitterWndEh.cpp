#include "stdafx.h"
#include "SplitterWndEh.h"


CSplitterWndEh::CSplitterWndEh()
{
}


CSplitterWndEh::~CSplitterWndEh()
{
}
BEGIN_MESSAGE_MAP(CSplitterWndEh, CSplitterWnd)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()


void CSplitterWndEh::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonDown(nFlags, point);
}


void CSplitterWndEh::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    CWnd::OnMouseMove(nFlags, point);
}


BOOL CSplitterWndEh::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: Add your message handler code here and/or call default

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CSplitterWndEh::HideSplitter()
{
    m_cxSplitterGap = 1;
    m_cySplitterGap = 1;
}