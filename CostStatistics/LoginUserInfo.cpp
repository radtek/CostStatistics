#include "stdafx.h"
#include "LoginUserInfo.h"



int		CLoginUserInfo::m_nUserID = 0;
CString	CLoginUserInfo::m_strUserName = "";
CString	CLoginUserInfo::m_strAccount = "";
CString	CLoginUserInfo::m_strPwd = "";
bool		CLoginUserInfo::m_bIsAdmin = false;

CLoginUserInfo::CLoginUserInfo()
{

}

CLoginUserInfo::~CLoginUserInfo()
{

}
