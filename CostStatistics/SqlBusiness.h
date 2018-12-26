#ifndef _SQL_BUSINESS_H_
#define _SQL_BUSINESS_H_

class CSqlBusiness
{
public:
	CSqlBusiness();
	~CSqlBusiness();

	//��֤�û�������
	static int VerifyUser(CString strUser,CString strPwd,SQL_RESULT& rt);

	//�û��Ƿ����
	static bool IsAccountExist(CString strUser);
	//����˻�
	static bool AddAccount(CString strName, CString strUser,CString strPwd);
	//ɾ���˻�
	static bool DelAccount(CString strUser);
	//��������
	static bool ResetPwd(CString strUser,CString strPwd);
protected:
private:
};
#endif