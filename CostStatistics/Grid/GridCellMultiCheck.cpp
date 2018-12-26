#include "stdafx.h"
#include "GridCell.h"
#include "GridCtrl.h"

#include "GridCellMultiCheck.h"


IMPLEMENT_DYNCREATE(CGridCellMultiCheck, CGridCell)

CGridCellMultiCheck::CGridCellMultiCheck() : CGridCell()
{
	m_dwStyle = GCS_HORIZONTAL;
}

CSize CGridCellMultiCheck::GetCellExtent(CDC* pDC)
{
    // Using SM_CXHSCROLL as a guide to the size of the checkbox
    int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();	
    CSize	cellSize = CGridCell::GetCellExtent(pDC);	
    cellSize.cx += nWidth;	
    cellSize.cy = max (cellSize.cy, nWidth);	
    return  cellSize;
}

// i/o:  i=dims of cell rect; o=dims of text rect
BOOL CGridCellMultiCheck::GetTextRect( LPRECT pRect)
{	
	return FALSE;
    BOOL bResult = CGridCell::GetTextRect(pRect);    
	
    return bResult;
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellMultiCheck::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
	// Store the cell's dimensions for later
    m_Rect = rect;
	m_id.row = nRow;
	m_id.col = nCol;

	CGridCtrl* pGrid = GetGrid();
	ASSERT(pGrid);

	if (!pGrid || !pDC)
		return FALSE;

	CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
	if (!pDefaultCell)
		return FALSE;

	COLORREF TextClr, TextBkClr;

	TextClr = (GetTextClr() == CLR_DEFAULT)? pDefaultCell->GetTextClr() : GetTextClr();
	if (GetBackClr() == CLR_DEFAULT)
		TextBkClr = pDefaultCell->GetBackClr();
	else
	{
		bEraseBkgnd = TRUE;
		TextBkClr = GetBackClr();
	}

	pDC->SetTextColor(TextClr);
	pDC->SetBkColor(TextBkClr);

	CFont *pFont = GetFontObject();
	if (pFont)
		pDC->SelectObject(pFont);

	if (GetGrid()->IsFocused(*this, nRow, nCol) || IsDropHighlighted())
	{
		// Always draw even in list mode so that we can tell where the
		// cursor is at.  Use the highlight colors though.
		// LUC
		if (GetGrid()->IsSelected(*this, nRow, nCol))
		{
			TextBkClr = ::GetSysColor(COLOR_HIGHLIGHT);
			TextClr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);			

			bEraseBkgnd = TRUE;
		}

		rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
		if (bEraseBkgnd)
		{
			TRY 
			{
				CBrush brush(TextBkClr);
				pDC->FillRect(rect, &brush);
			} 
			CATCH(CResourceException, e)
			{
				//e->ReportError();
			}
			END_CATCH
		}

		// Don't adjust frame rect if no grid lines so that the
		// whole cell is enclosed.
		if (pGrid->GetGridLines() != GVL_NONE)
		{
			rect.right--;
			rect.bottom--;
		}

		if (pGrid->GetFrameFocusCell())
		{
			// Use same color as text to outline the cell so that it shows
			// up if the background is black.
			TRY 
			{
				CBrush brush(TextClr);
				pDC->FrameRect(rect, &brush);
			}
			CATCH(CResourceException, e)
			{
				//e->ReportError();
			}
			END_CATCH
		}
		pDC->SetTextColor(TextClr);
		pDC->SetBkColor(TextBkClr);

		// Adjust rect after frame draw if no grid lines
		if(pGrid->GetGridLines() == GVL_NONE)
		{
			rect.right--;
			rect.bottom--;
		}

		//rect.DeflateRect(0,1,1,1);  - Removed by Yogurt
	}
	else if(GetGrid()->IsSelected(*this, nRow, nCol))
	{
		rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
		pDC->FillSolidRect(rect, ::GetSysColor(COLOR_HIGHLIGHT));
		rect.right--; rect.bottom--;
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
	{
		if (bEraseBkgnd)
		{
			rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
			CBrush brush(TextBkClr);
			pDC->FillRect(rect, &brush);
			rect.right--; rect.bottom--;
		}
		pDC->SetTextColor(TextClr);
	}

	for (int i = 0; i < m_Strings.GetSize(); ++i)
	{
		if (m_dwStyle == GCS_HORIZONTAL)
		{
			m_arrCheckRect[i] = GetCheckPlacement(rect);
			rect.left = m_arrCheckRect[i].right;

			if (m_arrCheckRect[i].right < rect.right && m_arrCheckRect[i].bottom < rect.bottom)
			{
				pDC->DrawFrameControl(m_arrCheckRect[i], DFC_BUTTON, 
					(m_arrCheck[i])? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);
			}

			CRect rcText = rect;
			rcText.DeflateRect(GetMargin(), 0);
			pDC->DrawText(m_Strings[i], rcText, DT_CALCRECT | GetFormat() | DT_NOPREFIX | DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			rcText.bottom = rect.bottom;
			rect.left = rcText.right + GetMargin();

			if (rcText.right <= rect.right && rcText.bottom <= rect.bottom)
				pDC->DrawText(m_Strings[i], rcText, GetFormat() | DT_NOPREFIX | DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
		else if (m_dwStyle == GCS_VERTICAL)
		{
			rect.left = m_Rect.left;
			rect.bottom = m_Rect.bottom;

			m_arrCheckRect[i] = GetCheckPlacement(rect);
			rect.left = m_arrCheckRect[i].right;

			if (m_arrCheckRect[i].right < rect.right && m_arrCheckRect[i].bottom < rect.bottom)
			{
				pDC->DrawFrameControl(m_arrCheckRect[i], DFC_BUTTON, 
					(m_arrCheck[i])? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);
			}

			rect.bottom = rect.top + GetGrid()->GetDefCellHeight();

			if (rect.bottom <= m_Rect.bottom + (LONG)GetMargin())
			{
				CRect rcText = rect;
				rcText.DeflateRect(GetMargin(), 0, GetMargin(), 0);
				pDC->DrawText(m_Strings[i], rcText, DT_CALCRECT | DT_NOPREFIX | DT_LEFT | DT_VCENTER | DT_SINGLELINE);

				if (rcText.right > rect.right)
				{
					rcText.right = rect.right - GetMargin();
				}

				if (rcText.bottom > rect.bottom)
				{
					rcText.bottom = rect.bottom;
				}

				if (rcText.right <= rect.right && rcText.bottom <= rect.bottom)
				{
					rcText.bottom = rect.bottom;
					pDC->DrawText(m_Strings[i], rcText, DT_NOPREFIX | DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				}
			}
			else
			{
				rect.bottom = m_Rect.bottom;
			}

			rect.top = rect.bottom;
		}
	}

	BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);
	return bResult;
}

void CGridCellMultiCheck::OnClick(CRect cellRect, CPoint PointCellRelative)
{
	// PointCellRelative is relative to the topleft of the cell. Convert to client coords
	PointCellRelative += m_Rect.TopLeft();

    // Bail if cell is read-only
    CCellID cell = GetGrid()->GetCellFromPt(PointCellRelative);	

	if(!cell.IsValid()) return;

    if (!GetGrid()->IsCellEditable(cell))		
        return;

	// GetCheckPlacement returns the checkbox dimensions in client coords. Only check/
	// uncheck if the user clicked in the box
	for (int i = 0; i < m_arrCheckRect.GetSize(); ++i)
	{
		if (m_arrCheckRect[i].PtInRect(PointCellRelative))
		{
			m_arrCheck[i] = !m_arrCheck[i];
			GetGrid()->InvalidateRect(m_Rect);
			SendMessageToParent(m_id.row, m_id.col, GVN_CHKCLICK, i);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

BOOL CGridCellMultiCheck::SetCheck(int nIndex, BOOL bChecked /*=TRUE*/)
{
	if (nIndex >= m_arrCheck.GetSize())
		return FALSE;

	m_arrCheck[nIndex] = bChecked;
	if (!m_Rect.IsRectEmpty())
		GetGrid()->InvalidateRect(m_Rect);

	return bChecked;
}

BOOL CGridCellMultiCheck::GetCheck(int nIndex)
{
	if (nIndex >= m_arrCheck.GetSize())
		return FALSE;

	return m_arrCheck[nIndex];
}

void CGridCellMultiCheck::SetOptions(const CStringArray& arr)
{
	m_Strings.RemoveAll();
	for (int i = 0; i < arr.GetSize(); ++i)
		m_Strings.Add(arr[i]);

	m_arrCheck.RemoveAll();
	m_arrCheck.SetSize(arr.GetSize());
	for (int i = 0; i < arr.GetSize(); ++i)
		m_arrCheck[i] = FALSE;

	m_arrCheckRect.RemoveAll();
	m_arrCheckRect.SetSize(arr.GetSize());
}

void CGridCellMultiCheck::GetOptions(CStringArray& arr)
{
	arr.Copy(m_Strings);
}

void CGridCellMultiCheck::SetData(const CStringArray& arr)
{
	m_arrData.RemoveAll();
	for (int i = 0; i < arr.GetSize(); ++i)
		m_arrData.Add(arr[i]);
}

void CGridCellMultiCheck::GetData(CStringArray& arr)
{
	arr.Copy(m_arrData);
}

void CGridCellMultiCheck::SetStyle(DWORD dwStyle)
{
	m_dwStyle = dwStyle;
}

//////////////////////////////////////////////////////////////////////
// Protected implementation
//////////////////////////////////////////////////////////////////////

// Returns the dimensions and placement of the checkbox in client coords.
CRect CGridCellMultiCheck::GetCheckPlacement(CRect rect)
{
	int nWidth = GetSystemMetrics(SM_CXHSCROLL);
	CRect place = rect + CSize(GetMargin(), GetMargin());
    place.right = place.left + nWidth;
    place.bottom = place.top + nWidth;

	 /*for centering
	//int nDiff = (place.Width() - nWidth)/2;
	int nDiff = place.Width()/2;
	if (nDiff > 0)
	{
		place.left += nDiff;
		place.right = place.left + nWidth;
	}
	//nDiff = (place.Height() - nWidth)/2;
	nDiff = place.Height()/2;
	if (nDiff > 0)
	{
		place.top += nDiff;
		place.bottom = place.top + nWidth;
	}
    */

    if (rect.Height() < nWidth + 2 * static_cast<int> (GetMargin())) 
    {		
        place.top = rect.top + (rect.Height() - nWidth) / 2;	    
        place.bottom = place.top + nWidth;	
    }

	return place;
}