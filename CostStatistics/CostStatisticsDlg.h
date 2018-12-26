
// CostStatisticsDlg.h : ͷ�ļ�
//

#pragma once
#include <map>
#include "SplitDlg/SplitDialog.h"

// CCostStatisticsDlg �Ի���
class CCostStatisticsDlg : public CSplitDialog, public IVTabItem, public ISpliterDialogListener 
{
// ����
public:
	CCostStatisticsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnGridClick(NMHDR * pNmhdr, LRESULT * pLresult);

	afx_msg void OnCancelAutoLogin();	//ȡ���Զ���¼
	afx_msg void OnMenuExitSystem();	//�˳�ϵͳ

	afx_msg void OnMenuAddAccount();	//����˺�
	afx_msg void OnMenuDelAccount();	//ɾ���˺�
	afx_msg void OnMenuChangePwd();		//�޸�����

	afx_msg void OnMenuManuBase();		//�������
	afx_msg void OnMenuAbout();          //����
	afx_msg void OnMenuStaffSalary();      //ְ��н��
	afx_msg void OnMenuWorkShopTotal();    //����ϼ�
	afx_msg void OnMenuCostTotal();        //������úϼ�
	afx_msg void OnMenuOverSeaProduce();   //פ���������

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
