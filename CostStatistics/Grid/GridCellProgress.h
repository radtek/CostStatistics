#pragma once
#include "gridcell.h"

class CGridCellProgress :
	public CGridCell
{
public:
	CGridCellProgress(void);
	~CGridCellProgress(void);
	DECLARE_DYNCREATE(CGridCellProgress)
	virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	virtual void EndEdit();
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
	void SetPecent(int nPercent);		//0-100
private:
	int m_nPercent;
};
