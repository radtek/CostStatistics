// ProjAddDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CostStatistics.h"
#include "ProjAddDlg.h"


// CProjAddDlg �Ի���

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
END_MESSAGE_MAP()


// CProjAddDlg ��Ϣ�������

BOOL CProjAddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ?????��??????????????

	return TRUE;  // return TRUE unless you set the focus to a control
	// ?��??: OCX ????????��??? FALSE
}
