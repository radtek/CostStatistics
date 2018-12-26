#ifndef _SECRET_HELPER_H_
#define _SECRET_HELPER_H_

#define SEC_SEPARATE  "!0?'\JAB" //�ָ���

class CSecretHelper
{
public:
	CSecretHelper();
	~CSecretHelper();
public:
	//��ס����
	void RememberPwd(std::string mUser, std::string mPwd,bool bLogin,bool bRemPwd);

	//��ȡ����
	std::string GetRememberedPwd();

	//��ȡ�Ƿ��Զ���¼,����������ȡ������
	bool IsAutoStart(std::string& mUser, std::string& mPwd);

	//�����Զ���¼
	bool SetAutoLogin(char* pVal);
protected:
	//��װ�����ļ�
	std::string AssmbleSecret(std::string mUser,std::string mPwd);

	//������װ�ļ�
	bool UnAssmbleSecret(std::string mSrc, std::string& mUser,std::string& mPwd);
private:
	std::string m_PwdPath;
};

#endif