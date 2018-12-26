#ifndef _SQL_BUSINESS_H_
#define _SQL_BUSINESS_H_

class CSqlBusiness
{
public:
	CSqlBusiness();
	~CSqlBusiness();

	//验证用户名密码
	static int VerifyUser(CString strUser,CString strPwd,SQL_RESULT& rt);

	//用户是否存在
	static bool IsAccountExist(CString strUser);
	//添加账户
	static bool AddAccount(CString strName, CString strUser,CString strPwd);
	//删除账户
	static bool DelAccount(CString strUser);
	//重置密码
	static bool ResetPwd(CString strUser,CString strPwd);
protected:
private:
};
#endif