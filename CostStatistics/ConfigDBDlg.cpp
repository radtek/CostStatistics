// ConfigDbServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CostStatisticsDlg.h"
#include "ConfigdbDlg.h"


// CConfigDbServerDlg 对话框

IMPLEMENT_DYNAMIC(CConfigDBDlg, CDialog)

CConfigDBDlg::CConfigDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDBDlg::IDD, pParent)
{

}

CConfigDBDlg::~CConfigDBDlg()
{
}

void CConfigDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_IP,m_EditIP);
	DDX_Control(pDX,IDC_EDIT_PORT,m_EditPort);
	DDX_Control(pDX,IDC_EDIT_NUMBER,m_EditNumber);
}


BEGIN_MESSAGE_MAP(CConfigDBDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigDBDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigDbServerDlg 消息处理程序


BOOL CConfigDBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	char szTemp[32] = {0};

	m_EditIP.SetWindowText(BaseLib::TSingleton<CServerCofig>::Instance()->GetIP());

	sprintf(szTemp,"%d",BaseLib::TSingleton<CServerCofig>::Instance()->GetPort());
	m_EditPort.SetWindowText(szTemp);

	memset(szTemp,0,sizeof(szTemp));
	sprintf(szTemp,"%d",BaseLib::TSingleton<CServerCofig>::Instance()->GetCode());
	m_EditNumber.SetWindowText(szTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConfigDBDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CString strIp = _T("");
	CString strPort = _T("");
	CString strNumber = _T("0");

	m_EditIP.GetWindowText(strIp);
	m_EditPort.GetWindowText(strPort);
	//m_EditNumber.GetWindowText(strNumber);

	BaseLib::TSingleton<CServerCofig>::Instance()->WriteConfig(strIp,strPort,strNumber);

	PRINT_LOG("配置数据库信息" << strIp << " " << strPort << " " << strNumber);

	OnOK();
}
