// DlgLeftWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "FrameLeftDlg.h"
#include "FrameRightDlg.h"
#include "MyMessageHandler.h"
#include "FrameDlg.h"

// CDlgLeftWnd 对话框


IMPLEMENT_DYNAMIC(CFrameLeftDlg, CDialog)

CFrameLeftDlg::CFrameLeftDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrameLeftDlg::IDD, pParent)
{
	m_pTreeList = NULL;
	m_pCurTreeListItem = NULL;
	m_vecTreeCopyItem.clear();
}

CFrameLeftDlg::~CFrameLeftDlg()
{
}

void CFrameLeftDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFrameLeftDlg, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TLN_SELCHANGED,2001,OnItemSelChanged)
	ON_NOTIFY(TLN_ITEMCHECK, 2001, OnTreeCheck)
	ON_NOTIFY(TLN_ITEM_RCLICK,2001,&CFrameLeftDlg::OnTreeRClick) /*右键树单击 */

	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgLeftWnd 消息处理程序

BOOL CFrameLeftDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CBitmap bmp;

	CreateTree();
	
	//启动加载客户树线程
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &LoadTreeData,this, 0, &m_uThreadId);
	if( m_hThread == NULL )
	{
		PRINT_LOG("启动初始化树数据失败！");
	}

	return TRUE; 
}


void CFrameLeftDlg::OnTreeCheck( NMHDR * pNMHDR, LRESULT * result )
{
	NMTREELIST * pNMTreelist = (NMTREELIST *)pNMHDR;
	if(!pNMTreelist ) return;

	//设置单选
	CTreeListItem * pItem = pNMTreelist->pItem;
	int nCol = pNMTreelist->iCol;
	if ( NULL == pItem )
	{
		return;
	}

	((CMyMessageHandler*)g_ztui->m_pMsgHandler)->OnTreeListCtrlSelChange(pItem);
}

void CFrameLeftDlg::CreateTree()
{
	CRect rect(0,25,100,200);
	m_pTreeList = new CTreeListCtrl;

	m_pTreeList->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP |WS_CLIPSIBLINGS, rect, this, 2001);
	

	m_pTreeList->SetStyle(0
		| TLC_TREELIST								// TreeList or List
		| TLC_MULTIPLESELECT						// single or multiple select
		| TLC_SHOWSELACTIVE							// show active column of selected item
		| TLC_SHOWSELALWAYS							// show selected item always
		//| TLC_SHOWSELFULLROWS						// show selected item in fullrow mode

		//| TLC_HEADER								// show header
		| TLC_HGRID									// show horizonal grid lines
		| TLC_VGRID									// show vertical grid lines
		| TLC_TGRID									// show tree horizonal grid lines ( when HGRID & VGRID )
		///| TLC_HGRID_EXT								// show extention horizonal grid lines
		| TLC_VGRID_EXT								// show extention vertical grid lines
		///| TLC_HGRID_FULL							// show full horizonal grid lines
		//		| TLC_READONLY								// read only
	
		| TLC_TREELINE								// show tree line
		| TLC_ROOTLINE								// show root line
		| TLC_BUTTON								// show expand/collapse button [+]
		//| TLC_CHECKBOX								// show check box
		//| TLC_LOCKBOX								// show lock box
		| TLC_IMAGE									// show image
		| TLC_HOTTRACK								// show hover text 
		//| TLC_IMAGE_PLUS							// show image plus

		//      | TLC_DRAG									// drag support
		| TLC_DROP									// drop support
		//		| TLC_DROPTHIS								// drop self support
		//		| TLC_DROPROOT								// drop on root support

		| TLC_HEADDRAGDROP							// head drag drop
		| TLC_HEADFULLDRAG							// head funn drag
		//		| TLC_NOAUTOCHECK							// do NOT auto set checkbox of parent & child
		//		| TLC_NOAUTOLOCK							// do NOT auto set lockbox of parent & child
		);

	//m_pTreeList->SetImageList(&m_imgList);
	
	m_ftList.CreatePointFont(100,"黑体");
	m_pTreeList->SetFont(&m_ftList);
	m_pTreeList->SetBkColor(RGB(255,255,255));
	m_pTreeList->SetItemHeight(30);
	
}
void CFrameLeftDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(&rect);
	if(m_pTreeList !=NULL)
	{
		m_pTreeList->MoveWindow(0,25,cx,cy);
		if (m_pTreeList->GetColumnCount() == 1)
		{
			CRect rect;
			m_pTreeList->GetWindowRect(rect);
			m_pTreeList->SetColumnWidth(0, rect.Width());
		}
		rect.left += 1;
		rect.top += 0;
		rect.bottom -= 2;
		rect.right -= 1;
		m_pTreeList->MoveWindow(&rect);
	}
}

BOOL CFrameLeftDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnEraseBkgnd(pDC);
}

void CFrameLeftDlg::OnItemSelChanged( NMHDR * pNotifyStruct, LRESULT * result )
{
	NMTREELIST * pNMTreelist = (NMTREELIST *)pNotifyStruct;
	if(!pNMTreelist ) return;

	CTreeListItem * pItem = pNMTreelist->pItem;
	int nCol = pNMTreelist->iCol;
	if ( NULL == pItem )
	{
		return;
	}

	g_pMainDlg->OnLeftTreeSelChange(pItem);
}

BOOL CFrameLeftDlg::PreTranslateMessage(MSG* pMsg)
{
	ASSERT(pMsg != NULL);
	if( pMsg->message == WM_KEYDOWN )
	{
		/*CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_MY_COMBOX);
		if( pMsg->wParam == VK_RETURN )
		{
			if( pCombox->GetFocus() )
			{
				OnButtonClick(IDC_MY_BUTTON);
			}

			return TRUE;
		}
		else if( pMsg->wParam == VK_ESCAPE )
		{
			return TRUE;
		}*/
	}

	return __super::PreTranslateMessage(pMsg);
}

void CFrameLeftDlg::OnTreeRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	return;

	NMTREELIST * pNMTreelist = (NMTREELIST *)pNMHDR;
	if(!pNMTreelist )
	{
		return;
	}


	m_pCurTreeListItem = pNMTreelist->pItem;

	//判断级数，最大允许创建两级
	int nCurLevel = 0;

	CPoint rPoint;
	CPoint rPointInTree;
	GetCursorPos(&rPoint);
	rPointInTree = rPoint;
	ScreenToClient(&rPointInTree);

	CMenu menu;
	CMenu submenu;  
	menu.CreateMenu();
	VERIFY(submenu.CreateMenu());

	if( nCurLevel == 0 )
	{
		submenu.AppendMenu(MF_STRING, 20001,_T("添加根分组")); 
	}
	else if( nCurLevel == 1 )
	{
		submenu.AppendMenu(MF_STRING, 20001,_T("添加子分组"));
		submenu.AppendMenu(MF_STRING, 20002,_T("删除分组"));
		submenu.AppendMenu(MF_STRING, 20003,_T("重命名"));
	}	
	else if( nCurLevel == 2 )
	{
		submenu.AppendMenu(MF_STRING, 20002,_T("删除分组"));
		submenu.AppendMenu(MF_STRING, 20003,_T("重命名"));

		if( m_vecTreeCopyItem.size() == 0 )
		{
			submenu.EnableMenuItem(20005, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}
	}
	else if( nCurLevel == 3 )
	{
		submenu.AppendMenu(MF_STRING, 20004,_T("剪切"));
	}
	else
	{
		return;
	}
	
	//GetItemSubData(m_pCurTreeListItem);
	
	menu.AppendMenu(MF_POPUP, (UINT_PTR)submenu.m_hMenu, TEXT("")); 
	TrackPopupMenuEx(menu.GetSubMenu(0)->m_hMenu, TPM_LEFTALIGN, rPoint.x, rPoint.y, m_hWnd, NULL);
	menu.Detach();
	menu.DestroyMenu(); 
}

LeftTreeInfo  *m_LeftTreeInfo = NULL;

void CFrameLeftDlg::InitTreeData()
{
	 m_LeftTreeInfo->loadLeftTree(m_pTreeList);
}

UINT32 WINAPI CFrameLeftDlg::LoadTreeData(void* pArgument)
{
	CFrameLeftDlg* pThis = (CFrameLeftDlg*)pArgument;

	pThis->InitTreeData();
	return 0;
}
