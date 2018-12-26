// ConfigDbServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CostStatisticsDlg.h"
#include "ConfigdbDlg.h"


// CConfigDbServerDlg �Ի���

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


// CConfigDbServerDlg ��Ϣ�������


BOOL CConfigDBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	char szTemp[32] = {0};

	m_EditIP.SetWindowText(BaseLib::TSingleton<CServerCofig>::Instance()->GetIP());

	sprintf(szTemp,"%d",BaseLib::TSingleton<CServerCofig>::Instance()->GetPort());
	m_EditPort.SetWindowText(szTemp);

	memset(szTemp,0,sizeof(szTemp));
	sprintf(szTemp,"%d",BaseLib::TSingleton<CServerCofig>::Instance()->GetCode());
	m_EditNumber.SetWindowText(szTemp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CConfigDBDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strIp = _T("");
	CString strPort = _T("");
	CString strNumber = _T("0");

	m_EditIP.GetWindowText(strIp);
	m_EditPort.GetWindowText(strPort);
	//m_EditNumber.GetWindowText(strNumber);

	BaseLib::TSingleton<CServerCofig>::Instance()->WriteConfig(strIp,strPort,strNumber);

	PRINT_LOG("�������ݿ���Ϣ" << strIp << " " << strPort << " " << strNumber);

	OnOK();
}
