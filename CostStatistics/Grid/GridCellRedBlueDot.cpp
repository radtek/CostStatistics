#include "StdAfx.h"
#include "GridCellRedBlueDot.h"



IMPLEMENT_DYNCREATE(CGridCellRedBlueDot, CGridCell)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CGridCellRedBlueDot::CGridCellRedBlueDot():
m_nDotType(0)
{
	
}


BOOL CGridCellRedBlueDot::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd)
{

	 BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);
	
	 if(m_nDotType <1 || m_nDotType>2)
	 {
		 return bResult;
	 }

	COLORREF clrOld = pDC->GetTextColor();
	
	CFont ft;
	ft.CreatePointFont(100,"simhei");

	pDC->SetBkMode(TRANSPARENT);

	
	pDC->SetTextColor(m_nDotType==1 ? RGB(255,0,0):RGB(0,0,255));

	pDC->DrawText("¡ñ",&rect,DT_VCENTER|DT_SINGLELINE|DT_CENTER);
	 return bResult;
}

void CGridCellRedBlueDot::SetDotType(int type)
{
	m_nDotType = type;
}