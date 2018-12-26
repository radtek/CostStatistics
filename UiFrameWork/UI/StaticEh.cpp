//

#include "stdafx.h"
#include "UI.h"
#include "StaticEh.h"
#include "util.h"
#include "ZTUI.h"

// CStaticEh

IMPLEMENT_DYNAMIC(CStaticEh, CStatic)

CStaticEh::CStaticEh()
{
	m_hBrush = NULL;
	m_hBrushHighlight = NULL;
	m_bChangeByDblClk = false;
	m_bTracking = false;
	m_bkgndColor = RGB(255, 255, 255);
	m_highlightBkgndColor = RGB(155, 155, 155);
	m_textColor = RGB(0, 0, 0);
	m_borderColor = RGB(50, 80, 150);
	m_borderType = PS_SOLID;
	m_bActive = false;
}

CStaticEh::~CStaticEh()
{
	if (m_hBrush)
	{
		DeleteObject(m_hBrush);
	}
	if (m_hBrushHighlight)
	{
		DeleteObject(m_hBrushHighlight);
	}
}


BEGIN_MESSAGE_MAP(CStaticEh, CStatic)
    ON_MESSAGE(WM_STATIC_RENAME, OnEdit)
    ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()


// CStaticEh message handlers

LRESULT CStaticEh::OnEdit(WPARAM wParam, LPARAM lParam)
{
	m_nID = wParam;

    CString strText;
    GetWindowText(strText);
	m_edit.SetFont(GetFont());
    m_edit.SetWindowText(strText);
	
	CRect rect;
	GetClientRect(&rect);
	//m_edit.MoveWindow(&rect);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(m_edit.m_hWnd, &rect, TRUE);

    m_edit.ShowWindow(SW_SHOW);
	m_edit.SetFocus();
	m_edit.ShowCaret();
    return 0;
}

int CStaticEh::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CStatic::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    m_edit.Create(WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | SS_CENTER, CRect(0, 0, 0, 0), this, 1000);
    m_edit.ShowWindow(SW_HIDE);
	if (g_ztui->m_pMsgHandler->OnCtrlGetFont())
	{
		m_edit.SetFont(g_ztui->m_pMsgHandler->OnCtrlGetFont());
	}

    return 0;
}

BOOL CStaticEh::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
		if (m_edit.IsWindowVisible())
		{
			CString strText;
			m_edit.GetWindowText(strText);
			SetWindowText(strText);
			m_edit.ShowWindow(SW_HIDE);

			GetParent()->SendMessage(WM_STATIC_REPOSITION, m_nID);
		}
    }
	
    return CStatic::PreTranslateMessage(pMsg);
}

void CStaticEh::EnableHotTrack(bool bEnable)
{
	SetChangeByDblClk(true);
}
HBRUSH CStaticEh::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	if (!m_hBrush)
	{
		m_hBrush = CreateSolidBrush(m_bkgndColor);
	}

	if (!m_hBrushHighlight)
	{
		m_hBrushHighlight = CreateSolidBrush(m_highlightBkgndColor);
	}

	pDC->SetTextColor(m_textColor);
	pDC->SetBkMode(TRANSPARENT); 

	return m_bActive ? m_hBrushHighlight : m_hBrush;
}

void CStaticEh::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//if (m_bChangeByDblClk)
	//{
 //       m_nID = GetDlgCtrlID();

	//	CString strText;
	//	GetWindowText(strText);
	//	m_edit.SetWindowText(strText);

	//	CRect rect;
	//	GetClientRect(&rect);
	//	//m_edit.MoveWindow(&rect);
 //       MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
 //       MyMoveWindow(m_edit.m_hWnd, &rect, TRUE);
	//	m_edit.SetFont(GetFont());
	//	m_edit.ShowWindow(SW_SHOW);
	//	m_edit.SetFocus();
	//	m_edit.ShowCaret();
	//}

	CStatic::OnLButtonDblClk(nFlags, point);
}

HBITMAP CStaticEh::SetBitmap(HBITMAP hBitmap)
{
	return CStatic::SetBitmap(TransparentBmp(hBitmap, m_bkgndColor, RGB(255,255,255)));
}

void CStaticEh::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    ClientToScreen(&point);
    GetParent()->SendMessage(WM_CONTEXT_MENU, point.x, point.y);

    CStatic::OnRButtonUp(nFlags, point);
}

void CStaticEh::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bChangeByDblClk)
	{
		if (!m_bTracking)
		{
			CRect rect;
			GetClientRect(rect);
			CDC* pDC = GetDC();

			CPen pen;
			pen.CreatePen(m_borderType, 1, m_borderColor);
			CPen* pOldPen = pDC->SelectObject(&pen);
			pDC->SelectStockObject(NULL_BRUSH);  

			pDC->Rectangle(rect);

			pDC->SelectObject(pOldPen);
			ReleaseDC(pDC);

			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE | TME_HOVER;
			tme.hwndTrack = this->m_hWnd;
			tme.dwHoverTime = 10;

			if (::_TrackMouseEvent(&tme))
			{
				m_bTracking = true;   
			}
		}
	}
	CStatic::OnMouseMove(nFlags, point);
}

void CStaticEh::OnMouseLeave()
{
	Invalidate();

	m_bTracking = false;

	CStatic::OnMouseLeave();
}

void CStaticEh::OnMouseHover(UINT nFlags, CPoint point)
{
//	m_bTracking = false;	

	CStatic::OnMouseHover(nFlags, point);
}
