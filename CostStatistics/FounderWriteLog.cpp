#include "stdafx.h"
#include "FounderWriteLog.h"
#include <io.h>
#include <time.h>
#include <stdio.h>
#include <process.h>  //_begintheadex用到的
#include <vector>

static CRITICAL_SECTION		g_cs_writelog;
static CRITICAL_SECTION		g_cs_logcache;
std::vector<std::string>	g_vecLocCache;


FounderWriteLog::CWriteLog::CWriteLog()
{
	::InitializeCriticalSection(&g_cs_writelog);
	::InitializeCriticalSection(&g_cs_logcache);
	
	m_bWrite = true;
	m_nLogMaxSize = 100;
	g_vecLocCache.clear();
	InitWriteLog();
}

FounderWriteLog::CWriteLog::~CWriteLog()
{
	::DeleteCriticalSection(&g_cs_writelog);
	::DeleteCriticalSection(&g_cs_logcache);

	m_LogFullPath = "";	
	m_bIsWriteLogRun = false;
	g_vecLocCache.clear();

}


void FounderWriteLog::CWriteLog::Init( bool bWrite, int nSize )
{
	m_bWrite = bWrite;

	if(nSize <= 0)
	{
		m_bWrite = false;
		m_nLogMaxSize = 100;// 100KB
		return;
	}

	m_nLogMaxSize = nSize;
}

void FounderWriteLog::CWriteLog::WriteLog( const char* pLogData )
{
	if(m_bWrite == false)
	{
		return;
	}

	std::string mTemp = std::string(pLogData);
	if( (mTemp == "") || (m_bIsWriteLogRun == false) )
	{
		return;
	}

	OutputDebugString(pLogData);
	::EnterCriticalSection(&g_cs_logcache);
	g_vecLocCache.push_back(mTemp);
	::LeaveCriticalSection(&g_cs_logcache);
}

UINT32 WINAPI FounderWriteLog::CWriteLog::EzWriteLogToLocalFileThread( void* pArguments )
{
	FounderWriteLog::CWriteLog* pWriteLog = (FounderWriteLog::CWriteLog*)pArguments;

	while(pWriteLog->m_bIsWriteLogRun == true)
	{
		pWriteLog->DoWrite();
	}

	return 0;
}


void FounderWriteLog::CWriteLog::DoWrite()
{
	std::string mLogData = GetDataFromVec();
	if(mLogData == "")
	{
		Sleep(1);
		return;
	}

	if(m_LogFullPath.size() <= 0)
	{
		Sleep(1);
		return;
	}

	EnterCriticalSection(&g_cs_writelog);
	FILE* pFileLog = fopen(m_LogFullPath.c_str(),"a+");
	if(pFileLog == NULL)
	{
		LeaveCriticalSection(&g_cs_writelog);
		Sleep(1);
		return;
	}

	fseek(pFileLog,0,SEEK_END);
	int nFileLen = ftell(pFileLog);

	//如果大于100KB，删除文件，重新建立
	if(nFileLen > m_nLogMaxSize*1024)
	{
		fclose(pFileLog);
		remove(m_LogFullPath.c_str());
		LeaveCriticalSection(&g_cs_writelog);
		Sleep(1);
		return;
	}

	char szLogData[1024] = {0};
	std::string mTime = GetCTTime();
	if(mLogData.length() > 1024)
	{
		mLogData = m_LogFullPath.substr(0,1022);
	}
	sprintf_s(szLogData,1024,"%s %s\n",mTime.c_str(),mLogData.c_str());

	fwrite(szLogData,strlen(szLogData),1,pFileLog);
	fclose(pFileLog);
	LeaveCriticalSection(&g_cs_writelog);
}

bool FounderWriteLog::CWriteLog::InitWriteLog()
{
	//获取当前目录
	m_LogFullPath = GetCurrentPath();

	//启动写日志线程
	m_bIsWriteLogRun = true;
	m_hWriteLogToLocalFile = (HANDLE)_beginthreadex( NULL, 0, &EzWriteLogToLocalFileThread, this, 0, NULL);

	if(m_hWriteLogToLocalFile != NULL)
	{
		return true;
	}

	return false;
}

std::string FounderWriteLog::CWriteLog::GetCurrentPath()
{
	//获取当前路径
	char szIniPath[_MAX_PATH] = {0};
	::GetModuleFileName ( NULL, szIniPath, sizeof (szIniPath) );
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	char szFileName[_MAX_EXT];
	::_splitpath (szIniPath, szDrive, szDir, szFileName, NULL);
	//::_makepath (szIniPath, szDrive, szDir, szFileName, "log");
	::_makepath (szIniPath, szDrive, szDir, "log", "log");

	return std::string(szIniPath);
}

std::string FounderWriteLog::CWriteLog::GetCTTime()
{
	char __szBuf__[64];
	struct tm __tm__; 
	time_t __t__ = time (NULL);
	tm *t = localtime (&__t__);
	__tm__ = *t;

	sprintf (__szBuf__, 
		"%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.3d",
		__tm__.tm_year + 1900, __tm__.tm_mon + 1, __tm__.tm_mday,
		__tm__.tm_hour, __tm__.tm_min, __tm__.tm_sec, GetTickCount() % 1000); 

	return __szBuf__;
}

std::string FounderWriteLog::CWriteLog::GetDataFromVec()
{
	std::string mRet = "";

	::EnterCriticalSection(&g_cs_logcache);
	if(g_vecLocCache.size() <= 0)
	{
		::LeaveCriticalSection(&g_cs_logcache);
		return mRet;
	}

  	mRet = *(g_vecLocCache.begin());
 	g_vecLocCache.erase(g_vecLocCache.begin());

	::LeaveCriticalSection(&g_cs_logcache);

	return mRet;
}

void FounderWriteLog::CWriteLog::StopLogService()
{
	m_bIsWriteLogRun = false;
}


