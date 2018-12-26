#pragma once

#include "resource.h"
#include "Common.h"
#include "SubWndHelper.h"

class CToolbarEh;
class CPersonWnd;
class CThingWnd : public CDialog
{
	DECLARE_DYNAMIC(CThingWnd)

public:
	CThingWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CThingWnd();

// Dialog Data
	enum { IDD = IDD_THING_WND };

public:
	virtual BOOL OnInitDialog();
	void ShowToolbar(BOOL bShow);
public:
	void UpdateWindowSize();
	CToolbarEh* CreateToolbar();
	
	CWnd* AddSubWnd(E_WindowType type, bool bIsLoad = false);
	void RemoveSubWnd(CWnd* pWnd);
	void MoveSubWnd(const CWnd* pWnd, int step);
	void MoveSubWndToTop(const CWnd* pWnd);
	void Reposition();
	void ExpandCollapse(bool bExpand);
    void RemoveAll();
	CPersonWnd* FindPerson(CString name);

	void Load(ThingData& data);
	void Save(ThingData & data);

public:
    CString m_name;
	vector<ToolbarBtnData> m_toolBarCfg;
	CToolbarEh* m_pToolbar;
    CWnd* m_pActivePerson;

protected:
	CImageList	m_treeImageList;
	list<CWnd*> m_testCtrlList;
	CSubWndHelper m_subWndHelper;
	int			m_ctrlId;
	HBRUSH		m_hBrush;
	int			m_resizeMoveFlag;
	int			m_cx;
	COLORREF	m_backColor;
	BOOL m_bShowToolbar;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK() override;
	virtual void OnCancel() override;

	DECLARE_MESSAGE_MAP()
protected:
	LRESULT OnToolbarClicked(WPARAM wParam, LPARAM lParam);
	LRESULT OnSubWndSizeChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
    LRESULT OnPersonActived(WPARAM wParam, LPARAM lParam);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

};
