// DlgUserMgr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CostStatistics.h"
#include "DlgUserMgr.h"
#include "MD5.h"


// CDlgUserMgr �Ի���

IMPLEMENT_DYNAMIC(CDlgUserMgr, CDialog)

CDlgUserMgr::CDlgUserMgr(EN_MYWND_TYPE enType,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserMgr::IDD, pParent),m_enType(enType)
{

}

CDlgUserMgr::~CDlgUserMgr()
{
}

void CDlgUserMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_NAME,m_EditName);
	DDX_Control(pDX,IDC_EDIT_USER,m_EditAccount);
	DDX_Control(pDX,IDC_EDIT_PWD,m_EditPwd);
	DDX_Control(pDX,IDC_EDIT_PWD2,m_EditPwd2);

	DDX_Control(pDX,IDC_STATIC_0,m_StaticName);
	DDX_Control(pDX,IDC_STATIC_1,m_StaticUser);
	DDX_Control(pDX,IDC_STATIC_2,m_StaticPwd);
	DDX_Control(pDX,IDC_STATIC_3,m_StaticPwd2);
}


BEGIN_MESSAGE_MAP(CDlgUserMgr, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgUserMgr::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgUserMgr::OnBnClickedCancel)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgUserMgr ��Ϣ�������


BOOL CDlgUserMgr::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	if( m_enType == EN_ADD )
	{
		this->SetWindowText("����˺�");
	}
	else if( m_enType == EN_DEL )
	{
		this->SetWindowText("ɾ���˺�");

		m_StaticName.ShowWindow(SW_HIDE);
		m_EditName.ShowWindow(SW_HIDE);

		m_StaticUser.ShowWindow(SW_HIDE);
		m_EditAccount.ShowWindow(SW_HIDE);

		m_EditPwd.ShowWindow(SW_SHOW);
		m_StaticPwd.ShowWindow(SW_SHOW);
		
		m_StaticPwd2.ShowWindow(SW_HIDE);
		m_EditPwd2.ShowWindow(SW_HIDE);

		m_StaticPwd.SetWindowText("�˺ţ�");
		m_EditPwd.SetPasswordChar(NULL);
	}
	else if( m_enType == EN_CNG )
	{
		m_StaticName.ShowWindow(SW_HIDE);
		m_EditName.ShowWindow(SW_HIDE);

		m_StaticPwd.SetWindowText("�����룺");
		this->SetWindowText("��������");
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgUserMgr::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}


void CDlgUserMgr::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strName = "";
	CString strAccount = "";
	CString strPwd1 = "";
	CString strPwd2 = "";

	m_EditName.GetWindowText(strName);
	m_EditAccount.GetWindowText(strAccount);
	m_EditPwd.GetWindowText(strPwd1);
	m_EditPwd2.GetWindowText(strPwd2);

	if( m_enType == EN_ADD || m_enType == EN_CNG )
	{
		if( ((m_enType == EN_ADD) && (strName.IsEmpty() || strPwd2.IsEmpty() || strAccount.IsEmpty() || strPwd2.IsEmpty())) ||
			((m_enType == EN_CNG) && (strPwd2.IsEmpty() || strAccount.IsEmpty() || strPwd2.IsEmpty())) )
		{
			::MessageBox(this->GetSafeHwnd(),"�������п��","��ʾ��",MB_OK|MB_ICONWARNING);
			return;
		}


		if( strPwd1.Compare(strPwd2) != 0  )
		{
			::MessageBox(this->GetSafeHwnd(),"�����������벻һ�£�","��ʾ��",MB_OK|MB_ICONWARNING);
			return;
		}
	}


	bool bRet = false;
	int nRet = -1;
	if( m_enType == EN_ADD )
	{
		if( CSqlBusiness::IsAccountExist(strAccount) )
		{
			::MessageBox(this->GetSafeHwnd(),"�˺��Ѵ��ڣ�","��ʾ��",MB_OK|MB_ICONWARNING);
			return;
		}

		MD5 md5Obj(strPwd1.GetBuffer());
		bRet = CSqlBusiness::AddAccount(strName,strAccount,md5Obj.md5().c_str());
		strPwd1.ReleaseBuffer();
	}
	if( m_enType == EN_DEL )
	{
		if( !CSqlBusiness::IsAccountExist(strAccount) )
		{
			::MessageBox(this->GetSafeHwnd(),"�˺Ų����ڣ�","��ʾ��",MB_OK|MB_ICONWARNING);
			return;
		}

		CString strAccountTmp = strPwd1;
		if( strAccountTmp.IsEmpty() )
		{
			::MessageBox(this->GetSafeHwnd(),"�˺Ų�����Ϊ�գ�","��ʾ��",MB_OK|MB_ICONWARNING);
			return;
		}

		nRet = CSqlBusiness::DelAccount(strAccountTmp);//strpwd1�д�ŵ����û���

	}
	else if( m_enType == EN_CNG )
	{
		if( !CSqlBusiness::IsAccountExist(strAccount) )
		{
			::MessageBox(this->GetSafeHwnd(),"�˺Ų����ڣ�","��ʾ��",MB_OK|MB_ICONWARNING);
			return;
		}

		MD5 md5Obj(strPwd1.GetBuffer());
		bRet = CSqlBusiness::ResetPwd(strAccount,md5Obj.md5().c_str());
		strPwd1.ReleaseBuffer();
	}

	if( bRet )
	{
		::MessageBox(this->GetSafeHwnd(),"�ɹ���","��ʾ��",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		::MessageBox(this->GetSafeHwnd(),"ʧ�ܣ�","��ʾ��",MB_OK|MB_ICONWARNING);
		return;
	}

	OnOK();
}

void CDlgUserMgr::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
