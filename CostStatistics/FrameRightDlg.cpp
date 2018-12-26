// FrameLogSendWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "FrameRightDlg.h"


// FrameRightControl 对话框

IMPLEMENT_DYNAMIC(CFrameRightDlg, CDialog)

CFrameRightDlg::CFrameRightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrameRightDlg::IDD, pParent)
{
	m_Font.CreatePointFont(110,"simhei");
}

CFrameRightDlg::~CFrameRightDlg()
{

}

void CFrameRightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TAB_MAIN,m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CFrameRightDlg, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// FrameRightControl 消息处理程序

BOOL CFrameRightDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始
	CreateTabCtrl();
	Repaint();
	ShowWindow(SW_MAXIMIZE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CFrameRightDlg::CreateTabCtrl()
{
	m_vTabItems.resize(1);
	m_vTabItems[0]		= dynamic_cast<IVTabItem*>(new CCostStatisticsDlg);
	int nTabItems = m_vTabItems.size();

	for (int i = 0; i < nTabItems; ++i)
	{
		m_tabCtrl.InsertItem( i, m_vTabItems[i]->GetTitle( ) );
		m_vTabItems[i]->CreatePage( &m_tabCtrl );
		m_vTabItems[i]->Show( SW_HIDE );		
	}	

	CSize size;
	size.cx = 1;
	size.cy = 1;
	m_tabCtrl.SetItemSize(size);

	TCITEM   item;   
	item.mask   =   TCIF_TEXT;  	
	if (nTabItems > 0)
	{
		m_curTabItem = 0;
		m_tabCtrl.SetCurSel( m_curTabItem );
		m_vTabItems[ m_curTabItem ]->Show( SW_SHOW );
	}
}

void CFrameRightDlg::ShowTabDlg(int index)
{
	CWaitCursor obj;
	g_pMainDlg->OnToolBarMenuClick(index);
}

void CFrameRightDlg::Repaint(int format/*=0*/)
{
	if( m_tabCtrl.GetSafeHwnd() != NULL )
	{
		CRect rect_client;
		GetClientRect(&rect_client);

		//TabCtrl控件
		CRect rect_TabCtrl;
		rect_TabCtrl = rect_client;
		m_tabCtrl.MoveWindow(rect_TabCtrl);

		RECT rectTabClient, rectPage;
		m_tabCtrl.GetClientRect(&rectTabClient);
		rectPage = rectTabClient;

		int nPageCount = m_vTabItems.size();
		for (int i = 0; i < nPageCount; ++i)
		{
			m_vTabItems[i]->Move(&rectPage);
		}
	}
}


void CFrameRightDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Repaint(0);
}


BOOL CFrameRightDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	ASSERT(pMsg != NULL);
	if( pMsg->message == WM_KEYDOWN )
	{
		//CEdit* pEdit = (CEdit*)GetDlgItem(IDC_MY_EDIT);
		//if( pMsg->wParam == VK_RETURN /*&& pEdit->GetSafeHwnd() == pMsg->hwnd*/ )
		//{
		//	if( pEdit->GetSafeHwnd() == pMsg->hwnd )
		//	{
		//		OnButtonClick(IDC_MY_BUTTON);
		//	}

		//	return TRUE;
		//}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
