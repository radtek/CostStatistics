#pragma once


// CStaticEh

class CStaticEh : public CStatic
{
	DECLARE_DYNAMIC(CStaticEh)

public:
	CStaticEh();
	virtual ~CStaticEh();

public:
	HBITMAP SetBitmap(HBITMAP hBitmap);
	void EnableHotTrack(bool bEnable);

protected:
	DECLARE_MESSAGE_MAP()


public:
	COLORREF m_bkgndColor;
	COLORREF m_highlightBkgndColor;
	COLORREF m_textColor;
	COLORREF m_borderColor;
	int m_borderType;
	bool m_bActive;

private:
	CBrush m_bkgndBrush;
	HBRUSH	m_hBrush;
	HBRUSH	m_hBrushHighlight;
    CEdit   m_edit;
    DWORD   m_nID;
	bool	m_bTracking;
	bool	m_bChangeByDblClk;


public:
	void SetChangeByDblClk(bool val = true) { m_bChangeByDblClk = val; }
	
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnEdit(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};


