#ifndef _DB_OPERATOR_H_
#define _DB_OPERATOR_H_

#include "DBResultOper.h"
#include "DBOperInterface.h"
#include <vector>

//数据库类型
typedef enum _enDbType
{
	MySQL	= 1,
	SQLite  = 2
}EN_DB_TYPE;



class CDbOperator
{
	/*单例模式实现*/
public:
	static CDbOperator* GetInstance();
private:
	CDbOperator();
	~CDbOperator();

public:
	/*数据库操作*/
	void SetDbInfo(EN_DB_TYPE enType = MySQL,CString strIp = "127.0.0.1",int nPort = 3306,CString strUser = "root",CString strPwd = "123456",CString strName = "icsysx");

	//打开数据库连接
	bool Open();

	//关闭连接
	bool Close();

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

	//获取数据
	CString GetValue(CString strKey,int nRow);

private:
	static CDbOperator* m_pInstance;
	EN_DB_TYPE m_enDbType;
	CDbOperatorInterface* m_pOperInterface;
	std::vector<CDBResultOper*> m_vecResObj;
};



#endif