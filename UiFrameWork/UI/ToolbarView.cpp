// MyToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "UI.h"
#include "ToolbarView.h"
#include "util.h"

#define h_toolbar 26

// CToolbarView

IMPLEMENT_DYNCREATE(CToolbarView, CView)

CToolbarView::CToolbarView()
{

}

CToolbarView::~CToolbarView()
{

}

BEGIN_MESSAGE_MAP(CToolbarView, CView)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CToolbarView drawing

void CToolbarView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CToolbarView diagnostics

#ifdef _DEBUG
void CToolbarView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CToolbarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CToolbarView message handlers


int CToolbarView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    CCreateContext cx;
    cx.m_pNewViewClass = RUNTIME_CLASS(CToolbarEh);
    CFrameWnd* pfWnd = (CFrameWnd*)this;
    m_pToolbar = (CToolbarEh*)pfWnd->CreateView(&cx);

    m_pToolbar->SetParent(this);
    m_pToolbar->EnableToolTips();
    CRect rc(0, 0, 3000, h_toolbar);
    //m_pToolbar->MoveWindow(&rc);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(m_pToolbar->m_hWnd, &rc, TRUE);
    m_pToolbar->ModifyStyleEx(WS_EX_WINDOWEDGE | WS_BORDER, 0);

    return 0;
}


void CToolbarView::OnSize(UINT nType, int cx, int cy)
{
    CView::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
/*    CRect rect1, rect2;
    m_tb->GetClientRect(&rect1);
    GetClientRect(rect2);
    m_tb->MoveWindow(CRect(rect1.left, rect1.top, rect2.right, rect1.bottom));*/
}

void CToolbarView::SetOwner(CWnd* pOwner)
{
    m_pToolbar->SetOwner(pOwner);
}
