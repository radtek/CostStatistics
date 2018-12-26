/*********************************************************************************
 描述：分割窗口函数
 作者：syxk development group
 时间：2012.11.15
 修改：暂无
*********************************************************************************/


// SplitDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SplitDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitDialog dialog


CSplitDialog::CSplitDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	//{{AFX_DATA_INIT(CSplitDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  m_pMainPane = NULL;
  m_bMovingSplitter = FALSE;
  m_movingPane = NULL;
  m_SpliterDialogListener = NULL;
}

CSplitDialog::~CSplitDialog()
{
  //free memory create with new...
  if (m_pMainPane)
    delete m_pMainPane;
}

void CSplitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplitDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplitDialog, CDialog)
	//{{AFX_MSG_MAP(CSplitDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
  ON_WM_SIZING()
  ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitDialog message handlers

//
//Need to call this function after all the controls have been added
//
void CSplitDialog::Initialize()
{
  if (m_pMainPane)
    m_pMainPane->MovePane();
}

void  CSplitDialog::SetSpliterDialogListener(ISpliterDialogListener * pListener)
{
	m_SpliterDialogListener = pListener;
}

void CSplitDialog::OnSizing( UINT nSide, LPRECT lpRect)
{
  //when we're sizing we have to make sure we don't
  //let the user resize smaller than the minimum values
  
  if (m_pMainPane) {
    int minheight = m_pMainPane->GetMinHeight() + m_OffsetRect.bottom + m_OffsetRect.top;
    if ((lpRect->bottom - lpRect->top) < minheight) {
      switch(nSide) {
      case WMSZ_BOTTOM:
      case WMSZ_BOTTOMLEFT:
      case WMSZ_BOTTOMRIGHT:
        lpRect->bottom = lpRect->top + minheight;
        break;
      case WMSZ_TOP:
      case WMSZ_TOPLEFT:
      case WMSZ_TOPRIGHT:
        lpRect->top = lpRect->bottom - minheight;
        break;
      }
    }

    int minwidth =  m_pMainPane->GetMinWidth() + m_OffsetRect.left + m_OffsetRect.right;
    if ((lpRect->right - lpRect->left) < minwidth) {
      switch(nSide) {
      case WMSZ_BOTTOMLEFT:
      case WMSZ_TOPLEFT:
      case WMSZ_LEFT:
        lpRect->left = lpRect->right - minwidth;
        break;
      case WMSZ_BOTTOMRIGHT:
      case WMSZ_TOPRIGHT:
      case WMSZ_RIGHT:
        lpRect->right = lpRect->left + minwidth;
        break;
      }
    }
  }

  CDialog::OnSizing(nSide,lpRect);
}

void CSplitDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

  //when the dialog has been sized, resize the contents
  if (m_pMainPane) m_pMainPane->MovePane(
    cx - (m_OffsetRect.right + m_OffsetRect.left), 
    cy - (m_OffsetRect.bottom + m_OffsetRect.top));
}

void CSplitDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	//CClientDC dc(this);
	//dc.DPtoLP(&point);

	char *cur;
	CPaneEX *pPane;

	//check if the point is on any splitter...
	if (m_pMainPane && (cur = m_pMainPane->IsPointInSplitter(point,&pPane)))
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(cur)));

	// if we're moving the splitter, resize everything for that specific splitter
	if (m_bMovingSplitter) {
		//m_movingPane->MoveSplitter(point);
		if (m_movingPane->IsValid(point))
			DrawDraggingBar(point,DRAGGING);

		if ( m_SpliterDialogListener != NULL )
		{
			m_SpliterDialogListener->OnSplitMoveing(m_movingPane,point.x,point.y);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CSplitDialog::PreTranslateMessage(MSG* pMsg) 
{
  //
  //if we are sizing the panes and Esc key is hit, cancel
  //the sizing operation!
  //
  if ((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE && m_bMovingSplitter)) {
    RedrawWindow(m_rectDragPrev);
    m_bMovingSplitter = FALSE;
    ReleaseCapture();
    return TRUE;
  }

	return CDialog::PreTranslateMessage(pMsg);
}


void CSplitDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//CClientDC dc( this );
	//dc.DPtoLP(&point);

	char *cur;

	// If the point is over the splitter ...
	if (m_pMainPane && (cur = m_pMainPane->IsPointInSplitter(point,&m_movingPane)))
	{
		// ... capture the mouse, change the cursor and set the moving splitter flag
		SetCapture();
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(cur)));
		m_bMovingSplitter = TRUE;
		//ScreenToClient(&point);
		DrawDraggingBar(point,DRAG_ENTER);

		if ( m_SpliterDialogListener != NULL )
		{
			m_SpliterDialogListener->OnSplitMoveBegin(m_movingPane,point.x,point.y);
		}

		CDialog::OnLButtonDown(nFlags, point);
	}
}

void CSplitDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bMovingSplitter) 
	{
		/*CClientDC dc( this );
		dc.DPtoLP( &point );*/
		DrawDraggingBar(point,DRAG_EXIT);
		// Reset the moving splitter flag
		m_bMovingSplitter = FALSE;
		//resize the panes
		m_movingPane->MoveSplitter(point);
		// Release the mouse
		ReleaseCapture();
		RedrawWindow(m_rectDragPrev);

		if ( m_SpliterDialogListener != NULL )
		{
			m_SpliterDialogListener->OnSplitMoveEnd(m_movingPane,point.x,point.y);
			Invalidate(TRUE);
		}
		else
		{
			//向前兼容代码
			::SendMessage(this->GetSafeHwnd(), WM_SIZE, NULL, 1);
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CSplitDialog::DrawDraggingBar(CPoint point,DRAGFLAG df)
{
	m_rectDragCurt=m_movingPane->m_rectSplitter;
  if(m_movingPane->m_nSplitType == CPaneEX::ST_HORIZONTAL)
	{
		m_rectDragCurt.top=point.y;
		m_rectDragCurt.bottom=point.y+ m_movingPane->m_rectSplitter.Height();
	}
	else
	{
		m_rectDragCurt.left=point.x;
		m_rectDragCurt.right=point.x+m_movingPane->m_rectSplitter.Width();
	}

	CSize size(m_rectDragCurt.Width(),m_rectDragCurt.Height());

	CDC *pDC = GetDC();	
	//ScreenToClient(m_rectDragCurt);
	switch(df)
	{
	case DRAG_ENTER:
		 pDC->DrawDragRect(m_rectDragCurt,size,NULL,size);
		 break;
	case DRAG_EXIT:	//fall through
	default:
		 pDC->DrawDragRect(m_rectDragCurt,size,m_rectDragPrev,size);
		 break;
	}

	ReleaseDC(pDC);
	m_rectDragPrev=m_rectDragCurt;
}

//
//This function will create a pain in the given parent pane.
//if no parent pane is given then the created pain will be
//set as the main one.
//
//nWhich sepecifies which pane we are creating, pass 1 to
//create the left/top panes or 2 to create the right/bottom panes
//
//nSplitType will specify the type of the split, this can be
//CPaneEX::HORIZONTAL  or CPaneEX:VERTICAL
//
//bSizeableControls tells the pane wether or not to have a resize bar
//between its 2 controls
//
CPaneEX *CSplitDialog::CreatePane(int nSplitType, CPaneEX *pParent, int nWhich, BOOL bSizeableControls)
{
  CPaneEX *pPane = new CPaneEX;

  pPane->m_nSplitType = nSplitType;
  pPane->m_bSizeableControls = bSizeableControls;

  if (pParent) {
    if (nWhich == 1) {
      pParent->m_nLeftType = CPaneEX::PT_PANE;
      pParent->left.pPane = pPane;
      pPane->m_bLeftVisible = TRUE;
    } else if (nWhich == 2) {
      pParent->m_nRightType = CPaneEX::PT_PANE;
      pParent->right.pPane = pPane;
      pPane->m_bRightVisible = TRUE;
    }
    //m_pMainPane->MovePane();
  } else {
	  CRect rect;
	  GetClientRect(&rect);
    
    m_OffsetRect.left = 0;
    m_OffsetRect.right = 0;
    m_OffsetRect.bottom = 0;
    m_OffsetRect.top = 0;

    pPane->MovePane(rect.left, rect.top, rect.Width(), rect.Height());

    m_pMainPane = pPane;
  }

  return pPane;
}

//
//This almost works like CreatePane but instead of embedding
//a pane inside another pane, it embeds the given Cwnd in the left/right
//of the given pane...
//
//minWidth/minHeight  specify the minimum dimensions of the CWnd while resizing
//Width/Height specify the original dimensions
//
BOOL CSplitDialog::SetPaneWnd(CPaneEX *pPane, CWnd *pWnd, int nWhich,int minWidth, int minHeight, int Width, int Height)
{

  if (Width!=0 && Height!=0)
    ::MoveWindow(pWnd->m_hWnd,0,0,Width,Height,TRUE);

  if (nWhich == 1) {
    pPane->m_nLeftType = CPaneEX::PT_WINDOW;
    pPane->left.pWnd = pWnd;
    pPane->m_nLeftMinWidth = minWidth;
    pPane->m_nLeftMinHeight = minHeight;
    pPane->m_bLeftVisible = TRUE;
  } else if (nWhich == 2) {
    pPane->m_nRightType = CPaneEX::PT_WINDOW;
    pPane->right.pWnd = pWnd;
    pPane->m_nRightMinWidth = minWidth;
    pPane->m_nRightMinHeight = minHeight;
    pPane->m_bRightVisible = TRUE;
  }

  //m_pMainPane->MovePane();

  return TRUE;
}

//
//This allows you to hide/show any pane any time!
//
BOOL CSplitDialog::ShowPane(CPaneEX *pPane, BOOL bShow,int nWhich)
{
  switch(nWhich) {
  case 1: //hide left
    pPane->ShowLeft(bShow);
    break;
  case 2: //hide right
    pPane->ShowRight(bShow);
    break;
  }

  //resize everything...
  m_pMainPane->MovePane();
  return TRUE;
}

//
//This will set the Main pane's rectangle to the given one
//
void CSplitDialog::SetMainRect(RECT rect)
{
  if (!m_pMainPane) return;

  RECT clrect;
  GetClientRect(&clrect);

  m_OffsetRect.left = rect.left;
  m_OffsetRect.top = rect.top;
  m_OffsetRect.bottom = clrect.bottom - rect.bottom;
  m_OffsetRect.right = clrect.right - rect.right;

  m_pMainPane->MovePane(rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top);
}

//
//just another form of the first one...
//
void CSplitDialog::SetMainRect(int X, int Y, int nWidth, int nHeight)
{
  RECT rect;
  rect.left = X;
  rect.top = Y;
  rect.right = rect.left + nWidth;
  rect.bottom = rect.top + nHeight;

  SetMainRect(rect);
}

//
//The four following functions will set an offset from the 
//dialog's borders.
//
void CSplitDialog::SetTopOffset(int topOffset)
{
  if (!m_pMainPane) return;
  RECT rect;
  rect.top = topOffset;
  rect.left = m_pMainPane->m_X;
  rect.bottom = m_pMainPane->m_nHeight + m_pMainPane->m_Y;
  rect.right = m_pMainPane->m_nWidth + m_pMainPane->m_X;
  SetMainRect(rect);
}

void CSplitDialog::SetLeftOffset(int leftOffset)
{
  if (!m_pMainPane) return;
  RECT rect;
  rect.top = m_pMainPane->m_Y;
  rect.bottom = m_pMainPane->m_Y + m_pMainPane->m_nHeight;
  rect.left = leftOffset;
  rect.right = m_pMainPane->m_nWidth + m_pMainPane->m_X;
  SetMainRect(rect);
}

void CSplitDialog::SetRightOffset(int rightOffset)
{
  if (!m_pMainPane) return;
  RECT rect,clrect;

  GetClientRect(&clrect);
  rect.top = m_pMainPane->m_Y;
  rect.bottom = m_pMainPane->m_nHeight + m_pMainPane->m_Y;
  rect.left = m_pMainPane->m_X;
  rect.right = clrect.right - rightOffset;
  SetMainRect(rect);
}

void CSplitDialog::SetBottomOffset(int bottomOffset)
{
  if (!m_pMainPane) return;
  RECT rect,clrect;

  GetClientRect(&clrect);
  rect.top = m_pMainPane->m_Y;
  rect.left = m_pMainPane->m_X;
  rect.bottom = clrect.bottom - bottomOffset;
  rect.right = m_pMainPane->m_nWidth + m_pMainPane->m_X;
  SetMainRect(rect);
}

