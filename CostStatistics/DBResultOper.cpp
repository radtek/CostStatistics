#include "stdafx.h"
#include "DBResultOper.h"



CDBResultOper::CDBResultOper()
{

}

CDBResultOper::CDBResultOper(SQL_RESULT& mapData)
{
	m_mapRawData.clear();
	m_mapRawData = mapData;
}

CDBResultOper::~CDBResultOper()
{

}

void CDBResultOper::SetResultMap( SQL_RESULT& mapData )
{
	m_mapRawData.clear();
	m_mapRawData = mapData;
}

CString CDBResultOper::GetValue( CString strKey,int nRow )
{
 	return m_mapRawData[strKey].at(nRow);
}

int CDBResultOper::GetRowCount()
{
	if( m_mapRawData.size() == 0 )
	{
		return 0;
	}

	return  m_mapRawData.begin()->second.size();
}