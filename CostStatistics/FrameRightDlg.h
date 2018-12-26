#pragma once

#include "resource.h"



#define LEFT_TREE_WIDTH 300 //左边树宽度


// CFrameLogSendWnd 对话框

class CFrameRightDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrameRightDlg)

public:
	friend class CSearchConditon;

public:
	CFrameRightDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFrameRightDlg();

// 对话框数据
	enum { IDD = IDD_FRAMERIGHT_DIALOG };

public:
	virtual BOOL OnInitDialog();
	void ShowTabDlg(int index);//显示对应的TabCtrl中的对话框

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	std::vector<IVTabItem *> m_vTabItems;   // 存放Tab的表内容
private:
	CTabCtrl      m_tabCtrl;
	int           m_curTabItem;

	int           m_Format;

	CFont m_Font;//
	CImageList  m_imgList;

private:
	void CreateTabCtrl();
	void Repaint(int format = 0);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
