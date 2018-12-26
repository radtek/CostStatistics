#include "stdafx.h"
#include "MMSystem.h"
#include "FixSplitterWnd.h"

IMPLEMENT_DYNAMIC(CFixSplitterWnd, CSplitterWnd)

CFixSplitterWnd::CFixSplitterWnd()
{
}

BEGIN_MESSAGE_MAP(CFixSplitterWnd, CSplitterWnd)
    ON_WM_LBUTTONDOWN()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CFixSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    //CSplitterWnd::OnLButtonDown(nFlags, point);
    return;
}


BOOL CFixSplitterWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    //return CSplitterWnd::OnSetCursor(pWnd, nHitTest, message);
    return FALSE;
}


void CFixSplitterWnd::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    //CSplitterWnd::OnMouseMove(nFlags, point);
    return;
}

void CFixSplitterWnd::OnDrawSplitter(CDC * pDC, ESplitType nType, const CRect & rect)
{
    CSplitterWnd::OnDrawSplitter(pDC, nType, rect);

    if (pDC != NULL && nType == splitBar)
    {
        CBrush brush;
        brush.CreateSolidBrush(RGB(202, 225, 243));
        pDC->FillRect(rect,&brush);
    }
} 