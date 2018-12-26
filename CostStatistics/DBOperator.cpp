#include "stdafx.h"
#include "DBOperator.h"
#include "MySqlOper.h"

CDbOperator* CDbOperator::m_pInstance = NULL;

CDbOperator::CDbOperator()
{
	m_pOperInterface = NULL;
}

CDbOperator::~CDbOperator()
{
	if( m_pInstance != NULL )
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}

	if( m_pOperInterface != NULL )
	{
		delete m_pOperInterface;
		m_pOperInterface = NULL;
	}
	
}

CDbOperator* CDbOperator::GetInstance()
{
	if( m_pInstance == NULL )
	{
		m_pInstance = new CDbOperator;
	}

	return m_pInstance;
}

void CDbOperator::SetDbInfo(EN_DB_TYPE enType /*= MySQL*/,CString strIp /*= "127.0.0.1"*/,int nPort /*= 3306*/,CString strUser /*= "root"*/,CString strPwd /*= "123456"*/,CString strName)
{
	m_enDbType = enType;

	if( m_enDbType == MySQL )
	{
		m_pOperInterface = new CMySqlOper;
		m_pOperInterface->SetDbInfo(strIp,nPort,strUser,strPwd,strName);
	}
	else
	{
		
	}
}


bool CDbOperator::Open()
{
	if( m_pOperInterface == NULL )
	{
		return false;
	}
	return m_pOperInterface->Open();
}

bool CDbOperator::Close()
{
	if( m_pOperInterface == NULL )
	{
		return false;
	}
	return m_pOperInterface->Close();
}


bool CDbOperator::Insert(CString strSql)
{
	if( m_pOperInterface == NULL )
	{
		return false;
	}
	return m_pOperInterface->Insert(strSql);
}

bool CDbOperator::Delete(CString strSql)
{
	if( m_pOperInterface == NULL )
	{
		return false;
	}
	return m_pOperInterface->Delete(strSql);
}

bool CDbOperator::Update(CString strSql)
{
	if( m_pOperInterface == NULL )
	{
		return false;
	}
	return m_pOperInterface->Update(strSql);
}

bool CDbOperator::Query(CString strSql,CString strKey,SQL_RESULT& result)
{
	if( m_pOperInterface == NULL )
	{
		return false;
	}

	 return m_pOperInterface->Query(strSql,strKey,result);
}

CString CDbOperator::GetValue(CString strKey,int nRow)
{
	//return m_ResObj.GetValue(strKey,nRow);
	return "";
}

bool CDbOperator::ExecuteBatch(CString strSql)
{
	if( m_pOperInterface == NULL )
	{
		return false;
	}

	return m_pOperInterface->ExecuteBatch(strSql);
}
