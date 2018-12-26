
// MainFrm.h : interface of the CZTUIMainFrame class
//

#pragma once
#include "SplitterWndEh.h"
#include "SplitterWndH.h"

class CZTUIMainFrame : public CFrameWnd
{
	
public:
	CZTUIMainFrame();
protected: 
	DECLARE_DYNAMIC(CZTUIMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CZTUIMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void SetLeftWndWidth(int w);
	int GetLeftWndWidth();
	void ShowHideLeftWnd();

public:

protected:
    CSplitterWndH    m_wndSplitter;
    CSplitterWndEh  m_wndLeftSplitter;
    CSplitterWndEh  m_wndRightSplitter;
	CToolBar	m_wndToolBar;
	CImageList	m_ImageList;

    BOOL m_bDragFullWindowsBak;


// Generated message map functions
protected:
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
};


