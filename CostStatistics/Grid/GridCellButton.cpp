
#include "stdafx.h"
#include "GridCell.h"
#include "GridCtrl.h"
#include "GridCellButton.h"


#pragma comment(lib, "gdiplus.lib") //在工程属性中添加亦可



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLOR_BTN_CHECKED	RGB(250, 250, 0)

IMPLEMENT_DYNCREATE(CGridCellButton, CGridCell)  

CGDIMgr gdrMgr;
CGDIMgr::CGDIMgr()
{
	GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL); 
}
CGDIMgr::~CGDIMgr()
{
	GdiplusShutdown(m_gdiplusToken); 
}
CGridCellButton::CGridCellButton(void) : CGridCell()
{
	m_bSearchMode = FALSE;
	m_bChecked = FALSE;
	m_btnDnPos = 0;			//没有按键按下

    m_clrText = RGB(0,0,0);
    m_clrMouseInText = RGB(255, 0, 0);
    m_clrBK = ::GetSysColor(COLOR_BTNFACE);
	m_bBtnVCenter = FALSE;
}   

CGridCellButton::~CGridCellButton(void)   
{   
	
}   

BOOL CGridCellButton::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)   
{   
    if( rect.Width() == 0 || rect.Height() == 0)  // prevents imagelist item from drawing even    
        return FALSE;                             //  though cell is hidden    
	
	COLORREF clrBK = RGB(255,255,255);

	// 设置单元格背景色
	CGridDefaultCell* pDefaultCell = (CGridDefaultCell*)GetDefaultCell();
	if (!pDefaultCell)
	{
		return FALSE;
	}

	if (GetBackClr() == CLR_DEFAULT)
	{
		clrBK = pDefaultCell->GetBackClr();
	}
	else
	{
		bEraseBkgnd = TRUE;
		clrBK = GetBackClr();
	}

	if(GetGrid()->IsSelected(*this, nRow, nCol))
	{
		clrBK = ::GetSysColor(COLOR_HIGHLIGHT);
	}
   
	CRect rcBk = rect;
	rcBk.bottom++;
	pDC->FillSolidRect(&rcBk,clrBK);
    pDC->SetBkMode(TRANSPARENT);   

    CGridCtrl * pGrid = GetGrid();
    ASSERT(pGrid);
    if (!pGrid) return FALSE;

    CFont Font;   
    if(GetFont())   
    {   
        try{   
            if(Font.CreateFontIndirect(GetFont()))   
                pDC->SelectObject(&Font);   
        }   
        catch(...)   
        {   
            throw;   
        }   
    }
	m_rect = rect;
    rect.top++;   
    rect.bottom--;   
	
	if ( m_bBtnVCenter )
	{
		int nHegiht  = pDC->GetTextExtent("哈哈2").cy;
		int nCenterV = (rect.top + rect.bottom) / 2;
		
		int nBTop    = nCenterV - nHegiht / 2 - 4;
		int nBBom    = nCenterV + nHegiht / 2 + 4;

		if ( rect.top < nBTop )
			 rect.top = nBTop;

		if ( rect.bottom > nBBom)
			rect.bottom = nBBom;
	}
		

	m_arrBtnRect.RemoveAll();
	vector<CString> strVec = GetTextVec();
	int btnCount = strVec.size();
	int width = rect.Width()/btnCount;
	for(int i = 0;i < btnCount;i++)
	{
		CRect dRct;
		dRct.left = i*width+rect.left;
		dRct.right = dRct.left + width;
		dRct.top = rect.top;
		dRct.bottom = rect.bottom;
		if(m_bSearchMode && i == 2 && m_bChecked)
		{
			
			pDC->FillSolidRect(&dRct, COLOR_BTN_CHECKED);
			pDC->Draw3dRect(dRct,RGB(160,160,160),RGB(160,160,160));   
			m_arrBtnRect.Add(dRct);
		}
		else
		{
			
			int cx = pDC->GetTextExtent(strVec[i]).cx;
			cx += 20;
			if(strVec.size()==1)
			{
				if(dRct.Width()>cx)
				{
					int xx = dRct.Width()-cx;
					xx /= 2;
					dRct.left+= xx;
					dRct.right-=xx;
					
				}
			}
			Gdiplus::Graphics graphics(pDC->GetSafeHdc()); 
			
			int r = GetRValue(clrBK);
			int g = GetGValue(clrBK);
			int b = GetBValue(clrBK);
			Gdiplus::SolidBrush gbrBk = (Gdiplus::Color(255,r,g,b));

			graphics.FillRectangle(&gbrBk,dRct.left,dRct.top,dRct.Width(),dRct.Height());

			////  刷子
			Gdiplus::LinearGradientBrush linGrBrush(Point(0, dRct.top),Point(0, dRct.bottom),Gdiplus::Color(255,255,255,255),Gdiplus::Color(255, 220, 220, 255));  //  绘制区域
			Gdiplus::LinearGradientBrush linGrBrush2(Point(0, dRct.top),Point(0, dRct.bottom),Gdiplus::Color(255,210,210,255),Gdiplus::Color(255, 220, 220, 255));  //  绘制区域
			Gdiplus::Pen ps(Gdiplus::Color(120,120,120,255));
			//

			if(m_btnDnPos > 0 && m_btnDnPos == i+1)
			{
				graphics.FillRectangle(&linGrBrush2, Gdiplus::Rect(dRct.left,dRct.top, dRct.Width(), dRct.Height()));
				
				graphics.DrawRectangle(&ps,Gdiplus::Rect(dRct.left,dRct.top, dRct.Width(), dRct.Height()));
				
				
			}
			else
			{
               graphics.FillRectangle(&linGrBrush, Gdiplus::Rect(dRct.left,dRct.top, dRct.Width(), dRct.Height()));
				
			   graphics.DrawRectangle(&ps,Gdiplus::Rect(dRct.left,dRct.top, dRct.Width(), dRct.Height()));

								
			}
			m_arrBtnRect.Add(dRct);
		}
		pDC->SetTextColor(m_clrText);
		pDC->DrawText(strVec[i], dRct,DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
    }


    m_nRow = nRow;
    m_nCol = nCol;

    return TRUE;   
}   

void CGridCellButton::OnClick(CRect cellRect,CPoint PointCellRelative)
{   
	if(m_btnDnPos<1) return;
	CGridCtrl* pCtrl = GetGrid();
	int xx = m_btnDnPos;

	if (m_btnDnPos !=0)
	{
		m_btnDnPos = 0;
	}
    // 向最终的父窗口发送一个消息
	SendMessageToParent(m_nRow, m_nCol, GVN_BTNCLICK,xx - 1);
	pCtrl->Refresh();
}

void CGridCellButton::OnClickDown(CRect cellRect,CPoint PointCellRelative)   
{
	m_btnDnPos = 0;
#if GRID_BUTTON_SHOW_TEXT
        vector<CString> strVec = GetTextVec();
        int btnCount = strVec.size();
#else
        vector<int> strVec = GetImageVec();
        int btnCount = strVec.size();
#endif
		if(m_arrBtnRect.GetCount()<btnCount) return;
        int width = m_rect.Width()/btnCount;
        for(int i = 0;i < btnCount;i++)
        {
			
            CRect dRct=m_arrBtnRect[i];
           CPoint ptCursor=PointCellRelative;
		   ptCursor.x += cellRect.left;
		   ptCursor.y += cellRect.top;

            if(	dRct.PtInRect(ptCursor))
            {
                m_btnDnPos = i + 1;
                if(m_bSearchMode && m_nRow == 1 && m_btnDnPos == 3)
                {
                    m_bChecked = !m_bChecked;
                }
                break;
            }
        }

		GetGrid()->Refresh();
}