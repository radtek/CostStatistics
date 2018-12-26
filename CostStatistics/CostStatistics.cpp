
// CostStatistics.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "CostStatistics.h"
#include "CostStatisticsDlg.h"
#include "FrameDlg.h"
#include "SecretHelper.h"
#include "MD5.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CCostStatisticsApp

BEGIN_MESSAGE_MAP(CCostStatisticsApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCostStatisticsApp 构造

CCostStatisticsApp::CCostStatisticsApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCostStatisticsApp 对象

CCostStatisticsApp theApp;


// CCostStatisticsApp 初始化

BOOL CCostStatisticsApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//CCostStatisticsDlg dlg;
	/*CFrameDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();*/

	SET_LOGINIT(TRUE,1024*1024);

	CFrameDlg dlgMain;
	CSecretHelper secretHelperObj;

	INT_PTR nResponse = NULL;

	std::string mPwd = "";
	std::string mUser = "";
	if( secretHelperObj.IsAutoStart(mUser,mPwd) )
	{
		//验证密码
		MD5 md5Obj(mPwd.c_str());
		SQL_RESULT rt;
		if( CSqlBusiness::VerifyUser(mUser.c_str(),md5Obj.md5().c_str(),rt) == 0 )
		{
			//互斥窗口
			HANDLE m_hMutex = CreateMutex(NULL, TRUE, _T("CostStatistics")); 
			if ((m_hMutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) 
			{
				PRINT_LOG("窗口已经存在");
				HWND oldHWnd = NULL;
				EnumWindows(EnumWndProc, (LPARAM)&oldHWnd);
				::ShowWindow(oldHWnd,SW_SHOW);    //激活找到的前一个程序
				::SetForegroundWindow(oldHWnd);   //把它设为前景窗口

				exit(-1);
				return FALSE;
			}

			m_pMainWnd = &dlgMain;
			dlgMain.DoModal();
		}
		else
		{
			::MessageBox(NULL,"用户名密码验证失败请重新填写！","提示：",MB_OK|MB_ICONINFORMATION);
			dlgLogIn.SetInfo(mUser.c_str(),mPwd.c_str());
			if( dlgLogIn.DoModal() == IDOK )
			{
				m_pMainWnd = &dlgMain;
				dlgMain.DoModal();
			}
		}
	}
	else
	{
		dlgLogIn.SetInfo(mUser.c_str(),mPwd.c_str());
		if( dlgLogIn.DoModal() == IDOK )
		{
			m_pMainWnd = &dlgMain;
			dlgMain.DoModal();
		}
	}

	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else
	{
		::MessageBox(NULL,"启动失败!","提示：",MB_OK|MB_ICONWARNING);
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
