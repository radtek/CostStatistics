// GridURLCell.cpp: implementation of the CGridURLCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridURLCell.h"
#include "GridCtrl.h"

IMPLEMENT_DYNCREATE(CGridURLCell, CGridCell)

#ifndef _WIN32_WCE
HCURSOR CGridURLCell::g_hLinkCursor = NULL;
#endif

// Possible prefixes that indicate a hyperlink
URLStruct CGridURLCell::g_szURIprefixes[] = { 
    { _T("www."),    _tcslen(_T("www."))    },
    { _T("http:"),   _tcslen(_T("http:"))   },
    { _T("mailto:"), _tcslen(_T("mailto:")) },
    { _T("ftp:"),    _tcslen(_T("ftp:"))    },
    { _T("https:"),  _tcslen(_T("https:"))  },
    { _T("news:"),   _tcslen(_T("news:"))   },
    { _T("gopher:"), _tcslen(_T("gopher:")) },
    { _T("telnet:"), _tcslen(_T("telnet:")) },
    { _T("url:"),    _tcslen(_T("url:"))    },
    { _T("file:"),   _tcslen(_T("file:"))   },
    { _T("ftp."),    _tcslen(_T("ftp."))    }
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridURLCell::CGridURLCell()
{
#ifndef _WIN32_WCE
    g_hLinkCursor = GetHandCursor();
#endif
	m_bLaunchUrl = TRUE;
	m_clrUrl = GetSysColor(COLOR_HIGHLIGHT);
	m_bOverUrl = false;
	m_nRow = -1;
	m_nCol = -1;
}

CGridURLCell::~CGridURLCell()
{
}

BOOL CGridURLCell::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd)
{
	m_nRow = nRow;
	m_nCol = nCol;
	// If url is present then change text color
	//if (HasUrl(GetText()))
	SetTextClr(m_clrUrl);

    // Good a place as any to store the bounds of the rect
    m_Rect = rect;
	
	CString txt = GetText();
	SetText("");
   CGridCell::Draw(pDC, nRow, nCol, rect, bEraseBkgnd);
	SetText(txt);
	
	int het = pDC->GetTextExtent(txt).cy;
	int wid = pDC->GetTextExtent(txt).cx;
	
	pDC->SetBkMode(TRANSPARENT);

	CPen ps(PS_SOLID,1,RGB(50,50,255));

	CPen* pOldPen = pDC->SelectObject(&ps);
	COLORREF oldColor =  pDC->SetTextColor(m_bOverUrl ? RGB(0,0,160):RGB(50,50,255));

	if(GetGrid()->IsSelected(*this, nRow, nCol))
	{
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		
	}

	LOGFONT lgft = *GetFont();
	if(m_bOverUrl)
	{
		lgft.lfUnderline = 1;
		
	}
	CFont curFt;
	curFt.CreateFontIndirect(&lgft);

	CFont* pOldFont = pDC->SelectObject(&curFt);
	
	rect.DeflateRect(4,4,4,4);
	int btm = rect.CenterPoint().y+het/2+1;
	if(m_nFormat & DT_RIGHT)
	{
		pDC->DrawText(txt,&rect,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		
	}
	else if(m_nFormat & DT_CENTER)
	{
		pDC->DrawText(txt,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		
	}
	else
	{
		pDC->DrawText(txt,&rect,DT_VCENTER|DT_SINGLELINE);
		
	}
	
	pDC->SelectObject(pOldPen);
	pDC->SetBkColor(oldColor);
	pDC->SelectObject(pOldFont);
	return TRUE;
}

#pragma warning(disable:4100)
BOOL CGridURLCell::Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar)
{
    return FALSE;
}
#pragma warning(default:4100)

void CGridURLCell::OnClick(CRect cellRect, CPoint PointCellRelative)
{
    CString strURL;
    if (GetAutoLaunchUrl() && OverURL(PointCellRelative, strURL))
		ShellExecute(NULL, _T("open"), strURL, NULL,NULL, SW_SHOW);
	else if(OverURL(PointCellRelative, strURL))
	{
		SendMessageToParent(m_nRow, m_nCol, GVN_URLCLICK);
	}
}

// Return TRUE if you set the cursor
BOOL CGridURLCell::OnSetCursor()
{
	CString strURL;
    CPoint pt(GetMessagePos());
    GetGrid()->ScreenToClient(&pt);
    
	BOOL old = m_bOverUrl;
	m_bOverUrl = OverURL(pt, strURL);

	if(old != m_bOverUrl)
	{
		GetGrid()->RedrawCell(m_nRow,m_nCol);
	}

    if (m_bOverUrl)
    {
		m_bOverUrl = true;
        SetCursor(g_hLinkCursor);
		return TRUE;
	}
	else
		return CGridCell::OnSetCursor();
}

#ifndef _WIN32_WCE
HCURSOR CGridURLCell::GetHandCursor()
{
	if (g_hLinkCursor == NULL)		// No cursor handle - load our own
	{
       g_hLinkCursor = LoadCursor(NULL,IDC_HAND );
	}

	return g_hLinkCursor;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions

BOOL CGridURLCell::HasUrl(CString str)
{
    int nNumPrefixes = sizeof(g_szURIprefixes) / sizeof(g_szURIprefixes[0]);
    for (int i = 0; i < nNumPrefixes; i++)
        //if (str.Left(g_szURIprefixes[i].nLength) == g_szURIprefixes[i].szURLPrefix)
        if (str.Find(g_szURIprefixes[i].szURLPrefix) >= 0)
            return TRUE;

    return FALSE;
}

// here we figure out if we are over a URL or not
BOOL CGridURLCell::OverURL(CPoint& pt, CString& strURL)
{
    //TRACE2("Checking point %d,%d\n",pt.x,pt.y);

	BOOL bOverURL = FALSE;
	CSize size = GetTextExtent(GetText());

	
	CPoint center = m_Rect.CenterPoint();

	if ((m_nFormat & DT_RIGHT) && pt.x >= (m_Rect.right - size.cx))
	{
		bOverURL = TRUE;
	}	
	else if ((m_nFormat & DT_CENTER) && 
             ((center.x - (size.cx/2)) <= pt.x) && (pt.x <= (center.x + (size.cx/2))) )
	{
		bOverURL = TRUE;
	}
	else if (pt.x <= (size.cx + m_Rect.left))
	{
		bOverURL = TRUE;
	}

	if(pt.y+m_Rect.top<m_Rect.CenterPoint().y-size.cy/2 || pt.y+m_Rect.top>=m_Rect.CenterPoint().y+size.cy/2)
	{
		bOverURL = FALSE;
	}

    if (!bOverURL)
        return FALSE;

	else
		return TRUE;
}

