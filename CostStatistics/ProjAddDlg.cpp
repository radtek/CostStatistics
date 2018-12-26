// ProjAddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CostStatistics.h"
#include "ProjAddDlg.h"


// CProjAddDlg 对话框

IMPLEMENT_DYNAMIC(CProjAddDlg, CDialog)

CProjAddDlg::CProjAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProjAddDlg::IDD, pParent)
{

}

CProjAddDlg::~CProjAddDlg()
{
}

void CProjAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProjAddDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CProjAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CProjAddDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CProjAddDlg 消息处理程序

BOOL CProjAddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ?????í??????????????

	return TRUE;  // return TRUE unless you set the focus to a control
	// ?ì??: OCX ????????·??? FALSE
}

void CProjAddDlg::OnBnClickedOk()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	OnOK();
}

void CProjAddDlg::OnBnClickedCancel()
{
	// TODO: ÔÚ´ËÌí¼Ó¿Ø¼þÍ¨Öª´¦Àí³ÌÐò´úÂë
	OnCancel();
}
