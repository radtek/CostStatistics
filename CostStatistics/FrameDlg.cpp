// FrameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FrameDlg.h"
#include "FrameLeftDlg.h"
#include "FrameRightDlg.h"
#include "CostStatistics.h"


#define  TREE_ID_CODE   1
#define  TREE_ID_TYPE   2

// CFrameDlg 对话框

IMPLEMENT_DYNAMIC(CFrameDlg, CDialog)

CFrameDlg::CFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrameDlg::IDD, pParent)
{
	m_pZTUIMainFrame = NULL;
}

CFrameDlg::~CFrameDlg()
{
}

void CFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFrameDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	
END_MESSAGE_MAP()



// CFrameDlg 消息处理程序

BOOL CFrameDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//设置窗口图标
	HICON m_hIcon;//定义一个icon句柄
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_LOGO/*IDR_MAINFRAME*/);//加载资源文件ID为IDR_MAINFRAME的资源
	SetIcon(m_hIcon, TRUE); 
	SetIcon(m_hIcon, FALSE);
	
	g_ztui->m_pMsgHandler = new CMyMessageHandler();
	m_pZTUIMainFrame = g_ztui->InitFrame(this);
	SetCursor(theApp.LoadStandardCursor(IDC_ARROW ));
	
	InitMyChildWnd(); 
	g_ztui->SetLeftWndWidth(LEFT_TREE_WIDTH);

	//初始化窗口标题
	CString strTitle = "成本统计系统";
	SetMyWindowText(strTitle);
	ShowWindow(SW_MAXIMIZE);

	::SetWindowPos(this->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOMOVE);
	::SetWindowPos(this->GetSafeHwnd(),HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOMOVE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CFrameDlg::InitMyChildWnd()
{
	//左侧窗口
	CFrameLeftDlg *pLeft = new CFrameLeftDlg;
	pLeft->Create(CFrameLeftDlg::IDD);
	g_ztui->m_pLeftWnd->AppendWnd(pLeft);

	//右侧窗口
	CFrameRightDlg *pRight = new CFrameRightDlg;
	pRight->Create(CFrameRightDlg::IDD);
	g_ztui->m_pRightWnd->AddCustomWnd(pRight);

	//主工具栏
	InitLeftToolbar();
	InitRightToolbar();
}

void CFrameDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ( m_pZTUIMainFrame != NULL && m_pZTUIMainFrame->GetSafeHwnd() )
	{
		m_pZTUIMainFrame->MoveWindow(0, 0, cx, cy);
		g_ztui->m_pRightWnd->Refresh();
	}
}

void CFrameDlg::InitLeftToolbar()
{
	ToolbarData  toolbar;
	toolbar.buttons.clear();
	int buttonId = 3000;

	g_ztui->m_pLeftWnd->m_pToolbar->Load(toolbar);
	g_ztui->m_pLeftWnd->m_pToolbar->m_bTabMode=true;
}

void CFrameDlg::InitRightToolbar()
{
	ToolbarData  toolbar;
	toolbar.buttons.clear();
	int buttonId = 10000;

	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"系统",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"账号管理",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"权限设置",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"数据导入",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"项目管理",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"其他设置",e_TextStatic));
	toolbar.buttons.push_back(ToolbarBtnData(buttonId++,"帮助",e_TextStatic));

	g_ztui->m_pRightWnd->m_pToolbar->Load(toolbar);
	g_ztui->m_pRightWnd->m_pToolbar->m_bTabMode = true;
}


void CFrameDlg::SetMyWindowText(CString strTitle)
{
	if( this->GetSafeHwnd() == NULL )
	{
		return;
	}

	CString strMyTitle = strTitle;
	this->SetWindowText(strMyTitle);
}


BOOL CFrameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if( pMsg->message == WM_KEYDOWN )
	{
		switch(pMsg-> wParam) 
		{ 
		case   VK_RETURN://屏蔽回车 
			return   TRUE;    
		case   VK_ESCAPE://屏蔽Esc 
			return   TRUE;  
		} 
	}

	return __super::PreTranslateMessage(pMsg);
}

void CFrameDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if( ::MessageBox(this->GetSafeHwnd(),"确定要退出系统？","提示：",MB_YESNO|MB_ICONQUESTION) == IDNO )
	{
		return;
	}
	
	//强制退出进程
	HANDLE hProcess = GetCurrentProcess();
	TerminateProcess(hProcess, 0);

	__super::OnClose();
}