#pragma once
#include "rightctrlwnd.h"
#include "libResource.h"
class CCustomWndContainer :
	public CRightCtrlWnd
{
	DECLARE_DYNAMIC(CCustomWndContainer)
public:
	CCustomWndContainer(CWnd* pParent =NULL);
	~CCustomWndContainer(void);
	
	enum { IDD = IDD_DLG_BLANK };
	void SetResizeMoveFlag(int flag = true) { m_resizeMoveFlag = flag; }
	int GetResizeMoveFlag() { return m_resizeMoveFlag; }

	virtual void Load(AbsCtrlData* pData){}
	virtual AbsCtrlData* Save(){ return NULL;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV suppor
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	CSize m_minSize;
	CWnd *m_pInnerWnd;
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnPaint();
protected:
	int m_resizeMoveFlag;
	UINT m_nHitTest;
};
