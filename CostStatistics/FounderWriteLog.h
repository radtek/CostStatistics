#ifndef _FOUNDER_WRITE_LOG
#define _FOUNDER_WRITE_LOG

//////////////////////////////////////////////////////////////////////////
/*	Function����־��ӡ��												*/
/*	Author��wushunli													*/
/*	Date��2013.7														*/
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
		void Init(bool bWrite, int nSize); //bWrite: �Ƿ�д��־��nSize:��־��С��λ��kb
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


//��־����
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

//��־����
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