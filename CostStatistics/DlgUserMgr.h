#pragma once

typedef enum _enMyWndType
{
	EN_ADD = 1,
	EN_DEL = 2,
	EN_CNG = 3
}EN_MYWND_TYPE;

// CDlgUserMgr 对话框

class CDlgUserMgr : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserMgr)

public:
	CDlgUserMgr(EN_MYWND_TYPE enType,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUserMgr();

// 对话框数据
	enum { IDD = IDD_USER_MGR_DLALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void Repaint();

private:
	EN_MYWND_TYPE m_enType;
	CEdit m_EditName;
	CEdit m_EditAccount;
	CEdit m_EditPwd;
	CEdit m_EditPwd2;
	CStatic m_StaticName;
	CStatic m_StaticUser;
	CStatic m_StaticPwd;
	CStatic m_StaticPwd2;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
