#ifndef _DB_RESULT_OPER_H_
#define _DB_RESULT_OPER_H_

#include <map>
#include <vector>


typedef std::map< CString,std::vector<CString> > SQL_RESULT;

class CDBResultOper
{
public:
	CDBResultOper();
	CDBResultOper(SQL_RESULT& mapData);
	~CDBResultOper();
public:
	void SetResultMap(SQL_RESULT& mapData);
	int GetRowCount();
	CString GetValue(CString strKey,int nRow);
protected:
private:
	SQL_RESULT m_mapRawData;
};
#endif