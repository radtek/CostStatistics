#pragma once
//#include "afxext.h"
class CSplitterWndEh : public CSplitterWnd
{
public:
    CSplitterWndEh();
    ~CSplitterWndEh();
    DECLARE_MESSAGE_MAP()
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

public:
    void HideSplitter();
};

