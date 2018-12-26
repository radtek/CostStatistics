#pragma once
#include "SubWndHelper.h"
#include "libResource.h"


class CToolbarEh;
class CRightCtrlWnd;
class ToolbarListener;
class CCustomWnd;



class CPersonWnd : public CDialog
{
	DECLARE_DYNAMIC(CPersonWnd)

public:
	CPersonWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPersonWnd();

// Dialog Data
	enum { IDD = IDD_PERSON_WND };
public:
	virtual BOOL OnInitDialog();
	ToolbarListener *m_pToolbarListener;
public:
	int GetItemCount();
	
    void Reposition();
    void UpdateWindowSize();
	void ShowToolbar(bool bShow);
    CWnd* AddSubWnd(CWnd *pWnd);
	CWnd* GetActivSubWnd();
    void RemoveSubWnd(CWnd* pWnd);
    void MoveSubWnd(const CWnd* pWnd, int step);
	void MoveSubWndToTop(const CWnd* pWnd);
    void SetResizeMoveFlag(int flag = true) { m_resizeMoveFlag = flag; }
    int GetResizeMoveFlag() { return m_resizeMoveFlag; }
	CToolbarEh* CreateToolbar();
	void PopupEditDlg();
	void ExpandCollapse(bool bExpand);
    void RemoveAll();
	
	BOOL m_bShowToolbar;
	void Load(PersonData& data);
	void Save(PersonData& data);
	
protected:
	void LoadDataToCtrl(CWnd* pCtrlWnd, AbsCtrlData* pData);
	CRightCtrlWnd* CreateCustomWnd(CWnd *pWnd, CSize* pSize);
public:
    CString m_name;
	CToolbarEh* m_pToolbar;
	CWnd*       m_pActiveCtrl;

protected:
	CImageList	m_treeImageList;
	list<CWnd*> m_testCtrlList;
	CSubWndHelper m_subWndHelper;
    int			m_ctrlId;
    HBRUSH		m_hBrush;
    int			m_resizeMoveFlag;
	int			m_cx;
	COLORREF	m_backColor;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK() override;
	virtual void OnCancel() override;

	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
    LRESULT OnSubWndSizeChanged(WPARAM wParam, LPARAM lParam);
    LRESULT OnToolbarClicked(WPARAM wParam, LPARAM lParam);
	LRESULT OnControlActived(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
public:
	afx_msg void OnDestroy();
};
