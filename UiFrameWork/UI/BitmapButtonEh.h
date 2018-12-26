#pragma once


// CBitmapButtonEh

class CBitmapButtonEh : public CBitmapButton
{
	DECLARE_DYNAMIC(CBitmapButtonEh)

public:
	CBitmapButtonEh();
	virtual ~CBitmapButtonEh();

	BOOL LoadBitmapsFromFiles(
		LPCTSTR lpszBitmapResource,
		LPCTSTR lpszBitmapResourceSel = NULL,
		LPCTSTR lpszBitmapResourceFocus = NULL,
		LPCTSTR lpszBitmapResourceDisabled = NULL);

	BOOL LoadBitmapsFromResource(
		UINT lpszBitmapResource,
		UINT lpszBitmapResourceSel = NULL,
		UINT lpszBitmapResourceFocus = NULL,
		UINT lpszBitmapResourceDisabled = NULL
		);
	void Highlight(bool bHighlight);

public:
	COLORREF m_bkgndColor;
	COLORREF m_highlightBkgndColor;
	COLORREF m_transparentMaskColor;
	COLORREF m_borderColor;

protected:
	CImage m_image;
	CImage m_imageHighlight;
	bool m_bTracking;
	CBitmap m_bitmap2;

	HBITMAP m_hBmp;
	HBITMAP m_hBmpHighlight;

	bool m_bBHighlight;

protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};


