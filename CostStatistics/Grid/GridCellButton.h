#if !defined(AFX_GRIDCELLBUTTON_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_GRIDCELLBUTTON_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GridCell.h"
#include <GdiPlus.h>
using namespace Gdiplus;            //使用GDI+的命名空间, 若不用的话每次使用Gdiplus时均加上命名空间亦可
//#include "SyxkDB_types.h"
#define GRID_BUTTON_SHOW_TEXT  TRUE

class CGridCellButton :	public CGridCell
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellButton)
public: 
    CGridCellButton(void); 
    ~CGridCellButton(void); 
public: 
    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd =TRUE); 
    virtual void OnClick(CRect cellRect, CPoint PointCellRelative); 
    virtual void OnClickDown(CRect cellRect, CPoint PointCellRelative);

    virtual void SetBkColor(COLORREF clrBk) { m_clrBK = clrBk;}
public: 
	CRect m_rect;
	int m_btnDnPos;
    int m_nRow;
    int m_nCol;
	BOOL m_bSearchMode;
	BOOL m_bChecked;
	
    COLORREF m_clrText;
    COLORREF m_clrBK;
    COLORREF m_clrMouseInText;
	
	CArray<CRect> m_arrBtnRect;
	BOOL          m_bBtnVCenter;

};

class CGDIMgr
{
public:
	CGDIMgr();
	~CGDIMgr();
protected:
	ULONG_PTR m_gdiplusToken;
	GdiplusStartupInput m_gdiplusStartupInput; 
};
#endif