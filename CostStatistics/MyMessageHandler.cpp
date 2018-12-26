#include "StdAfx.h"
#include "MyMessageHandler.h"
#include "FrameRightDlg.h"


CMyMessageHandler::CMyMessageHandler(void)
{
	m_ftToolbar.CreatePointFont(100,"Microsoft YaHei");
	m_nTooBarID = 2000;
}

CMyMessageHandler::~CMyMessageHandler(void)
{

}

void CMyMessageHandler::OnTreeListCtrlSelChange(CTreeListItem* pItem)
{
	CFrameRightDlg *pDlg = dynamic_cast<CFrameRightDlg*> (g_ztui->m_pRightWnd->GetCustomWnd());
	if(pDlg != NULL)
	{
		//pDlg->OnSearchData(pItem,m_nTooBarID);
	}
}

void CMyMessageHandler::OnToolbarClick(CToolbarEh* toolbar, int id)
{
	//记录当前的toolbarid
	m_nTooBarID = id;

	if (toolbar == g_ztui->m_pLeftWnd->m_pToolbar)
	{
		OnToolbarClickOfLeftWnd((CLeftWnd*)toolbar->GetOwner(), id);
	}
	else if(toolbar == g_ztui->m_pRightWnd->m_pToolbar)
	{			
		CFrameRightDlg *pDlg = dynamic_cast<CFrameRightDlg*>(g_ztui->m_pRightWnd->GetCustomWnd());
		{
			pDlg->ShowTabDlg( id - 10000);
		}
	}
	else if (toolbar->GetOwner()->IsKindOf(RUNTIME_CLASS(CPersonWnd)))
	{
		OnToolbarClickOfPersonWnd((CPersonWnd*)toolbar->GetOwner(), id);
	}
	else if (toolbar->GetOwner()->IsKindOf(RUNTIME_CLASS(CThingWnd)))
	{
		OnToolbarClickOfThingWnd((CThingWnd*)toolbar->GetOwner(), id);
	}
}


void CMyMessageHandler::OnToolbarClickOfLeftWnd(CLeftWnd* pLeftWnd, int id)
{
	
}

void CMyMessageHandler::OnToolbarClickOfPersonWnd(CPersonWnd* pPersonWnd, int id)
{
	
}
void CMyMessageHandler::OnToolbarClickOfThingWnd(CThingWnd* pThingWnd, int id)
{

}

void CMyMessageHandler::OnBtnClick(CString strText,int nID)
{
	CFrameRightDlg *pDlg = dynamic_cast<CFrameRightDlg*> (g_ztui->m_pRightWnd->GetCustomWnd());
	if(pDlg != NULL)
	{
		//pDlg->OnBtnClick(strText,nID,m_nTooBarID);
	}
}
