#ifndef _FOUNDER_WRITE_LOG
#define _FOUNDER_WRITE_LOG

//////////////////////////////////////////////////////////////////////////
/*	Function：日志打印类												*/
/*	Author：wushunli													*/
/*	Date：2013.7														*/
//////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <string>
#include <sstream>
#include "Templete.h"

namespace FounderWriteLog
{
	class CWriteLog
	{
	public:
		CWriteLog();
		~CWriteLog();
	public:
		void Init(bool bWrite, int nSize); //bWrite: 是否写日志，nSize:日志大小单位是kb
		void WriteLog(const char* pLogData);
		static UINT32 WINAPI EzWriteLogToLocalFileThread(void* pArguments);
		void StopLogService();
	private:
		bool InitWriteLog();
		void DoWrite();
		std::string GetCTTime();
		std::string GetCurrentPath();
		std::string GetDataFromVec();
	public:
		bool m_bIsWriteLogRun;
	private:
		std::string m_LogFullPath ;
		HANDLE m_hWriteLogToLocalFile;
		bool m_bWrite;
		int  m_nLogMaxSize;
	};
}


//日志函数
#define SET_LOGINIT(BWRITE,LOGSIZE)\
{\
	BaseLib::TSingleton<FounderWriteLog::CWriteLog>::Instance()->Init(BWRITE,LOGSIZE);\
}

#define PRINT_LOG(INFO) \
{\
	std::ostringstream StrInfo;\
	StrInfo << INFO;\
	BaseLib::TSingleton<FounderWriteLog::CWriteLog>::Instance()->WriteLog((char*)StrInfo.str().c_str());\
}

//日志函数
#include <sstream>
#define SET_LOGINIT(BWRITE,LOGSIZE)\
{\
	BaseLib::TSingleton<FounderWriteLog::CWriteLog>::Instance()->Init(BWRITE,LOGSIZE);\
}

#define PRINT_LOG(INFO) \
{\
	std::ostringstream StrInfo;\
	StrInfo << INFO;\
	BaseLib::TSingleton<FounderWriteLog::CWriteLog>::Instance()->WriteLog((char*)StrInfo.str().c_str());\
}


#endif