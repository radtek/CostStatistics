#pragma once
#include "SubWndHelper.h"
#include "zt_mfc/zt_view.h"
#include "Common.h"

#include "PersonWnd.h"

class CToolbarEh;
class CThingWnd;
class CRightWnd : public ZTCScrollView
{
	DECLARE_DYNCREATE(CRightWnd)

protected:
	CRightWnd();           // protected constructor used by dynamic creation
	virtual ~CRightWnd();

public:
    CString m_name;
    CToolbarEh* m_pToolbar;
    CWnd* m_pActiveThing;

protected:
	CSubWndHelper m_subWndHelper;
	bool m_bInited;
    int m_cx;
    COLORREF m_backColor;


public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
    void Load(RightWndData & cfg, bool bLoadToolbar = true);
	void Save(RightWndData & cfg);
    void RemoveSubWnd(CWnd* pWnd);
    void MoveSubWnd(const CWnd* pWnd, int step);
	void MoveSubWndToTop(const CWnd* pWnd);
	CThingWnd* AddSubWnd(bool bIsLoad = false);
    void RemoveAll(bool bRemoveToolbar = true);
    void Refresh();

	void AddCustomWnd(CWnd* pWnd);
	CWnd* GetCustomWnd();
	
	CThingWnd* FindThing(CString name);

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	void Reposition();

	CWnd* m_pCustomWnd;
	

	void UpdateScrollSize();


    DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	LRESULT OnSubWndSizeChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnToolbarClicked(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    LRESULT OnThingActived(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
};


