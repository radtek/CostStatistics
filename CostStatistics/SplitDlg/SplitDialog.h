/*********************************************************************************
 描述：分割窗口函数声明
 作者：syxk development group
 时间：2012.11.15
 修改：暂无
*********************************************************************************/


#if !defined(_SPLITDIALOG_H_INCLUDED_)
#define _SPLITDIALOG_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplitDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitDialog dialog

#include "PaneEX.h"

//
// Sometimes need a listener instead of WM_SIZE message (IM team 2016/12/28)
//
class ISpliterDialogListener {

public:
	virtual void OnSplitMoveBegin(CPaneEX * pPanel,int nSplitPosX,int nSplitPosY) {};
	virtual void OnSplitMoveing(CPaneEX * pPanel,int nSplitPosX,int nSplitPosY) {};
	virtual void OnSplitMoveEnd(CPaneEX * pPanel,int nSplitPosX,int nSplitPosY) {};
};

class CSplitDialog : public CDialog
{
// Construction
public:
	CSplitDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
  virtual ~CSplitDialog();

  CPaneEX *m_pMainPane; //The main Pane

  void Initialize();
  CPaneEX *CreatePane(int nSplitType, CPaneEX *pParent = NULL, int nWhich = 0, BOOL bSizeableControls = TRUE);
  BOOL SetPaneWnd(CPaneEX *pPane, CWnd *pWnd, int nWhich, int minWidth = 100, int minHeight = 100, int Width = 0, int Height = 0);
  BOOL ShowPane(CPaneEX *pPane, BOOL bShow = TRUE, int nWhich = 0);
  void SetMainRect(RECT rect);
  void SetMainRect(int X, int Y, int nWidth, int nHeight);
  void SetTopOffset(int topOffset);
  void SetLeftOffset(int leftOffset);
  void SetRightOffset(int rightOffset);
  void SetBottomOffset(int bottomOffset);

	//{{AFX_DATA(CSplitDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

public:
	void  SetSpliterDialogListener(ISpliterDialogListener * pListener);

protected:
	ISpliterDialogListener * m_SpliterDialogListener;

  enum DRAGFLAG { DRAG_ENTER=0, DRAG_EXIT=1, DRAGGING=2 };
  BOOL m_bMovingSplitter; //Needed to know when we are sizing the splitter
  CPaneEX * m_movingPane; //Needed when sizing the splitter to keep track of the pane being sized
  RECT m_OffsetRect;  //this holds the offsets from the dialog borders
	CRect	m_rectDragPrev; //this is required to erase the old drag rectangle
  CRect m_rectDragCurt; //this is required to draw the current drag rectangle
  void DrawDraggingBar(CPoint point,DRAGFLAG df);
	// Generated message map functions
	//{{AFX_MSG(CSplitDialog)
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnSizing( UINT nSide, LPRECT lpRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_SPLITDIALOG_H_INCLUDED_)
