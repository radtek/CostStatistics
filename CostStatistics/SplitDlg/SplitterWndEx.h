#pragma once

class CSplitterWndEx1 : public CSplitterWnd
{
    DECLARE_DYNCREATE(CSplitterWndEx1)

protected: 
    int m_nHidedCol;     //   hided   column   number,   -1   if   all   columns   are   shown 
    int m_nHidedRow;

public: 
    CSplitterWndEx1();

    void ShowColumn();
    void HideColumn(int colHide);

    void ShowRow();
    void HideRow(int rowHide);

    virtual void OnDrawSplitter( CDC* pDC, ESplitType nType, const CRect& rect );

    DECLARE_MESSAGE_MAP() 
}; 