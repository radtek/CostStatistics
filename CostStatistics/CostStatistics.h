
// CostStatistics.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "LoginDlg.h"


// CCostStatisticsApp:
// �йش����ʵ�֣������ CostStatistics.cpp
//

class CCostStatisticsApp : public CWinAppEx
{
public:
	CCostStatisticsApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	CLoginDlg dlgLogIn;
};

extern CCostStatisticsApp theApp;