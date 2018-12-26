#pragma once

#include "StaticEh.h"
#include "Common.h"
#include "zt_mfc/zt_view.h"



struct ToolbarOption : public ToolbarBtnData
{
    UINT nIDSelect;
    UINT nIDRename;
    CWnd* pWnd;

	struct BmpMenu
	{
		UINT nIDBmp;
		CString strImgFile;
		CBitmap* pBmp;
	};

	list<BmpMenu*> bmpList1;
	list<BmpMenu*> bmpList2;


    ToolbarOption()
    {
		nIDSelect = 0;
        nIDRename = 0;
        pWnd = NULL;
    }
};


// CToolbarEh view

class CToolbarEh : public ZTCView
{
	DECLARE_DYNCREATE(CToolbarEh)
	
    virtual ~CToolbarEh();

    void DeleteAll(bool bDestroy);

protected:
	CToolbarEh();           // protected constructor used by dynamic creation

public:
    virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void AddFocusBtn();
	bool IsActive(){return m_active;}
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT OnReposition(WPARAM wParam, LPARAM lParam); 
    afx_msg LRESULT OnContextMenu(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

    void AddText(LPCTSTR lpszText, UINT nID, bool bShow = true, bool bButton=false);
	void AddButton(LPCTSTR lpszText, UINT nID, LPCTSTR lpszBitmapFile, bool bShow = true);
	void AddTwoStateButton(LPCTSTR lpszText, UINT nID, LPCTSTR lpszBitmapFile1, LPCTSTR lpszBitmapFile2, E_ButtonState nImg, bool bShow = true);
    void EnableToolTips();
	void SetOwner(CWnd* pOwner) { m_pOwner = pOwner; }
	CWnd* GetOwner() { return m_pOwner; }
	void SetBitMapList(CString strImageFolder);
    void RemoveAll();
	E_ButtonState GetButtonState(int pos); //pos base 0
	void SetButtonState(int pos, E_ButtonState state);
	CString GetButtonText(int pos);
	void SetButtonText(int pos, LPCTSTR text);

	void Active(bool bActive);

	void Save(OUT ToolbarData & cfg);
	void Load(IN ToolbarData & cfg);

protected:
    void CreatePopupMenu(int x, int y);
	void Reposition();
	void DrawBorder(CDC *pDC);

public:
	CString m_strImageFolder;
	COLORREF m_transparentMaskColor;
	COLORREF m_textColor;
	COLORREF m_bkgndColor;
	COLORREF m_highlightBkgndColor;
	COLORREF m_borderColor;
	bool m_bTabMode;
protected:
    bool    m_bInited;
    bool    m_bEnableToolTips;
    int     m_xMargin;
	int		m_nMaxIndex;
    UINT     m_nMenuID;
    CMenu   m_menu;
    CWnd*   m_pOwner;
	list<CString> m_bmpFileList;
    CToolTipCtrl m_wndToolTip;
    vector<ToolbarOption*>  m_toolbarList;
	bool m_active;
	CButton m_focusBtn;

protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void ShowHideItem(ToolbarOption* tb, bool bShow);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	CFont m_ftNormal;
	CFont m_ftBold;
};


