
// MainFrm.cpp : implementation of the CZTUIMainFrame class
//

#include "stdafx.h"
#include "UI.h"

#include "ZTUIMainFrame.h"
#include "LeftWnd.h"
#include "RightWnd.h"
#include "Toolbareh.h"
#include "ZTUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CZTUIMainFrame

IMPLEMENT_DYNAMIC(CZTUIMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CZTUIMainFrame, CFrameWnd)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CZTUIMainFrame construction/destruction

CZTUIMainFrame::CZTUIMainFrame()
{
	// TODO: add member initialization code here
}

CZTUIMainFrame::~CZTUIMainFrame()
{
   SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, TRUE, NULL, 0);
}

BOOL CZTUIMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CZTUIMainFrame diagnostics

#ifdef _DEBUG
void CZTUIMainFrame::AssertValid() const
{
	//CFrameWnd::AssertValid();
}

void CZTUIMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CZTUIMainFrame message handlers

void CZTUIMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	//m_wndView.SetFocus();
// 	m_wndRightSplitter.GetPane(1, 0)->SetFocus();

}

BOOL CZTUIMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
 	if (m_wndRightSplitter.GetPane(1, 0)->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
 		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CZTUIMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// split
	m_wndSplitter.CreateStatic(this, 1, 2);
	CRect rect;
	GetClientRect(&rect);
	CSize size = rect.Size();
    m_wndSplitter.SetColumnInfo(0, size.cx / 4, 50);
    
    size.cy = 26;
    m_wndLeftSplitter.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_wndSplitter.IdFromRowCol(0, 0));
    m_wndLeftSplitter.CreateView(0, 0, RUNTIME_CLASS(CToolbarEh), size, pContext);
    m_wndLeftSplitter.CreateView(1, 0, RUNTIME_CLASS(CLeftWnd), size, pContext);
    m_wndLeftSplitter.HideSplitter();

    CLeftWnd* pLeftWnd = (CLeftWnd*)m_wndLeftSplitter.GetPane(1, 0);
    CToolbarEh* pToolbar = (CToolbarEh*)m_wndLeftSplitter.GetPane(0, 0);
	pToolbar->EnableToolTips();
    pToolbar->SetOwner(pLeftWnd);
    pLeftWnd->m_pToolbar = pToolbar;
    g_ztui->m_pLeftWnd = pLeftWnd;

    m_wndRightSplitter.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_wndSplitter.IdFromRowCol(0, 1));
    m_wndRightSplitter.CreateView(0, 0, RUNTIME_CLASS(CToolbarEh), size, pContext);
    m_wndRightSplitter.CreateView(1, 0, RUNTIME_CLASS(CRightWnd), size, pContext);
    m_wndRightSplitter.HideSplitter();

	CRightWnd* pRightWnd = (CRightWnd*)m_wndRightSplitter.GetPane(1, 0);
    pToolbar = (CToolbarEh*)m_wndRightSplitter.GetPane(0, 0);
	pToolbar->EnableToolTips();
    pToolbar->SetOwner(pRightWnd);
	pRightWnd->m_pToolbar = pToolbar;
	g_ztui->m_pRightWnd = pRightWnd;


	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CZTUIMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
   SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, FALSE, &m_bDragFullWindowsBak, 0);
   SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, FALSE, NULL, 0);


	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, NULL, dwExStyle, pContext);
}

void CZTUIMainFrame::SetLeftWndWidth(int w)
{
	m_wndSplitter.SetColumnInfo(0, w, 0);
	m_wndSplitter.RecalcLayout();  
}


void CZTUIMainFrame::ShowHideLeftWnd()
{
	if (m_wndSplitter.HaveHiddenColumn())
	{
		m_wndSplitter.ShowColumn();
		if (GetParent() && GetParent()->m_hWnd)
		{
			GetParent()->SendMessage(WM_LEFTWND_SHOWHIDE, -m_wndSplitter.GetHideColWidth());
		}
	}
	else
	{
		m_wndSplitter.HideColumn(0);
		if (GetParent() && GetParent()->m_hWnd)
		{
			GetParent()->SendMessage(WM_LEFTWND_SHOWHIDE, m_wndSplitter.GetHideColWidth());
		}
	}
}

int CZTUIMainFrame::GetLeftWndWidth()
{
	int w, h;
	m_wndSplitter.GetColumnInfo(0, w, h);  
	return w;
}

