#pragma once
#include "gridcell.h"


class CGridCellRedBlueDot :
	public CGridCell
{
DECLARE_DYNCREATE(CGridCellRedBlueDot)
public:
	CGridCellRedBlueDot();
	
	
	void SetDotType(int m_nType);		//0:无效  1:红点 2:蓝点
protected:
	
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
	int m_nDotType;
};
