// RightCtrlWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "UI.h"
#include "RightCtrlWnd.h"
#include "ZTUI.h"

IMPLEMENT_DYNAMIC(CRightCtrlWnd, CDialog)

CRightCtrlWnd::CRightCtrlWnd(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_hBrush = NULL;
    m_backColor = g_ztui->m_pMsgHandler->OnGetCtrlBackColor();
	m_borderColor = g_ztui->m_pMsgHandler->OnGetCtrlBorderColor();

	m_active = false;
}

CRightCtrlWnd::~CRightCtrlWnd()
{
	if (NULL != m_hBrush)
	{
		::DeleteObject(m_hBrush);
	}
}

void CRightCtrlWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRightCtrlWnd, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(MSG_CHILD_ITEM_ACTIVE,&CRightCtrlWnd::OnChildItemActivate)
END_MESSAGE_MAP()


BOOL CRightCtrlWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect(-1, -1, -1, -1);
	m_focusBtn.Create(_T("Button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rect, this, 6000);
	PostMessage(MSG_CHILD_ITEM_ACTIVE);
	return TRUE;  // return TRUE unless you set the focus to a control
}


HRESULT CRightCtrlWnd::OnChildItemActivate(WPARAM w, LPARAM l)
{
	GetParent()->SendMessage(WM_CONTROL_ACTIVED, (WPARAM)this);
	GetParent()->GetParent()->SendMessage(WM_PERSON_ACTIVE, (WPARAM)GetParent());
	GetParent()->GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent()->GetParent());
	return 1;
}

void CRightCtrlWnd::OnOK()
{
	// do nothing.
}

void CRightCtrlWnd::OnCancel()
{
	// do nothing.
}

HBRUSH CRightCtrlWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		if (NULL == m_hBrush)
		{	
			// create brush first time we need it
			m_hBrush = ::CreateSolidBrush(m_backColor);
		}
		return m_hBrush;
	}

	return hbr;
}

void CRightCtrlWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_focusBtn.SetFocus();

	SendActiveMessage();

	CDialog::OnLButtonDown(nFlags, point);
}

void CRightCtrlWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	SendActiveMessage();

	CDialog::OnRButtonDown(nFlags, point);
}

void CRightCtrlWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	SendActiveMessage();

	CDialog::OnMButtonDown(nFlags, point);
}


BOOL CRightCtrlWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SendActiveMessage();

	return CDialog::OnCommand(wParam, lParam);
}

void CRightCtrlWnd::SendActiveMessage()
{
	if (GetParent() && GetParent()->m_hWnd)
	{
		GetParent()->SendMessage(WM_CONTROL_ACTIVED, (WPARAM)this);
		if (GetParent()->GetParent() && GetParent()->GetParent()->m_hWnd)
		{
			GetParent()->GetParent()->SendMessage(WM_PERSON_ACTIVE, (WPARAM)GetParent());
		}
		if (GetParent()->GetParent()->GetParent() && GetParent()->GetParent()->GetParent()->m_hWnd)
		{
			GetParent()->GetParent()->GetParent()->SendMessage(WM_THING_ACTIVE, (WPARAM)GetParent()->GetParent());
		}
	}
}

LRESULT CRightCtrlWnd::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::DefWindowProc(message, wParam, lParam);
}

BOOL CRightCtrlWnd::OnEraseBkgnd(CDC* pDC)
{
	return __super::OnEraseBkgnd(pDC);
}


void CRightCtrlWnd::OnPaint()
{
	
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call __super::OnPaint() for painting messages

	DrawBorder(&dc);
}

#include "PersonWnd.h"
void CRightCtrlWnd::DrawBorder(CDC *pDC)
{
	CPersonWnd *pPerson = dynamic_cast<CPersonWnd*>(GetParent());
	
	if (m_hWnd )
	{
		CRect rtWnd;
		GetClientRect(&rtWnd);
		pDC->FillSolidRect(&rtWnd,m_active ? RGB(190,130,0) : RGB(240,240,240));
	}
}