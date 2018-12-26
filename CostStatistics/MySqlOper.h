#ifndef _MYSQL_OPERATOR_H_

#define _MYSQL_OPERATOR_H_

#include "DBOperInterface.h"
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <string>
// #include <wchar.h>
// #include <algorithm>
#include "mysql/include/mysql.h"

class CMySqlOper : public CDbOperatorInterface
{
public:
	CMySqlOper();
	~CMySqlOper();
public:

	//���ݿ���Ϣ
	void SetDbInfo(CString strIp = "127.0.0.1",int nPort = 3306,CString strUser = "root",CString strPwd = "123456",CString strName = "icsysx");

	//�����ݿ�����
	virtual bool Open();

	//�ر�����
	virtual bool Close();

	//��
	bool Insert(CString strSql);

	//ɾ
	bool Delete(CString strSql);

	//��
	bool Update(CString strSql);

	//��
	bool Query(CString strSql,CString strKey,SQL_RESULT& result);

	//����
	bool ExecuteBatch(CString strSql);
private:
	CString m_strIP;
	int m_nPort;
	CString m_strUser;
	CString m_strPwd;
	CString m_strName;

private:
	bool OperatorDBSqlRead(CString sql,CString strKey,SQL_RESULT& result);//���ݿ������ȡ
	bool OperatorDBSqlWrite(CString sql);//���ݿ����д��
};

#endif