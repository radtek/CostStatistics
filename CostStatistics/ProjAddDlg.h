#pragma once


// CProjAddDlg 对话框

class CProjAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CProjAddDlg)

public:
	CProjAddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProjAddDlg();

// 对话框数据
	enum { IDD = IDD_PROJ_ADD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
