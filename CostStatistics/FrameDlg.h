#pragma once

#include "resource.h"


// CFrameDlg �Ի���

class CFrameDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrameDlg)

public:
	CFrameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFrameDlg();

// �Ի�������
	enum { IDD = IDD_FRAME_DIALOG };

public:
	//ICModuleCommunListener����Ϣ, ��������
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CFrameWnd * m_pZTUIMainFrame;
public:
	virtual BOOL OnInitDialog();
	void SetMyWindowText(CString strTitle); //���õ�ǰ��������

private:
	void InitMyChildWnd();

	void InitLeftToolbar();		//��ʼ�����Pane�Ĺ�����
	void InitRightToolbar();	//��ʼ���ұ�Pane�Ĺ�����
	
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
