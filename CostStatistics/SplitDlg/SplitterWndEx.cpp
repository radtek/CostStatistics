#include "stdafx.h"
#include "MMSystem.h"
#include "SplitterWndEx.h"

IMPLEMENT_DYNAMIC(CSplitterWndEx1, CSplitterWnd)

CSplitterWndEx1::CSplitterWndEx1() : m_nHidedCol(-1), m_nHidedRow(-1)
{
}

BEGIN_MESSAGE_MAP(CSplitterWndEx1, CSplitterWnd)
END_MESSAGE_MAP()

void CSplitterWndEx1::ShowColumn() 
{ 
    ASSERT_VALID(this);
    ASSERT(m_nCols < m_nMaxCols);
    ASSERT(m_nHidedCol != -1);

    int colNew = m_nHidedCol;
    m_nHidedCol = -1;
    int cxNew = m_pColInfo[m_nCols].nCurSize; 
    m_nCols++;   // add a column 
    ASSERT(m_nCols == m_nMaxCols);

    // fill the hided column 
    int col; 
    for (int row = 0; row < m_nRows; row++) 
    { 
        CWnd* pPaneShow = GetDlgItem( 
            AFX_IDW_PANE_FIRST + row * 16 + m_nCols); 
        ASSERT(pPaneShow != NULL); 
        pPaneShow-> ShowWindow(SW_SHOWNA); 

        for (col = m_nCols - 2; col >= colNew; col--) 
        { 
            CWnd* pPane = GetPane(row, col); 
            ASSERT(pPane != NULL); 
            pPane-> SetDlgCtrlID(IdFromRowCol(row, col + 1)); 
        } 

        pPaneShow-> SetDlgCtrlID(IdFromRowCol(row, colNew)); 
    } 

    // new panes have been created -- recalculate layout 
    for (col = colNew + 1; col < m_nCols; col++)
        m_pColInfo[col].nIdealSize = m_pColInfo[col - 1].nCurSize; 
    m_pColInfo[colNew].nIdealSize = cxNew; 
    RecalcLayout(); 
} 

void CSplitterWndEx1::HideColumn(int colHide)
{
    ASSERT_VALID(this);
    ASSERT(m_nCols > 1);
    ASSERT(colHide < m_nCols);
    ASSERT(m_nHidedCol == -1);

    m_nHidedCol = colHide;

    // if the column has an active window -- change it
    int rowActive, colActive;
    if (GetActivePane(&rowActive, &colActive) != NULL && colActive == colHide)
    {
        if (++colActive >= m_nCols)
            colActive = 0;
        SetActivePane(rowActive, colActive);
    }

    // hide all column panes
    for (int row = 0; row < m_nRows; row++)
    {
        CWnd* pPaneHide = GetPane(row, colHide);
        ASSERT(pPaneHide != NULL);
        pPaneHide-> ShowWindow(SW_HIDE);
        pPaneHide-> SetDlgCtrlID(AFX_IDW_PANE_FIRST + row * 16 + m_nCols);

        for (int col = colHide + 1; col < m_nCols; col++)
        {
            CWnd* pPane = GetPane(row, col);
            ASSERT(pPane != NULL);
            pPane-> SetDlgCtrlID(IdFromRowCol(row, col - 1));
        }
    }
    m_nCols--;
    m_pColInfo[m_nCols].nCurSize = m_pColInfo[colHide].nCurSize;
    RecalcLayout();
}

void CSplitterWndEx1::ShowRow() 
{ 
    ASSERT_VALID(this);
    ASSERT(m_nRows < m_nMaxRows);
    ASSERT(m_nHidedRow != -1);

    int rowNew = m_nHidedRow;
    m_nHidedRow = -1;
    int cxNew = m_pRowInfo[m_nRows].nCurSize;
    m_nRows++;   // add a row
    ASSERT(m_nRows == m_nMaxRows);

    // fill the hided row
    int row;
    for (int col = 0; col < m_nCols; col++)
    {
        CWnd * pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + col * 16 + m_nRows);
        ASSERT(pPaneShow != NULL);
        pPaneShow-> ShowWindow(SW_SHOWNA);

        for (row = m_nRows - 2; row >= rowNew; row--)
        {
            CWnd* pPane = GetPane(row, col);
            ASSERT(pPane != NULL);
            pPane-> SetDlgCtrlID(IdFromRowCol(row + 1, col));
        }

        pPaneShow-> SetDlgCtrlID(IdFromRowCol(rowNew, col));
    }

    // new panes have been created -- recalculate layout
    for (row = rowNew + 1; row < m_nRows; row++)
        m_pRowInfo[row].nIdealSize = m_pRowInfo[row - 1].nCurSize;
    m_pRowInfo[rowNew].nIdealSize = cxNew;
    RecalcLayout();
} 

void CSplitterWndEx1::HideRow(int rowHide)
{
    ASSERT_VALID(this);
    ASSERT(m_nRows > 1);
    ASSERT(rowHide < m_nRows);
    ASSERT(m_nHidedRow == -1);

    m_nHidedRow = rowHide;

    // if the row has an active window -- change it
    int rowActive, colActive;
    if (GetActivePane(&rowActive, &colActive) != NULL && rowActive == rowHide)
    {
        if (++rowActive >= m_nRows)
            rowActive = 0;
        SetActivePane(rowActive, colActive);
    }

    // hide all row panes
    for (int col = 0; col < m_nCols; col++)
    {
        CWnd* pPaneHide = GetPane(rowHide, col);
        ASSERT(pPaneHide != NULL);
        pPaneHide-> ShowWindow(SW_HIDE);
        pPaneHide-> SetDlgCtrlID(AFX_IDW_PANE_FIRST + col * 16 + m_nRows);

        for (int row = rowHide + 1; row < m_nRows; row++)
        {
            CWnd* pPane = GetPane(row, col);
            ASSERT(pPane != NULL);
            pPane->SetDlgCtrlID(IdFromRowCol(row - 1, col));
        }
    }
    m_nRows--;
    m_pRowInfo[m_nRows].nCurSize = m_pRowInfo[rowHide].nCurSize;
    RecalcLayout();
}

void CSplitterWndEx1::OnDrawSplitter(CDC * pDC, ESplitType nType, const CRect & rect)
{
    CSplitterWnd::OnDrawSplitter(pDC, nType, rect);

    if (pDC != NULL && nType == splitBar)
    {
        CBrush brush;
        brush.CreateSolidBrush(RGB(202, 225, 243));
        pDC->FillRect(rect,&brush);
    }
} 