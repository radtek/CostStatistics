#pragma once


// CProjMgrDlg �Ի���

class CProjMgrDlg : public CDialog
{
	DECLARE_DYNAMIC(CProjMgrDlg)

public:
	CProjMgrDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CProjMgrDlg();

// �Ի�������
	enum { IDD = IDD_PROJ_MGR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	void CreateGrid();
	void InitHeader();
	void Repaint();
private:
	CFont       m_Font;
	CGridCtrl	m_Grid;
	std::map<int, GDS::GridColHeadAttr> m_mapHeader;
	//CButtonST		m_BtnAdd;
	CMenu*			m_pMenu;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnAdd();
	afx_msg void OnImport();
};
