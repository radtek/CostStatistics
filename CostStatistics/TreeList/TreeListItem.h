// TreeListItem.h: interface for the CTreeListItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TREELISTITEM_H__CD98CFA2_C685_40FE_8F08_66A005EB3845__INCLUDED_)
#define AFX_TREELISTITEM_H__CD98CFA2_C685_40FE_8F08_66A005EB3845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
#include <set>
using namespace std;

#define TLIS_SELECTED			0x00000001		// selected
#define TLIS_EXPANDED			0x00000002		// expanded
#define TLIS_EXPANDEDONCE		0x00000004		// expanded once

#define TLIS_SHOWCHECKBOX		0x00000010		// show check box
#define TLIS_SHOWLOCKBOX		0x00000020		// show lock box
#define TLIS_SHOWTREEIMAGE		0x00000040		// show image

#define TLIS_SHOWBKCOLOR		0x00000100		// owner background color
#define TLIS_SHOWTEXTCOLOR		0x00000200		// owner text color

#define TLIS_CHECKED			0x00010000		// [V]
#define TLIS_CHECKED_NULL		0x00020000		// [ ]

#define TLIS_CHECKEDPART		0x00030000		// [?]
#define TLIS_CHECKEDMASK		0x00030000		// TLI_CHECKED | TLI_CHECKED_NULL

#define TLIS_LOCKED				0x01000000		// [L]
#define TLIS_LOCKED_NULL		0x02000000		// [ ]

#define TLIS_LOCKEDPART			0x03000000		// [?]
#define	TLIS_LOCKEDMASK			0x03000000		// TLI_LOCKED | TLI_LOCKED_NULL

#define TLIS_ALLSTATE			0xFFFFFFFF

class CShadowItem;



class CTreeListItem
{
public:
	CTreeListItem( int nSubItem );
	virtual ~CTreeListItem();

// Members
public:
	CTreeListItem*	m_pParent;
	CTreeListItem*	m_pChild;
	CTreeListItem*	m_pPrev;
	CTreeListItem*	m_pNext;
	int				m_nLevel;
	int				m_nChild;
	int				m_nVisibleChild;

	DWORD			m_dwState;
	DWORD			m_dwData;
	DWORD			m_dwData2;

	int				m_nImage;
	int				m_nSelectedImage;
	int				m_nExpandImage;
	int				m_nExpandSelectedImage;


	int				m_nImagePlus;
	int				m_nSelectedImagePlus;
	int				m_nExpandImagePlus;
	int				m_nExpandSelectedImagePlus;
	 void        SetNodeID( int NodeID ){ m_nNodeID = NodeID; }
	COLORREF        m_clrBk;
	COLORREF m_clrTxt;
//	CImageList*		m_pImageList;
	COLORREF		m_color;

	CStringArray		m_arSubItems;
	CArray<int>		m_arColImage;
	CArray<int> m_arrData;
	int m_nNodeID;
	bool            m_bShadow;
	bool			m_bShowShadow;
	vector<CShadowItem*>	m_shadowList;
	CShadowItem* GetShadowItem(int idx)
	{
		return m_shadowList[idx];
	}
	
// Oprations
public:
	void		Expand();
	void		Collapse();

	DWORD		GetState( DWORD dwStateMask = TLIS_ALLSTATE );
	void		SetState( DWORD dwAddStatus, DWORD dwRemoveStatus = 0 );

	void		GetImage( int& nImage, int& nExpandImage, int& nSelectedImage, int& nExpandSelectedImage );
	void		SetImage( int nImage, int nExpandImage, int nSelectedImage, int nExpandSelectedImage );
	void		GetImagePlus( int& nImagePlus, int& nExpandImagePlus, int& nSelectedImagePlus, int& nExpandSelectedImagePlus );
	void		SetImagePlus( int nImagePlus, int nExpandImagePlus, int nSelectedImagePlus, int nExpandSelectedImagePlus );
	void		SetColImage(int nCol, int nImage);
	LPCTSTR		GetText( int nIndex = 0 );
	BOOL		SetText( LPCTSTR lpszItem, int nIndex = 0 );

	DWORD		GetData(int nSubItem = 0);
	void		SetData( DWORD dwData,int nSubItem=0 );
	DWORD		GetData2();
	void		SetData2( DWORD dwData );

	BOOL		IsShowTreeImage();
	void		ShowTreeImage( BOOL bShow );

	BOOL		IsShowCheckBox();
	void		ShowCheckBox( BOOL bShow );

	BOOL		IsShowLockBox();
	void		ShowLockBox( BOOL bShow );

	int			GetSelected();
	void		SetSelected( BOOL bSelected = TRUE );

	int			GetCheck();
	void		SetCheck( BOOL bCheck = TRUE );

	int			GetLock();
	void		SetLock( BOOL bLock = TRUE );
public:
	int nCreatorId;
	int m_nOrderNum;
	CString m_strData;
//	void		SetImageList( CImageList* pImageList );
//	void		SetImage( int nIndex );
//	int			GetImage();
};


class CShadowItem
{
public:
	CShadowItem(CTreeListItem* pParent)
	{
		m_nImage = -1;
		m_clrBk = RGB(255,255,255);
		m_clrTxt = RGB(0,0,0);
		m_pParent = pParent;
		m_pRealNode = NULL;
	}
	void SetText(CString str, int nCol=0)
	{
		m_arrTxt[nCol] = str;
		if(m_pRealNode)
		{
			m_pRealNode->SetText(str,nCol);
		}
	}
	void SetData(DWORD data, int nCol=0)
	{
		m_arrData[nCol] = data;
		if(m_pRealNode)
		{
			m_pRealNode->SetData(data,nCol);
		}
	}
	void SetBkColor(DWORD clr)
	{
		m_clrBk = clr;
		if(m_pRealNode)
		{
			m_pRealNode->m_clrBk = clr;
		}
	}
	void SetTxtColor(DWORD clr)
	{
		m_clrTxt = clr;
		if(m_pRealNode)
		{
			m_pRealNode->m_clrTxt = clr;
		}
	}
	void SetImage(int nImage)
	{
		m_nImage = nImage;
		if(m_pRealNode)
		{
			m_pRealNode->SetImage(nImage,nImage,nImage,nImage);
		}
	}

	CString GetText(int nCol=0)
	{
		return m_arrTxt[nCol];
	}

	DWORD GetData(int nCol=0)
	{
		return m_arrData[nCol];
	}

	void SetRealNode(CTreeListItem* pItem)
	{
		m_pRealNode = pItem;
	}
public:
	CTreeListItem* m_pParent;
	CString m_strData;
	int m_nImage;
	DWORD m_clrBk;
	DWORD m_clrTxt;
	vector<CString>   m_arrTxt;
	vector<DWORD> m_arrData;

	CTreeListItem* m_pRealNode;			//真实节点。关闭时为NULL
};
#endif // !defined(AFX_TREELISTITEM_H__CD98CFA2_C685_40FE_8F08_66A005EB3845__INCLUDED_)
