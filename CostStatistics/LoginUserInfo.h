#ifndef _LOGIN_USER_INFO_H_
#define _LOGIN_USER_INFO_H_



class CLoginUserInfo
{
public:
	CLoginUserInfo();
	~CLoginUserInfo();
public:
	static int		m_nUserID ;
	static CString	m_strUserName;
	static CString	m_strAccount;
	static CString	m_strPwd;
	static bool		m_bIsAdmin;
};



#endif