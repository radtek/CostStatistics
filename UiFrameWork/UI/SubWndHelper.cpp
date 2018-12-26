#include "stdafx.h"
#include "SubWndHelper.h"
#include "Common.h"
#include "zt_mfc\zt_view.h"
#include "util.h"
#include "toolbareh.h"
#include "PersonWnd.h"
#include "ThingWnd.h"
#include "RightCtrlWnd.h"


CSubWndHelper::CSubWndHelper()
{
	m_bMin = false;
	m_nMinHight = 0;
}

CSubWndHelper::~CSubWndHelper()
{
	for (list<WndInfo>::iterator it = m_subWndList.begin(); it != m_subWndList.end(); ++it)
	{
		if (it->bNeedDel && it->pWnd)
		{
			delete (it->pWnd);
		}
	}
}

void CSubWndHelper::RemoveAll()
{
	for (list<WndInfo>::iterator it = m_subWndList.begin(); it != m_subWndList.end(); ++it)
	{
		if (it->bNeedDel && it->pWnd)
		{
			delete (it->pWnd);
		}
	}
	m_subWndList.clear();
}

void CSubWndHelper::AddSubWnd(CWnd* pWnd, E_WindowType type, int customType, bool bAutoWidth, bool bNeedDel)
{
	if (!pWnd || !pWnd->m_hWnd)
	{
		return;
	}

	CPoint offset(0,0);
	ZTCScrollView *scrollView = dynamic_cast<ZTCScrollView*>(m_pOwnerWnd);
	if (scrollView)
	{
		offset = scrollView->GetScrollPosition();
	}

	CPoint point = m_beginPoint;
	CRect rect;

	if (!m_subWndList.empty())
	{
		CWnd* wnd = m_subWndList.back().pWnd;
		wnd->GetWindowRect(rect);
		m_pOwnerWnd->ScreenToClient(rect);
		point.x = m_beginPoint.x - offset.x;
		point.y = rect.bottom + m_yMargin;
	}

	pWnd->GetWindowRect(rect);
	m_pOwnerWnd->ScreenToClient(rect);
	rect.MoveToXY(point);

	if (m_subWndList.empty())
	{
		rect -= offset;
	}
	//pWnd->MoveWindow(rect);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(pWnd->m_hWnd, rect, TRUE);
	pWnd->ShowWindow(SW_SHOW);

	m_subWndList.push_back(WndInfo(pWnd, type, bAutoWidth, bNeedDel,customType));
	Active(pWnd);
}


void CSubWndHelper::RemoveSubWnd(const CWnd* pWnd)
{

	for (list<WndInfo>::iterator it = m_subWndList.begin(); it != m_subWndList.end(); ++it)
	{
		if (pWnd == it->pWnd)
		{
			
			m_subWndList.erase(it);
			break;
		}

	}
}

void CSubWndHelper::MoveSubWnd(const CWnd* pWnd, int step)
{
	if (!pWnd || 0 == step)
	{
		return;
	}

	for (list<WndInfo>::iterator it = m_subWndList.begin(); it != m_subWndList.end(); ++it)
	{
		if (pWnd == it->pWnd)
		{
			list<WndInfo>::iterator it2 = it;
			if (step > 0)
			{
				for (int i=0; i <= step && it2 != m_subWndList.end(); ++i, ++it2)
				{
				}
			}
			else
			{
				for (int i=0; i < abs(step) && it2 != m_subWndList.begin(); ++i, --it2)
				{
				}
			}
			m_subWndList.insert(it2, *it);
			m_subWndList.erase(it);

			break;
		}
	}
}

void CSubWndHelper::MoveSubWndToTop(const CWnd* pWnd)
{
	MoveSubWnd(pWnd, -1*m_subWndList.size());
}


// 重新在纵向铺开所有子窗口
void CSubWndHelper::RepositionY()
{
	CPoint offset(0,0);
	ZTCScrollView *scrollView = dynamic_cast<ZTCScrollView*>(m_pOwnerWnd);
	if (scrollView)
	{
		offset = scrollView->GetScrollPosition();
	}

	int y = 0;
	for (list<WndInfo>::iterator it = m_subWndList.begin(); it != m_subWndList.end(); ++it)
	{
		CRect rect;
		it->pWnd->GetWindowRect(rect);
		m_pOwnerWnd->ScreenToClient(rect);

		if (it == m_subWndList.begin())
		{
			rect.MoveToXY(CPoint(rect.left, m_beginPoint.y));
			rect -= CPoint(0,offset.y);
		}
		else
		{
			rect.MoveToXY(CPoint(rect.left, y));
		}

		//it->pWnd->MoveWindow(rect);
        //MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
        MyMoveWindow(it->pWnd->m_hWnd, rect, TRUE);
		
		y = rect.bottom + m_yMargin;
	}
}

// 父窗口宽度变化时，同步调整所有AutoWidth子窗口的宽度。
void CSubWndHelper::RepositionX(int cx)
{
    for (list<WndInfo>::iterator it = m_subWndList.begin(); it != m_subWndList.end(); ++it)
    {
		if (it->bAutoWidth)
		{
			CRect rect;
			it->pWnd->GetWindowRect(rect);
			m_pOwnerWnd->ScreenToClient(rect);

			rect.right = rect.left + cx;
			//it->pWnd->MoveWindow(rect);
            MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
            MyMoveWindow(it->pWnd->m_hWnd, rect, TRUE);
		}
   }
}

UINT CSubWndHelper::GetHight()
{
	UINT h = 0;
	if (!m_subWndList.empty())
	{
		if (!m_bMin)
		{
			CWnd* pWnd = m_subWndList.back().pWnd;
			CRect rect;
			pWnd->GetWindowRect(rect);
			m_pOwnerWnd->ScreenToClient(rect);
			h = rect.bottom + m_yMargin;

			CPoint offset(0,0);
			ZTCScrollView *scrollView = dynamic_cast<ZTCScrollView*>(m_pOwnerWnd);
			if (scrollView)
			{
				offset = scrollView->GetScrollPosition();
				h += offset.y;
			}
		}
		else
		{
			h = m_nMinHight;
		}
	}

	return h;
}

void CSubWndHelper::Active(CWnd* pWnd)
{
	for (list<WndInfo>::iterator it = m_subWndList.begin(); it != m_subWndList.end(); ++it)
	{
		CToolbarEh* pToolBar = NULL;
		CRightCtrlWnd* pCtrl = NULL;
		
		switch (it->eType)
		{
		case e_person:
			pToolBar = dynamic_cast<CPersonWnd*>(it->pWnd)->m_pToolbar;
			break;
		case e_thing:
			pToolBar = dynamic_cast<CThingWnd*>(it->pWnd)->m_pToolbar;
			break;
		case e_custom:
			pCtrl = dynamic_cast<CRightCtrlWnd*>(it->pWnd);
			break;
		}

		if (pWnd == it->pWnd)
		{
			if (pToolBar)
			{
				pToolBar->Active(true);
			}
			else if (pCtrl)
			{
				pCtrl->m_active = true;
				pCtrl->RedrawWindow();
			}
		}
		else
		{
			if (pToolBar)
			{
				pToolBar->Active(false);
			}
			else if (pCtrl)
			{
				pCtrl->m_active = false;
				pCtrl->RedrawWindow();
			}
		}
	}
}

CWnd* CSubWndHelper::FindByName(CString name)
{
	CWnd* pWnd = NULL;
	for (list<WndInfo>::iterator it = m_subWndList.begin(); it != m_subWndList.end(); ++it)
	{
		CString strName;

		switch (it->eType)
		{
		case e_person:
			strName = dynamic_cast<CPersonWnd*>(it->pWnd)->m_name;
			break;
		case e_thing:
			strName = dynamic_cast<CThingWnd*>(it->pWnd)->m_name;
			break;
		case  e_custom:
// 			strName = dynamic_cast<CRightCtrlWnd*>(it->pWnd).m_name;
			break;
		}

		if (name == strName)
		{
			pWnd = it->pWnd;
			break;
		}
	}

	return pWnd;
}
