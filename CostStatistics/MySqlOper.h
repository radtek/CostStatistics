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

	//数据库信息
	void SetDbInfo(CString strIp = "127.0.0.1",int nPort = 3306,CString strUser = "root",CString strPwd = "123456",CString strName = "icsysx");

	//打开数据库连接
	virtual bool Open();

	//关闭连接
	virtual bool Close();

	//增
	bool Insert(CString strSql);

	//删
	bool Delete(CString strSql);

	//改
	bool Update(CString strSql);

	//查
	bool Query(CString strSql,CString strKey,SQL_RESULT& result);

	//批量
	bool ExecuteBatch(CString strSql);
private:
	CString m_strIP;
	int m_nPort;
	CString m_strUser;
	CString m_strPwd;
	CString m_strName;

private:
	bool OperatorDBSqlRead(CString sql,CString strKey,SQL_RESULT& result);//数据库操作读取
	bool OperatorDBSqlWrite(CString sql);//数据库操作写入
};

#endif