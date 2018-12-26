
// CostStatisticsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CostStatistics.h"
#include "CostStatisticsDlg.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <wchar.h>
#include <algorithm>

#include "FrameLeftDlg.h"
#include "ImportExcel.h"

#include "./mysql/include/mysql.h"
#include "DBResultOper.h"
#include "DlgUserMgr.h"
#include "AssmebleGrid.h"
#include "DlgPrivilidge.h"
#include "ProjMgrDlg.h"
#include "GridHeaderDefine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCostStatisticsDlg* g_pMainDlg;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX,IDC_LIST2,m_list);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CCostStatisticsDlg 对话框

CCostStatisticsDlg::CCostStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CSplitDialog(CCostStatisticsDlg::IDD, pParent)
{
	SetSpliterDialogListener(this);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_LOGO/*IDR_MAINFRAME*/);
}

void CCostStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCostStatisticsDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK,GRID_ID,&CCostStatisticsDlg::OnGridClick)
	ON_COMMAND(MENU_DELETE_AUTOLOGIN, &CCostStatisticsDlg::OnCancelAutoLogin)	//取消自动登录
	ON_COMMAND(MENU_EXIT_SYSTEM, &CCostStatisticsDlg::OnMenuExitSystem)			//退出系统
	ON_COMMAND(MENU_ADD_ACCOUNT, &CCostStatisticsDlg::OnMenuAddAccount)			//添加账号
	ON_COMMAND(MENU_DEL_ACCOUNT, &CCostStatisticsDlg::OnMenuDelAccount)			//删除账号
	ON_COMMAND(MENU_CHANGE_PWD,&CCostStatisticsDlg::OnMenuChangePwd)			//修改密码

	ON_COMMAND(MENU_MANU_BASE,&CCostStatisticsDlg::OnMenuManuBase)				//制造基地

	ON_COMMAND(MENU_ABOUT, &CCostStatisticsDlg::OnMenuAbout)					//关于

    ON_COMMAND(MENU_STAFF_SALARY, &CCostStatisticsDlg::OnMenuStaffSalary)				//职工薪酬
	ON_COMMAND(MENU_WORKSHOP_TOTAL, &CCostStatisticsDlg::OnMenuWorkShopTotal)			//车间合计
	ON_COMMAND(MENU_COST_TOTAL, &CCostStatisticsDlg::OnMenuCostTotal)			        //制造费用合计
	ON_COMMAND(MENU_OVERSEASBASE_PRODUCE,&CCostStatisticsDlg::OnMenuOverSeaProduce)		//驻外基地生产


END_MESSAGE_MAP()


// CCostStatisticsDlg 消息处理程序

BOOL CCostStatisticsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	g_pMainDlg = this;

	this->ShowWindow(SW_SHOWMAXIMIZED);
	m_pPaneMain = NULL;

	InitInterface();

	//readData();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCostStatisticsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCostStatisticsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	/*CPaintDC dc(this);
	CRect Rect;
	GetClientRect(&Rect);
	Rect.SetRect(0,0,Rect.Width(),40);
	dc.FillSolidRect(Rect,RGB(108,123,139));

	CFont font;
	font.CreatePointFont(150, _T("宋体"));

	CFont* pOldFont=dc.SelectObject(&font);
	dc.SetTextColor(RGB(255,255,255));

	dc.SelectObject(pOldFont);*/
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCostStatisticsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCostStatisticsDlg::OnSize(UINT nType, int cx, int cy)
{
 	__super::OnSize(nType, cx, cy);
 	Repaint();	
}

CString CCostStatisticsDlg::GetTitle()
{
	return TEXT(""); 
}

CDialog * CCostStatisticsDlg::GetPage()
{
	return  this ;
}

void CCostStatisticsDlg::Print()
{

}

void CCostStatisticsDlg::Export()
{

}

BOOL CCostStatisticsDlg::CreatePage(CWnd * p)
{
	return Create( IDD , p );
}

BOOL CCostStatisticsDlg::Show(int nCmdShow)
{
	return ShowWindow( nCmdShow );
}

void CCostStatisticsDlg::Move(LPCRECT rect)
{
	return MoveWindow( rect );
}

void CCostStatisticsDlg::InitInterface()
{
	CreateGrid();
	CAssmbleGrid::ChangeGridHeader(0,m_Grid);
	//InitGridHeader();

	CRect rect;
	this->GetClientRect(&rect);
	m_pPaneMain  = CreatePane(CPaneEX::ST_HORIZONTAL);
	SetPaneWnd(m_pPaneMain, &m_Grid,1, 1, 1, rect.Width(),rect.Height());

	Initialize();
}

void CCostStatisticsDlg::Repaint()
{
	if ( m_pPaneMain != NULL )
	{
		CRect rect;
		GetClientRect(&rect);

		m_pPaneMain->MovePane(0,50,rect.Width(),rect.Height() - 52);

		m_Grid.MoveWindow(0,50,rect.Width(),rect.Height() - 45);
	}
}

int CCostStatisticsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rect;
	::GetWindowRect(::GetDesktopWindow(), &rect);
	rect.DeflateRect(10, 50, 10, 50);	
	MoveWindow(&rect);
	return 0;
}

void CCostStatisticsDlg::CreateGrid()
{
	CRect rect;
	GetClientRect(&rect);
	m_Grid.Create( CRect(0, 40,rect.Width(),rect.Height()), this, GRID_ID);
	for ( int i = 0 ; i < 20 ; i++ )
	{
		m_Grid.InsertColumn("");
	}
	m_Grid.InsertRow("");
	m_Grid.InsertRow("");

	m_Grid.SetDoubleBuffering();
	m_Grid.SetBkColor(RGB(255, 255, 255));
	m_Grid.SetFixedBkColor(RGB(	230,230,250));
}

void CCostStatisticsDlg::readData()
{
	std::map< CString,std::vector<CString> > mapRet;
	mapRet.clear();
	//CDbOperator::GetInstance()->SetDbInfo(MySQL,DBHOST,DBPORT,DBUSER,DBPASS,DBNAME);
	CDbOperator::GetInstance()->Query("select * from z_user limit 0,1000","key1,key2,key3,key4,key5",mapRet);

 	CDBResultOper obj;
 	obj.SetResultMap(mapRet);
 	int nRow = obj.GetRowCount();

	CString str = "";
  	for(int nRetCount = 0;nRetCount < nRow;nRetCount++)
  	{
 		int rows = m_Grid.InsertRow("");
		str.Format("%d",rows);
		m_Grid.SetItemText( rows,1,str);
		m_Grid.SetItemText( rows,2,obj.GetValue("key2",nRetCount));
		m_Grid.SetItemText( rows,3,obj.GetValue("key3",nRetCount));
		m_Grid.SetItemText( rows,4,obj.GetValue("key4",nRetCount));
		m_Grid.SetItemText( rows,5,obj.GetValue("key5",nRetCount));
  	}

	return;
}

void CCostStatisticsDlg::OnGridClick(NMHDR * pNmhdr, LRESULT * pLresult)
{
	return;
}

//取消自动登录
void CCostStatisticsDlg::OnCancelAutoLogin()   
{
	CString strSecretPath = CCommon::GetSecretPath();
	::DeleteFile(strSecretPath);
}

//退出系统
void CCostStatisticsDlg::OnMenuExitSystem()
{
	if( ::MessageBox(this->GetSafeHwnd(),"确定要退出系统？","提示：",MB_YESNO|MB_ICONQUESTION) == IDNO )
	{
		return;
	}

	//强制退出进程
	HANDLE hProcess = GetCurrentProcess();
	TerminateProcess(hProcess, 0);
}

//添加账号
void CCostStatisticsDlg::OnMenuAddAccount()
{
	CDlgUserMgr dlg(EN_ADD);
	dlg.DoModal();
}

//删除账号
void CCostStatisticsDlg::OnMenuDelAccount()
{
	CDlgUserMgr dlg(EN_DEL);
	dlg.DoModal();
}

//修改密码
void CCostStatisticsDlg::OnMenuChangePwd()
{
	CDlgUserMgr dlg(EN_CNG);
	dlg.DoModal();
}

//关于
void CCostStatisticsDlg::OnMenuAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

//制造基地
void CCostStatisticsDlg::OnMenuManuBase()
{
	AfxMessageBox("制造基地");
}

void CCostStatisticsDlg::OnToolBarMenuClick(int nID)
{
	if( nID == 0 )  //系统
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());

		submenu.AppendMenu(MF_STRING, MENU_DELETE_AUTOLOGIN, TEXT("取消自动登录"));
		submenu.AppendMenu(MF_STRING, MENU_EXIT_SYSTEM, TEXT("退出系统"));

		menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT(""));
		TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, point.x, point.y, m_hWnd,NULL);
		menu.DestroyMenu();
	}
	else if ( nID == 1 ) //账号管理
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());


		submenu.AppendMenu(MF_STRING, MENU_ADD_ACCOUNT, TEXT("添加账号"));
		submenu.AppendMenu(MF_STRING, MENU_DEL_ACCOUNT, TEXT("删除账号"));
		submenu.AppendMenu(MF_STRING, MENU_CHANGE_PWD, TEXT("重置密码"));

		if( !IsAdmin(CLoginUserInfo::m_nUserID) )
		{
			submenu.EnableMenuItem(MENU_ADD_ACCOUNT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			submenu.EnableMenuItem(MENU_DEL_ACCOUNT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			submenu.EnableMenuItem(MENU_CHANGE_PWD, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}

		menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT(""));
		TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, point.x, point.y, m_hWnd,NULL);
		menu.DestroyMenu();
	}
	else if( nID == 2 ) //权限设置
	{
		RoleInfo role;
		CDlgPrivilidge	dlgPrivilidgeObj;

		role.name = "录入权限";
		role.value = 1;
		dlgPrivilidgeObj.m_arrRoles.Add(role);

		role.name = "删除权限";
		role.value = 2;
		dlgPrivilidgeObj.m_arrRoles.Add(role);

		dlgPrivilidgeObj.SetModalKey("CostStatitics");
		if( dlgPrivilidgeObj.DoModal() == IDOK )
		{
			
		}
	}
	else if( nID == 3 ) //数据导入
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());


		submenu.AppendMenu(MF_STRING, MENU_STAFF_SALARY, TEXT("职工薪酬"));
		submenu.AppendMenu(MF_STRING, MENU_WORKSHOP_TOTAL, TEXT("车间合计"));
		submenu.AppendMenu(MF_STRING, MENU_COST_TOTAL, TEXT("制造费用统计"));
		submenu.AppendMenu(MF_STRING, MENU_OVERSEASBASE_PRODUCE, TEXT("驻外基地生产情况"));

		menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT(""));
		TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, point.x, point.y, m_hWnd,NULL);
		menu.DestroyMenu();
	}
	else if( nID == 4 ) //项目管理
	{
		CProjMgrDlg dlg;
		dlg.DoModal();
	}
	else if( nID == 5 ) //树设置
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());


		submenu.AppendMenu(MF_STRING, MENU_MANU_BASE, TEXT("制造基地管理"));

		menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT(""));
		TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, point.x, point.y, m_hWnd,NULL);
		menu.DestroyMenu();
	}
	else if( nID == 6 ) //帮助
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());


		submenu.AppendMenu(MF_STRING, MENU_ABOUT, TEXT("关于"));

		menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT(""));
		TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, point.x, point.y, m_hWnd,NULL);
		menu.DestroyMenu();
	}
}

void CCostStatisticsDlg::OnLeftTreeSelChange(CTreeListItem* pItem)
{
	int nType = 0;
	CString strType = pItem->GetText();

	if( strcmp(pItem->GetText(),"汇总信息") == 0 )
	{
		nType = 0;
	}
	else if( strcmp(pItem->GetText(),"材料劳务运输") == 0 )
	{
		nType = 1;
	}
	else if( strType.Compare("材料费") == 0  )
	{
		nType = 11;
	}
	else if(strType.Compare("劳务费") == 0 )
	{
		nType = 12;
	}
	else if( strType.Compare("运输费") == 0 )
	{
		nType = 13;
	}
	else if( strcmp(pItem->GetText(),"职工薪酬统计") == 0 )
	{
		nType = 2;
	}
	else if( strcmp(pItem->GetText(),"人工费合计") == 0 )
	{
		nType = 3;
	}
	else if( strType == "职工薪酬" )
	{
		nType = 4;
	}
	else if( strType == "直接人工汇总")
	{
		nType = 41;
	}
	else if( strType == "电器调试车间")
	{
		nType = 411;
	}
	else if( strType == "组装车间")
	{
		nType = 412;
	}
	else if( strType == "铆焊车间")
	{
		nType = 413;
	}
	else if( strType == "液压车间")
	{
		nType = 414;
	}
	else if( strType == "流体车间")
	{
		nType = 415;
	}
	else if( strType == "机加工车间")
	{
		nType = 416;
	}
	else if( strType == "综合班")
	{
		nType = 417;
	}
	else if( strType == "物资中心")
	{
		nType = 418;
	}
	else if( strType == "电控柜制作车间")
	{
		nType = 419;
	}
	else if(strType =="制造人工分摊")
	{
		nType = 42;
	}
	else if( strType == "后勤人工分摊")
	{
		nType = 43;
	}
	else if( strType == "项目人工分摊")
	{
		nType = 44;
	}
	else if( strcmp(pItem->GetText(),"车间合计") == 0 )
	{
		nType = 5;
	}
	else if(strType == "电气调试车间"|| strType =="组装车间"|| strType =="液压车间"|| strType =="流体车间"
		|| strType =="铆焊车间"|| strType =="机加工车间" || strType =="综合班"|| strType =="物资中心" || 
		strType == "电控柜制作车间")
	{
		nType = 6;
	}
	else if( strcmp(pItem->GetText(),"制造费用统计表") == 0 )
	{
		nType = 7;
	}
	else if(strType == "直接制造费用" )
	{
		nType = 71;
	}
	else if( strType == "间接制造费用" )
	{
		nType = 72;
	}
	else if( strType =="项目部费用" )
	{
		nType = 73;
	}
	else if( strcmp(pItem->GetText(),"驻外基地生产情况") == 0 )
	{
		nType = 9;
	}
	else if(strType =="郑州" || strType =="天津"|| strType =="顺德"|| strType =="长沙" || strType =="成都")
	{
		nType = 10;
	}
	CAssmbleGrid::ChangeGridHeader(nType,m_Grid);

	//Repaint();
}
void CCostStatisticsDlg::OnMenuStaffSalary()
{
     ImportExcel dlg;
	 dlg.ImportStaffSalary();
}

void CCostStatisticsDlg::OnMenuWorkShopTotal()
{
	ImportExcel dlg;
	dlg.ImportWorkShopTotal();
}

void CCostStatisticsDlg::OnMenuCostTotal()
{
	ImportExcel dlg;
	dlg.ImportCostTotal();
}

void CCostStatisticsDlg::OnMenuOverSeaProduce()
{
	ImportExcel dlg;
	dlg.ImportOverSeaProduce();
}