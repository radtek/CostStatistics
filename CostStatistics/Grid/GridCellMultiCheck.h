#pragma once


#include "GridCell.h"

#define GCS_HORIZONTAL    1
#define GCS_VERTICAL      2


class CGridCellMultiCheck : public CGridCell
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellMultiCheck)

public:
    CGridCellMultiCheck();

public:
	BOOL SetCheck(int nIndex, BOOL bChecked = TRUE);
	BOOL GetCheck(int nIndex);
	void SetOptions(const CStringArray& arr);
	void GetOptions(CStringArray& arr);
	void SetData(const CStringArray& arr);
	void GetData(CStringArray& arr);
	void SetStyle(DWORD dwStyle);

// Operations
	virtual CSize GetCellExtent(CDC* pDC);
    virtual void OnClick( CRect cellRect, CPoint PointCellRelative);
    virtual BOOL GetTextRect( LPRECT pRect);
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

protected:
	CRect GetCheckPlacement(CRect rect);

protected:
    CRect m_Rect;
	CCellID m_id;
	DWORD         m_dwStyle;
	CStringArray  m_Strings;
	CStringArray  m_arrData;
	CArray<BOOL>  m_arrCheck;
	CArray<CRect> m_arrCheckRect;
};
