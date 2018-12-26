#ifndef _SECRET_HELPER_H_
#define _SECRET_HELPER_H_

#define SEC_SEPARATE  "!0?'\JAB" //分隔符

class CSecretHelper
{
public:
	CSecretHelper();
	~CSecretHelper();
public:
	//记住密码
	void RememberPwd(std::string mUser, std::string mPwd,bool bLogin,bool bRemPwd);

	//获取密码
	std::string GetRememberedPwd();

	//获取是否自动登录,出参数：读取的密码
	bool IsAutoStart(std::string& mUser, std::string& mPwd);

	//设置自动登录
	bool SetAutoLogin(char* pVal);
protected:
	//组装保存文件
	std::string AssmbleSecret(std::string mUser,std::string mPwd);

	//解析组装文件
	bool UnAssmbleSecret(std::string mSrc, std::string& mUser,std::string& mPwd);
private:
	std::string m_PwdPath;
};

#endif