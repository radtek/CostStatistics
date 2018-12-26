// DlgPrivilidge.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPrivilidge.h"
#include <algorithm>


// CDlgPrivilidge 对话框

bool JudgePrivilidge(int userID, CString modalKey,int nPrivilidge)
{
	CString sql;
	sql.Format("select Role from z_pub_modal_user_role where UserID = %d and ModalKey = '%s' and Role = %d",userID,modalKey,nPrivilidge);

	SQL_RESULT rt;
	if( !CDbOperator::GetInstance()->Query(sql,"role",rt) )
	{
		return false;
	}

	CDBResultOper obj;
	obj.SetResultMap(rt);
	int nRow = obj.GetRowCount();

	if( nRow == 0 )
	{
		return false;
	}

	return true;
}

bool IsAdmin(int nUserID)
{
	CDlgPrivilidge obj;
	return obj.IsUserAdmin(nUserID);
}

bool ASCOrder ( int elem1, int elem2 )
{
	return elem1 < elem2;
}


static int PRParseRole(CString str,CArray<int>& arr)
{
	str.Trim();
	if(str == "") return 0;

	while(true)
	{
		if(str == "") break;
		int pos = str.Find(',');
		if(pos <=0)
		{
			arr.Add(atoi(str));
			break;
		}
		else
		{
			CString left = str.Left(pos);
			arr.Add(atoi(left));
			str = str.Mid(pos+1);
			str.Trim();
		}
	}

	return arr.GetCount();
}


IMPLEMENT_DYNAMIC(CDlgPrivilidge, CDialog)

CDlgPrivilidge::CDlgPrivilidge(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrivilidge::IDD, pParent)
{
	m_enPrililidgeType	= EN_PRIVILIDGE_APPROVAL;
	m_bHaveModifyed		= false;
	m_arrUser.clear();
}

CDlgPrivilidge::~CDlgPrivilidge()
{
}

void CDlgPrivilidge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPrivilidge, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPrivilidge::OnBnClickedOk)
	ON_NOTIFY(TLN_ITEM_DCLICK,101,OnListDbClick)
END_MESSAGE_MAP()


// CDlgPrivilidge 消息处理程序
void CDlgPrivilidge::OnListDbClick(NMHDR* p,LRESULT* pL)
{
	NM_LISTVIEW * pNMListView = (NM_LISTVIEW *)p;
	int nCol   = pNMListView->iSubItem;

	int nTotalRow = m_treeList.GetCount();
	if( nTotalRow == 0 )
	{
		return;
	}

	//判断统级别是否已经有勾选项
	bool bManager = false;
	for(int i=0;i<m_arrUser.size();i++)
	{
		if( m_arrUser[i].userID == CLoginUserInfo::m_nUserID )
		{
			if(m_arrUser[i].bManager)
			{
				bManager = true;
			}

			break;
		}
	}

	if(!bManager)
	{
		MessageBox("您不是管理员,不能分配权限");
		return;
	}

	CTreeListItem *pItem = m_treeList.GetSelectedItem();
	if(pItem == NULL) return;

	int nRow = 0;
	CTreeListItem* pFirst = m_treeList.GetRootItem();
	while(pFirst)
	{
		if(pFirst == pItem) break;
		pFirst = pFirst->m_pNext;
		nRow++;
	}

	
	int nRole = nCol -1;

	if(nRole<0 || nRole>=m_arrRoles.GetCount())
	{
		return;
	}
	
	
	CString xx = pItem->GetText(nCol);
	if(xx == "")
	{
		pItem->SetText("√",nCol);
	}
	else
	{
		pItem->SetText("",nCol);
	}
	m_treeList.Invalidate();
	m_arrUser[nRow].arrRoles.clear();
	for(int i=0;i<m_arrRoles.GetCount();i++)
	{
		CString curRow = pItem->GetText(i+1);
		if(curRow != "")
		{
			m_arrUser[nRow].arrRoles.push_back(m_arrRoles[i].value);
		}
	}
	
	m_bHaveModifyed = true;
}
BOOL CDlgPrivilidge::OnInitDialog()
{
	SetWindowText("权限设置 (管理员通过双击单元格为成员设置权限)");
	CDialog::OnInitDialog();
	
	CRect rc;
	GetClientRect(&rc);
	rc.bottom -= 40;

	
	InitList(m_treeList,101);
	

	m_treeList.MoveWindow(0,0,rc.Width(),rc.Height());
	
	m_treeList.InsertColumn("人员",TLF_DEFAULT_LEFT,160);
	
	
	//添加群成员
	
	//加载权限
	for(int i=0;i<m_arrRoles.GetCount();i++)
	{
		int dftWid =(rc.Width()-180)/ (m_arrRoles.GetCount());
		if(dftWid<100)
		{
			dftWid = 100;
		}
		m_treeList.InsertColumn(m_arrRoles[i].name,TLF_DEFAULT_LEFT,dftWid);
		
	}

	//获取群成员信息并加载数据到列表中
	LoadBindGroup(); 
	if( m_treeList.GetSafeHwnd() != NULL  )
	{
		for(int i=0;i<m_arrUser.size();i++)
		{
			CTreeListItem* pItem = m_treeList.InsertItem(m_arrUser[i].userName);
			if(m_arrUser[i].bManager)
			{
				pItem->SetText(CString(pItem->GetText()) + "(管理员)");
			}

			for(int j=0;j<m_arrUser[i].arrRoles.size();j++)
			{
				int role = m_arrUser[i].arrRoles[j];
				for(int k=0;k<m_arrRoles.GetCount();k++)
				{
					if(role == m_arrRoles[k].value)
					{
						pItem->SetText("√",k+1);
					}
				}
			}
		}

	}

	//判断当前账户是否是管理员,IsUserManager 必须放在LoadBindGroup之后
	if( !IsUserAdmin(CLoginUserInfo::m_nUserID) )
	{
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CDlgPrivilidge::InitList(CTreeListCtrl& listCtrl,int id)
{
	listCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP, CRect(0,0,0,0), this, id);

	DWORD style = TLC_TREELIST								// TreeList or List
		| TLC_MULTIPLESELECT						// single or multiple select
		| TLC_SHOWSELACTIVE							// show active column of selected item
		| TLC_SHOWSELALWAYS							// show selected item always
		| TLC_SHOWSELFULLROWS						// show selected item in fullrow mode
		| TLC_HGRID									// show horizonal grid lines
		| TLC_VGRID									// show vertical grid lines
		| TLC_TGRID									// show tree horizonal grid lines ( when HGRID & VGRID )
		| TLC_HGRID_EXT								// show extention horizonal grid lines
		| TLC_VGRID_EXT								// show extention vertical grid lines
		| TLC_HGRID_FULL							// show full horizonal grid lines

		| TLC_BUTTON								// show expand/collapse button [+]

		| TLC_IMAGE									// show image
		| TLC_HOTTRACK								// show hover text 

		| TLC_DROP									// drop support

		| TLC_HEADDRAGDROP							// head drag drop
		| TLC_HEADFULLDRAG							// head funn drag
		|TLC_HEADER
		;

	style &= ~TLC_TREELINE;
	listCtrl.SetStyle(style);

	listCtrl.SetItemHeight(26);

	listCtrl.GetHeaderCtrl()->SetHeaderHeight(26);
}

void CDlgPrivilidge::OnBnClickedOk()
{
	if( !m_bHaveModifyed )
	{
		OnOK();
		return;
	}

	CStringArray arrSQL;
	CString sql;
	CString strIn;

	for(int i=0;i<m_arrUser.size();i++)
	{
		strIn.AppendFormat("%d,",m_arrUser[i].userID);
	}
	strIn.TrimRight(',');

	sql.Format("DELETE FROM z_pub_modal_user_role  WHERE ModalKey = '%s' AND UserID in (%s)  AND GroupID = %d",m_strModal,strIn,0);
	
	arrSQL.Add(sql);
	
	sql = "INSERT INTO z_pub_modal_user_role(UserID,Role,Authorizer,ModalKey,GroupID) VALUES ";

	int pvCount = 0;
	for(int i=0;i<m_arrUser.size();i++)
	{
		for(int j=0;j<m_arrUser[i].arrRoles.size();j++)
		{
			sql.AppendFormat("(%d,%d,%d,'%s',%d),",m_arrUser[i].userID,m_arrUser[i].arrRoles[j],CLoginUserInfo::m_nUserID,m_strModal,0);
			pvCount++;
		}
	}

	if(pvCount>0)
	{
		sql.TrimRight(',');
		arrSQL.Add(sql);
	}

	if( ExcuteBatchSQL(arrSQL) != 0 )
	{
		::MessageBox(this->GetSafeHwnd(),"保存数据库失败","提示：",MB_OK|MB_ICONWARNING);
		return;
	}

	::MessageBox(this->GetSafeHwnd(),"保存成功!","提示：",MB_OK|MB_ICONINFORMATION);
	OnOK();
}

void CDlgPrivilidge::OnListClick(NMHDR* p,LRESULT* pL)
{	
}

void CDlgPrivilidge::SetModalKey(CString strModal)
{
	m_strModal = strModal;
}


void CDlgPrivilidge::LoadBindGroup()
{
	if( m_arrUser.size() > 0 )
	{
		return;
	}

	int nUserID = CLoginUserInfo::m_nUserID;

	CString strSql;
	strSql.Format("SELECT t1.ItemID,\
				  t1.Name,\
				  t1.IsAdmin,\
				  (SELECT group_concat(t2.Role) FROM z_pub_modal_user_role t2 where  t2.ModalKey = '%s' AND t2.UserID = t1.ItemID) \
				  FROM z_user t1 \
				  WHERE t1.SCZT = 0",m_strModal);

	CString strKey = "UserID,Name,IsAdmin,Role";

	SQL_RESULT rt;
	if( CDbOperator::GetInstance()->Query(strSql,strKey,rt) )
	{
		CDBResultOper obj;
		obj.SetResultMap(rt);
		int nRow = obj.GetRowCount();

		if( nRow == 0 )
		{
			PRINT_LOG("获取权限信息失败！");
			return;
		}

		for(int i=0;i<nRow;i++)
		{
			CString strName = obj.GetValue("Name",i);
			int nUserID = atoi(obj.GetValue("UserID",i));
			bool bManager = atoi(obj.GetValue("IsAdmin",i)) == 1 ? true : false;


			CString strRol= obj.GetValue("Role",i);

			CArray<int> arrRol;
			PRParseRole(strRol,arrRol);


			bool bFind = false;
			for(int j=0;j<m_arrUser.size();j++)
			{
				if(m_arrUser[j].userID == nUserID)
				{
					if(!m_arrUser[j].bManager && bManager)
					{
						m_arrUser[j].bManager = true;
					}

					bFind = true;
					break;
				}
			}

			if(!bFind)
			{
				UserRole user;
				user.bManager = bManager;

				for(int j=0;j<arrRol.GetCount();j++)
				{
					user.arrRoles.push_back(arrRol[j]);
				}
				user.userID = nUserID;
				user.userName = strName;
				m_arrUser.push_back(user);
			}
		}
	}
}


int CDlgPrivilidge::ExcuteBatchSQL(CStringArray& arrSQL)
{
	if(arrSQL.IsEmpty()) return -1;

	CString strSql = "";
	for(int i=0;i<arrSQL.GetCount();i++)
	{
		strSql.Append(arrSQL[i]);
		strSql.Append(";");
	}

	if( CDbOperator::GetInstance()->ExecuteBatch(strSql) )
	{
		return 0;
	}
	
	return -1;
}

bool CDlgPrivilidge::IsUserAdmin(int nUserID)
{
	bool bRet = false;

	LoadBindGroup();

	for(int j=0;j<m_arrUser.size();j++)
	{
		if( m_arrUser[j].userID == CLoginUserInfo::m_nUserID )
		{
			if( m_arrUser[j].bManager )
			{
				bRet = true;
				break;
			}
		}
	}

	return bRet;
}

void CDlgPrivilidge::GetPrivlilidgeUserInfo(CString strMutex,std::vector<UserRole>& vecInfo)
{
	std::ostringstream oSql;
	oSql.str("");

	oSql << "SELECT DISTINCT "
		<< " UserID,"
		<< " Role,"
		<< "(SELECT tu.Name FROM z_user tu WHERE tu.ItemID = UserID ) AS UserName"
		<< " FROM z_pub_modal_user_role"
		<< " WHERE ModalKey = " << "\'" << strMutex << "\'"
		<< " ORDER BY role ASC";

	SQL_RESULT rt;
	if( CDbOperator::GetInstance()->Query(oSql.str().c_str(),"UserID,Role,UserName",rt) )
	{
		CDBResultOper obj;
		obj.SetResultMap(rt);
		int nRow = obj.GetRowCount();

		for( int i = 0;nRow;i++ )
		{
			UserRole stRole;

			stRole.userID   = atoi(obj.GetValue("UserID",i));
			stRole.userName = obj.GetValue("Name",i);
			stRole.arrRoles.push_back(atoi(obj.GetValue("Role",i)));

			vecInfo.push_back(stRole);
		}
	}
}

void CDlgPrivilidge::GetManagerUserID(CString strMutex,std::vector<int>& vecMgrID)
{
	m_strModal = strMutex;

	LoadBindGroup();

	for(int i=0;i<m_arrUser.size();i++)
	{
		if(m_arrUser[i].bManager)
		{
			vecMgrID.push_back(m_arrUser[i].userID);
		}		
	}
}


void CDlgPrivilidge::GetManagerUserInfo(CString strMutex,std::vector<UserRole>& vecMgrInfo)
{
	m_strModal = strMutex;

	LoadBindGroup();

	for(int i=0;i<m_arrUser.size();i++)
	{
		if(m_arrUser[i].bManager)
		{
			vecMgrInfo.push_back(m_arrUser[i]);
		}		
	}
}


void CDlgPrivilidge::SetType(EN_PRIVILIDGE_TYPE enType /*= EN_PRIVILIDGE_APPROVAL*/)
{
	m_enPrililidgeType = enType;
}	

std::vector<int> CDlgPrivilidge::GetApprovalLevelFromUser(CString strMutex,int nUserID)
{
	std::vector<int> vecRet;

	m_strModal = strMutex;

	LoadBindGroup();

	for(int i=0;i<m_arrUser.size();i++)
	{
		if(m_arrUser[i].bManager)
		{
			if( m_arrUser[i].userID == nUserID )
			{
				for(int j = 0; j< m_arrUser[i].arrRoles.size();j++ )
				{
					vecRet.push_back(m_arrUser[i].arrRoles[j]);
				}
			}
		}		
	}

	std::sort(vecRet.begin(),vecRet.end(),ASCOrder);

	return vecRet;
}
