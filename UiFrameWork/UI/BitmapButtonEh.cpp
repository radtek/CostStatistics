// BitmapButtonEh.cpp : implementation file
//

#include "stdafx.h"
#include "UI.h"
#include "BitmapButtonEh.h"
#include "util.h"


// CBitmapButtonEh

IMPLEMENT_DYNAMIC(CBitmapButtonEh, CBitmapButton)

CBitmapButtonEh::CBitmapButtonEh()
{
	m_bTracking = false;
	m_bkgndColor = RGB(255, 255, 255);
	m_highlightBkgndColor = RGB(255, 255, 255);
	m_transparentMaskColor = RGB(255, 255, 255);
	m_borderColor = RGB(128, 128, 128);
	m_hBmp = NULL;
	m_hBmpHighlight = NULL;
	m_bBHighlight = false;
}

CBitmapButtonEh::~CBitmapButtonEh()
{
}


BEGIN_MESSAGE_MAP(CBitmapButtonEh, CBitmapButton)
    ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()



BOOL CBitmapButtonEh::LoadBitmapsFromFiles(
	LPCTSTR lpszBitmapResource,
	LPCTSTR lpszBitmapResourceSel,
	LPCTSTR lpszBitmapResourceFocus,
	LPCTSTR lpszBitmapResourceDisabled)
{
	// delete old bitmaps (if present)
	m_bitmap.DeleteObject();
	m_bitmap2.DeleteObject();
	m_bitmapSel.DeleteObject();
	m_bitmapFocus.DeleteObject();
	m_bitmapDisabled.DeleteObject();

	if (FAILED(m_image.Load(lpszBitmapResource)))
	{
		TRACE(traceAppMsg, 0, "Failed to load bitmap for normal image.\n");
		return FALSE;   // need this one image
	}
	if (FAILED(m_imageHighlight.Load(lpszBitmapResource)))
	{
		TRACE(traceAppMsg, 0, "Failed to load bitmap for normal image.\n");
		return FALSE;   // need this one image
	}

	m_hBmp = TransparentBmp(m_image.Detach(), m_bkgndColor, m_transparentMaskColor);
	m_hBmpHighlight = TransparentBmp(m_imageHighlight.Detach(), m_highlightBkgndColor, m_transparentMaskColor);

	if (m_bBHighlight)
	{
		m_bitmap.Attach(m_hBmpHighlight);
		m_bitmap2.Attach(m_hBmp);
	}
	else
	{
		m_bitmap.Attach(m_hBmp);
		m_bitmap2.Attach(m_hBmpHighlight);
	}

	BOOL bAllLoaded = TRUE;
	if (lpszBitmapResourceSel != NULL)
	{
		if (!m_bitmapSel.LoadBitmap(lpszBitmapResourceSel))
		{
			TRACE(traceAppMsg, 0, "Failed to load bitmap for selected image.\n");
			bAllLoaded = FALSE;
		}
	}
	if (lpszBitmapResourceFocus != NULL)
	{
		if (!m_bitmapFocus.LoadBitmap(lpszBitmapResourceFocus))
			bAllLoaded = FALSE;
	}
	if (lpszBitmapResourceDisabled != NULL)
	{
		if (!m_bitmapDisabled.LoadBitmap(lpszBitmapResourceDisabled))
			bAllLoaded = FALSE;
	}
	return bAllLoaded;
}


BOOL CBitmapButtonEh::LoadBitmapsFromResource(
							 UINT lpszBitmapResource,
							 UINT lpszBitmapResourceSel,
							 UINT lpszBitmapResourceFocus,
							 UINT lpszBitmapResourceDisabled
							 )
{
	m_bitmap.DeleteObject();
	m_bitmap2.DeleteObject();
	m_bitmapSel.DeleteObject();
	m_bitmapFocus.DeleteObject();
	m_bitmapDisabled.DeleteObject();
	
	CBitmap tmp,tmp2;
	if(!tmp.LoadBitmap(lpszBitmapResource)) return FALSE;
	if(!tmp2.LoadBitmap(lpszBitmapResource)) return FALSE;

	m_image.Attach((HBITMAP)tmp.m_hObject);
	m_imageHighlight.Attach((HBITMAP)tmp2.m_hObject);

	tmp.Detach();
	tmp2.Detach();
	
	m_hBmp = TransparentBmp(m_image.Detach(), m_bkgndColor, m_transparentMaskColor);
	m_hBmpHighlight = TransparentBmp(m_imageHighlight.Detach(), m_highlightBkgndColor, m_transparentMaskColor);

	if (m_bBHighlight)
	{
		m_bitmap.Attach(m_hBmpHighlight);
		m_bitmap2.Attach(m_hBmp);
	}
	else
	{
		m_bitmap.Attach(m_hBmp);
		m_bitmap2.Attach(m_hBmpHighlight);
	}

	BOOL bAllLoaded = TRUE;
	if (lpszBitmapResourceSel != NULL)
	{
		if (!m_bitmapSel.LoadBitmap(lpszBitmapResourceSel))
		{
			TRACE(traceAppMsg, 0, "Failed to load bitmap for selected image.\n");
			bAllLoaded = FALSE;
		}
	}
	if (lpszBitmapResourceFocus != NULL)
	{
		if (!m_bitmapFocus.LoadBitmap(lpszBitmapResourceFocus))
			bAllLoaded = FALSE;
	}
	if (lpszBitmapResourceDisabled != NULL)
	{
		if (!m_bitmapDisabled.LoadBitmap(lpszBitmapResourceDisabled))
			bAllLoaded = FALSE;
	}
	return bAllLoaded;
}
void CBitmapButtonEh::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    ClientToScreen(&point);
    GetParent()->SendMessage(WM_CONTEXT_MENU, point.x, point.y);

    CBitmapButton::OnRButtonUp(nFlags, point);
}

void CBitmapButtonEh::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bTracking)
	{
		CRect rect;
		GetClientRect(rect);
		CDC* pDC = GetDC();

		CPen pen;
		pen.CreatePen(PS_SOLID, 1, m_borderColor);
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

	CBitmapButton::OnMouseMove(nFlags, point);
}

void CBitmapButtonEh::OnMouseLeave()
{
	Invalidate();

	m_bTracking = false;

	CBitmapButton::OnMouseLeave();
}

void CBitmapButtonEh::Highlight(bool bHighlight)
{
	m_bBHighlight = bHighlight;
	if (m_bBHighlight)
	{
		m_bitmap.Detach();
		m_bitmap2.Detach();
		m_bitmap.Attach(m_hBmpHighlight);
		m_bitmap2.Attach(m_hBmp);
	}
	else
	{
		m_bitmap.Detach();
		m_bitmap2.Detach();
		m_bitmap.Attach(m_hBmp);
		m_bitmap2.Attach(m_hBmpHighlight);
	}
}