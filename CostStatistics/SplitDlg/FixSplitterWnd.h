#pragma once

class CFixSplitterWnd : public CSplitterWnd
{
    DECLARE_DYNCREATE(CFixSplitterWnd)

public: 
    CFixSplitterWnd();     

    DECLARE_MESSAGE_MAP()

protected:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

    virtual void OnDrawSplitter( CDC* pDC, ESplitType nType, const CRect& rect );
}; 