#pragma once

#include "Common.h"

class CRightCtrlWnd : public CDialog, public CAbsCtrl
{
	DECLARE_DYNAMIC(CRightCtrlWnd)

public:
	CRightCtrlWnd(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CRightCtrlWnd();

public:
	bool m_active;
	CString m_name;
	virtual BOOL IsScrollBarShow(){return FALSE;}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK() override;
	virtual void OnCancel() override;
	void DrawBorder(CDC *pDC);
	
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()
	HBRUSH m_hBrush;
    COLORREF m_backColor;
	COLORREF m_borderColor;
	CButton m_focusBtn;

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	HRESULT OnChildItemActivate(WPARAM w, LPARAM l);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void SendActiveMessage();

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
