#include "stdafx.h"
#include "MySqlOper.h"


CMySqlOper::CMySqlOper()
{
	m_strIP = "";
	m_nPort = 0;
	m_strUser = "";
	m_strPwd = "";
	m_strName = "";
}

CMySqlOper::~CMySqlOper()
{

}


void CMySqlOper::SetDbInfo( CString strIp /*= "127.0.0.1"*/,int nPort /*= 3306*/,CString strUser /*= "root"*/,CString strPwd /*= "123456"*/,CString strName/*= "数据库名"*/ )
{
	m_strIP = strIp;
	m_nPort = nPort;
	m_strUser = strUser;
	m_strPwd = strPwd;
	m_strName = strName;
}

bool CMySqlOper::Open()
{
	return true;
}

bool CMySqlOper::Close()
{
	return true;
}


bool CMySqlOper::Insert(CString strSql)
{
	if (OperatorDBSqlWrite(strSql) == false)
	{
		return false;
	}
	return true;
}

bool CMySqlOper::Delete(CString strSql)
{
	if (OperatorDBSqlWrite(strSql) == false)
	{
		return false;
	}
	return true;
}

bool CMySqlOper::Update(CString strSql)
{
	if (OperatorDBSqlWrite(strSql) == false)
	{
		return false;
	}
	return true;
}

bool CMySqlOper::Query(CString strSql,CString strKey,SQL_RESULT& result)
{
	if (OperatorDBSqlRead(strSql,strKey,result) == false)
	{
		return false;
	}
	return true;
}

bool CMySqlOper::OperatorDBSqlRead(CString sql,CString strKey,SQL_RESULT& result)
{
	MYSQL_RES * pStResult;
	MYSQL_ROW ppRawRet;
	MYSQL_FIELD * pStField;
	unsigned int unColCount;
	unsigned int nCol;

	std::vector<CString> vecTmp;
	std::vector<CString> vecHead;

	const char *pStatement = sql;
	mysql_library_init(0, NULL, NULL);
	MYSQL *mysql = mysql_init(NULL);
	unsigned int timeout = 3000;
	mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

 	if (mysql_real_connect(mysql, m_strIP, m_strUser, m_strPwd, m_strName, m_nPort, NULL, 0) == NULL)
 	{
		mysql_close(mysql);
		mysql_library_end();
		::MessageBox(NULL,"连接数据库失败，请检查网络是否正常","提示：",MB_OK|MB_ICONWARNING);
		PRINT_LOG("连接数据库失败，请检查网络是否正常");
		return false;
	}
 	int nRet = mysql_query(mysql,"set names gbk");//防止乱码。设置和数据库的编码一致就不会乱码
 
 	if (0 != mysql_real_query(mysql, pStatement, strlen(pStatement)))
 	{
 		mysql_close(mysql);
 		mysql_library_end();
		::MessageBox(NULL,"连接数据库失败，请检查网络是否正常","提示：",MB_OK|MB_ICONWARNING);
		PRINT_LOG("查询失败");
 		return false;
 	}
 	pStResult = mysql_store_result(mysql);
 	if (pStResult == NULL)
 	{
 		mysql_close(mysql);
 		mysql_library_end();
 		return false;
 	}
 	unColCount = mysql_num_fields(pStResult);//总列数

 
	std::vector<CString> vecHeadReal;
 	while (NULL != (pStField = mysql_fetch_field(pStResult)))
 	{
 		CString ab(pStField->name);
 
 		//result.insert(make_pair(pStField->name,vecTmp));
 		vecHeadReal.push_back(pStField->name);
 	}

	vecHead.clear();

	//分割key
	char* pTemp = strtok(strKey.GetBuffer(0),",");
	while( pTemp )
	{
		vecHead.push_back(pTemp);
		result.insert(make_pair(pTemp,vecTmp));
		pTemp = strtok(NULL,",");
	}
	strKey.ReleaseBuffer();
	if( vecHead.size() > vecHeadReal.size() )
	{
		return false;
	}

	
 	CString strTemp;
 	while (NULL != (ppRawRet = mysql_fetch_row(pStResult)))
 	{
 		unsigned long *lengths;
 		lengths = mysql_fetch_lengths(pStResult);
 		//for (nCol = 0; nCol < unColCount; nCol++)
		for (nCol = 0; nCol < vecHead.size(); nCol++)
 		{
 			result[vecHead[nCol]].push_back(ppRawRet[nCol] ? ppRawRet[nCol] : "NULL");
 		}
 	} 
 	mysql_free_result(pStResult);
 	mysql_close(mysql);
 	mysql_library_end();
	return true;
}

bool CMySqlOper::OperatorDBSqlWrite( CString sql )
{
	MYSQL_RES * pStResult;
	MYSQL_ROW ppRawRet;
	MYSQL_FIELD * pStField;
	unsigned int unColCount;
	unsigned int nCol;

	std::map< CString,std::vector<CString> > mapRet;
	std::vector<CString> vecTmp;
	std::vector<CString> vecHead;
	mapRet.clear();

	const char *pStatement = sql;
	mysql_library_init(0, NULL, NULL);
	MYSQL *mysql = mysql_init(NULL);
	unsigned int timeout = 3000;
	mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

	if (mysql_real_connect(mysql, m_strIP, m_strUser, m_strPwd, m_strName, m_nPort, NULL, 0) == NULL)
	{
		mysql_close(mysql);
		mysql_library_end();
		PRINT_LOG("连接服务器失败");
		return false;
	}
	int nRet = mysql_query(mysql,"set names gbk");//防止乱码。设置和数据库的编码一致就不会乱码

	if (0 != mysql_real_query(mysql, pStatement, strlen(pStatement)))
	{
		mysql_close(mysql);
		mysql_library_end();
		PRINT_LOG("查询失败");
		return false;
	}
	return true;
}

bool CMySqlOper::ExecuteBatch(CString strSql)
{
	MYSQL_RES * pStResult;
	MYSQL_ROW ppRawRet;
	MYSQL_FIELD * pStField;
	unsigned int unColCount;
	unsigned int nCol;

	const char *pStatement = strSql;
	mysql_library_init(0, NULL, NULL);
	MYSQL *mysql = mysql_init(NULL);
	unsigned int timeout = 3000;
	mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

	if (mysql_real_connect(mysql, m_strIP, m_strUser, m_strPwd, m_strName, m_nPort, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
	{
		int n = mysql_errno(mysql);
		string strError = mysql_error(mysql);
		PRINT_LOG("error id: " << mysql_errno(mysql) << strError.c_str());
		return false;
	}
	int nRet = mysql_query(mysql,"set names gbk");//防止乱码。设置和数据库的编码一致就不会乱码

	mysql_set_server_option(mysql,MYSQL_OPTION_MULTI_STATEMENTS_ON);
	nRet = mysql_real_query(mysql, pStatement, strlen(pStatement));
	mysql_set_server_option(mysql,MYSQL_OPTION_MULTI_STATEMENTS_ON);

	if (0 != nRet )
	{
		mysql_close(mysql);
		mysql_library_end();
		PRINT_LOG("查询失败");
		return false;
	}

	return true;
}
