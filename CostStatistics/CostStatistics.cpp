
// CostStatistics.cpp : ����Ӧ�ó��������Ϊ��
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


// CCostStatisticsApp ����

CCostStatisticsApp::CCostStatisticsApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCostStatisticsApp ����

CCostStatisticsApp theApp;


// CCostStatisticsApp ��ʼ��

BOOL CCostStatisticsApp::InitInstance()
{
	CWinAppEx::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

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
		//��֤����
		MD5 md5Obj(mPwd.c_str());
		SQL_RESULT rt;
		if( CSqlBusiness::VerifyUser(mUser.c_str(),md5Obj.md5().c_str(),rt) == 0 )
		{
			//���ⴰ��
			HANDLE m_hMutex = CreateMutex(NULL, TRUE, _T("CostStatistics")); 
			if ((m_hMutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) 
			{
				PRINT_LOG("�����Ѿ�����");
				HWND oldHWnd = NULL;
				EnumWindows(EnumWndProc, (LPARAM)&oldHWnd);
				::ShowWindow(oldHWnd,SW_SHOW);    //�����ҵ���ǰһ������
				::SetForegroundWindow(oldHWnd);   //������Ϊǰ������

				exit(-1);
				return FALSE;
			}

			m_pMainWnd = &dlgMain;
			dlgMain.DoModal();
		}
		else
		{
			::MessageBox(NULL,"�û���������֤ʧ����������д��","��ʾ��",MB_OK|MB_ICONINFORMATION);
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
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else
	{
		::MessageBox(NULL,"����ʧ��!","��ʾ��",MB_OK|MB_ICONWARNING);
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
