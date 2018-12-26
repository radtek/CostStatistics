#include "stdafx.h"
#include "SqlBusiness.h"
#include "DBOperator.h"



CSqlBusiness::CSqlBusiness()
{

}

CSqlBusiness::~CSqlBusiness()
{

}

//验证用户名密码
int CSqlBusiness::VerifyUser(CString strUser,CString strPwd,SQL_RESULT& rt)
{
	CString strSql = "";
	strSql.Format("SELECT ItemID,Name,Login,Pass,IsAdmin FROM z_user WHERE Login = \'%s\' AND SCZT = 0",strUser);

	if( CDbOperator::GetInstance()->Query(strSql,"ItemID,Name,Account,Pwd,IsAdmind",rt) )
	{
		CDBResultOper obj;
		obj.SetResultMap(rt);
		int nRow = obj.GetRowCount();

		if( nRow == 0 )
		{
			return 3;
		}
		
		CString strPwdDb = obj.GetValue("Pwd",0);

		strPwdDb.MakeLower();
		strPwd.MakeLower();

		if( strPwdDb.Compare(strPwd) != 0 )
		{
			return 2;
		}
	}

	return 0;
}


bool CSqlBusiness::IsAccountExist(CString strUser)
{
	CString strSql = "";
	strSql.Format("SELECT Login,Pass FROM z_user WHERE Login = \'%s\' AND SCZT = 0",strUser);

	SQL_RESULT rt;
	if( CDbOperator::GetInstance()->Query(strSql,"User,Pwd",rt) )
	{
		CDBResultOper obj;
		obj.SetResultMap(rt);
		int nRow = obj.GetRowCount();

		if( nRow == 0 )
		{
			return false;
		}

		return true;
	}

	return false;
}


bool CSqlBusiness::AddAccount(CString strName, CString strUser,CString strPwd)
{
	CString strSql = "";
	strSql.Format("INSERT INTO z_user (`Name`, `Login`, `Pass`, `SCZT`, `CreateTime`, `LastTime`) VALUES (\'%s\',\'%s\',\'%s\',0,NOW(),NOW());",strName,strUser,strPwd);

	return CDbOperator::GetInstance()->Insert(strSql);
}

bool CSqlBusiness::DelAccount(CString strUser)
{
	CString strSql = "";
	strSql.Format("UPDATE z_user SET SCZT = 1 WHERE Login = \'%s\';",strUser);
	return CDbOperator::GetInstance()->Update(strSql);
}

bool CSqlBusiness::ResetPwd(CString strUser,CString strPwd)
{
	CString strSql = "";
	strSql.Format("UPDATE z_user SET Pass = \'%s\' WHERE Login = \'%s\' AND SCZT = 0;",strPwd,strUser);
	return CDbOperator::GetInstance()->Update(strSql);
}
