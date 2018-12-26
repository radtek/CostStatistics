// RightWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UI.h"
#include "RightWnd.h"
#include "PersonWnd.h"
#include "ThingWnd.h"
#include "ToolbarEh.h"
#include "ZTUI.h"
#include "RightCtrlWnd.h"

#define x_margin 5
#define y_margin 12
#define h_toolbar 26

// CRightWnd

IMPLEMENT_DYNCREATE(CRightWnd, ZTCScrollView)

CRightWnd::CRightWnd()
{
	m_pCustomWnd = NULL;
	m_bInited = false;
	m_subWndHelper.m_yMargin = y_margin;
	m_subWndHelper.m_beginPoint.x = x_margin;
	m_subWndHelper.m_beginPoint.y = 5;
	m_subWndHelper.m_pOwnerWnd = this;
    m_cx = -1;
	m_pToolbar = NULL;
    m_pActiveThing = NULL;
    m_backColor = g_ztui->m_pMsgHandler->OnGetRightWndBackColor();
}

CRightWnd::~CRightWnd()
{
	
}


BEGIN_MESSAGE_MAP(CRightWnd, ZTCScrollView)
	ON_MESSAGE(WM_SUB_WND_SIZE_CHANGED, &OnSubWndSizeChanged)
    ON_MESSAGE(WM_TOOLBAR_MESSAGE, &OnToolbarClicked)
    ON_WM_MOUSEWHEEL()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_MESSAGE(WM_THING_ACTIVE, &OnThingActived)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRightWnd drawing

void CRightWnd::OnInitialUpdate()
{
	if (m_bInited)
	{
		return;
	}
	m_bInited = true;

	ZTCScrollView::OnInitialUpdate();
    
	CSize sizeTotal;
	sizeTotal.cx = 0;
	sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal, CSize(0, 0), CSize(80, 80));

	Reposition();
}


void CRightWnd::AddCustomWnd(CWnd* pWnd)
{
	if(m_pCustomWnd)
	{
		if(m_pCustomWnd->m_hWnd)
		{
			m_pCustomWnd ->DestroyWindow();
			delete m_pCustomWnd;
		}
		m_pCustomWnd = NULL;
	}
	
	m_pCustomWnd = pWnd;
	m_pCustomWnd->SetParent(this);

	CRect rc;
	GetClientRect(&rc);
	m_pCustomWnd->MoveWindow(&rc);
}
CWnd* CRightWnd::GetCustomWnd()
{
	return m_pCustomWnd;
}

void CRightWnd::OnDraw(CDC* pDC)
{
	// TODO: add draw code here
}


// CRightWnd diagnostics

#ifdef _DEBUG
void CRightWnd::AssertValid() const
{
	//ZTCScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightWnd::Dump(CDumpContext& dc) const
{
	ZTCScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRightWnd message handlers


CThingWnd* CRightWnd::AddSubWnd(bool bIsLoad)
{
	if(m_pCustomWnd)
	{
		return NULL;
	}
	

    CRect wndRect;
    GetWindowRect(&wndRect);
    CPoint point(x_margin, y_margin);
	SIZE size = { wndRect.Width() - 2 * x_margin, h_toolbar+2};

	CThingWnd * pThingWnd = new CThingWnd();
	CRect rect(point, size);
	pThingWnd->Create(IDD_THING_WND, this);
	pThingWnd->ShowWindow(SW_HIDE);
	//pThingWnd->MoveWindow(rect);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(pThingWnd->m_hWnd, rect, TRUE);

	m_subWndHelper.AddSubWnd(pThingWnd, e_thing,0);

	if (!bIsLoad)
	{
		g_ztui->m_pMsgHandler->OnInitThingWnd(pThingWnd);
	}

	Reposition();

	if (!bIsLoad)
	{
		// 设置焦点并滚动滚动条（如果已经超过窗口底部）
		pThingWnd->SetFocus();
		pThingWnd->GetWindowRect(wndRect);
		g_ztui->m_pRightWnd->ScreenToClient(wndRect);
		CRect rightRect;
		g_ztui->m_pRightWnd->GetWindowRect(rightRect);
		if (wndRect.bottom>rightRect.Height())
		{
			CPoint offset = g_ztui->m_pRightWnd->GetScrollPosition();
			offset.y +=( wndRect.bottom - rightRect.Height()) + y_margin;
			g_ztui->m_pRightWnd->ScrollToPosition(offset);
		}
	}


	return pThingWnd;
}

void CRightWnd::RemoveSubWnd(CWnd* pWnd)
{
	if (!pWnd || !pWnd->m_hWnd)
	{
		return;
	}
	
	if(m_pCustomWnd == pWnd)
	{
		if(m_pCustomWnd->m_hWnd)
		{
			m_pCustomWnd->DestroyWindow();
			delete m_pCustomWnd;
			m_pCustomWnd = NULL;
		}
		return;
	}
	m_subWndHelper.RemoveSubWnd(pWnd);

	CThingWnd* pSubWnd = dynamic_cast<CThingWnd*>(pWnd);
	if (pSubWnd)
	{
        pSubWnd->RemoveAll();
		pSubWnd->EndDialog(IDOK);
		delete pSubWnd;
	}

	Reposition();
}

void CRightWnd::MoveSubWnd(const CWnd* pWnd, int step)
{
    if (pWnd)
    {
        m_subWndHelper.MoveSubWnd(pWnd, step);

        Reposition();
    }
}

void CRightWnd::MoveSubWndToTop(const CWnd* pWnd)
{
	if (pWnd)
	{
		m_subWndHelper.MoveSubWndToTop(pWnd);

		Reposition();
	}
}

void CRightWnd::Reposition()
{
	if(m_pCustomWnd)
	{
		return;
	}


	m_subWndHelper.RepositionY();

	UpdateScrollSize();
}

void CRightWnd::UpdateScrollSize()
{
	CSize sizeTotal;
 	sizeTotal.cx = 0;
 	sizeTotal.cy = m_subWndHelper.GetHight();
 	SetScrollSizes(MM_TEXT, sizeTotal, CSize(0, 0), CSize(80, 80));
}

LRESULT CRightWnd::OnToolbarClicked(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CRightWnd::OnSubWndSizeChanged(WPARAM wParam, LPARAM lParam)
{
	Reposition();
	return 0;
}

BOOL CRightWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default

    if (zDelta < 0)
    {
        OnVScroll(SB_LINEDOWN, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
    }
    else if (zDelta > 0)
    {
        OnVScroll(SB_LINEUP, GetScrollPos(SB_VERT), GetScrollBarCtrl(SB_VERT));
    }

    return ZTCScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CRightWnd::OnSize(UINT nType, int cx, int cy)
{
    ZTCScrollView::OnSize(nType, cx, cy);
	
	if(m_pCustomWnd && m_pCustomWnd->m_hWnd)
	{
		m_pCustomWnd->MoveWindow(0,0,cx,cy);
		return;
	}

    if (m_cx != cx)
    {
        // change sub windows cx
        m_subWndHelper.RepositionX(cx - 2 * x_margin);

        m_cx = cx;
    }
}


BOOL CRightWnd::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    CBrush brush;
    brush.CreateSolidBrush(m_backColor);
    pDC->GetClipBox(rect);
    pDC->FillRect(rect, &brush);
    return TRUE;
}

void CRightWnd::Load(RightWndData & cfg, bool bLoadToolbar)
{
    if (bLoadToolbar)
    {
	    m_pToolbar->Load(cfg.toolbar);
    }

	for (vector<ThingData*>::iterator it = cfg.things.begin(); it != cfg.things.end(); ++it)
	{
		CThingWnd* pThingWnd = AddSubWnd(true);
		pThingWnd->Load(**it);
	}
}

void CRightWnd::Save(RightWndData & cfg)
{
	m_pToolbar->Save(cfg.toolbar);

	for (list<WndInfo>::iterator it = m_subWndHelper.m_subWndList.begin();
		it != m_subWndHelper.m_subWndList.end(); ++it)
	{
		ASSERT(e_thing == it->eType);
		CThingWnd* pThingWnd = (CThingWnd*)it->pWnd;
		ThingData* pThingWndCfg = new ThingData();
		pThingWnd->Save(*pThingWndCfg);
		cfg.things.push_back(pThingWndCfg);
	}
}

LRESULT CRightWnd::OnThingActived(WPARAM wParam, LPARAM lParam)
{
	if (m_pActiveThing == (CWnd*)wParam)
	{
		return 0;
	}

    m_pActiveThing = (CWnd*)wParam;

	m_subWndHelper.Active(m_pActiveThing);

    return 0;
}

void CRightWnd::RemoveAll(bool bRemoveToolbar)
{
    if (bRemoveToolbar)
    {
        m_pToolbar->RemoveAll();
    }

    while (m_subWndHelper.m_subWndList.size() > 0)
    {
        RemoveSubWnd(m_subWndHelper.m_subWndList.back().pWnd);
    }
}

void CRightWnd::Refresh()
{
    //Invalidate();
    //UpdateWindow();
}

CThingWnd* CRightWnd::FindThing(CString name)
{
	return dynamic_cast<CThingWnd*>(m_subWndHelper.FindByName(name));
}


void CRightWnd::OnDestroy()
{
	if(m_pCustomWnd)
	{
		if(m_pCustomWnd->m_hWnd)
		{
			m_pCustomWnd->DestroyWindow();
			delete m_pCustomWnd;
			m_pCustomWnd = NULL;
		}
		m_pCustomWnd = NULL;
	}

	zt_CScrollView::OnDestroy();
	
	// TODO: 在此处添加消息处理程序代码
}
