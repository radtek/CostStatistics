// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LoginDlg.h"
#include "SecretHelper.h"
#include "MD5.h"
#include "ConfigDBDlg.h"
#include "CostStatisticsDlg.h"

// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CString strUser, CString strPwd,CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent),m_strUser(strUser),m_strPwd(strPwd)
{
	m_bLinkDataBaseOk = false;
}

CLoginDlg::CLoginDlg(CWnd* pParent /*= NULL*/): CDialog(CLoginDlg::IDD, pParent)
{
	m_bLinkDataBaseOk = false;
}



CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_BTN_SETTING,m_BtnSetting);
	DDX_Control(pDX,IDOK,m_BtnOK);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLoginDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_SETTING,&CLoginDlg::OnBnClickedSetting)
	ON_MESSAGE(MSG_INIT_OK,&CLoginDlg::OnInitOK)
	ON_MESSAGE(MSG_INIT_CALCEL,&CLoginDlg::OnInitCancel)
	ON_BN_CLICKED(IDC_CHECK_AUTO_LOGIN, &CLoginDlg::OnBnClickedCheckAutoLogin)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_LOGO/*IDR_MAINFRAME*/);
	SetIcon(hIcon, TRUE);		// 设置大图标
	SetIcon(hIcon, FALSE);		// 设置小图标

	((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->SetCheck(TRUE);

	GetDlgItem(IDC_EDIT1)->SetWindowText(m_strUser);
	GetDlgItem(IDC_EDIT2)->SetWindowText(m_strPwd);

	CreateInitDataThread();

	m_BtnOK.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(204,232,207));
	m_BtnOK.SetFlat(FALSE);

	m_BtnSetting.SetIcon(IDI_ICON_SETTING);
	m_BtnSetting.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(204,232,207)); 
	m_BtnSetting.DrawBorder(FALSE); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CWaitCursor obj;

	m_strUser = _T("");
	m_strPwd = _T("123456");

	GetDlgItem(IDC_EDIT1)->GetWindowText(m_strUser);
	GetDlgItem(IDC_EDIT2)->GetWindowText(m_strPwd);

	if( m_strUser.IsEmpty()  )
	{
		::MessageBox(this->GetSafeHwnd(),"用户名不允许为空！","提示：",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_EDIT1)->SetFocus();
		return;
	}

	if( m_strPwd.IsEmpty() )
	{
		::MessageBox(this->GetSafeHwnd(),"密码不允许为空！","提示：",MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_EDIT2)->SetFocus();
		return;
	}

	//int nPwdLen = m_strPwd.GetLength();
	////验证数字、字母
	//for(int i = 0;i<nPwdLen;i++)
	//{
	//	if(!isalnum(m_strPwd.GetAt(i)))
	//	{
	//		::MessageBox(this->GetSafeHwnd(),"输入密码只能为数字、字母！","提示：",MB_OK|MB_ICONWARNING);
	//		return;
	//	}
	//}

	
	CString strMsgInfo = _T("");

	//初始化数据库
	CDbOperator::GetInstance()->SetDbInfo(MySQL,
		BaseLib::TSingleton<CServerCofig>::Instance()->GetIP(),
		BaseLib::TSingleton<CServerCofig>::Instance()->GetPort(),
		"root",
		"123456",
		"costanalysis");

	MD5 md5Obj(m_strPwd.GetBuffer());
	CString strEncryPwd = md5Obj.md5().c_str();
	SQL_RESULT rt;
	int nResult = CSqlBusiness::VerifyUser(m_strUser,strEncryPwd,rt);
	if( nResult == 0 )
	{
		//ItemID,Name,Account,Pwd,IsAdmind

		//记录当前登录账户信息
		CDBResultOper obj;
		obj.SetResultMap(rt);
		int nRow = obj.GetRowCount();

		if( nRow != 0 )
		{
			CLoginUserInfo::m_nUserID	= atoi(obj.GetValue("ItemID",0));
			CLoginUserInfo::m_strUserName = obj.GetValue("Name",0);
			CLoginUserInfo::m_strAccount = m_strUser;
			CLoginUserInfo::m_strPwd = strEncryPwd;
			CLoginUserInfo::m_bIsAdmin = atoi(obj.GetValue("IsAdmind",0));
		}
		
		//记住密码
		BOOL bAutoLogin =  ((CButton*)GetDlgItem(IDC_CHECK_AUTO_LOGIN))->GetCheck();
		BOOL bCheckRemeber = ((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->GetCheck();
		CSecretHelper secretObj;
		secretObj.RememberPwd(m_strUser.GetBuffer(0),m_strPwd.GetBuffer(0),bAutoLogin,bCheckRemeber);

		Sleep(1000);
		this->ShowWindow(SW_HIDE);
	}
	else if( nResult == 1 )
	{
		strMsgInfo = "网络错误，请重试！";
	}
	else if( nResult == 2 )//密码错误
	{
		strMsgInfo = "密码错误！";
	}
	else if( nResult == 3 ) //用户不存在
	{
		strMsgInfo = "用户不存在,请联系管理员！";
	}
	
	if( !strMsgInfo.IsEmpty() )
	{
		this->ShowWindow(SW_SHOW);
		::MessageBox(this->GetSafeHwnd(),strMsgInfo,"提示：",MB_OK|MB_ICONWARNING);
		return;
	}

	Sleep(500);

	
	
	OnOK();
}

void CLoginDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
	OnCancel();
}


bool CLoginDlg::CreateInitDataThread()
{
	unsigned threadID;

	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &InitDataThread, this, 0, &threadID );

	if( m_hThread == NULL )
	{
		return false;
	}

	return true;
}

UINT32 WINAPI CLoginDlg::InitDataThread(void* pArgument)
{
	CLoginDlg* pThis = (CLoginDlg*)pArgument;

	return pThis->Init();
}

bool CLoginDlg::Init()
{
	bool bRet = false;

	while( TRUE )
	{
		//SetInitInfo("正在初始化数据服务器，请稍后......");

		if(TRUE /* g_DBHelper.Init(
			BaseLib::TSingleton<CServerCofig>::Instance()->GetIP(),
			BaseLib::TSingleton<CServerCofig>::Instance()->GetPort(),
			0)*/ )
		{
			break;
		}
		else
		{
			m_bLinkDataBaseOk = false;
			//SetInitInfo("初始化数据服务器失败！");
		}

		SetWindowPos(&this->wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
		this->ShowWindow(SW_HIDE);

		CConfigDBDlg dlg;

		if( dlg.DoModal() == IDCANCEL )
		{
			PostMessage(MSG_INIT_CALCEL,0,0);
		}
		else
		{
			SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE); 
			this->ShowWindow(SW_SHOW);
		}

		Sleep(100);
	}

	//PostMessage(MSG_INIT_OK,0,0);

	m_bLinkDataBaseOk = true;

	return bRet;
}

LRESULT CLoginDlg::OnInitOK(WPARAM wParam,LPARAM lParam)
{
	CCostStatisticsDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CLoginDlg::OnInitCancel(WPARAM wParam,LPARAM lParam)
{
	//取消初始化，退成程序
	//ICSocketBase::ReleaseNetWork();
	ExitProcess(0);
	return 0;
}

void CLoginDlg::OnBnClickedCheckAutoLogin()
{
	// TODO: 在此添加控件通知处理程序代码

	if( ((CButton*)GetDlgItem(IDC_CHECK_AUTO_LOGIN))->GetCheck() )
	{
		((CButton*)GetDlgItem(IDC_CHECK_REMEMBER))->SetCheck(TRUE);
	}
	
}

void CLoginDlg::SetInfo(CString strUser, CString strPwd)
{
	m_strUser = strUser;
	m_strPwd = strPwd;
}

void CLoginDlg::OnBnClickedSetting()
{
	CConfigDBDlg dlg;
	dlg.DoModal();
}

void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	CBitmap   bitmap;                            //定义位图
	bitmap.LoadBitmap(IDB_BITMAP_LOGINBK);    //这个IDB_BITMAP1要自己添加
	CBrush   brush;                              
	brush.CreatePatternBrush(&bitmap);
	CBrush*   pOldBrush   =   dc.SelectObject(&brush);

	CRect Rect;
	GetClientRect(&Rect);
	dc.Rectangle(0,0,Rect.Width(),Rect.Height());

	dc.SelectObject(pOldBrush);
}

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	UINT id = pWnd->GetDlgCtrlID();
	if( id == IDC_STATIC_PWD || 
		id == IDC_STATIC_USER ||
		id == IDC_CHECK_REMEMBER ||
		id == IDC_CHECK_AUTO_LOGIN  )
	{
		pDC->SetBkMode(TRANSPARENT);

		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CDC* dc = GetDC();

		pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);  //把父窗口背景图片先画到按钮上

		ReleaseDC(dc);

		hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
	}   

	return hbr;
}
