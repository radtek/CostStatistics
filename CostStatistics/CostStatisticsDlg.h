
// CostStatisticsDlg.h : 头文件
//

#pragma once
#include <map>
#include "SplitDlg/SplitDialog.h"

// CCostStatisticsDlg 对话框
class CCostStatisticsDlg : public CSplitDialog, public IVTabItem, public ISpliterDialogListener 
{
// 构造
public:
	CCostStatisticsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnGridClick(NMHDR * pNmhdr, LRESULT * pLresult);

	afx_msg void OnCancelAutoLogin();	//取消自动登录
	afx_msg void OnMenuExitSystem();	//退出系统

	afx_msg void OnMenuAddAccount();	//添加账号
	afx_msg void OnMenuDelAccount();	//删除账号
	afx_msg void OnMenuChangePwd();		//修改密码

	afx_msg void OnMenuManuBase();		//制造基地
	afx_msg void OnMenuAbout();          //关于
	afx_msg void OnMenuStaffSalary();      //职工薪酬
	afx_msg void OnMenuWorkShopTotal();    //车间合计
	afx_msg void OnMenuCostTotal();        //制造费用合计
	afx_msg void OnMenuOverSeaProduce();   //驻外基地生产

	DECLARE_MESSAGE_MAP()
public:
	virtual CString		GetTitle();	
	virtual CDialog *	GetPage();
	virtual void		Print();
	virtual void		Export();
	virtual BOOL        CreatePage( CWnd * p );
	virtual BOOL		Show( int nCmdShow ); 
	virtual void		Move( LPCRECT rect ); 

public:
	void OnToolBarMenuClick(int nID);
	void OnLeftTreeSelChange(CTreeListItem* pItem);

private:
	void InitInterface();
	void Repaint();

	void CreateGrid();

	void readData();

	
private:
	CPaneEX*	m_pPaneMain;
	CGridCtrl	m_Grid;
	CTreeListCtrl m_list;
	std::map<int, GDS::GridColHeadAttr> m_mapHeader;
	
};
