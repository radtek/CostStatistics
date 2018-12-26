//
// TreeGridCell.cpp : implementation file
//

#include "StdAfx.h"
#include "TreeGridCtrl.h"
#include "TreeGridCell.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CTreeGridCell, CGridCell)

CTreeGridCell::CTreeGridCell() : CTreeCell(this)
{
	
}

CSize CTreeGridCell::GetCellExtent(CDC* pDC)
{
	CSize size = CGridCell::GetCellExtent(pDC);
	OnGetTreeCellExtent(pDC, size);

	return size;
}

 BOOL CTreeGridCell::GetTextRect( LPRECT pRect)
 {
	BOOL ret = CGridCell::GetTextRect(pRect);
	 CRect rcTreeBtn(0,0,100,30);
	
	 if(MeasureTreeButtonRect(rcTreeBtn))
	 {
		 pRect->left += rcTreeBtn.Width() + GetTreeBtnRightPadding();
		
	 }
	 
	 return ret;
 }

BOOL CTreeGridCell::Draw(CDC* pDC, int nItem, int nSubItem, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
	BOOL bRet = FALSE;
	if(OnDrawingTreeCell(pDC, nItem, nSubItem, rect, bEraseBkgnd))
		bRet = CGridCell::Draw(pDC, nItem, nSubItem, rect, bEraseBkgnd);
	
	return bRet;
}

BOOL CTreeGridCell::Edit(int nItem, int nSubItem, CRect rect, CPoint  point, UINT nID, UINT nChar)
{
	BOOL bRet = FALSE;
	if(OnEditingTreeCell(nItem, nSubItem, rect, point, nID, nChar))
		bRet = CGridCell::Edit(nItem, nSubItem, rect, point, nID, nChar);

	return bRet;
}

void CTreeGridCell::OnCopy(const CGridCellBase& cell)
{
	const CTreeCell* pCell = dynamic_cast<const CTreeCell*>(&cell);
	if(pCell)
	{
		this->CopyFromTreeCell(*pCell);
	}	
}