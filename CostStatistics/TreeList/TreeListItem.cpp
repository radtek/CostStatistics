// TreeListItem.cpp: implementation of the CTreeListItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeListItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTreeListItem::CTreeListItem( int nSubItem ) :
	m_pParent( NULL ),
	m_pChild( NULL ),
	m_pPrev( NULL ),
	m_pNext( NULL ),
	m_dwState( TLIS_SHOWCHECKBOX | TLIS_SHOWLOCKBOX | TLIS_CHECKED_NULL | TLIS_LOCKED_NULL ),
	m_dwData( 0 ),
	m_dwData2( 0 ),
	m_nImage( 0 ),
	m_clrBk(RGB(255, 255, 255)),
	m_nSelectedImage( 0 ),
	m_nExpandImage( 0 ),
	m_nExpandSelectedImage( 0 ),
	m_nImagePlus( 0 ),
	m_nSelectedImagePlus( 0 ),
	m_nExpandImagePlus( 0 ),
	m_nExpandSelectedImagePlus( 0 ),
//	m_pImageList( NULL ),
	m_nLevel( 0 ),
	m_nChild( 1 ),
	m_nVisibleChild( 1 ),
	m_color(0x80000000),
    m_bShadow(false),
	m_bShowShadow(false)
{
	m_nNodeID = 0;
	nCreatorId = 0;
	m_nOrderNum = 0;
	m_clrTxt = 0;
	for( int iSubItem = 0; iSubItem < nSubItem; iSubItem++ )
	{
		m_arSubItems.Add( "" );
		m_arColImage.Add(-1);
		m_arrData.Add(0);
	}
}

CTreeListItem::~CTreeListItem()
{
	if(m_bShadow)
	{
		if(m_pParent)
		{
			for(int i=0;i<m_pParent->m_shadowList.size();i++)
			{
				if(m_pParent->m_shadowList[i]->m_pRealNode == this)
				{
					delete m_pParent->m_shadowList[i];
					m_pParent->m_shadowList.erase(m_pParent->m_shadowList.begin()+i);
					break;
				}
			}
		}
	}
	else
	{
		for(int i=0;i<m_shadowList.size();i++)
		{
			delete m_shadowList[i];
		}
	}
}


void CTreeListItem::Expand()
{
	SetState( TLIS_EXPANDED, 0 );
	//m_bShowShadow = true;
}

void CTreeListItem::Collapse()
{
	SetState( 0, TLIS_EXPANDED );
	//m_bShowShadow = false;
}

DWORD CTreeListItem::GetState( DWORD dwStateMask )
{
	return m_dwState&dwStateMask;
}

void CTreeListItem::SetState( DWORD dwAddStatus, DWORD dwRemoveStatus )
{
	if( dwAddStatus&TLIS_EXPANDED )
		dwAddStatus |= TLIS_EXPANDEDONCE;

	if( dwRemoveStatus&TLIS_EXPANDEDONCE )
		dwRemoveStatus |= TLIS_EXPANDED;

	m_dwState |= dwAddStatus;
	m_dwState &=~dwRemoveStatus;
	return;
}

void CTreeListItem::GetImage( int& nImage, int& nSelectedImage, int& nExpandImage, int& nExpandSelectedImage )
{
	nImage					= m_nImage;
	nSelectedImage			= m_nSelectedImage;
	nExpandImage			= nExpandImage;
	nExpandSelectedImage	= nExpandSelectedImage;
}

void CTreeListItem::SetImage( int nImage, int nSelectedImage, int nExpandImage, int nExpandSelectedImage )
{
	m_nImage				= nImage;
	m_nSelectedImage		= nSelectedImage;
	m_nExpandImage			= nExpandImage;
	m_nExpandSelectedImage	= nExpandSelectedImage;
}

void CTreeListItem::GetImagePlus( int& nImagePlus, int& nSelectedImagePlus, int& nExpandImagePlus, int& nExpandSelectedImagePlus )
{
	nImagePlus					= m_nImagePlus;
	nSelectedImagePlus			= m_nSelectedImagePlus;
	nExpandImagePlus			= nExpandImagePlus;
	nExpandSelectedImagePlus	= nExpandSelectedImagePlus;
}

void CTreeListItem::SetImagePlus( int nImagePlus, int nSelectedImagePlus, int nExpandImagePlus, int nExpandSelectedImagePlus )
{
	m_nImagePlus				= nImagePlus;
	m_nSelectedImagePlus		= nSelectedImagePlus;
	m_nExpandImagePlus			= nExpandImagePlus;
	m_nExpandSelectedImagePlus	= nExpandSelectedImagePlus;
}

LPCTSTR CTreeListItem::GetText( int nIndex )
{
	if( nIndex >= m_arSubItems.GetSize() )
	{
		return NULL;
	}

	return m_arSubItems[nIndex];

}

BOOL CTreeListItem::SetText( LPCTSTR lpszItem, int nIndex )
{
	if( nIndex >= m_arSubItems.GetSize() )
		return FALSE;

		m_arSubItems[nIndex] = lpszItem;
		
	

	return TRUE;
}

void	CTreeListItem::SetColImage(int nCol, int nImage)
{
	if( nCol >= m_arSubItems.GetSize() || nCol<0)
		return ;

	m_arColImage[nCol] = nImage;

}

DWORD CTreeListItem::GetData(int nCol)
{
	if(nCol==0)
		return m_dwData;
	else
	{
		return m_arrData[nCol];
	}
}

void CTreeListItem::SetData( DWORD dwData,int nCol )
{
	if(nCol ==0)
		m_dwData = dwData;
	else
	{
		m_arrData[nCol] = dwData;
	}
}

DWORD CTreeListItem::GetData2()
{
	return m_dwData2;
}

void CTreeListItem::SetData2( DWORD dwData )
{
	m_dwData2 = dwData;
}

BOOL CTreeListItem::IsShowTreeImage()
{
	if( m_dwState&TLIS_SHOWTREEIMAGE )
		return TRUE;
	else
		return FALSE;
}

void CTreeListItem::ShowTreeImage( BOOL bShow )
{
	if( bShow )
		m_dwState |= TLIS_SHOWTREEIMAGE;
	else
		m_dwState &=~TLIS_SHOWTREEIMAGE;
}

BOOL CTreeListItem::IsShowCheckBox()
{
	if( m_dwState&TLIS_SHOWCHECKBOX )
		return TRUE;
	else
		return FALSE;
}

void CTreeListItem::ShowCheckBox( BOOL bShow )
{
	if( bShow )
		m_dwState |= TLIS_SHOWCHECKBOX;
	else
		m_dwState &=~TLIS_SHOWCHECKBOX;
}

BOOL CTreeListItem::IsShowLockBox()
{
	if( m_dwState&TLIS_SHOWLOCKBOX )
		return TRUE;
	else
		return FALSE;
}

void CTreeListItem::ShowLockBox( BOOL bShow )
{
	if( bShow )
		m_dwState |= TLIS_SHOWLOCKBOX;
	else
		m_dwState &=~TLIS_SHOWLOCKBOX;
}

BOOL CTreeListItem::GetSelected()
{
	if( GetState()&TLIS_SELECTED )
		return TRUE;
	else
		return FALSE;
}

void CTreeListItem::SetSelected( BOOL bSelected )
{
	if( bSelected )
		SetState( TLIS_SELECTED, 0 );
	else
		SetState( 0, TLIS_SELECTED );
}

int CTreeListItem::GetCheck()
{
	ASSERT( GetState()&TLIS_CHECKED || GetState()&TLIS_CHECKED_NULL );
	
	if( ( GetState()&TLIS_CHECKEDMASK) == (DWORD)TLIS_CHECKEDPART )
		return 2;
	
	if( GetState()&TLIS_CHECKED )
		return 1;

	return 0;
}

void CTreeListItem::SetCheck( BOOL bCheck )
{
	if( bCheck )
		SetState( TLIS_CHECKED, TLIS_CHECKED_NULL );
	else
		SetState( TLIS_CHECKED_NULL, TLIS_CHECKED );
}

int CTreeListItem::GetLock()
{
	ASSERT( GetState()&TLIS_LOCKED || GetState()&TLIS_LOCKED_NULL );

	if( ( GetState()&TLIS_LOCKEDMASK) == (DWORD)TLIS_LOCKEDPART )
		return 2;
	
	if( GetState()&TLIS_LOCKED )
		return 1;

	return 0;
}

void CTreeListItem::SetLock( BOOL bLock )
{
	if( bLock )
		SetState( TLIS_LOCKED, TLIS_LOCKED_NULL );
	else
		SetState( TLIS_LOCKED_NULL, TLIS_LOCKED );
}
/*
void CTreeListItem::SetImageList( CImageList* pImageList )
{
	ASSERT( pImageList != NULL );
	m_pImageList = pImageList;
}

void CTreeListItem::SetImage( int nIndex )
{
	return;
}

int CTreeListItem::GetImage()
{
	return 0;
}
*/
