#pragma once


// CProjAddDlg �Ի���

class CProjAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CProjAddDlg)

public:
	CProjAddDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjAddDlg();

// �Ի�������
	enum { IDD = IDD_PROJ_ADD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
