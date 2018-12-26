#pragma once
#include "../UiFrameWork/UI/MessageHandler.h"
class CTreeListCtrl;
class CTreeListItem;
class CLeftWnd;
class CMyMessageHandler:public CMessageHandler
{
public:
	CMyMessageHandler(void);
	~CMyMessageHandler(void);
public:
	virtual void OnToolbarClick(CToolbarEh* toolbar, int id);		//œ˚œ¢”≥…‰
	virtual CFont* OnToolbarGetFont() { return &m_ftToolbar; }
	void OnTreeListCtrlSelChange(CTreeListItem* pItem);
	void OnTreeListCtrl(CTreeListItem* pItem);
	void OnBtnClick(CString strText,int nID);

protected:
	void OnToolbarClickOfPersonWnd(CPersonWnd* pPersonWnd, int id);
	void OnToolbarClickOfThingWnd(CThingWnd* pThingWnd, int id);
	void OnToolbarClickOfLeftWnd(CLeftWnd* pLeftWnd, int id);
protected:
	CFont m_ftToolbar;
	DWORD m_clrToolbarTxt;
	int m_nTooBarID;
};
