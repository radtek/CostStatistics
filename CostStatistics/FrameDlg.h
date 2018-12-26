#pragma once

#include "resource.h"


// CFrameDlg 对话框

class CFrameDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrameDlg)

public:
	CFrameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFrameDlg();

// 对话框数据
	enum { IDD = IDD_FRAME_DIALOG };

public:
	//ICModuleCommunListener的消息, 来自网络
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CFrameWnd * m_pZTUIMainFrame;
public:
	virtual BOOL OnInitDialog();
	void SetMyWindowText(CString strTitle); //设置当前窗口名称

private:
	void InitMyChildWnd();

	void InitLeftToolbar();		//初始化左边Pane的工具条
	void InitRightToolbar();	//初始化右边Pane的工具条
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCancel();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();

private:
	CString          m_sDeptList;
	CTreeListItem*   m_companyTreeItem;

	bool			m_bLoadTwoLevelCmp;
	map<int, CString>   m_deptNameMap; //[deptId, deptName];
};
