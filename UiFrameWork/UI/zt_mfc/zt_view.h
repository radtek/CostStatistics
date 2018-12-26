#pragma once

class AFX_NOVTABLE zt_CView : public CWnd
{
	DECLARE_DYNAMIC(zt_CView)

// Constructors
protected:
	zt_CView();

// Attributes
//public:
//	CDocument* GetDocument() const;

// Operations
public:
	// for standard printing setup (override OnPreparePrinting)
	BOOL DoPreparePrinting(CPrintInfo* pInfo);

// Overridables
public:
	virtual BOOL IsSelected(const CObject* pDocItem) const; // support for OLE

	// OLE scrolling support (used for drag/drop as well)
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

	// OLE drag/drop support
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,
		DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual BOOL OnDrop(COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDropEx(COleDataObject* pDataObject,
		DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);
	virtual DROPEFFECT OnDragScroll(DWORD dwKeyState, CPoint point);

	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	virtual void OnInitialUpdate(); // called first time after construct

protected:
	// Activation
	virtual void OnActivateView(BOOL bActivate, zt_CView* pActivateView,
					zt_CView* pDeactiveView);
	virtual void OnActivateFrame(UINT nState, CFrameWnd* pFrameWnd);

	// General drawing/updating
	virtual void OnUpdate(zt_CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnDraw(CDC* pDC) = 0;

	// Printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
		// must override to enable printing and print preview

	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// Advanced: end print preview mode, move to point
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point,
		CPreviewView* pView);

// Implementation
public:
	virtual ~zt_CView() = 0;
#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG

	// Advanced: for implementing custom print preview
	BOOL DoPrintPreview(UINT nIDResource, zt_CView* pPrintView,
			CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState);

	virtual void CalcWindowRect(LPRECT lpClientRect,
		UINT nAdjustType = adjustBorder);
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	static CSplitterWnd* PASCAL GetParentSplitter(
		const CWnd* pWnd, BOOL bAnyState);

protected:
	CDocument* m_pDocument;

public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();

	// friend classes that call protected zt_CView overridables
	friend class CDocument;
	friend class CDocTemplate;
	friend class CPreviewView;
	friend class CFrameWnd;
	friend class CMDIFrameWnd;
	friend class CMDIChildWnd;
	friend class CSplitterWnd;
	friend class COleServerDoc;
	friend class CDocObjectServer;

	//{{AFX_MSG(zt_CView)
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	// commands
	afx_msg void OnUpdateSplitCmd(CCmdUI* pCmdUI);
	afx_msg BOOL OnSplitCmd(UINT nID);
	afx_msg void OnUpdateNextPaneMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnNextPaneCmd(UINT nID);

	// not mapped commands - must be mapped in derived class
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


class _zt_AFX_MOUSEANCHORWND;

class zt_CScrollView : public zt_CView
{
	DECLARE_DYNAMIC(zt_CScrollView)

	// Constructors
protected:
	zt_CScrollView();

public:
	static AFX_DATA const SIZE sizeDefault;
	// used to specify default calculated page and line sizes

	// in logical units - call one of the following Set routines
	void SetScaleToFitSize(SIZE sizeTotal);
	void SetScrollSizes(int nMapMode, SIZE sizeTotal,
		const SIZE& sizePage = sizeDefault,
		const SIZE& sizeLine = sizeDefault);

	// Attributes
public:
	CPoint GetScrollPosition() const;       // upper corner of scrolling
	CSize GetTotalSize() const;             // logical size

	void CheckScrollBars(BOOL& bHasHorzBar, BOOL& bHasVertBar) const;

	// for device units
	CPoint GetDeviceScrollPosition() const;
	void GetDeviceScrollSizes(int& nMapMode, SIZE& sizeTotal,
		SIZE& sizePage, SIZE& sizeLine) const;

	// Operations
public:
	void ScrollToPosition(POINT pt);    // set upper left position
	void FillOutsideRect(CDC* pDC, CBrush* pBrush);
	void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
	BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);

	// Implementation
protected:
	_zt_AFX_MOUSEANCHORWND* m_pAnchorWindow; // window for wheel mouse anchor
	friend class _zt_AFX_MOUSEANCHORWND;
	int m_nMapMode;				 // mapping mode for window creation
	CSize m_totalLog;           // total size in logical units (no rounding)
	CSize m_totalDev;           // total size in device units
	CSize m_pageDev;            // per page scroll size in device units
	CSize m_lineDev;            // per line scroll size in device units

	BOOL m_bCenter;             // Center output if larger than total size
	BOOL m_bInsideUpdate;       // internal state for OnSize callback
	void CenterOnPoint(CPoint ptCenter);
	void ScrollToDevicePosition(POINT ptDev); // explicit scrolling no checking

protected:
	virtual void OnDraw(CDC* pDC) = 0;      // pass on pure virtual

	void UpdateBars();          // adjust scrollbars etc
	BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
	// size with no bars
	void GetScrollBarSizes(CSize& sizeSb);
	void GetScrollBarState(CSize sizeClient, CSize& needSb,
		CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);

public:
	virtual ~zt_CScrollView();
#ifdef _DEBUG
	virtual void Dump(CDumpContext&) const;
	virtual void AssertValid() const;
#endif //_DEBUG
	virtual void CalcWindowRect(LPRECT lpClientRect,
		UINT nAdjustType = adjustBorder);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

	virtual CSize GetWheelScrollDistance(CSize sizeDistance,
		BOOL bHorz, BOOL bVert);

	// scrolling implementation support for OLE
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

	//{{AFX_MSG(zt_CScrollView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
	//	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT HandleMButtonDown(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

struct zt_AUX_DATA
{
	// system metrics
	int cxVScroll, cyHScroll;
	//int cxIcon, cyIcon;

	//int cxBorder2, cyBorder2;

	//// device metrics for screen
	//int cxPixelsPerInch, cyPixelsPerInch;

	//// convenient system color
	//HBRUSH hbrWindowFrame;
	//HBRUSH hbrBtnFace;

	//// color values of system colors used for CToolBar
	//COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
	//COLORREF clrBtnText, clrWindowFrame;

	//// standard cursors
	//HCURSOR hcurWait;
	//HCURSOR hcurArrow;
	//HCURSOR hcurHelp;       // cursor used in Shift+F1 help

	//// special GDI objects allocated on demand
	//HFONT   hStatusFont;
	//HFONT   hToolTipsFont;
	//HBITMAP hbmMenuDot;

	//// Implementation
	//AUX_DATA();
	//~AUX_DATA();
	//void UpdateSysColors();
	//void UpdateSysMetrics();
};



extern AFX_DATA zt_AUX_DATA zt_afxData;
