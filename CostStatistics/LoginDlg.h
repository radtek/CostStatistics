#pragma once


// CLoginDlg �Ի���

#include "resource.h"

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);
	CLoginDlg(CString strUser, CString strPwd, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoginDlg();

// �Ի�������
	enum { IDD = IDD_LOGIN_DIALOG };

public:
	//�������ݳ�ʼ���߳�
	bool CreateInitDataThread();
	static UINT32 WINAPI InitDataThread(void* pArgument);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSetting();
	afx_msg LRESULT OnInitOK(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnInitCancel(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedCheckAutoLogin();
	virtual BOOL OnInitDialog();
public:
	void SetInfo(CString strUser, CString strPwd);
private:
	bool Init();

private:
	HANDLE m_hThread;
	bool m_bLinkDataBaseOk;
	TCHAR m_szSecretPath[MAX_PATH];       //�����ļ�·��
	char m_szSecret[32];					//�����ļ�����

	CString m_strUser;
	CString m_strPwd;

	CButtonST m_BtnSetting;
	CButtonST m_BtnOK;
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
