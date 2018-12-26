// GridCellAttachments.cpp: implementation of the CGridURLCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCellAttachments.h"
#include "GridCtrl.h"

IMPLEMENT_DYNCREATE(CGridCellAttachments, CGridCell)

#ifndef _WIN32_WCE
HCURSOR CGridCellAttachments::g_hLinkCursor = NULL;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridCellAttachments::CGridCellAttachments()
{
#ifndef _WIN32_WCE
    g_hLinkCursor = GetHandCursor();
#endif
	m_bLaunchUrl = TRUE;
	m_clrUrl	=	RGB(0,0,128);	//GetSysColor(COLOR_HIGHLIGHT);

#ifdef _WIN32_WCE
    m_nFormat = DT_LEFT|DT_VCENTER|DT_NOPREFIX;
#else
    m_nFormat = DT_LEFT|DT_VCENTER|DT_NOPREFIX | DT_END_ELLIPSIS;
#endif

	m_nRow = -1;
	m_nCol = -1;
}

CGridCellAttachments::~CGridCellAttachments()
{
	while(m_ElementArray.GetSize()>0){
		ElementInfo*	pElementInfo	=	m_ElementArray.GetAt(0);
		if(pElementInfo)
			delete	pElementInfo;
		m_ElementArray.RemoveAt(0);
	}
}

BOOL CGridCellAttachments::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd)
{
	m_nRow = nRow;
	m_nCol = nCol;

    // Good a place as any to store the bounds of the rect
    m_Rect = rect;

	m_nFormat	=	this->GetFormat() & ~DT_SINGLELINE;
    return CGridCell::Draw(pDC, nRow, nCol, rect, bEraseBkgnd);
}

#pragma warning(disable:4100)
BOOL CGridCellAttachments::Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar)
{
    return FALSE;
}

#pragma warning(default:4100)

void CGridCellAttachments::OnClick(CRect cellRect, CPoint PointCellRelative)
{
#ifndef _WIN32_WCE
    CString strURL;
    if (GetAutoLaunchUrl() && IsOverHyperlink(PointCellRelative, strURL))
		ShellExecute(NULL, _T("open"), strURL, NULL,NULL, SW_SHOW);
	else if(IsOverHyperlink(PointCellRelative, strURL))
	{
		SendMessageToParent(m_nRow, m_nCol, GVN_URLCLICK);
	}
#endif
}

// Return TRUE if you set the cursor
BOOL CGridCellAttachments::OnSetCursor()
{
#ifndef _WIN32_WCE
    CString strURL;
    CPoint pt(GetMessagePos());
    GetGrid()->ScreenToClient(&pt);
    pt = pt - m_Rect.TopLeft();
    if (IsOverHyperlink(pt, strURL)){
        SetCursor(g_hLinkCursor);
		return TRUE;
	}
	else
#endif
		return CGridCell::OnSetCursor();
}

#ifndef _WIN32_WCE
HCURSOR CGridCellAttachments::GetHandCursor()
{
	if (g_hLinkCursor == NULL)		// No cursor handle - load our own
	{
		g_hLinkCursor	=	AfxGetApp()->LoadStandardCursor(IDC_HAND);
	}
	return g_hLinkCursor;
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions


BOOL	CGridCellAttachments::IsOverHyperlink(CPoint& pt, CString& szPath)
{
	CGridCtrl* pGrid = GetGrid();
	ASSERT(pGrid);

	int	nLineTop	=	GetMargin();	//行距离单元格上边的距离
	for(int i=0; i<m_ElementArray.GetSize(); i++){
		ElementInfo*	pElementInfo	=	m_ElementArray.GetAt(i);
		CString	szTitle	=	pElementInfo->szTitle;

		//空行只取行高
		if(szTitle.IsEmpty()){
			/*
			TEXTMETRIC tm;
			CGridCtrl* pGrid = GetGrid();
			CDC*	pDc	=	pGrid->GetDC();
			pDc->GetTextMetrics(&tm);
			*/
			CSize	sizeLine	=	GetTextExtent(_T(" "));
			sizeLine	-=	CSize(2*GetMargin(), 2*GetMargin());	//GetTextExtent加上了Margin，必须去掉
		
			nLineTop	+=	sizeLine.cy;
			continue;
		}

		// 获取单元格图片显示区域
		int nImageWid = 0;
		if (pGrid->GetImageList() && GetImage() >= 0)
		{
			IMAGEINFO Info;
			if (pGrid->GetImageList()->GetImageInfo(GetImage(), &Info))
			{
				// Added by Yogurt
				int nImageWidth = Info.rcImage.right-Info.rcImage.left;
				int nImageHeight = Info.rcImage.bottom-Info.rcImage.top;

				int margin = m_Rect.Height()-nImageHeight;
				CPoint topLeft = m_Rect.TopLeft();
				
				if(margin>0)
				{
					topLeft.y += margin/2;
				}
				nImageWid = nImageWidth;
			}
		}

		//取得行文字的区域
		CSize	sizeLine	=	GetTextExtent(szTitle);
		sizeLine	-=	CSize(2*GetMargin(), 2*GetMargin());	//GetTextExtent加上了Margin，必须去掉
		
		CRect	rectLine(	0,	nLineTop,	0,	nLineTop+sizeLine.cy	);
		if(	m_nFormat & DT_RIGHT	) {
			rectLine.left	=	m_Rect.Width()-sizeLine.cx;
		} else if(	m_nFormat & DT_CENTER	) {
			rectLine.left	=	(m_Rect.Width() - sizeLine.cx - nImageWid - GetMargin())/2 + nImageWid + GetMargin();
			rectLine.right	=	rectLine.left + sizeLine.cx;
		} else {
			rectLine.left   =   GetMargin() + nImageWid + GetMargin();
			rectLine.right	=	GetMargin() + nImageWid + GetMargin() + sizeLine.cx;
		}

		if(rectLine.PtInRect(pt)){
			szPath	=	pElementInfo->szPath;
			return	TRUE;
		}

		nLineTop	+=	sizeLine.cy;
	}
	return	FALSE;
}

void	CGridCellAttachments::SetText(LPCTSTR szText)	{ 
	m_szRawText		=	szText;
	
	m_strText		=	_T("");
	if(this->ParseElement()){
		for(int i=0; i<m_ElementArray.GetSize(); i++){
			ElementInfo*	pElementInfo	=	m_ElementArray.GetAt(i);
			if(i>0)
				m_strText	+=	_T("\n");
			m_strText	+=	pElementInfo->szTitle;
		}
	}

	SetTextClr(m_clrUrl);

	LOGFONT* pLogFont = new LOGFONT;
	if (pLogFont != NULL)
	{
		memcpy(pLogFont, GetFont(), sizeof(LOGFONT));
		pLogFont->lfUnderline = TRUE;

		SetFont(pLogFont);

		delete pLogFont;
		pLogFont = NULL;
	}

}

BOOL	CGridCellAttachments::ParseElement( ){
	while(m_ElementArray.GetSize()>0){
		ElementInfo*	pElementInfo	=	m_ElementArray.GetAt(0);
		if(pElementInfo)
			delete	pElementInfo;
		m_ElementArray.RemoveAt(0);
	}

	CString	strText		=	m_szRawText;
	strText.Replace(	_T("\r\n"),	_T("\n")	);
	strText.Replace(	_T("\r"),	_T("\n")	);

	CString	strLineText	=	_T("");
	for (int i = 0; i < strText.GetLength(); i++){
		if (strText[i] != '\n'){
			strLineText	+=	strText[i];
		}
		if (strText[i] == '\n' || i==strText.GetLength()-1	){
			int		nIndex	=	strLineText.Find(ELEMENT_DELIMITER);
			CString	szPath	=	(nIndex>=0) ? (strLineText.Mid(0,nIndex)) : strLineText;
			CString	szTitle	=	(nIndex>=0) ? (strLineText.Mid(nIndex+1)) : _T("");
			if(szTitle.IsEmpty())
				szTitle	=	szPath;
			
			ElementInfo*	pElementInfo	=	new	ElementInfo();
			pElementInfo->szRawText	=	strLineText;
			pElementInfo->szPath	=	szPath;
			pElementInfo->szTitle	=	szTitle;
			m_ElementArray.Add(pElementInfo);

			strLineText	=	_T("");
		}		
	}
	return	TRUE;
}
