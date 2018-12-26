#ifndef _DB_OPERATOR_H_
#define _DB_OPERATOR_H_

#include "DBResultOper.h"
#include "DBOperInterface.h"
#include <vector>

//���ݿ�����
typedef enum _enDbType
{
	MySQL	= 1,
	SQLite  = 2
}EN_DB_TYPE;



class CDbOperator
{
	/*����ģʽʵ��*/
public:
	static CDbOperator* GetInstance();
private:
	CDbOperator();
	~CDbOperator();

public:
	/*���ݿ����*/
	void SetDbInfo(EN_DB_TYPE enType = MySQL,CString strIp = "127.0.0.1",int nPort = 3306,CString strUser = "root",CString strPwd = "123456",CString strName = "icsysx");

	//�����ݿ�����
	bool Open();

	//�ر�����
	bool Close();

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

	//��ȡ����
	CString GetValue(CString strKey,int nRow);

private:
	static CDbOperator* m_pInstance;
	EN_DB_TYPE m_enDbType;
	CDbOperatorInterface* m_pOperInterface;
	std::vector<CDBResultOper*> m_vecResObj;
};



#endif