/*********************************************************************************
 描述：分割窗口函数声明
 作者：syxk development group
 时间：2012.11.15
 修改：暂无
*********************************************************************************/

#if !defined(_PANE_H_INCLUDED_)
#define _PANE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pane.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaneEX

class CPaneEX {
public:
  enum PaneTypes {  //the different pane types
    PT_INVALID,
    PT_PANE,
    PT_WINDOW
  };

  enum SplitTypes { //the different split types, only 2...
    ST_HORIZONTAL,
    ST_VERTICAL
  };

  union item {  //the items held in the left and right
    CWnd *pWnd;
    CPaneEX *pPane;
  } left,right;

  //
  //The info about every pane
  //
  int m_nLeftType;
  int m_nRightType;
  int m_nSplitType;
  int m_nPerc;

  int m_nWidth;
  int m_nHeight;
  int m_nLeftMinWidth;
  int m_nLeftMinHeight;
  int m_nRightMinWidth;
  int m_nRightMinHeight;
  int m_X, m_Y;
  BOOL m_bVisible;
  BOOL m_bLeftVisible;
  BOOL m_bRightVisible;
  int m_nSavedVal;

  CRect m_rectSplitter;
  BOOL m_bSizeableControls;

  CPaneEX();
  ~CPaneEX();

  BOOL IsLeftVisible();
  BOOL IsRightVisible();
  void Show(BOOL bShow);
  void ShowLeft(BOOL bShow);
  void ShowRight(BOOL bShow);

  int GetLeftWidth();
  int GetLeftHeight();
  void SetLeftWidth(int nWidth);
  void SetLeftHeight(int nHeight);
  BOOL GetWndSize(SIZE &size, int nWhich);

  int GetMinWidth();
  int GetMinHeight();
  int GetLeftMinWidth();
  int GetLeftMinHeight();
  int GetRightMinHeight();
  int GetRightMinWidth();

  char *IsPointInSplitter(CPoint point, CPaneEX **dpPane);
  void MoveSplitter(CPoint point);
  BOOL IsValid(CPoint point);

  void MovePane(int nWidth = -1, int nHeight = -1);
  void MovePane(int X, int Y, int nWidth, int nHeight);

};

#endif