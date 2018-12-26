// OleDropTargetEh.cpp : implementation file
//

#include "stdafx.h"
//#include "../ZTUI.h"
#include "TreeGrid.h"
#include "OleDropTargetEh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// COleDropTargetEh

// IMPLEMENT_DYNAMIC(COleDropTargetEh, COleDropTarget)


COleDropTargetEh::COleDropTargetEh()
{
}

COleDropTargetEh::~COleDropTargetEh()
{
}


BEGIN_MESSAGE_MAP(COleDropTargetEh, COleDropTarget)
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// default implementation of drag/drop scrolling

DROPEFFECT COleDropTargetEh::OnDragScroll(CWnd* pWnd, DWORD dwKeyState,
										CPoint point)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pWnd);

	return DROPEFFECT_NONE;


// 	// CWnds are allowed, but don't support autoscrolling
// 	if (!pWnd->IsKindOf(RUNTIME_CLASS(CTreeListCtrl)))
// 		return DROPEFFECT_NONE;
// 	CTreeListCtrl* pView = (CTreeListCtrl*)pWnd;
// 	DROPEFFECT dropEffect = pView->OnDragScroll(dwKeyState, point);
// 
// 	// DROPEFFECT_SCROLL means do the default
// 	if (dropEffect != DROPEFFECT_SCROLL)
// 		return dropEffect;
// 
// 	// get client rectangle of destination window
// 	CRect rectClient;
// 	pWnd->GetClientRect(&rectClient);
// 	CRect rect = rectClient;
// 
// 	// hit-test against inset region
// 	UINT nTimerID = 0xffff;
// 	rect.InflateRect(-nScrollInset, -nScrollInset);
// 	CSplitterWnd* pSplitter = NULL;
// 	if (rectClient.PtInRect(point) && !rect.PtInRect(point))
// 	{
// 		// determine which way to scroll along both X & Y axis
// 		if (point.x < rect.left)
// 			nTimerID = MAKEWORD(SB_LINEUP, HIBYTE(nTimerID));
// 		else if (point.x >= rect.right)
// 			nTimerID = MAKEWORD(SB_LINEDOWN, HIBYTE(nTimerID));
// 		if (point.y < rect.top)
// 			nTimerID = MAKEWORD(LOBYTE(nTimerID), SB_LINEUP);
// 		else if (point.y >= rect.bottom)
// 			nTimerID = MAKEWORD(LOBYTE(nTimerID), SB_LINEDOWN);
// 		ASSERT(nTimerID != 0xffff);
// 
// 		// check for valid scroll first
// 		pSplitter = CTreeListCtrl::GetParentSplitter(pView, FALSE);
// 		BOOL bEnableScroll = FALSE;
// 		if (pSplitter != NULL)
// 			bEnableScroll = pSplitter->DoScroll(pView, nTimerID, FALSE);
// 		else
// 			bEnableScroll = pView->OnScroll(nTimerID, 0, FALSE);
// 		if (!bEnableScroll)
// 			nTimerID = 0xffff;
// 	}
// 
// 	if (nTimerID == 0xffff)
// 	{
// 		if (m_nTimerID != 0xffff)
// 		{
// 			// send fake OnDragEnter when transition from scroll->normal
// 			COleDataObject dataObject;
// 			dataObject.Attach(m_lpDataObject, FALSE);
// 			OnDragEnter(pWnd, &dataObject, dwKeyState, point);
// 			m_nTimerID = 0xffff;
// 		}
// 		return DROPEFFECT_NONE;
// 	}
// 
// 	// save tick count when timer ID changes
// 	DWORD dwTick = GetTickCount();
// 	if (nTimerID != m_nTimerID)
// 	{
// 		m_dwLastTick = dwTick;
// 		m_nScrollDelay = nScrollDelay;
// 	}
// 
// 	// scroll if necessary
// 	if (dwTick - m_dwLastTick > m_nScrollDelay)
// 	{
// 		if (pSplitter != NULL)
// 			pSplitter->DoScroll(pView, nTimerID, TRUE);
// 		else
// 			pView->OnScroll(nTimerID, 0, TRUE);
// 		m_dwLastTick = dwTick;
// 		m_nScrollDelay = nScrollInterval;
// 	}
// 	if (m_nTimerID == 0xffff)
// 	{
// 		// send fake OnDragLeave when transitioning from normal->scroll
// 		OnDragLeave(pWnd);
// 	}
// 
// 	m_nTimerID = nTimerID;
// 	// check for force link
// 	if ((dwKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
// 		dropEffect = DROPEFFECT_SCROLL|DROPEFFECT_LINK;
// 	// check for force copy
// 	else if ((dwKeyState & MK_CONTROL) == MK_CONTROL)
// 		dropEffect = DROPEFFECT_SCROLL|DROPEFFECT_COPY;
// 	// check for force move
// 	else if ((dwKeyState & MK_ALT) == MK_ALT ||
// 		(dwKeyState & MK_SHIFT) == MK_SHIFT)
// 		dropEffect = DROPEFFECT_SCROLL|DROPEFFECT_MOVE;
// 	// default -- recommended action is move
// 	else
// 		dropEffect = DROPEFFECT_SCROLL|DROPEFFECT_MOVE;
// 	return dropEffect;
}

/////////////////////////////////////////////////////////////////////////////
// COleDropTargetEh drop/ drop query handling

DROPEFFECT COleDropTargetEh::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject,
									   DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CTreeListCtrl)))
		return DROPEFFECT_NONE;

	// default delegates to view
	CTreeListCtrl* pView = (CTreeListCtrl*)pWnd;
	ASSERT_VALID(pView);
	return pView->OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT COleDropTargetEh::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject,
									  DWORD dwKeyState, CPoint point)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CTreeListCtrl)))
		return DROPEFFECT_NONE;

	// default delegates to view
	CTreeListCtrl* pView = (CTreeListCtrl*)pWnd;
	ASSERT_VALID(pView);
	return pView->OnDragOver(pDataObject, dwKeyState, point);
}

BOOL COleDropTargetEh::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
							DROPEFFECT dropEffect, CPoint point)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CTreeListCtrl)))
		return DROPEFFECT_NONE;

	// default delegates to view
	CTreeListCtrl* pView = (CTreeListCtrl*)pWnd;
	ASSERT_VALID(pView);
	return pView->OnDrop(pDataObject, dropEffect, point);
}

DROPEFFECT COleDropTargetEh::OnDropEx(CWnd* pWnd, COleDataObject* pDataObject,
									DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CTreeListCtrl)))
		return (DROPEFFECT)-1;  // not implemented

	// default delegates to view
	CTreeListCtrl* pView = (CTreeListCtrl*)pWnd;
	ASSERT_VALID(pView);
	return pView->OnDropEx(pDataObject, dropEffect, dropEffectList, point);
}

void COleDropTargetEh::OnDragLeave(CWnd* pWnd)
{
	ASSERT_VALID(this);

	if (!pWnd->IsKindOf(RUNTIME_CLASS(CTreeListCtrl)))
		return;

	// default delegates to view
	CTreeListCtrl* pView = (CTreeListCtrl*)pWnd;
	ASSERT_VALID(pView);
	pView->OnDragLeave();
	return;
}

