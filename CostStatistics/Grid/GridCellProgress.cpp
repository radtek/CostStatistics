#include "StdAfx.h"
#include "GridCellProgress.h"

IMPLEMENT_DYNCREATE(CGridCellProgress, CGridCell)  

CGridCellProgress::CGridCellProgress(void):
m_nPercent(0)
{
}

CGridCellProgress::~CGridCellProgress(void)
{
}


BOOL CGridCellProgress::Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar)
{
	return TRUE;
}
void CGridCellProgress::EndEdit()
{

}
BOOL CGridCellProgress::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd)
{
	if(rect.right-rect.left<10) return false;

	pDC->FillSolidRect(&rect,RGB(255,255,255));

	rect.right-=4;
	CFont ft;
	ft.CreatePointFont(90,"simsun");
	CFont *pOldFt = pDC->SelectObject(&ft);
	

	CString str;
	str.Format("%d%%",m_nPercent);
	
	COLORREF clrFont = RGB(0,0,60);

	
	
	int ftWid = pDC->GetTextExtent("100%").cx;
	
	
	
	int centerY = rect.CenterPoint().y;

	rect.top = centerY-8;
	rect.bottom = centerY+8;
	
	CRect rcLabel = rect;
	rcLabel.left+=2;
	rcLabel.right-=ftWid+4;

	COLORREF oldClr = pDC->SetTextColor(clrFont);

	pDC->FillSolidRect(&rcLabel,RGB(200,230,255));
	
	
	CRect rcBar = rcLabel;
	rcBar.right = min(rcLabel.right, (int)(rcLabel.left+ rcLabel.Width()*m_nPercent/100.0));
	
	rcBar.top++;
	rcBar.bottom--;
	rcBar.left++;
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(&rcBar,RGB(0,100,200));
	
	pDC->Draw3dRect(&rcLabel,RGB(200,200,255),RGB(200,200,255));
	
	pDC->DrawText(str,&rect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

	pDC->SelectObject(pOldFt);
	pDC->SetTextColor(oldClr);
	
	return TRUE;
}

void CGridCellProgress::SetPecent(int nPercent)
{
	if(nPercent<0) nPercent =0;
		
	m_nPercent = nPercent;
	m_strText.Format("%d%%",m_nPercent);
}