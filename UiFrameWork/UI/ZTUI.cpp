#include "StdAfx.h"
#include "UI.h"
#include "ZTUI.h"
#include "ZTUIMainFrame.h"
#include "resource.h"
#include "util.h"
#include "ThingWnd.h"

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib,"GdiPlus.lib")



ZTUI* ZTUI::GetInstance()
{
	// need to call the destructor to free m_pMsgHandler
	static ZTUI instance;

	return &instance;
}

ZTUI::ZTUI(void)
	: m_pFrame(NULL)
	, m_pRightWnd(NULL)
    , m_pLeftWnd(NULL)
	, m_pMsgHandler(NULL)
{
}

ZTUI::~ZTUI(void)
{
	if (m_pMsgHandler)
	{
		delete m_pMsgHandler;
		m_pMsgHandler = NULL;
	}
}

CFrameWnd* ZTUI::InitFrame(CWnd * pParent)
{
    //初始化GDIPlus

	CBitmap bmp;
	if(!bmp.LoadBitmap(IDB_NOPICTURE))
	{
		AfxMessageBox("请将\"ZTUILib.res\"文件添加到项目中");
		exit(0);
	}
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

#ifdef USE_ZTUILIB
	m_pFrame = new CZTUIMainFrame;

	if (pParent == NULL)
	{
		m_pFrame->LoadFrame(IDR_ZTUI_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
			NULL);
	}
	else
	{
		m_pFrame->LoadFrame(IDR_ZTUI_MAINFRAME,
			WS_CHILD | WS_VISIBLE, pParent,
			NULL);
		CRect rect;
		pParent->GetClientRect(rect);
		m_pFrame->MoveWindow(rect);
	}

	m_pFrame->ShowWindow(SW_SHOW);
	m_pFrame->UpdateWindow();

	return m_pFrame;
#else
	return m_pFrame = (CZTUIMainFrame*)theApp.m_pMainWnd;
#endif
}

void ZTUI::SetLeftWndWidth(int w)
{
	m_pFrame->SetLeftWndWidth(w);
}

int ZTUI::GetLeftWndWidth()
{
	return m_pFrame->GetLeftWndWidth();
}

void ZTUI::ShowHideLeftWnd()
{
	return m_pFrame->ShowHideLeftWnd();
}

