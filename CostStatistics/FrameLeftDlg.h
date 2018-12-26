#pragma once
#include "LeftTreeInfo.h"



class CFrameLeftDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrameLeftDlg)

public:
	CFrameLeftDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFrameLeftDlg();

// 对话框数据
	enum { IDD = IDD_FRAMELEFT_DIALOG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemSelChanged( NMHDR * pNotifyStruct, LRESULT * result );
	afx_msg void OnTreeCheck( NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg void OnTreeRClick( NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()
	
private:
	void CreateTree();
	void InitTreeData();
	static UINT32 WINAPI LoadTreeData(void* pArgument);

private:
	CImageList m_imgList;
	CFont m_ftList;
	CTreeListCtrl* m_pTreeList;
	CTreeListItem* m_pCurTreeListItem;
	std::vector<CTreeListItem*> m_vecTreeCopyItem;

	HANDLE m_hThread;
	unsigned int m_uThreadId;

};