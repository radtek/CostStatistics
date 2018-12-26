
// CostStatisticsDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CCostStatisticsDlg �Ի���

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
	ON_COMMAND(MENU_DELETE_AUTOLOGIN, &CCostStatisticsDlg::OnCancelAutoLogin)	//ȡ���Զ���¼
	ON_COMMAND(MENU_EXIT_SYSTEM, &CCostStatisticsDlg::OnMenuExitSystem)			//�˳�ϵͳ
	ON_COMMAND(MENU_ADD_ACCOUNT, &CCostStatisticsDlg::OnMenuAddAccount)			//����˺�
	ON_COMMAND(MENU_DEL_ACCOUNT, &CCostStatisticsDlg::OnMenuDelAccount)			//ɾ���˺�
	ON_COMMAND(MENU_CHANGE_PWD,&CCostStatisticsDlg::OnMenuChangePwd)			//�޸�����

	ON_COMMAND(MENU_MANU_BASE,&CCostStatisticsDlg::OnMenuManuBase)				//�������

	ON_COMMAND(MENU_ABOUT, &CCostStatisticsDlg::OnMenuAbout)					//����

    ON_COMMAND(MENU_STAFF_SALARY, &CCostStatisticsDlg::OnMenuStaffSalary)				//ְ��н��
	ON_COMMAND(MENU_WORKSHOP_TOTAL, &CCostStatisticsDlg::OnMenuWorkShopTotal)			//����ϼ�
	ON_COMMAND(MENU_COST_TOTAL, &CCostStatisticsDlg::OnMenuCostTotal)			        //������úϼ�
	ON_COMMAND(MENU_OVERSEASBASE_PRODUCE,&CCostStatisticsDlg::OnMenuOverSeaProduce)		//פ���������


END_MESSAGE_MAP()


// CCostStatisticsDlg ��Ϣ�������

BOOL CCostStatisticsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	g_pMainDlg = this;

	this->ShowWindow(SW_SHOWMAXIMIZED);
	m_pPaneMain = NULL;

	InitInterface();

	//readData();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCostStatisticsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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
	font.CreatePointFont(150, _T("����"));

	CFont* pOldFont=dc.SelectObject(&font);
	dc.SetTextColor(RGB(255,255,255));

	dc.SelectObject(pOldFont);*/
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

	// TODO:  �ڴ������ר�õĴ�������
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

//ȡ���Զ���¼
void CCostStatisticsDlg::OnCancelAutoLogin()   
{
	CString strSecretPath = CCommon::GetSecretPath();
	::DeleteFile(strSecretPath);
}

//�˳�ϵͳ
void CCostStatisticsDlg::OnMenuExitSystem()
{
	if( ::MessageBox(this->GetSafeHwnd(),"ȷ��Ҫ�˳�ϵͳ��","��ʾ��",MB_YESNO|MB_ICONQUESTION) == IDNO )
	{
		return;
	}

	//ǿ���˳�����
	HANDLE hProcess = GetCurrentProcess();
	TerminateProcess(hProcess, 0);
}

//����˺�
void CCostStatisticsDlg::OnMenuAddAccount()
{
	CDlgUserMgr dlg(EN_ADD);
	dlg.DoModal();
}

//ɾ���˺�
void CCostStatisticsDlg::OnMenuDelAccount()
{
	CDlgUserMgr dlg(EN_DEL);
	dlg.DoModal();
}

//�޸�����
void CCostStatisticsDlg::OnMenuChangePwd()
{
	CDlgUserMgr dlg(EN_CNG);
	dlg.DoModal();
}

//����
void CCostStatisticsDlg::OnMenuAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

//�������
void CCostStatisticsDlg::OnMenuManuBase()
{
	AfxMessageBox("�������");
}

void CCostStatisticsDlg::OnToolBarMenuClick(int nID)
{
	if( nID == 0 )  //ϵͳ
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());

		submenu.AppendMenu(MF_STRING, MENU_DELETE_AUTOLOGIN, TEXT("ȡ���Զ���¼"));
		submenu.AppendMenu(MF_STRING, MENU_EXIT_SYSTEM, TEXT("�˳�ϵͳ"));

		menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT(""));
		TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, point.x, point.y, m_hWnd,NULL);
		menu.DestroyMenu();
	}
	else if ( nID == 1 ) //�˺Ź���
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());


		submenu.AppendMenu(MF_STRING, MENU_ADD_ACCOUNT, TEXT("����˺�"));
		submenu.AppendMenu(MF_STRING, MENU_DEL_ACCOUNT, TEXT("ɾ���˺�"));
		submenu.AppendMenu(MF_STRING, MENU_CHANGE_PWD, TEXT("��������"));

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
	else if( nID == 2 ) //Ȩ������
	{
		RoleInfo role;
		CDlgPrivilidge	dlgPrivilidgeObj;

		role.name = "¼��Ȩ��";
		role.value = 1;
		dlgPrivilidgeObj.m_arrRoles.Add(role);

		role.name = "ɾ��Ȩ��";
		role.value = 2;
		dlgPrivilidgeObj.m_arrRoles.Add(role);

		dlgPrivilidgeObj.SetModalKey("CostStatitics");
		if( dlgPrivilidgeObj.DoModal() == IDOK )
		{
			
		}
	}
	else if( nID == 3 ) //���ݵ���
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());


		submenu.AppendMenu(MF_STRING, MENU_STAFF_SALARY, TEXT("ְ��н��"));
		submenu.AppendMenu(MF_STRING, MENU_WORKSHOP_TOTAL, TEXT("����ϼ�"));
		submenu.AppendMenu(MF_STRING, MENU_COST_TOTAL, TEXT("�������ͳ��"));
		submenu.AppendMenu(MF_STRING, MENU_OVERSEASBASE_PRODUCE, TEXT("פ������������"));

		menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT(""));
		TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, point.x, point.y, m_hWnd,NULL);
		menu.DestroyMenu();
	}
	else if( nID == 4 ) //��Ŀ����
	{
		CProjMgrDlg dlg;
		dlg.DoModal();
	}
	else if( nID == 5 ) //������
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());


		submenu.AppendMenu(MF_STRING, MENU_MANU_BASE, TEXT("������ع���"));

		menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT(""));
		TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, point.x, point.y, m_hWnd,NULL);
		menu.DestroyMenu();
	}
	else if( nID == 6 ) //����
	{
		CPoint point;
		GetCursorPos(&point);
		point.y = 50;
		point.x -= 30;

		CMenu menu;
		CMenu submenu;
		VERIFY(menu.CreateMenu());
		VERIFY(submenu.CreateMenu());


		submenu.AppendMenu(MF_STRING, MENU_ABOUT, TEXT("����"));

		menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT(""));
		TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, point.x, point.y, m_hWnd,NULL);
		menu.DestroyMenu();
	}
}

void CCostStatisticsDlg::OnLeftTreeSelChange(CTreeListItem* pItem)
{
	int nType = 0;
	CString strType = pItem->GetText();

	if( strcmp(pItem->GetText(),"������Ϣ") == 0 )
	{
		nType = 0;
	}
	else if( strcmp(pItem->GetText(),"������������") == 0 )
	{
		nType = 1;
	}
	else if( strType.Compare("���Ϸ�") == 0  )
	{
		nType = 11;
	}
	else if(strType.Compare("�����") == 0 )
	{
		nType = 12;
	}
	else if( strType.Compare("�����") == 0 )
	{
		nType = 13;
	}
	else if( strcmp(pItem->GetText(),"ְ��н��ͳ��") == 0 )
	{
		nType = 2;
	}
	else if( strcmp(pItem->GetText(),"�˹��Ѻϼ�") == 0 )
	{
		nType = 3;
	}
	else if( strType == "ְ��н��" )
	{
		nType = 4;
	}
	else if( strType == "ֱ���˹�����")
	{
		nType = 41;
	}
	else if( strType == "�������Գ���")
	{
		nType = 411;
	}
	else if( strType == "��װ����")
	{
		nType = 412;
	}
	else if( strType == "í������")
	{
		nType = 413;
	}
	else if( strType == "Һѹ����")
	{
		nType = 414;
	}
	else if( strType == "���峵��")
	{
		nType = 415;
	}
	else if( strType == "���ӹ�����")
	{
		nType = 416;
	}
	else if( strType == "�ۺϰ�")
	{
		nType = 417;
	}
	else if( strType == "��������")
	{
		nType = 418;
	}
	else if( strType == "��ع���������")
	{
		nType = 419;
	}
	else if(strType =="�����˹���̯")
	{
		nType = 42;
	}
	else if( strType == "�����˹���̯")
	{
		nType = 43;
	}
	else if( strType == "��Ŀ�˹���̯")
	{
		nType = 44;
	}
	else if( strcmp(pItem->GetText(),"����ϼ�") == 0 )
	{
		nType = 5;
	}
	else if(strType == "�������Գ���"|| strType =="��װ����"|| strType =="Һѹ����"|| strType =="���峵��"
		|| strType =="í������"|| strType =="���ӹ�����" || strType =="�ۺϰ�"|| strType =="��������" || 
		strType == "��ع���������")
	{
		nType = 6;
	}
	else if( strcmp(pItem->GetText(),"�������ͳ�Ʊ�") == 0 )
	{
		nType = 7;
	}
	else if(strType == "ֱ���������" )
	{
		nType = 71;
	}
	else if( strType == "����������" )
	{
		nType = 72;
	}
	else if( strType =="��Ŀ������" )
	{
		nType = 73;
	}
	else if( strcmp(pItem->GetText(),"פ������������") == 0 )
	{
		nType = 9;
	}
	else if(strType =="֣��" || strType =="���"|| strType =="˳��"|| strType =="��ɳ" || strType =="�ɶ�")
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