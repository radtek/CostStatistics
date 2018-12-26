 
#include "stdafx.h"
#include "gridctrl.h"

#ifndef GRIDCONTROL_NO_TITLETIPS

#include "TitleTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleTip

CTitleTip::CTitleTip()
{
	// Register the window class if it has not already been registered.
	WNDCLASS wndcls;

	afxCurrentInstanceHandle = _AtlBaseModule.GetModuleInstance();
	afxCurrentResourceHandle = _AtlBaseModule.GetResourceInstance();
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, TITLETIP_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style			= CS_SAVEBITS;
		wndcls.lpfnWndProc		= ::DefWindowProc;
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0;
		wndcls.hInstance		= hInst;
		wndcls.hIcon			= NULL;
		wndcls.hCursor			= LoadCursor( hInst, IDC_ARROW );
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK +1);
		wndcls.lpszMenuName		= NULL;
		wndcls.lpszClassName	= TITLETIP_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}

    m_dwLastLButtonDown = ULONG_MAX;
    m_dwDblClickMsecs   = GetDoubleClickTime();
    m_bCreated          = FALSE;
    m_pParentWnd        = NULL;
}

CTitleTip::~CTitleTip()
{
}


BEGIN_MESSAGE_MAP(CTitleTip, CWnd)
	//{{AFX_MSG_MAP(CTitleTip)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTitleTip message handlers

BOOL CTitleTip::Create(CWnd * pParentWnd)
{
	ASSERT_VALID(pParentWnd);

    // Already created?
    if (m_bCreated)
        return TRUE;

	DWORD dwStyle = WS_BORDER | WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	m_pParentWnd = pParentWnd;

	m_bCreated = CreateEx(dwExStyle, TITLETIP_CLASSNAME, NULL, dwStyle, 
                          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		                  NULL, NULL, NULL );

    return m_bCreated;
}

BOOL CTitleTip::DestroyWindow() 
{
    m_bCreated = FALSE;
	
	return CWnd::DestroyWindow();
}

// Show 		 - Show the titletip if needed
// rectTitle	 - The rectangle within which the original 
//				    title is constrained - in client coordinates
// lpszTitleText - The text to be displayed
// xoffset		 - Number of pixel that the text is offset from
//				   left border of the cell
void  CTitleTip::DrawMyText(CString str, CRect& rc,CDC *pDC,bool bOnlyCalu)
{
	str.Trim();
	str.Replace("\r\n","\n");
	CStringArray arr;

	CString tmp;
	while(true)
	{
		if(str =="") break;
		int pos = str.Find('\n');
		if(pos==-1)
		{
			arr.Add(str);
			break;
		}
		else
		{
			arr.Add(str.Left(pos));
			str = str.Mid(pos+1);
		}
	}

	if(arr.IsEmpty()) return;

	if(bOnlyCalu)
	{
		rc.right = rc.left;
		rc.bottom = rc.top;
	}

	int Space = 3;
	for(int i=0;i<arr.GetCount();i++)
	{
		CSize sz = pDC->GetTextExtent(arr[i]);
		if(bOnlyCalu)
		{
			rc.right = max(rc.right,rc.left+sz.cx);
			rc.bottom += sz.cy+Space;
		}
		else
		{
			pDC->DrawText(arr[i],CRect(rc.left,rc.top+i*(sz.cy+Space),rc.right,rc.bottom),DT_LEFT|DT_SINGLELINE);
		}

	}

}

void CTitleTip::Show(CRect rectTitle, LPCTSTR lpszTitleText, int xoffset /*=0*/,
                     LPRECT lpHoverRect /*=NULL*/,
                     const LOGFONT* lpLogFont /*=NULL*/,
                     COLORREF crTextClr /* CLR_DEFAULT */,
                     COLORREF crBackClr /* CLR_DEFAULT */,
					 LPCTSTR lpszOldTitleText /*=NULL*/)
{
    if (!IsWindow(m_hWnd))
        Create(m_pParentWnd);

	ASSERT( ::IsWindow( GetSafeHwnd() ) );

    if (rectTitle.IsRectEmpty())
        return;

	// If titletip is already displayed, don't do anything.
	if( IsWindowVisible() ) 
		return;

    m_rectHover = (lpHoverRect != NULL)? lpHoverRect : rectTitle;
    m_rectHover.right++; m_rectHover.bottom++;

	m_pParentWnd->ClientToScreen( m_rectHover );
    ScreenToClient( m_rectHover );

	// Do not display the titletip is app does not have focus
	if( GetFocus() == NULL )
		return;

	// Define the rectangle outside which the titletip will be hidden.
	// We add a buffer of one pixel around the rectangle
	m_rectTitle.top    = -1;
	m_rectTitle.left   = -xoffset-1;
	m_rectTitle.right  = rectTitle.Width()-xoffset;
	m_rectTitle.bottom = rectTitle.Height()+1;

	// Determine the width of the text
	m_pParentWnd->ClientToScreen( rectTitle );

	CClientDC dc(this);
	CString strTitle = _T("");
    strTitle += _T(" ");
    strTitle += lpszTitleText; 
    strTitle += _T(" ");

	CFont font, *pOldFont = NULL;
    if (lpLogFont)
    {
        font.CreateFontIndirect(lpLogFont);
	    pOldFont = dc.SelectObject( &font );
    }
    else
    {
        // use same font as ctrl
	    pOldFont = dc.SelectObject( m_pParentWnd->GetFont() );
    }

	CSize size;

	if (lpszOldTitleText != NULL && _tcslen(strTitle) <  _tcslen(lpszOldTitleText))
	{
		size = dc.GetTextExtent( lpszOldTitleText );
		size.cx += 15;
	}
	else
	{
		size = dc.GetTextExtent( strTitle );
	}

    TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);
    size.cx += tm.tmOverhang;
	
	CSize sz = dc.GetTextExtent(strTitle);

	CRect rectDisplay = rectTitle;
	rectDisplay.left += xoffset;
	
	CRect drawRect(0,0,100,100);
	DrawMyText(strTitle,drawRect,&dc,true);
	
	rectDisplay.right = rectDisplay.left + drawRect.Width();
	rectDisplay.bottom = rectDisplay.top + drawRect.Height();
    // Do not display if the text fits within available space
    if ( rectDisplay.right >= rectTitle.right-xoffset )
    {
		// Show the titletip
        SetWindowPos( &wndTop, rectDisplay.left, rectDisplay.top, 
            rectDisplay.Width()+12, rectDisplay.Height()+12, 
            SWP_SHOWWINDOW|SWP_NOACTIVATE );
        
        // FNA - handle colors correctly
        if (crBackClr != CLR_DEFAULT)
        {
		    CBrush backBrush(crBackClr);
		    CBrush* pOldBrush = dc.SelectObject(&backBrush);
		    CRect rect;
		    dc.GetClipBox(&rect);     // Erase the area needed 

		    dc.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(),  PATCOPY);
		    dc.SelectObject(pOldBrush);
	    }
        // Set color
        if (crTextClr != CLR_DEFAULT)//FNA
            dc.SetTextColor(crTextClr);//FA

        dc.SetBkMode( TRANSPARENT );
        //dc.TextOut( 0, 0, strTitle );
		dc.SetTextColor(RGB(0,0,0));
		
		
		DrawMyText(strTitle,CRect(6,6,rectDisplay.Width()+12,rectDisplay.Height()+12),&dc,false);
        SetCapture();
    }
    
    dc.SelectObject( pOldFont );
}

void CTitleTip::Hide()
{
  	if (!::IsWindow(GetSafeHwnd()))
        return;

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
        ReleaseCapture();

	ShowWindow( SW_HIDE );
}

void CTitleTip::OnMouseMove(UINT nFlags, CPoint point) 
{
    if (!m_rectHover.PtInRect(point)) 
    {
        Hide();
        
        // Forward the message
        ClientToScreen( &point );
        CWnd *pWnd = WindowFromPoint( point );
        if ( pWnd == this ) 
            pWnd = m_pParentWnd;
        
        int hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
        
        if (hittest == HTCLIENT) {
            pWnd->ScreenToClient( &point );
            pWnd->PostMessage( WM_MOUSEMOVE, nFlags, MAKELONG(point.x,point.y) );
        } else {
            pWnd->PostMessage( WM_NCMOUSEMOVE, hittest, MAKELONG(point.x,point.y) );
        }
    }
}

BOOL CTitleTip::PreTranslateMessage(MSG* pMsg) 
{
    // Used to qualify WM_LBUTTONDOWN messages as double-clicks
    DWORD dwTick=0;
    BOOL bDoubleClick=FALSE;

    CWnd *pWnd;
	int hittest;
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
       // Get tick count since last LButtonDown
        dwTick = GetTickCount();
        bDoubleClick = ((dwTick - m_dwLastLButtonDown) <= m_dwDblClickMsecs);
        m_dwLastLButtonDown = dwTick;
        // NOTE: DO NOT ADD break; STATEMENT HERE! Let code fall through

	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
		POINTS pts = MAKEPOINTS( pMsg->lParam );
		POINT  point;
		point.x = pts.x;
		point.y = pts.y;

		ClientToScreen( &point );
        Hide();

		//pWnd = WindowFromPoint( point );
		pWnd = CWnd::FromHandle(pMsg->hwnd);
		if (!pWnd)
			return CWnd::PreTranslateMessage(pMsg);

		if( pWnd->GetSafeHwnd() == GetSafeHwnd()) 
		{
			pWnd = m_pParentWnd;
			
		}
		hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));

		if (hittest == HTCLIENT) 
		{
			pWnd->ScreenToClient( &point );
			pMsg->lParam = MAKELONG(point.x,point.y);
		}
		else 
		{
			switch (pMsg->message) {
			case WM_LBUTTONDOWN: 
				pMsg->message = WM_NCLBUTTONDOWN;
				break;
			case WM_RBUTTONDOWN: 
				pMsg->message = WM_NCRBUTTONDOWN;
				break;
			case WM_MBUTTONDOWN: 
				pMsg->message = WM_NCMBUTTONDOWN;
				break;
			}
			pMsg->wParam = hittest;
			pMsg->lParam = MAKELONG(point.x,point.y);
		}


        // If this is the 2nd WM_LBUTTONDOWN in x milliseconds,
        // post a WM_LBUTTONDBLCLK message instead of a single click.
        pWnd->PostMessage(  bDoubleClick ? WM_LBUTTONDBLCLK : pMsg->message,
                            pMsg->wParam,
                            pMsg->lParam);
		return TRUE;
		}
		
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
        Hide();
		m_pParentWnd->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
		return TRUE;
	}

	if( GetFocus() == NULL )
	{
        Hide();
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

#endif // GRIDCONTROL_NO_TITLETIPS
void CTitleTip::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	//CRect rc;
	//GetClientRect(&rc);

	//if(rc.PtInRect(point))  return;


	ClientToScreen(&point);
	Hide();
	m_pParentWnd->ScreenToClient(&point);
	m_pParentWnd->PostMessage(WM_LBUTTONDOWN,MK_LBUTTON,MAKELONG(point.x,point.y));
	
	return;
	CWnd::OnLButtonDown(nFlags, point);
}

void CTitleTip::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CRect rc;
	//GetClientRect(&rc);

	//if(rc.PtInRect(point))  return;


	ClientToScreen(&point);
	Hide();
	m_pParentWnd->ScreenToClient(&point);
	m_pParentWnd->PostMessage(WM_LBUTTONUP,MK_LBUTTON,MAKELONG(point.x,point.y));

	return;
	CWnd::OnLButtonUp(nFlags, point);
}

void CTitleTip::OnRButtonDown(UINT nFlags, CPoint point)
{
	/*CRect rc;
	GetClientRect(&rc);

	if(rc.PtInRect(point))  return;*/


	ClientToScreen(&point);
	Hide();
	m_pParentWnd->ScreenToClient(&point);
	m_pParentWnd->SendMessage(WM_RBUTTONDOWN,MK_RBUTTON,MAKELONG(point.x,point.y));

	return;
	CWnd::OnRButtonDown(nFlags, point);
}

void CTitleTip::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*CRect rc;
	GetClientRect(&rc);

	if(rc.PtInRect(point))  return;*/


	ClientToScreen(&point);
	Hide();
		m_pParentWnd->ScreenToClient(&point);
	m_pParentWnd->SendMessage(WM_RBUTTONUP,MK_RBUTTON,MAKELONG(point.x,point.y));

	return;
	CWnd::OnRButtonUp(nFlags, point);
}
