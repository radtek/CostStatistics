#include "stdafx.h"
#include "SecretHelper.h"


CSecretHelper::CSecretHelper()
{
	m_PwdPath = CCommon::GetSecretPath();
}

CSecretHelper::~CSecretHelper()
{

}

void CSecretHelper::RememberPwd(std::string mUser, std::string mPwd,bool bLogin,bool bRemPwd)
{
	DWORD nSize = 0;
	::DeleteFile(m_PwdPath.c_str());

	HANDLE hFile = CreateFile(m_PwdPath.c_str(), GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, 0);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		::MessageBox(NULL,"打开存储安全认证密码文件失败!","提示：",MB_OK|MB_ICONWARNING);
		return;
	}

	if( bLogin || bRemPwd )
	{
		if(bLogin)
		{
			char	tmpbuf[1] = {'1'};
			DWORD nSize = 0;
			SetFilePointer(hFile, 0, NULL ,FILE_BEGIN);
			WriteFile(hFile, tmpbuf, 1, &nSize, NULL);	
		} 
		else
		{
			char	tmpbuf[1] = {'0'};
			DWORD nSize = 0;
			SetFilePointer(hFile, 0, NULL ,FILE_BEGIN);
			WriteFile(hFile, tmpbuf, 1, &nSize, NULL);	
		}

		if (bRemPwd)
		{
			std::string mData = AssmbleSecret(mUser,mPwd);

			char szBuf[MAX_PATH] = {0};
			lstrcpy(szBuf,mData.c_str());

			int nDataLen = mData.length();
			//加密
			for(int i=0; i<nDataLen; i++)
			{
				szBuf[i] = ~szBuf[i];
			}

			SetFilePointer(hFile, 1, NULL, FILE_BEGIN);
			WriteFile(hFile, szBuf, nDataLen, &nSize, NULL);
		}
		CloseHandle(hFile);
	} 
	else 
	{
		CloseHandle(hFile);
		if(CCommon::FileIsExist(m_PwdPath.c_str()))
		{
			SetFileAttributes(m_PwdPath.c_str(),FILE_ATTRIBUTE_NORMAL); 
			DeleteFile(m_PwdPath.c_str());
		}
	}
}

std::string CSecretHelper::GetRememberedPwd()
{
	std::string mRet = "";
	char password[MAX_PATH] = {0};

	if( !CCommon::FileIsExist(m_PwdPath.c_str()) )            //没有密码文件  
	{
		return mRet;
	}	
	else 
	{
		HANDLE hFile = CreateFile(m_PwdPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if( INVALID_HANDLE_VALUE == hFile )
		{
			return mRet;
		}
		else
		{
			char	readbuf[32] = {0};
			DWORD	nReadSize = 0;
			if( !ReadFile(hFile, readbuf, 32,  &nReadSize, NULL) )
			{
				CloseHandle(hFile);
				return mRet;
			}
			else
			{
				CloseHandle(hFile);
				//解密
				int i = 1;
				for(; i < strlen(readbuf+1)+1; i++)
				{
					readbuf[i] = ~readbuf[i];
				}

				strcpy((char*)password, readbuf + 1);

				mRet = password;
			}
		}
	}

	return mRet;
}

bool CSecretHelper::IsAutoStart(std::string& mUser,std::string& mPwd)
{
	//~safedata.dat
	if( !CCommon::FileIsExist( m_PwdPath.c_str()) )            //没有密码文件  
	{
		return false;
	}
	else 
	{
		HANDLE hFile = CreateFile(m_PwdPath.c_str(), GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
		if(INVALID_HANDLE_VALUE == hFile)
		{
			return false;
		}
		else
		{
			char	readbuf[MAX_PATH] = {0};
			DWORD	nReadSize = 0;
			if( !ReadFile(hFile, readbuf, MAX_PATH,  &nReadSize, NULL) )
			{
				CloseHandle(hFile);
				return false;
			}
			else
			{
				CloseHandle(hFile);

				//解密
				int i = 1;
				for(; i<strlen(readbuf+1)+1; i++)
				{
					readbuf[i] = ~readbuf[i];
				}

				//解析字符串
				UnAssmbleSecret(readbuf,mUser,mPwd);

				//读取自动启动符号
				//如果查询自动启动为true，那么再查询密码，之后进行safepin验证
				if('1' == readbuf[0])
				{
					return true;
				}
				//非自动登录
				else
				{
					return false;
				}
			}
		}
	}

	return false;
}

bool CSecretHelper::SetAutoLogin( char* pVal )
{
	if( pVal == NULL )
	{
		return false;
	}

	bool bFlag  = false;
	if( CCommon::FileIsExist(m_PwdPath.c_str()))//有密码文件  不修改密码字符串
	{
		bFlag  = true;
	}

	HANDLE hFile  = CreateFile(m_PwdPath.c_str(), GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_HIDDEN, 0);
	if( INVALID_HANDLE_VALUE  == hFile )
	{
		return false;
	}
	else
	{
		DWORD nSize  = 0;
		SetFilePointer(hFile, 0, NULL ,FILE_BEGIN);
		WriteFile(hFile, pVal, 1, &nSize, NULL);	

		//加密
		if( !bFlag )
		{  
			//有密码文件  不修改密码字符串
			TCHAR tmpbuf[MAX_PATH] ={0};
			memset(tmpbuf, 0x00, 16);
			
			CString strUser = ""/*g_DataBuf.GetCurUser()*/;
			CString strPwd = ""/*g_DataBuf.GetPwd()*/;

			memcpy(tmpbuf,strPwd,strPwd.GetLength());
			for(int i =0; i<strlen(tmpbuf); i++)
			{
				tmpbuf[i]  = ~tmpbuf[i];
			}

			SetFilePointer(hFile, 1, NULL, FILE_BEGIN);
			WriteFile(hFile, tmpbuf, sizeof(tmpbuf), &nSize, NULL);
		}

		CloseHandle(hFile);
	}
}

std::string CSecretHelper::AssmbleSecret(std::string mUser,std::string mPwd)
{
	std::string mData = mUser;
	mData += SEC_SEPARATE;
	mData += mPwd;

	return mData;
}

bool CSecretHelper::UnAssmbleSecret(std::string mSrc, std::string& mUser,std::string& mPwd)
{
	mUser = "";
	mPwd = "";

	std::string mData = mSrc;
	std::string::size_type nPos = mData.find(SEC_SEPARATE);
	if( nPos != std::string::npos )
	{
		mUser = mData.substr(1,nPos - 1);
		mPwd = mData.substr(nPos + strlen(SEC_SEPARATE));

		return true;
	}

	return false;
}

