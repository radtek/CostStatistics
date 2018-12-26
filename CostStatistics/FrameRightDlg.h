#pragma once

#include "resource.h"



#define LEFT_TREE_WIDTH 300 //��������


// CFrameLogSendWnd �Ի���

class CFrameRightDlg : public CDialog
{
	DECLARE_DYNAMIC(CFrameRightDlg)

public:
	friend class CSearchConditon;

public:
	CFrameRightDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFrameRightDlg();

// �Ի�������
	enum { IDD = IDD_FRAMERIGHT_DIALOG };

public:
	virtual BOOL OnInitDialog();
	void ShowTabDlg(int index);//��ʾ��Ӧ��TabCtrl�еĶԻ���

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	std::vector<IVTabItem *> m_vTabItems;   // ���Tab�ı�����
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
