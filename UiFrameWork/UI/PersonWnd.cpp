#include "stdafx.h"
#include "UI.h"
#include "PersonWnd.h"

#include "ToolbarEh.h"

#include "ZTUI.h"

#include "RightCtrlWnd.h"

#define x_margin 4
#define y_margin 4
#define h_toolbar 26


IMPLEMENT_DYNAMIC(CPersonWnd, CDialog)

CPersonWnd::CPersonWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CPersonWnd::IDD, pParent)
{
	
	m_subWndHelper.m_yMargin = y_margin;
	m_subWndHelper.m_beginPoint.x = x_margin;
	m_subWndHelper.m_beginPoint.y = h_toolbar + y_margin;
	m_subWndHelper.m_pOwnerWnd = this;
	m_subWndHelper.MinHight(h_toolbar);

	m_bShowToolbar = TRUE;
    m_ctrlId = 0;
    m_hBrush = NULL;
    m_resizeMoveFlag = 0;
	m_cx = -1;
	m_pActiveCtrl = NULL;
	m_pToolbar = NULL;
	m_backColor = g_ztui->m_pMsgHandler->OnGetPersonWndBackColor();

}

CPersonWnd::~CPersonWnd()
{
	for (list<CWnd*>::iterator it = m_testCtrlList.begin(); it != m_testCtrlList.end(); ++it)
	{
		if (*it)
		{
			delete (*it);
		}
	}

    if (NULL != m_hBrush)
    {
        ::DeleteObject(m_hBrush);
    }


}

void CPersonWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPersonWnd, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_SUB_WND_SIZE_CHANGED, &OnSubWndSizeChanged)
	ON_MESSAGE(WM_TOOLBAR_MESSAGE, &OnToolbarClicked)
	ON_MESSAGE(WM_CONTROL_ACTIVED, &OnControlActived)
    ON_WM_SETFOCUS()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



BOOL CPersonWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_treeImageList.Create( IDB_BITMAP_TREE, 16, 4, 0xFF00FF );

	m_pToolbar = CreateToolbar();
	m_pToolbar->SetOwner(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CPersonWnd::OnNcHitTest(CPoint point)
{
    if (m_resizeMoveFlag)
    {
        #include "WndResize.h"
    }

	return CDialog::OnNcHitTest(point);
}

void CPersonWnd::OnOK()
{
	// fix the bug: When enter the "Enter", the window disappear.
}

void CPersonWnd::OnCancel()
{
	// fix the bug: When enter the "ESC", the window disappear.
}

void CPersonWnd::OnSize(UINT nType, int cx, int cy)
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

void CPersonWnd::Reposition()
{
    m_subWndHelper.RepositionY();

    UpdateWindowSize();
}
int CPersonWnd::GetItemCount()
{
	return m_subWndHelper.m_subWndList.size();
}

void CPersonWnd::ShowToolbar(bool bShow)
{
	if(m_pToolbar)
	{
		m_pToolbar->ShowWindow(bShow?SW_SHOW:SW_HIDE);
		m_subWndHelper.m_beginPoint.y = (bShow ? (h_toolbar + y_margin) : (y_margin));
		Reposition();
	}
}
void CPersonWnd::UpdateWindowSize()
{
    UINT h = m_subWndHelper.GetHight();
    CRect rect;
    GetWindowRect(rect);
    GetParent()->ScreenToClient(rect);
	rect.bottom = rect.top + max(h, h_toolbar+2);
	
    //MoveWindow(rect);
    //MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(m_hWnd, rect, TRUE);

}

#include "CustomWndContainer.h"
CRightCtrlWnd* CPersonWnd::CreateCustomWnd(CWnd *pWnd, CSize* pSize)
{
	CPoint point(x_margin, y_margin);
	

  CCustomWndContainer*	pCtrl = new CCustomWndContainer();

	pCtrl->Create(CCustomWndContainer::IDD, this);
	pCtrl->ModifyStyle(NULL,WS_CLIPCHILDREN);


	CRect rcCustm;
	pWnd->GetWindowRect(&rcCustm);
	
	pSize->cx = rcCustm.Width()+2*x_margin;
	pSize->cy = rcCustm.Height()+8;
	
	
	pCtrl->m_pInnerWnd = pWnd;
	pWnd->SetParent(pCtrl);
	pWnd ->ShowWindow(SW_SHOW);
	
	return dynamic_cast<CRightCtrlWnd*>(pCtrl);
}


CWnd* CPersonWnd::GetActivSubWnd()
{
	if(m_pActiveCtrl)
	{
		CCustomWndContainer *pContainer = dynamic_cast<CCustomWndContainer*>(m_pActiveCtrl);
		if(pContainer)
		{
			return pContainer->m_pInnerWnd;
		}
	}
	
	return NULL;
}
CWnd* CPersonWnd::AddSubWnd(CWnd *pWnd)
{
	CSize sz(0,0);
	CSize *pSize = &sz;

	bool bIsLoad = pSize ? true : false;
	CRect wndRect;
	GetWindowRect(wndRect);
    CPoint point(x_margin, y_margin);
	CRect rect;
	
    CRightCtrlWnd * pCtrl = CreateCustomWnd(pWnd, pSize);
	
	bool bAutoWidth = true;
 
	if (g_ztui->m_pMsgHandler->OnCtrlGetFont())
	{
		pCtrl->SetFont(g_ztui->m_pMsgHandler->OnCtrlGetFont());
	}

	pCtrl->ShowWindow(SW_HIDE);
	rect.right = pSize->cx+rect.left;
	rect.bottom = rect.top + pSize->cy;

	rect += point;
	
    MyMoveWindow(pCtrl->m_hWnd, &rect, TRUE);
	
    m_subWndHelper.AddSubWnd(pCtrl, e_custom, bAutoWidth);
	if(m_subWndHelper.m_subWndList.size()==1)
	{
		m_pActiveCtrl = pCtrl;
	}
    Reposition();
	
	CRect rcMy;
	GetClientRect(&rcMy);
	
	
	pCtrl->GetWindowRect(&rect);
	ScreenToClient(&rect);

	rect.left = x_margin;
	rect.right = rcMy.right - x_margin;
	pCtrl->MoveWindow(&rect);
	if (!bIsLoad) // is not call from load()
	{
		// 设置焦点并滚动滚动条（如果已经超过窗口底部）
		pCtrl->SetFocus();
		pCtrl->GetWindowRect(wndRect);
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

	return pCtrl;
}

void CPersonWnd::RemoveSubWnd(CWnd* pWnd)
{
    if (!pWnd || !pWnd->m_hWnd)
    {
        return;
    }
	
	CWnd *pParent = pWnd->GetParent();
	if(dynamic_cast<CRightCtrlWnd*>(pParent) != NULL)
	{
		pWnd = pParent;
	}

    m_subWndHelper.RemoveSubWnd(pWnd);
	
	
    if (pWnd)
    {
        pWnd->DestroyWindow();
        delete pWnd;
    }
	
	if(m_pActiveCtrl == pWnd)
	{
		m_pActiveCtrl = NULL;
	}
	

    Reposition();
}


void CPersonWnd::MoveSubWnd(const CWnd* pWnd, int step)
{
	if (pWnd)
	{
		m_subWndHelper.MoveSubWnd(pWnd, step);

		Reposition();
	}
}

void CPersonWnd::MoveSubWndToTop(const CWnd* pWnd)
{
	if (pWnd)
	{
		m_subWndHelper.MoveSubWndToTop(pWnd);

		Reposition();
	}
}

LRESULT CPersonWnd::OnSubWndSizeChanged(WPARAM wParam, LPARAM lParam)
{
    Reposition();
    return 0;
}

LRESULT CPersonWnd::OnToolbarClicked(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CPersonWnd::OnControlActived(WPARAM wParam, LPARAM lParam)
{
	if (m_pActiveCtrl == (CWnd*)wParam)
	{
		return 0;
	}

	m_pActiveCtrl = (CWnd*)wParam;

	m_subWndHelper.Active(m_pActiveCtrl);

	return 0;
}

HBRUSH CPersonWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    //if (nCtlColor != CTLCOLOR_EDIT && nCtlColor != CTLCOLOR_LISTBOX)
    if (nCtlColor == CTLCOLOR_DLG)
    {
        if (NULL == m_hBrush)
        {	
            // create brush first time we need it
			m_hBrush = ::CreateSolidBrush(m_pToolbar->IsActive() ? RGB(170,231,170) : RGB(231,231,246));
        }
        return m_hBrush;
    }

    // TODO:  Return a different brush if the default is not desired
    return hbr;
}


CToolbarEh* CPersonWnd::CreateToolbar()
{
	CCreateContext context;     
	context.m_pNewViewClass = RUNTIME_CLASS( CToolbarEh ) ;     
	CFrameWnd* pfWnd = (CFrameWnd*)this;      
	CToolbarEh* bar = (CToolbarEh*)pfWnd->CreateView( &context );
	bar->m_bkgndColor = g_ztui->m_pMsgHandler->OnGetPersonWndToolbarBackColor();
	bar->m_highlightBkgndColor = g_ztui->m_pMsgHandler->OnGetPersonWndToolbarHighlightColor();
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

void CPersonWnd::Load(PersonData& data)
{
	m_name = data.name;
	m_ctrlId = data.idPerson;

	m_bShowToolbar = data.bShowToolbar;
	m_pToolbar->Load(data.toolbar);


	ExpandCollapse(m_pToolbar->GetButtonState(0) == e_BtnState1);
	g_ztui->m_pMsgHandler->OnLoadPersonWnd(this, data);
	
	ShowToolbar(m_bShowToolbar);
}

void CPersonWnd::Save(PersonData& data)
{
	data.name = m_name;
	data.idPerson = m_ctrlId;
	data.bShowToolbar = m_bShowToolbar;
	m_pToolbar->Save(data.toolbar);

	for (list<WndInfo>::iterator it = m_subWndHelper.m_subWndList.begin();
		it != m_subWndHelper.m_subWndList.end(); ++it)
	{
		CWnd* pWnd = it->pWnd;
		ASSERT(pWnd);
		CRightCtrlWnd* pRightCtrlWnd = dynamic_cast<CRightCtrlWnd*>(pWnd);
		ASSERT(pRightCtrlWnd);
		CtrlData* pCtrlData = new CtrlData();
		pCtrlData->name = pRightCtrlWnd->m_name;
		pCtrlData->type = it->eType;
		pCtrlData->customType = it->customType;
		CRect rect;
		pWnd->GetWindowRect(rect);
		pCtrlData->size = rect.Size();

		CAbsCtrl* pCtrl = dynamic_cast<CAbsCtrl*> (pWnd);
		ASSERT(pCtrl);
		pCtrlData->data = pCtrl->Save();

		data.ctrls.push_back(pCtrlData);
	}

	g_ztui->m_pMsgHandler->OnSavePersonWnd(this);
}

void CPersonWnd::LoadDataToCtrl(CWnd* pCtrlWnd, AbsCtrlData* pData)
{
	if (!pCtrlWnd || !pData)
	{
		return;
	}

	CAbsCtrl* pCtrl = dynamic_cast<CAbsCtrl*>(pCtrlWnd);
	ASSERT(pCtrl);
	pCtrl->Load(pData);
}

void CPersonWnd::ExpandCollapse(bool bExpand)
{
	m_subWndHelper.Min(!bExpand);
	Reposition();
}

void CPersonWnd::OnSetFocus(CWnd* pOldWnd)
{
    CDialog::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    GetParent()->SendMessage(WM_PERSON_ACTIVE, (WPARAM)this);
    GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent());

}

void CPersonWnd::RemoveAll()
{
    while (m_subWndHelper.m_subWndList.size() > 0)
    {
        RemoveSubWnd(m_subWndHelper.m_subWndList.back().pWnd);
    }
}




void CPersonWnd::OnDestroy()
{

	for (list<CWnd*>::iterator it = m_testCtrlList.begin(); it != m_testCtrlList.end(); ++it)
	{
		if (*it)
		{
			(*it)->DestroyWindow();
			delete (*it);
		}
	}

	CDialog::OnDestroy();

	
}
