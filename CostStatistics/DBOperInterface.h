#ifndef _DB_OPERATOR_INTERFACE_H_
#define _DB_OPERATOR_INTERFACE_H_



class CDbOperatorInterface
{
public:
	CDbOperatorInterface();
	~CDbOperatorInterface();
public:

	virtual void SetDbInfo(CString strIp,int nPort,CString strUser,CString strPwd ,CString strName) = 0;

	//打开数据库连接
	virtual bool Open() = 0;

	//关闭连接
	virtual bool Close() = 0;

	//增
	virtual bool Insert(CString strSql) = 0;

	//删
	virtual bool Delete(CString strSql)  = 0;

	//改
	virtual bool Update(CString strSql)  = 0;

	//查
	virtual bool Query(CString strSql,CString strKey,SQL_RESULT& result)  = 0;

	//批量
	virtual bool ExecuteBatch(CString strSql) = 0;
protected:
private:
};
#endif