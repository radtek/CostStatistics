#include "stdafx.h"
#include "SplitterWndH.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSplitterWndH::CSplitterWndH()
	: m_nHiddenCol(-1)
	, m_nHideColWidth(0)
{
}

CSplitterWndH::~CSplitterWndH()
{
}

void CSplitterWndH::HideColumn(int nCol)
{
	if (m_nHiddenCol != -1) return;
	ASSERT_VALID(this);
	ASSERT(m_nCols > 1);
	ASSERT(nCol < m_nCols);

	int nMin;
	GetColumnInfo(nCol, m_nHideColWidth, nMin);

	m_nHiddenCol = nCol;

	// if the column has an active window -- change it
	int rowActive, colActive;
	if (GetActivePane(&rowActive, &colActive) != NULL && colActive == nCol)
	{
		if (++colActive >= m_nCols)
			colActive = 0;
		SetActivePane(rowActive, colActive);
	}

    // hide all column panes
    for (int row = 0; row < m_nRows; row++)
    {
        CWnd* pPaneHide = GetPane(row, nCol);
        ASSERT(pPaneHide != NULL);
        pPaneHide->ShowWindow(SW_HIDE);
        pPaneHide->SetDlgCtrlID(AFX_IDW_PANE_FIRST + row * 16 + m_nCols);

        for (int col = nCol + 1; col < m_nCols; col++)
        {
            CWnd* pPane = GetPane(row, col);
            ASSERT(pPane != NULL);
            pPane->SetDlgCtrlID(IdFromRowCol(row, col - 1));
        }
    }
    m_nCols--;
    m_pColInfo[m_nCols].nCurSize = m_pColInfo[nCol].nCurSize;
    RecalcLayout();
}

void CSplitterWndH::ShowColumn()
{
	if (m_nHiddenCol == -1) return;
    ASSERT_VALID(this);
    ASSERT(m_nCols < m_nMaxCols);

    int colNew = m_nHiddenCol;
    m_nHiddenCol = -1;
    m_nCols++;  // add a column
    ASSERT(m_nCols == m_nMaxCols);

    // fill the hidden column
    int col;
    for (int row = 0; row < m_nRows; row++)
    {
        CWnd* pPaneShow = GetDlgItem(AFX_IDW_PANE_FIRST + row * 16 + m_nCols);
        ASSERT(pPaneShow != NULL);
        pPaneShow->ShowWindow(SW_SHOWNA);

        for (col = m_nCols - 2; col >= colNew; col--)
        {
            CWnd* pPane = GetPane(row, col);
            ASSERT(pPane != NULL);
            pPane->SetDlgCtrlID(IdFromRowCol(row, col + 1));
        }

        pPaneShow->SetDlgCtrlID(IdFromRowCol(row, colNew));
    }

	SetColumnInfo(colNew, m_nHideColWidth, 0);
    RecalcLayout();
}

