// ToolbarWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UI.h"
#include "ToolbarEh.h"
#include "StaticEh.h"
#include "BitmapButtonEh.h"
#include "ZTUI.h"
#include "PersonWnd.h"
#include "ThingWnd.h"

#define H   21
#define W   21
#define GAP	8
#define y_margin 2


IMPLEMENT_DYNCREATE(CToolbarEh, ZTCView)

CToolbarEh::CToolbarEh()
{
	m_bTabMode = false;
    m_bInited = false;
    m_bEnableToolTips = false;
    m_xMargin = 0;
	m_nMaxIndex = 0;
    m_nMenuID = 10000;
    m_pOwner = NULL;

	m_bkgndColor = RGB(216,216,216);
	m_highlightBkgndColor = RGB(116,116,116);
	m_transparentMaskColor = RGB(255,255,255);
	m_textColor = RGB(82,125,214);
	m_borderColor = RGB(230,230,230);

	m_active = false;

    m_strImageFolder = g_ztui->m_pMsgHandler->OnGetSmallPictureDir();

}

CToolbarEh::~CToolbarEh()
{
    DeleteAll(false);
}

BEGIN_MESSAGE_MAP(CToolbarEh, ZTCView)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_RBUTTONUP()
    ON_MESSAGE(WM_STATIC_REPOSITION, OnReposition)
    ON_MESSAGE(WM_CONTEXT_MENU, OnContextMenu)
	ON_WM_ERASEBKGND()
    ON_WM_LBUTTONDOWN()
    ON_WM_MBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CToolbarEh drawing

void CToolbarEh::OnDraw(CDC* pDC)
{
    //	CDocument* pDoc = GetDocument();
        // TODO: add draw code here
}


// CToolbarEh diagnostics

#ifdef _DEBUG
void CToolbarEh::AssertValid() const
{
    ZTCView::AssertValid();
}

#ifndef _WIN32_WCE
void CToolbarEh::Dump(CDumpContext& dc) const
{
    ZTCView::Dump(dc);
}
#endif
#endif //_DEBUG


// CToolbarEh message handlers

int CToolbarEh::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (ZTCView::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }
	
	CFont *pFont = g_ztui->m_pMsgHandler->OnToolbarGetFont();
	
	LOGFONT lg;

	pFont->GetLogFont(&lg);

	m_ftNormal.CreateFontIndirect(&lg);

	lg.lfWeight = 700;

	m_ftBold.CreateFontIndirect(&lg);
    return 0;
}

void CToolbarEh::OnInitialUpdate()
{
    if (m_bInited)
        return;
    m_bInited = true;

	// Remove edge.

	DWORD dwExStyle = GetExStyle()& ~WS_EX_CLIENTEDGE;
	SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle);
	SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	
	SetBitMapList(m_strImageFolder);

	AddFocusBtn();

    ZTCView::OnInitialUpdate();
}

void CToolbarEh::OnSize(UINT nType, int cx, int cy)
{
    ZTCView::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here

}


BOOL CToolbarEh::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (NULL != m_wndToolTip.GetSafeHwnd())
    {
        m_wndToolTip.RelayEvent(pMsg);
    }

    return ZTCView::PreTranslateMessage(pMsg);
}


void CToolbarEh::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    ClientToScreen(&point);

    //CreatePopupMenu(point.x, point.y);

    ZTCView::OnRButtonUp(nFlags, point);
}

LRESULT CToolbarEh::OnReposition(WPARAM wParam, LPARAM lParam)
{
	for (vector<ToolbarOption*>::iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
	{
		if ((*it)->id == wParam)
		{
			ASSERT((*it)->show == true && (*it)->type == e_TextBtn);
			CStaticEh* sta = (CStaticEh*)GetDlgItem((*it)->id);
			ASSERT(sta);
			sta->GetWindowText((*it)->text);
			if (m_bEnableToolTips)
			{
				m_wndToolTip.UpdateTipText((*it)->text, sta);
			}
		}
	}

	Reposition();

    return 0;
}

void CToolbarEh::AddText(LPCTSTR lpszText, UINT nID, bool bShow, bool bButton)
{
    CStaticEh* sta = NULL;
    SIZE size;
    sta = new CStaticEh;
	sta->m_bkgndColor = m_bkgndColor;
	sta->m_highlightBkgndColor = m_highlightBkgndColor;
	sta->m_textColor = m_textColor;
	sta->SetChangeByDblClk(bButton);
    sta->Create(lpszText, WS_CHILD | WS_VISIBLE | SS_NOTIFY | SS_CENTERIMAGE | SS_CENTER, CRect(0, 0, 0, 0), this, nID);
	CFont *ftTmp = g_ztui->m_pMsgHandler->OnToolbarGetFont();
	if (ftTmp)
	{
		sta->SetFont(ftTmp);
	}
	
	
	sta->m_textColor = g_ztui->m_pMsgHandler->OnToolbarTextColor();

	GetTextExtentPoint32(GetDC()->GetSafeHdc(), lpszText, CString(lpszText).GetLength(), &size);
	size.cx = (int)(size.cx*1.15);
	//sta->MoveWindow(m_xMargin + GAP, y_margin, size.cx, H);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(sta->m_hWnd, m_xMargin + GAP, y_margin, size.cx, H, TRUE);
    m_xMargin += GAP + size.cx;

    if (m_bEnableToolTips)
    {
        m_wndToolTip.AddTool(sta, lpszText);
    }

	ToolbarOption* tb = new ToolbarOption;
    memset(tb, 0, sizeof(tb));
	tb->type = e_TextBtn;
    tb->show = bShow;
    tb->id = nID;
    tb->pWnd = sta;
    tb->text = lpszText;
    m_toolbarList.push_back(tb);

	if (!bShow)
	{
		ShowHideItem(tb, bShow);
	}
}
 
void CToolbarEh::AddButton(LPCTSTR lpszText, UINT nID, LPCTSTR lpszBitmapFile, bool bShow)
{
	CBitmapButtonEh* btn = NULL;
	btn = new CBitmapButtonEh;
	btn->m_bkgndColor = m_bkgndColor;
	btn->m_highlightBkgndColor = m_highlightBkgndColor;
	btn->m_transparentMaskColor = m_transparentMaskColor;
	if (!btn->LoadBitmapsFromFiles(m_strImageFolder+lpszBitmapFile))
	{
		BOOL ret = btn->LoadBitmapsFromResource(IDB_NOPICTURE);
		
		ASSERT(ret);
	}
	
	btn->Create(lpszText, WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_OWNERDRAW, CRect(0, 0, 0, 0), this, nID);

	//btn->MoveWindow(m_xMargin + GAP, y_margin, W, H);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(btn->m_hWnd, m_xMargin + GAP, y_margin, W, H, TRUE);
	m_xMargin += GAP + W;

	if (m_bEnableToolTips)
	{
		m_wndToolTip.AddTool(btn, lpszText);
	}

	ToolbarOption* tb = new ToolbarOption;
	memset(tb, 0, sizeof(tb));
	tb->type = e_ImageBtn;
	tb->show = bShow;
	tb->image1 = lpszBitmapFile;
	tb->btnState = e_BtnState1;
	tb->id = nID;
	tb->pWnd = btn;
	tb->text = lpszText;
	m_toolbarList.push_back(tb);

	if (!bShow)
	{
		ShowHideItem(tb, bShow);
	}
}

void CToolbarEh::AddTwoStateButton(LPCTSTR lpszText, UINT nID, LPCTSTR lpszBitmapFile1, LPCTSTR lpszBitmapFile2, E_ButtonState nImg, bool bShow)
{
	CBitmapButtonEh* btn = NULL;
	btn = new CBitmapButtonEh;
	btn->m_bkgndColor = m_bkgndColor;
	btn->m_highlightBkgndColor = m_highlightBkgndColor;
	btn->m_transparentMaskColor = m_transparentMaskColor;
	CString strBitmapFile = (nImg == e_BtnState1) ? lpszBitmapFile1 : lpszBitmapFile2;
	if (!btn->LoadBitmapsFromFiles(m_strImageFolder+strBitmapFile))
	{
		BOOL ret = btn->LoadBitmapsFromResource(IDB_NOPICTURE);
		ASSERT(ret);
	}
	btn->Create(lpszText, WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_OWNERDRAW, CRect(0, 0, 0, 0), this, nID);
	//btn->MoveWindow(m_xMargin + GAP, y_margin, W, H);
    MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
    MyMoveWindow(btn->m_hWnd, m_xMargin + GAP, y_margin, W, H, TRUE);
	m_xMargin += GAP + W;

	if (m_bEnableToolTips)
	{
		m_wndToolTip.AddTool(btn, lpszText);
	}

	ToolbarOption* tb = new ToolbarOption;
	memset(tb, 0, sizeof(tb));
	tb->type = e_TwoStatesImageBtn;
	tb->show = bShow;
	tb->image1 = lpszBitmapFile1;
	tb->image2 = lpszBitmapFile2;
	tb->btnState = nImg;
	tb->id = nID;
	tb->pWnd = btn;
	tb->text = lpszText;
	m_toolbarList.push_back(tb);

	if (!bShow)
	{
		ShowHideItem(tb, bShow);
	}
}

void CToolbarEh::EnableToolTips()
{
    m_bEnableToolTips = true;
    ZTCView::EnableToolTips(TRUE);
    m_wndToolTip.Create(this);
    m_wndToolTip.Activate(TRUE);
}

void CToolbarEh::CreatePopupMenu(int x, int y)
{
	m_menu.DestroyMenu();
    m_menu.CreatePopupMenu();

    for (vector<ToolbarOption*>::iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
    {
        if ((*it)->type == e_TextBtn)
        {
            UINT nFlags;
            if ((*it)->show)
            {
                nFlags = MF_STRING | MF_POPUP | MF_CHECKED;
            }
            else
            {
                nFlags = MF_STRING | MF_POPUP | MF_UNCHECKED;
            }
            CMenu subMenu;
            subMenu.CreateMenu();
            subMenu.AppendMenu(MF_STRING, m_nMenuID, _T("显示/隐藏"));
            (*it)->nIDSelect = m_nMenuID++;
            subMenu.AppendMenu(MF_STRING, m_nMenuID, _T("重命名"));
            (*it)->nIDRename = m_nMenuID++;
            m_menu.AppendMenu(nFlags, (UINT_PTR)subMenu.m_hMenu, (*it)->text);
            subMenu.Detach();
        }
        else
        {
            UINT nFlags;
            if ((*it)->show)
            {
                nFlags = MF_STRING | MF_POPUP | MF_CHECKED;
            }
            else
            {
                nFlags = MF_STRING | MF_POPUP | MF_UNCHECKED;
            }

			CMenu subMenu;
			subMenu.CreateMenu();
			subMenu.AppendMenu(MF_STRING, m_nMenuID, _T("显示/隐藏"));
			(*it)->nIDSelect = m_nMenuID++;

			CMenu subSubMenu;
			subSubMenu.CreateMenu();
			if ((*it)->bmpList1.empty())
			{
				for (list<CString>::iterator bmpIt = m_bmpFileList.begin(); bmpIt != m_bmpFileList.end(); ++bmpIt)
				{
					CImage img;
					img.Load(m_strImageFolder + *bmpIt);
					CBitmap* bmp = new CBitmap;
					bmp->Attach(img.Detach());
                    if ((*it)->image1 == (*bmpIt))
                    {
                        subSubMenu.AppendMenu(MF_BITMAP | MF_CHECKED, m_nMenuID, bmp);
                    }
                    else
                    {
                        subSubMenu.AppendMenu(MF_BITMAP | MF_UNCHECKED, m_nMenuID, bmp);
                    }
					ToolbarOption::BmpMenu* bmpMenu = new ToolbarOption::BmpMenu;
					bmpMenu->nIDBmp = m_nMenuID++;
					bmpMenu->pBmp = bmp;
					bmpMenu->strImgFile = *bmpIt;
					(*it)->bmpList1.push_back(bmpMenu);
				}
			}
			else
			{
				for (list<ToolbarOption::BmpMenu*>::iterator bmpIt = (*it)->bmpList1.begin(); bmpIt != (*it)->bmpList1.end(); ++bmpIt)
				{
					if ((*bmpIt)->pBmp)
					{
                        if ((*it)->image1 == (*bmpIt)->strImgFile)
                        {
                            subSubMenu.AppendMenu(MF_BITMAP | MF_CHECKED, (*bmpIt)->nIDBmp, (*bmpIt)->pBmp);
                        }
                        else
                        {
                            subSubMenu.AppendMenu(MF_BITMAP | MF_UNCHECKED, (*bmpIt)->nIDBmp, (*bmpIt)->pBmp);
                        }
					}
				}
			}

			subMenu.AppendMenu(MF_STRING | MF_POPUP, (UINT_PTR)subSubMenu.m_hMenu, _T("备选图标"));
			subSubMenu.Detach();

			if ((*it)->image2.IsEmpty() == false)
			{
				subSubMenu.CreateMenu();
				if ((*it)->bmpList2.empty())
				{
					for (list<CString>::iterator bmpIt = m_bmpFileList.begin(); bmpIt != m_bmpFileList.end(); ++bmpIt)
					{
						CImage img;
						img.Load(m_strImageFolder + *bmpIt);
						CBitmap* bmp = new CBitmap;
						bmp->Attach(img.Detach());
                        if ((*it)->image2 == (*bmpIt))
                        {
                            subSubMenu.AppendMenu(MF_BITMAP | MF_CHECKED, m_nMenuID, bmp);
                        }
                        else
                        {
                            subSubMenu.AppendMenu(MF_BITMAP | MF_UNCHECKED, m_nMenuID, bmp);
                        }
						ToolbarOption::BmpMenu* bmpMenu = new ToolbarOption::BmpMenu;
						bmpMenu->nIDBmp = m_nMenuID++;
						bmpMenu->pBmp = bmp;
						bmpMenu->strImgFile = *bmpIt;
						(*it)->bmpList2.push_back(bmpMenu);
					}
				}
				else
				{
					for (list<ToolbarOption::BmpMenu*>::iterator bmpIt = (*it)->bmpList2.begin(); bmpIt != (*it)->bmpList2.end(); ++bmpIt)
					{
						if ((*bmpIt)->pBmp)
						{
                            if ((*it)->image2 == (*bmpIt)->strImgFile)
                            {
                                subSubMenu.AppendMenu(MF_BITMAP | MF_CHECKED, (*bmpIt)->nIDBmp, (*bmpIt)->pBmp);
                            }
                            else
                            {
                                subSubMenu.AppendMenu(MF_BITMAP | MF_UNCHECKED, (*bmpIt)->nIDBmp, (*bmpIt)->pBmp);
                            }
						}
					}
				}

				subMenu.AppendMenu(MF_STRING | MF_POPUP, (UINT_PTR)subSubMenu.m_hMenu, _T("备选图标"));
				subSubMenu.Detach();
			}

            m_menu.AppendMenu(nFlags, (UINT_PTR)subMenu.m_hMenu, (*it)->text);
			subMenu.Detach();
        }
    }
	m_menu.TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
}


BOOL CToolbarEh::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class
    int nCode = HIWORD(wParam);;
    UINT nID = LOWORD(wParam);
    if (CN_COMMAND == nCode)
    {
		
		if(m_bTabMode)
		{
			for (vector<ToolbarOption*>::iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
			{
				if ((*it)->type == e_TextBtn)
				{
					GetDlgItem((*it)->id)->SetFont(nID == (*it)->id ? &m_ftBold : &m_ftNormal);
				}
			}
		}
        for (vector<ToolbarOption*>::iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
        {
			
		
            if (nID == (*it)->id)
            {
				if ((*it)->type != e_TextBtn && (*it)->image2.IsEmpty() == false)
				{
					if (e_BtnState1 == (*it)->btnState)
					{
						if (!((CBitmapButtonEh*)GetDlgItem((*it)->id))->LoadBitmapsFromFiles(m_strImageFolder+(*it)->image2))
						{
							((CBitmapButtonEh*)GetDlgItem((*it)->id))->LoadBitmapsFromResource(IDB_NOPICTURE);
						}
						(*it)->btnState = e_BtnState2;
					}
					else
					{
						if (!((CBitmapButtonEh*)GetDlgItem((*it)->id))->LoadBitmapsFromFiles(m_strImageFolder+(*it)->image1))
						{
							((CBitmapButtonEh*)GetDlgItem((*it)->id))->LoadBitmapsFromResource(IDB_NOPICTURE);
						}
						(*it)->btnState = e_BtnState1;
					}
					//Invalidate();
				}
				
				g_ztui->m_pMsgHandler->OnToolbarClick(this, nID);

                if (m_pOwner && m_pOwner->m_hWnd)
                {
                    m_pOwner->SendMessage(WM_TOOLBAR_MESSAGE, nID);
                }
                else if (GetParent() && GetParent()->m_hWnd)
                {
                    GetParent()->SendMessage(WM_TOOLBAR_MESSAGE, nID);
                }
			
				g_ztui->m_pRightWnd->Refresh();

                break;
            }
            else if (nID == (*it)->nIDRename)
            {
                GetDlgItem((*it)->id)->SendMessage(WM_STATIC_RENAME, (*it)->id);
                break;
            }
            else if (nID == (*it)->nIDSelect)
            {
				(*it)->show = !(*it)->show;
				ShowHideItem(*it, (*it)->show);
				break;
            }
			else
			{
				bool bFind = false;
				for (list<ToolbarOption::BmpMenu*>::iterator bmpIt = (*it)->bmpList1.begin(); bmpIt != (*it)->bmpList1.end(); ++bmpIt)
				{
					if (nID == (*bmpIt)->nIDBmp)
					{
						bFind = true;

						if (e_BtnState1 == (*it)->btnState)
						{
							if (!((CBitmapButtonEh*)GetDlgItem((*it)->id))->LoadBitmapsFromFiles(m_strImageFolder+(*bmpIt)->strImgFile))
							{
								((CBitmapButtonEh*)GetDlgItem((*it)->id))->LoadBitmapsFromResource(IDB_NOPICTURE);
							}
							Invalidate();
						}
						(*it)->image1 = (*bmpIt)->strImgFile;
						break;
					}
				}
				if (bFind)
				{
					break;
				}
				else
				{
					for (list<ToolbarOption::BmpMenu*>::iterator bmpIt = (*it)->bmpList2.begin(); bmpIt != (*it)->bmpList2.end(); ++bmpIt)
					{
						if (nID == (*bmpIt)->nIDBmp)
						{
							bFind = true;

							if (e_BtnState2 == (*it)->btnState)
							{
								if (!((CBitmapButtonEh*)GetDlgItem((*it)->id))->LoadBitmapsFromFiles(m_strImageFolder+(*bmpIt)->strImgFile))
								{
									((CBitmapButtonEh*)GetDlgItem((*it)->id))->LoadBitmapsFromResource(IDB_NOPICTURE);
								}
								Invalidate();
							}
							(*it)->image2 = (*bmpIt)->strImgFile;
							break;
						}
					}
				}
				if (bFind)
				{
					break;
				}
			}
        }

        if (GetParent()->IsKindOf(RUNTIME_CLASS(CPersonWnd)))
        {
            GetParent()->GetParent()->SendMessage(WM_PERSON_ACTIVE, (WPARAM)GetParent());
            GetParent()->GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent()->GetParent());
        }
        else if (GetParent()->IsKindOf(RUNTIME_CLASS(CThingWnd)))
        {
            GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent());
        }
    }

    return ZTCView::OnCommand(wParam, lParam);
}

void CToolbarEh::ShowHideItem(ToolbarOption* tb, bool bShow)
{
	if (bShow && !tb->pWnd)
	{
		switch (tb->type)
		{
		case e_TextBtn:
			tb->pWnd = new CStaticEh;
			((CStaticEh*)tb->pWnd)->m_bkgndColor = m_bkgndColor;
			((CStaticEh*)tb->pWnd)->m_textColor = m_textColor;
			break;
		case e_ImageBtn:
		case e_TwoStatesImageBtn:
			tb->pWnd = new CBitmapButtonEh;
			break;
		default:
			ASSERT(FALSE);
			break;
		}
	}
	tb->pWnd->ShowWindow(bShow ? SW_SHOW : SW_HIDE);

	Reposition();
}

void CToolbarEh::SetBitMapList(CString strImageFolder)
{
	m_strImageFolder = strImageFolder;
	// load all bitmap
	m_bmpFileList.clear();
	WIN32_FIND_DATA findData;
	BOOL bRet = FALSE;
	HANDLE hFind = FindFirstFile(m_strImageFolder + _T("*.bmp"), &findData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			m_bmpFileList.push_back(findData.cFileName);

			bRet = FindNextFile(hFind, &findData);
		} while (bRet);
		FindClose(hFind);
	}
	hFind = FindFirstFile(m_strImageFolder + _T("*.png"), &findData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			m_bmpFileList.push_back(findData.cFileName);

			bRet = FindNextFile(hFind, &findData);
		} while (bRet);
		FindClose(hFind);
	}
}

BOOL CToolbarEh::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	CBrush brush;
	if (m_active)
	{
		brush.CreateSolidBrush(m_highlightBkgndColor);
	}
	else
	{
		brush.CreateSolidBrush(m_bkgndColor);
	}
	pDC->GetClipBox(rect);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	DrawBorder(pDC);

	return TRUE;

// 	return ZTCView::OnEraseBkgnd(pDC);
}

void CToolbarEh::Load(IN ToolbarData & cfg)
{
	// TODO:other parameters

	for (vector<ToolbarBtnData>::iterator it = cfg.buttons.begin(); it != cfg.buttons.end(); ++it)
	{
		switch (it->type)
		{
		case e_TextBtn:
			AddText(it->text, it->id, it->show,true);
			break;
		case e_ImageBtn:
			AddButton(it->text, it->id, it->image1, it->show);
			break;
		case e_TwoStatesImageBtn:
			AddTwoStateButton(it->text, it->id, it->image1, it->image2, it->btnState, it->show);
			break;
		case e_TextStatic:
			AddText(it->text, it->id, it->show,false);
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	Active(m_active);
}


void CToolbarEh::Save(OUT ToolbarData & cfg)
{
	// TODO:other parameters

	for (vector<ToolbarOption*>::iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
	{
		cfg.buttons.push_back(ToolbarBtnData((*it)->id, (*it)->text, (*it)->type, (*it)->image1, (*it)->image2, (*it)->btnState, (*it)->show));
	}
}

void CToolbarEh::Reposition()
{
	m_xMargin = 0;
	for (vector<ToolbarOption*>::iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
	{
		if ((*it)->show)
		{
			m_xMargin += GAP;
			switch ((*it)->type)
			{
			case e_TextBtn:
			{
				SIZE size;
				GetTextExtentPoint32(GetDC()->GetSafeHdc(), (*it)->text, (*it)->text.GetLength(), &size);
				size.cx = (int)(size.cx*1.15);
				//(*it)->pWnd->MoveWindow(m_xMargin, y_margin, size.cx, H);
                MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
                MyMoveWindow((*it)->pWnd->m_hWnd, m_xMargin, y_margin, size.cx, H, TRUE);
				m_xMargin += size.cx;
			}
			break;
			case e_ImageBtn:
			case e_TwoStatesImageBtn:
			{
				//(*it)->pWnd->MoveWindow(m_xMargin, y_margin, W, H);
                MyOutputDebugStringA("--- MoveWindow in  %s", __FUNCTION__);
                MyMoveWindow((*it)->pWnd->m_hWnd, m_xMargin, y_margin, W, H, TRUE);
				m_xMargin += W;
			}
			break;
			default:
			ASSERT(FALSE);
			break;
			}
		}
	}

	RedrawWindow();
}

void CToolbarEh::OnSetFocus(CWnd* pOldWnd)
{
    ZTCView::OnSetFocus(pOldWnd);

    // TODO: Add your message handler code here
    if (GetParent()->IsKindOf(RUNTIME_CLASS(CPersonWnd)))
    {
        GetParent()->GetParent()->SendMessage(WM_PERSON_ACTIVE, (WPARAM)GetParent());
        GetParent()->GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent()->GetParent());
    }
    else if (GetParent()->IsKindOf(RUNTIME_CLASS(CThingWnd)))
    {
        GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent());
    }
}

void CToolbarEh::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_focusBtn.SetFocus();

	if (GetParent()->IsKindOf(RUNTIME_CLASS(CPersonWnd)))
    {
        GetParent()->GetParent()->SendMessage(WM_PERSON_ACTIVE, (WPARAM)GetParent());
        GetParent()->GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent()->GetParent());
    }
    else if (GetParent()->IsKindOf(RUNTIME_CLASS(CThingWnd)))
    {
        GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent());
    }

    ZTCView::OnLButtonDown(nFlags, point);
}

void CToolbarEh::OnRButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here
    if (GetParent()->IsKindOf(RUNTIME_CLASS(CPersonWnd)))
    {
        GetParent()->GetParent()->SendMessage(WM_PERSON_ACTIVE, (WPARAM)GetParent());
        GetParent()->GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent()->GetParent());
    }
    else if (GetParent()->IsKindOf(RUNTIME_CLASS(CThingWnd)))
    {
        GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent());
    }

    ZTCView::OnRButtonDown(nFlags, point);
}

void CToolbarEh::OnMButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here
    if (GetParent()->IsKindOf(RUNTIME_CLASS(CPersonWnd)))
    {
        GetParent()->GetParent()->SendMessage(WM_PERSON_ACTIVE, (WPARAM)GetParent());
        GetParent()->GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent()->GetParent());
    }
    else if (GetParent()->IsKindOf(RUNTIME_CLASS(CThingWnd)))
    {
        GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent());
    }

    ZTCView::OnMButtonDown(nFlags, point);
}

void CToolbarEh::DeleteAll(bool bDestroy)
{
    for (vector<ToolbarOption*>::iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
    {
        if (*it)
        {
            if (bDestroy)
            {
                (*it)->pWnd->DestroyWindow();
            }
            if ((*it)->pWnd)
            {
                delete (*it)->pWnd;
            }
            if ((*it)->bmpList1.empty() == false)
            {
                for (list<ToolbarOption::BmpMenu*>::iterator bmpIt = (*it)->bmpList1.begin(); bmpIt != (*it)->bmpList1.end(); ++bmpIt)
                {
                    if ((*bmpIt)->pBmp)
                    {
                        delete (*bmpIt)->pBmp;
                    }
                    delete *bmpIt;
                }
            }
            if ((*it)->bmpList2.empty() == false)
            {
                for (list<ToolbarOption::BmpMenu*>::iterator bmpIt = (*it)->bmpList2.begin(); bmpIt != (*it)->bmpList2.end(); ++bmpIt)
                {
                    if ((*bmpIt)->pBmp)
                    {
                        delete (*bmpIt)->pBmp;
                    }
                    delete *bmpIt;
                }
            }
            delete *it;
        }
    }

    m_toolbarList.clear();
    m_xMargin = 0;
}


void CToolbarEh::RemoveAll()
{
    DeleteAll(true);
}

LRESULT CToolbarEh::OnContextMenu(WPARAM wParam, LPARAM lParam)
{
    CreatePopupMenu(wParam, lParam);
    return 0;
}

E_ButtonState CToolbarEh::GetButtonState(int pos)
{
	if ( 0 <= pos && pos < m_toolbarList.size())
	{
		ToolbarOption* option = m_toolbarList[pos];
		if (option->type == e_TwoStatesImageBtn)
		{
			return option->btnState;
		}
	}

	return e_BtnStateError;
}

void CToolbarEh::SetButtonState(int pos, E_ButtonState state)
{
	if ( 0 <= pos && pos < m_toolbarList.size())
	{
		ToolbarOption* option = m_toolbarList[pos];
		if (option->type == e_TwoStatesImageBtn)
		{
			if (e_BtnState1 == state)
			{
				if (!((CBitmapButtonEh*)GetDlgItem(option->id))->LoadBitmapsFromFiles(m_strImageFolder+option->image1))
				{
					((CBitmapButtonEh*)GetDlgItem(option->id))->LoadBitmapsFromResource(IDB_NOPICTURE);
				}
				option->btnState = e_BtnState1;
			}
			else if (e_BtnState2 == state)
			{
				if (!((CBitmapButtonEh*)GetDlgItem(option->id))->LoadBitmapsFromFiles(m_strImageFolder+option->image2))
				{
					((CBitmapButtonEh*)GetDlgItem(option->id))->LoadBitmapsFromResource(IDB_NOPICTURE);
				}
				option->btnState = e_BtnState2;
			}
		}
	}
}


CString CToolbarEh::GetButtonText(int pos)
{
	if ( 0 <= pos && pos < m_toolbarList.size())
	{
		ToolbarOption* option = m_toolbarList[pos];
		if (option->type == e_TextBtn)
		{
			return option->text;
		}
	}

	return _T("");
}

void CToolbarEh::SetButtonText(int pos, LPCTSTR text)
{
	if ( 0 <= pos && pos < m_toolbarList.size())
	{
		ToolbarOption* option = m_toolbarList[pos];
		if (option->type == e_TextBtn)
		{
			option->text = text;
			GetDlgItem(option->id)->SetWindowText(text);
			Reposition();
		}
	}
}

void CToolbarEh::Active(bool bActive)
{
	m_active = bActive;
	for (vector<ToolbarOption*>::iterator it = m_toolbarList.begin(); it != m_toolbarList.end(); ++it)
	{
		if ((*it)->type == e_TextBtn)
		{
			CStaticEh* pStatic = dynamic_cast<CStaticEh*>((*it)->pWnd);
			pStatic->m_bActive = m_active;

		}
		else
		{
			CBitmapButtonEh* pButton = dynamic_cast<CBitmapButtonEh*>((*it)->pWnd);
			pButton->Highlight(bActive);
		}
	}
	RedrawWindow();
}

void CToolbarEh::AddFocusBtn()
{
	if (!IsWindow(m_focusBtn.m_hWnd))
	{
		CRect rect(-1, -1, -1, -1);
		m_focusBtn.Create(_T("Button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, 6000);
	}
}

void CToolbarEh::DrawBorder(CDC *pDC)
{
	if (m_hWnd )
	{
		CBrush Brush(m_borderColor);
		CBrush* pOldBrush = pDC->SelectObject(&Brush);

		CRect rtWnd;
		GetWindowRect(&rtWnd);

		CPoint point;
		point.x = rtWnd.Width();
		point.y = 1;
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		point.x = 1;
		point.y = rtWnd.Height();
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		point.x = rtWnd.Width();
		point.y = 1;
		pDC->PatBlt(0, rtWnd.Height()-point.y, point.x, point.y, PATCOPY);
		point.x = 1;
		point.y = rtWnd.Height();
		pDC->PatBlt(rtWnd.Width()-point.x, 0, point.x, point.y, PATCOPY);
	}
}