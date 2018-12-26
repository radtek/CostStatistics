// ThingWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UI.h"
#include "ThingWnd.h"
#include "ToolbarEh.h"
#include "PersonWnd.h"
#include "ZTUI.h"

#define x_margin 2
#define y_margin 10
#define h_toolbar 26


// CThingWnd dialog

IMPLEMENT_DYNAMIC(CThingWnd, CDialog)

CThingWnd::CThingWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CThingWnd::IDD, pParent)
{
	m_bShowToolbar = TRUE;
	m_subWndHelper.m_yMargin = y_margin;
	m_subWndHelper.m_beginPoint.x = x_margin;
	m_subWndHelper.m_beginPoint.y = h_toolbar + 3;
	m_subWndHelper.m_pOwnerWnd = this;
	m_subWndHelper.MinHight(h_toolbar);
	m_ctrlId = 3000;
	m_hBrush = NULL;
	m_resizeMoveFlag = 0;
	m_cx = -1;
	m_pToolbar = NULL;
    m_pActivePerson = NULL;
	m_backColor = g_ztui->m_pMsgHandler->OnGetThingWndBackColor();
}

CThingWnd::~CThingWnd()
{
	if (NULL != m_hBrush)
	{
		::DeleteObject(m_hBrush);
	}
}

void CThingWnd::ShowToolbar(BOOL bShow)
{
	m_bShowToolbar = bShow;
	m_subWndHelper.m_beginPoint.y = m_bShowToolbar ? h_toolbar + 3 : 3;
	if(m_pToolbar)
	{
		m_pToolbar->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
	}
	Reposition();
}

void CThingWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CThingWnd, CDialog)
	ON_MESSAGE(WM_TOOLBAR_MESSAGE, &OnToolbarClicked)
	ON_MESSAGE(WM_SUB_WND_SIZE_CHANGED, &OnSubWndSizeChanged)
	ON_WM_SIZE()
    ON_MESSAGE(WM_PERSON_ACTIVE, &OnPersonActived)
	ON_WM_CTLCOLOR()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CThingWnd message handlers

BOOL CThingWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pToolbar = CreateToolbar();
	m_pToolbar->SetOwner(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CToolbarEh* CThingWnd::CreateToolbar()
{
	CCreateContext context;     
	context.m_pNewViewClass = RUNTIME_CLASS( CToolbarEh ) ;     
	CFrameWnd* pfWnd = (CFrameWnd*)this;      
	CToolbarEh* bar = (CToolbarEh*)pfWnd->CreateView( &context );
	bar->m_bkgndColor = g_ztui->m_pMsgHandler->OnGetThingWndToolbarBackColor();
	bar->m_highlightBkgndColor = g_ztui->m_pMsgHandler->OnGetThingWndToolbarHighlightColor();
    bar->OnInitialUpdate();
	bar->ShowWindow(SW_HIDE);

	// Remove edge.
	DWORD dwExStyle = bar->GetExStyle()& ~WS_EX_CLIENTEDGE;
	SetWindowLong(bar->m_hWnd,GWL_EXSTYLE,dwExStyle);
	bar->SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	bar->SetParent( this ); 
	bar->EnableToolTips();

	//bar->MoveWindow(0, 0, 3000, h_toolbar);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(bar->m_hWnd, 0, 0, 3000, h_toolbar, TRUE);

	CRect rect;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);
	rect.bottom = rect.top + h_toolbar;
	//MoveWindow(rect);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(m_hWnd, rect, TRUE);

	bar->ShowWindow(SW_SHOW);

	return bar;
}


LRESULT CThingWnd::OnToolbarClicked(WPARAM wParam, LPARAM lParam)
{
	return 0;
}




CWnd* CThingWnd::AddSubWnd(E_WindowType type, bool bIsLoad)
{
	CRect wndRect;
	GetWindowRect(wndRect);
	CPoint point(x_margin, y_margin);
	CRect rect;

	CWnd * pWnd = NULL;
	bool bAutoWidth = true;
	switch (type)
	{
	// only person window now.
	case e_person:
		{
			pWnd = new CPersonWnd();
			rect.SetRect(0, 0, wndRect.Width()-2*x_margin, h_toolbar+2);
			((CPersonWnd*)pWnd)->Create(IDD_PERSON_WND, this);
		}
		break;

	default:
		return NULL;
		break;
	}
	pWnd->ShowWindow(SW_HIDE);
	rect += point;
	//pWnd->MoveWindow(&rect);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(pWnd->m_hWnd, &rect, TRUE);

	m_subWndHelper.AddSubWnd(pWnd, type, bAutoWidth);

	if (!bIsLoad)
	{
		g_ztui->m_pMsgHandler->OnInitPersonWnd((CPersonWnd*)pWnd);
	}


	Reposition();

	if (!bIsLoad)
	{
		// 设置焦点并滚动滚动条（如果已经超过窗口底部）
		pWnd->SetFocus();
		pWnd->GetWindowRect(wndRect);
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

	return pWnd;
}

void CThingWnd::RemoveSubWnd(CWnd* pWnd)
{
	if (!pWnd || !pWnd->m_hWnd)
	{
		return;
	}

	m_subWndHelper.RemoveSubWnd(pWnd);

	CPersonWnd* pSubWnd = dynamic_cast<CPersonWnd*>(pWnd);
	if (pSubWnd)
	{
        pSubWnd->RemoveAll();
		pSubWnd->EndDialog(IDOK);
		delete pSubWnd;
	}

	Reposition();
}

void CThingWnd::MoveSubWnd(const CWnd* pWnd, int step)
{
    if (pWnd)
    {
        m_subWndHelper.MoveSubWnd(pWnd, step);

        Reposition();
    }
}

void CThingWnd::MoveSubWndToTop(const CWnd* pWnd)
{
	if (pWnd)
	{
		m_subWndHelper.MoveSubWndToTop(pWnd);

		Reposition();
	}
}

void CThingWnd::Reposition()
{
	m_subWndHelper.RepositionY();

	UpdateWindowSize();
}

void CThingWnd::UpdateWindowSize()
{
	UINT h = m_subWndHelper.GetHight();
	CRect rect;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);
	rect.bottom = rect.top + max(h, h_toolbar+2);
	//MoveWindow(rect);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(m_hWnd, rect, TRUE);

}

LRESULT CThingWnd::OnSubWndSizeChanged(WPARAM wParam, LPARAM lParam)
{
	Reposition();
	return 0;
}

void CThingWnd::OnOK()
{
	// fix the bug: When enter the "Enter", the window disappear.
}

void CThingWnd::OnCancel()
{
	// fix the bug: When enter the "ESC", the window disappear.
}
void CThingWnd::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_cx != cx)
	{
		// change sub windows cx
		m_subWndHelper.RepositionX(cx - 2 * x_margin);

		m_cx = cx;
	}
	GetParent()->SendMessage(WM_SUB_WND_SIZE_CHANGED);
}

void CThingWnd::ExpandCollapse(bool bExpand)
{
	m_subWndHelper.Min(!bExpand);
	Reposition();
}

void CThingWnd::Load(ThingData& data)
{
	m_name = data.name;
	m_bShowToolbar = data.bShowToolbar;
	m_pToolbar->Load(data.toolbar);

	for (vector<PersonData*>::iterator it = data.persons.begin(); it != data.persons.end(); ++it)
	{
		CPersonWnd* pPersonWnd = (CPersonWnd*)AddSubWnd(e_person, true);
		pPersonWnd->Load(**it);
	}
	ExpandCollapse(m_pToolbar->GetButtonState(0) == e_BtnState1);
	g_ztui->m_pMsgHandler->OnLoadThingWnd(this, data);

	Reposition();
}


void CThingWnd::Save(ThingData & data)
{
	data.name = m_name;
	data.bShowToolbar = m_bShowToolbar;
	m_pToolbar->Save(data.toolbar);

	for (list<WndInfo>::iterator it = m_subWndHelper.m_subWndList.begin();
		it != m_subWndHelper.m_subWndList.end(); ++it)
	{
		ASSERT(e_person == it->eType);
		CPersonWnd* pPersonWnd = (CPersonWnd*)it->pWnd;
		PersonData* pPersonWndCfg = new PersonData();
		pPersonWnd->Save(*pPersonWndCfg);
		data.persons.push_back(pPersonWndCfg);
	}

	g_ztui->m_pMsgHandler->OnSaveThingWnd(this);
}

LRESULT CThingWnd::OnPersonActived(WPARAM wParam, LPARAM lParam)
{
	if (m_pActivePerson == (CWnd*)wParam)
	{
		return 0;
	}
    m_pActivePerson = (CWnd*)wParam;

	m_subWndHelper.Active(m_pActivePerson);

    return 0;
}

void CThingWnd::OnSetFocus(CWnd* pOldWnd)
{
    CDialog::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)this);
}

void CThingWnd::RemoveAll()
{
    while (m_subWndHelper.m_subWndList.size() > 0)
    {
        RemoveSubWnd(m_subWndHelper.m_subWndList.back().pWnd);
    }
}

HBRUSH CThingWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//if (nCtlColor != CTLCOLOR_EDIT && nCtlColor != CTLCOLOR_LISTBOX)
	if (nCtlColor == CTLCOLOR_DLG)
	{
		if (NULL == m_hBrush)
		{	
			// create brush first time we need it
			m_hBrush = ::CreateSolidBrush(m_backColor);
		}

		return m_hBrush;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

CPersonWnd* CThingWnd::FindPerson(CString name)
{
	return dynamic_cast<CPersonWnd*>(m_subWndHelper.FindByName(name));
}


