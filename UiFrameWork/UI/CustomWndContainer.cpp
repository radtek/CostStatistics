#include "StdAfx.h"
#include "CustomWndContainer.h"
#include "ZTUI.h"


IMPLEMENT_DYNAMIC(CCustomWndContainer, CRightCtrlWnd)

CCustomWndContainer::CCustomWndContainer(CWnd* pParent /*=NULL*/):
CRightCtrlWnd(CCustomWndContainer::IDD, pParent),
m_pInnerWnd(NULL)
{
	m_minSize = 20;
	m_resizeMoveFlag = RSM_RIGHT | RSM_BOTTOM;
	m_nHitTest = 0;
}

CCustomWndContainer::~CCustomWndContainer(void)
{
}


BEGIN_MESSAGE_MAP(CCustomWndContainer, CRightCtrlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()

	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_SETFOCUS()
	ON_WM_MOVE()
	ON_WM_PAINT()
END_MESSAGE_MAP()




void CCustomWndContainer::DoDataExchange(CDataExchange* pDX)
{
	CRightCtrlWnd::DoDataExchange(pDX);
}

int CCustomWndContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}


void CCustomWndContainer::OnDestroy()
{
	CRightCtrlWnd::OnDestroy();

	if(m_pInnerWnd)
	{
		if(m_pInnerWnd->m_hWnd)
		{
			m_pInnerWnd->DestroyWindow();
		}
		delete m_pInnerWnd;
		m_pInnerWnd = NULL;
	}
}

void CCustomWndContainer::OnSize(UINT nType, int cx, int cy)
{
	CRightCtrlWnd::OnSize(nType, cx, cy);

	if(m_pInnerWnd && m_pInnerWnd->m_hWnd)
	{
		m_pInnerWnd->MoveWindow(4,4,cx-8,cy-8);
	}
	
	if(m_pInnerWnd)
	{
		Invalidate();
		GetParent()->PostMessage(WM_SUB_WND_SIZE_CHANGED);
	}
	
}

void CCustomWndContainer::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRightCtrlWnd::OnLButtonDown(nFlags, point);
}

void CCustomWndContainer::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	m_nHitTest = nHitTest;

	if (   m_nHitTest == HTBOTTOMLEFT
		|| m_nHitTest == HTBOTTOMRIGHT
		|| m_nHitTest == HTBOTTOM
		)
	{
		SetCapture();
	}

	CRightCtrlWnd::OnNcLButtonDown(nHitTest, point);
}

void CCustomWndContainer::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (   m_nHitTest == HTBOTTOMLEFT
		|| m_nHitTest == HTBOTTOMRIGHT
		|| m_nHitTest == HTBOTTOM
		)
	{
		CPoint pt = point;
		CRect rect;
		GetWindowRect(rect);
		GetParent()->ScreenToClient(rect);

		rect.bottom = rect.top + max(m_minSize.cy, pt.y);

		if (m_nHitTest == HTBOTTOMLEFT)
		{
			rect.left = rect.right - max(m_minSize.cx, rect.Width()-pt.x);
		}
		if (m_nHitTest == HTBOTTOMRIGHT)
		{
			rect.right = rect.left + max(m_minSize.cx, pt.x);
		}

		//MoveWindow(rect);
	
		MyMoveWindow(m_hWnd, rect, TRUE);

		ReleaseCapture();
		m_nHitTest = HTNOWHERE;
		g_ztui->m_pRightWnd->Refresh();
		m_pInnerWnd->Invalidate();
	}

	CRightCtrlWnd::OnLButtonUp(nFlags, point);
}

LRESULT CCustomWndContainer::OnNcHitTest(CPoint point)
{
	if (m_resizeMoveFlag)
	{
#include "WndResize.h"
	}


	return CRightCtrlWnd::OnNcHitTest(point);
}

void CCustomWndContainer::OnSetFocus(CWnd* pOldWnd)
{
	CRightCtrlWnd::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
}

void CCustomWndContainer::OnMove(int x, int y)
{
	CRightCtrlWnd::OnMove(x, y);

	if(m_pInnerWnd)
	{
		m_pInnerWnd->Invalidate();
	}
}

void CCustomWndContainer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	DrawBorder(&dc);
	if(m_pInnerWnd)
	{
		m_pInnerWnd->Invalidate();
	}
}
