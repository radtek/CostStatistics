#ifndef _DB_OPERATOR_INTERFACE_H_
#define _DB_OPERATOR_INTERFACE_H_



class CDbOperatorInterface
{
public:
	CDbOperatorInterface();
	~CDbOperatorInterface();
public:

	virtual void SetDbInfo(CString strIp,int nPort,CString strUser,CString strPwd ,CString strName) = 0;

	//�����ݿ�����
	virtual bool Open() = 0;

	//�ر�����
	virtual bool Close() = 0;

	//��
	virtual bool Insert(CString strSql) = 0;

	//ɾ
	virtual bool Delete(CString strSql)  = 0;

	//��
	virtual bool Update(CString strSql)  = 0;

	//��
	virtual bool Query(CString strSql,CString strKey,SQL_RESULT& result)  = 0;

	//����
	virtual bool ExecuteBatch(CString strSql) = 0;
protected:
private:
};
#endif