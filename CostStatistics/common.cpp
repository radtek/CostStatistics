#include "stdafx.h"
#include "common.h" 

#include <Windows.h>
#pragma comment(lib,"Version.lib")

CCommon::CCommon()
{
	
}

CCommon::~CCommon()
{

}



int CCommon::FindItemFromArray(CStringArray& strArray, CString& strElement)
{
	int nCount = strArray.GetSize();

	for (int i = 0; i < nCount; ++i)
	{
		if ( strElement == strArray.GetAt(i) )
		{
			return i;
		}
	}

	return -1;
}

void CCommon::DeleteLastChar(CString& str)
{
	int nLen = str.GetLength();
	if( nLen <= 1 )
	{
		return;
	}

	str.Delete(nLen - 1,1);
}

CString CCommon::GetCurPath()
{
	char szCurPath[MAX_PATH] = {0};  
	GetModuleFileName(NULL,(LPSTR)szCurPath,sizeof(szCurPath));

	CString strPath = szCurPath;

	strPath = strPath.Left(strPath.ReverseFind('\\') + 1);
	return strPath;
}


CString CCommon::FormatTime(CString strDateTime)
{
	//2016 06 15 00 00 00
	int nLen = strDateTime.GetLength();
	if( nLen != 14 )
	{
		return strDateTime;
	}

	std::string strTmp = strDateTime;
	std::string strYear = strTmp.substr(0,4);
	std::string strMonth = strTmp.substr(4,2);
	std::string strDay = strTmp.substr(6,2);
	std::string strHour = strTmp.substr(8,2);
	std::string strMin	= strTmp.substr(10,2);
	std::string strSecond = strTmp.substr(12,2);

	std::ostringstream o;
	o << strYear << "-" << strMonth << "-" << strDay << " " << strHour << ":" << strMin << ":" << strSecond;

	return o.str().c_str();
}

CString CCommon::FormatDate(CString strDateTime)
{
	//2016-03-02 00:00:00
	int nLen = strDateTime.GetLength();
	if( nLen != 19 )
	{
		return strDateTime;
	}

	std::string strRet = strDateTime;
	strRet = strRet.substr(0,10);

	return strRet.c_str();
}

__int64 CCommon::GetMyTicketCount()
{
	__int64 nRet = 0;

	/*if( g_WindowVer == 2 )
	{
		nRet = ::GetTickCount();
	}
	else if ( g_WindowVer == 5 )
	{
		nRet = ::GetTickCount64();
	}*/

	nRet = ::GetTickCount();

	return nRet;
}

//是否是日期20160606或者2016/06/06格式的字符串
bool CCommon::IsDate(CString strDate)
{
	if( strDate.IsEmpty() )
	{
		return false;
	}

	if( IncludeChinese(strDate.GetBuffer(0)) == 1 )
	{
		return false;
	}

	for(int i = 0;i<strDate.GetLength();i++)
	{
		char ch = strDate.GetAt(i);

		if( ch == '/' || ch == '-')
		{
			continue;
		}

		if( isdigit(ch) == 0 )
		{
			return false;
		}
	}

	return true;
}

int CCommon::IncludeChinese(char *str)//返回0：无中文，返回1：有中文
{
	char c;
	while(1)
	{
		c = *str++;
		if (c==0) break;  //如果到字符串尾则说明该字符串没有中文字符
		if (c&0x80)        //如果字符高位为1且下一字符高位也是1则有中文字符
		{
			if (*str & 0x80) return 1;
		}
	}

	return 0;
}

CString CCommon::GetIntFromString(CString str)
{
	char szTmp[11] = {0};
	int cnt_int = 0;

	for(int i = 0;i<str.GetLength();i++)
	{
		char ch = str.GetAt(i);

		if(ch>='0'&& ch<='9') //如果是数字.
		{
			cnt_int *= 10;
			cnt_int += ch -'0'; //数字字符的ascii-字符'0'的ascii码就等于该数字.
		}
		/*else
		{
			break;
		}*/
	}

	cnt_int = cnt_int == 0 ? 1 : cnt_int;
	sprintf(szTmp,"%d",cnt_int);

	return szTmp;
}

bool CCommon::IsFloat(CString strData)
{
	if( strData.Find(".") != -1 )
	{
		return true;
	}

	return false;
}

CString CCommon::GetSQPUser(CString strUser)
{
	strUser.Append("_SQP_user");
	return strUser;
}

CString CCommon::GetRidoffUserTail(CString strUser)
{
	std::string::size_type nPos = 0;
	std::string mTemp = strUser.GetBuffer(0);

	nPos = mTemp.find("_SQP_user");
	if( nPos != std::string::npos )
	{
		mTemp = mTemp.substr(0,nPos);
	}

	return mTemp.c_str();
}

CString CCommon::GetFileTimeString()
{
	CTime time = CTime::GetCurrentTime(); ///构造CTime对象

	int nYear = time.GetYear(); ///年
	int nMonth = time.GetMonth(); ///月
	int nDay = time.GetDay(); ///日
	int nHour = time.GetHour(); ///小时
	int nMinute = time.GetMinute(); ///分钟
	int nSecond = time.GetSecond(); ///秒

	char szTemp[32] = {0};
	sprintf(szTemp,_T("%04d%02d%02d%02d%02d%02d%"),nYear,nMonth,nDay,nHour,nMinute,nSecond);

	return szTemp;
}

bool CCommon::FileIsExist(LPCTSTR szFileFullPathName)
{
	WIN32_FIND_DATA Win32_Find_Data;   
	HANDLE hFindFile;   

	hFindFile = FindFirstFile(szFileFullPathName,&Win32_Find_Data);   

	if(INVALID_HANDLE_VALUE == hFindFile)   
	{   
		return false;   
	}   
	else   
	{   
		FindClose(hFindFile);   
		return true;   
	}   

	return false;
}

CString CCommon::GetSecretPath()
{
	CString strSecretPath = CCommon::GetCurPath();
	strSecretPath.Append("~secret.dat");

	return strSecretPath;
}

std::string CCommon::ConvertChineseToPinyin(std::string strSrc)
{
	static const  int pyvalue[] = {-20319,-20317,-20304,-20295,-20292,-20283,-20265,-20257,-20242,-20230,-20051,-20036,-20032,-20026,  
		-20002,-19990,-19986,-19982,-19976,-19805,-19784,-19775,-19774,-19763,-19756,-19751,-19746,-19741,-19739,-19728,  
		-19725,-19715,-19540,-19531,-19525,-19515,-19500,-19484,-19479,-19467,-19289,-19288,-19281,-19275,-19270,-19263,  
		-19261,-19249,-19243,-19242,-19238,-19235,-19227,-19224,-19218,-19212,-19038,-19023,-19018,-19006,-19003,-18996,  
		-18977,-18961,-18952,-18783,-18774,-18773,-18763,-18756,-18741,-18735,-18731,-18722,-18710,-18697,-18696,-18526,  
		-18518,-18501,-18490,-18478,-18463,-18448,-18447,-18446,-18239,-18237,-18231,-18220,-18211,-18201,-18184,-18183,  
		-18181,-18012,-17997,-17988,-17970,-17964,-17961,-17950,-17947,-17931,-17928,-17922,-17759,-17752,-17733,-17730,  
		-17721,-17703,-17701,-17697,-17692,-17683,-17676,-17496,-17487,-17482,-17468,-17454,-17433,-17427,-17417,-17202,  
		-17185,-16983,-16970,-16942,-16915,-16733,-16708,-16706,-16689,-16664,-16657,-16647,-16474,-16470,-16465,-16459,  
		-16452,-16448,-16433,-16429,-16427,-16423,-16419,-16412,-16407,-16403,-16401,-16393,-16220,-16216,-16212,-16205,  
		-16202,-16187,-16180,-16171,-16169,-16158,-16155,-15959,-15958,-15944,-15933,-15920,-15915,-15903,-15889,-15878,  
		-15707,-15701,-15681,-15667,-15661,-15659,-15652,-15640,-15631,-15625,-15454,-15448,-15436,-15435,-15419,-15416,  
		-15408,-15394,-15385,-15377,-15375,-15369,-15363,-15362,-15183,-15180,-15165,-15158,-15153,-15150,-15149,-15144,  
		-15143,-15141,-15140,-15139,-15128,-15121,-15119,-15117,-15110,-15109,-14941,-14937,-14933,-14930,-14929,-14928,  
		-14926,-14922,-14921,-14914,-14908,-14902,-14894,-14889,-14882,-14873,-14871,-14857,-14678,-14674,-14670,-14668,  
		-14663,-14654,-14645,-14630,-14594,-14429,-14407,-14399,-14384,-14379,-14368,-14355,-14353,-14345,-14170,-14159,  
		-14151,-14149,-14145,-14140,-14137,-14135,-14125,-14123,-14122,-14112,-14109,-14099,-14097,-14094,-14092,-14090,  
		-14087,-14083,-13917,-13914,-13910,-13907,-13906,-13905,-13896,-13894,-13878,-13870,-13859,-13847,-13831,-13658,  
		-13611,-13601,-13406,-13404,-13400,-13398,-13395,-13391,-13387,-13383,-13367,-13359,-13356,-13343,-13340,-13329,  
		-13326,-13318,-13147,-13138,-13120,-13107,-13096,-13095,-13091,-13076,-13068,-13063,-13060,-12888,-12875,-12871,  
		-12860,-12858,-12852,-12849,-12838,-12831,-12829,-12812,-12802,-12607,-12597,-12594,-12585,-12556,-12359,-12346,  
		-12320,-12300,-12120,-12099,-12089,-12074,-12067,-12058,-12039,-11867,-11861,-11847,-11831,-11798,-11781,-11604,  
		-11589,-11536,-11358,-11340,-11339,-11324,-11303,-11097,-11077,-11067,-11055,-11052,-11045,-11041,-11038,-11024,  
		-11020,-11019,-11018,-11014,-10838,-10832,-10815,-10800,-10790,-10780,-10764,-10587,-10544,-10533,-10519,-10331,  
		-10329,-10328,-10322,-10315,-10309,-10307,-10296,-10281,-10274,-10270,-10262,-10260,-10256,-10254};  

	static const char pystr[396][7] = {"a","ai","an","ang","ao","ba","bai","ban","bang","bao","bei","ben","beng","bi","bian","biao",  
		"bie","bin","bing","bo","bu","ca","cai","can","cang","cao","ce","ceng","cha","chai","chan","chang","chao","che","chen",  
		"cheng","chi","chong","chou","chu","chuai","chuan","chuang","chui","chun","chuo","ci","cong","cou","cu","cuan","cui",  
		"cun","cuo","da","dai","dan","dang","dao","de","deng","di","dian","diao","die","ding","diu","dong","dou","du","duan",  
		"dui","dun","duo","e","en","er","fa","fan","fang","fei","fen","feng","fo","fou","fu","ga","gai","gan","gang","gao",  
		"ge","gei","gen","geng","gong","gou","gu","gua","guai","guan","guang","gui","gun","guo","ha","hai","han","hang",  
		"hao","he","hei","hen","heng","hong","hou","hu","hua","huai","huan","huang","hui","hun","huo","ji","jia","jian",  
		"jiang","jiao","jie","jin","jing","jiong","jiu","ju","juan","jue","jun","ka","kai","kan","kang","kao","ke","ken",  
		"keng","kong","kou","ku","kua","kuai","kuan","kuang","kui","kun","kuo","la","lai","lan","lang","lao","le","lei",  
		"leng","li","lia","lian","liang","liao","lie","lin","ling","liu","long","lou","lu","lv","luan","lue","lun","luo",  
		"ma","mai","man","mang","mao","me","mei","men","meng","mi","mian","miao","mie","min","ming","miu","mo","mou","mu",  
		"na","nai","nan","nang","nao","ne","nei","nen","neng","ni","nian","niang","niao","nie","nin","ning","niu","nong",  
		"nu","nv","nuan","nue","nuo","o","ou","pa","pai","pan","pang","pao","pei","pen","peng","pi","pian","piao","pie",  
		"pin","ping","po","pu","qi","qia","qian","qiang","qiao","qie","qin","qing","qiong","qiu","qu","quan","que","qun",  
		"ran","rang","rao","re","ren","reng","ri","rong","rou","ru","ruan","rui","run","ruo","sa","sai","san","sang",  
		"sao","se","sen","seng","sha","shai","shan","shang","shao","she","shen","sheng","shi","shou","shu","shua",  
		"shuai","shuan","shuang","shui","shun","shuo","si","song","sou","su","suan","sui","sun","suo","ta","tai",  
		"tan","tang","tao","te","teng","ti","tian","tiao","tie","ting","tong","tou","tu","tuan","tui","tun","tuo",  
		"wa","wai","wan","wang","wei","wen","weng","wo","wu","xi","xia","xian","xiang","xiao","xie","xin","xing",  
		"xiong","xiu","xu","xuan","xue","xun","ya","yan","yang","yao","ye","yi","yin","ying","yo","yong","you",  
		"yu","yuan","yue","yun","za","zai","zan","zang","zao","ze","zei","zen","zeng","zha","zhai","zhan","zhang",  
		"zhao","zhe","zhen","zheng","zhi","zhong","zhou","zhu","zhua","zhuai","zhuan","zhuang","zhui","zhun","zhuo",  
		"zi","zong","zou","zu","zuan","zui","zun","zuo"};  

	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, strSrc.c_str(), strlen( strSrc.c_str()), NULL, 0);  
	wchar_t* wszString = new wchar_t[wcsLen + 1];  
	::MultiByteToWideChar(CP_ACP, NULL,  strSrc.c_str(), strlen( strSrc.c_str()), wszString, wcsLen);  
	wszString[wcsLen] = '\0';  

	int chrasc = 0;           

	char* pcReturnString = NULL;  

	//unicode转换为ANSI  
	int length = WideCharToMultiByte(CP_ACP, 0, wszString, -1, 0, 0, 0, 0 );   
	char* nowchar = new char[length+1];  
	memset(nowchar,0,length+1);  
	WideCharToMultiByte( CP_ACP, 0, wszString, -1, nowchar,length, 0, 0 );  

	//转换ANSI,字符部分不变，汉字转换成相应的拼音  
	char *returnstr = new char[6*length +1];  
	memset(returnstr,0,6*length +1);  

	int offset = 0;   
	for (int j = 0; j < length;) // 循环处理字节数组  
	{  
		if (nowchar[j] >= 0 && nowchar[j] < 128) // 非汉字处理  
		{  

			returnstr[offset] = nowchar[j];  
			offset++;  
			j++;  
			continue;  
		}  

		// 汉字处理  
		chrasc = nowchar[j] * 256 + nowchar[j+1]+256;  

		if (chrasc > 0 && chrasc < 160)  
		{  
			returnstr[offset]= nowchar[j];  
			offset++;  
			j++;  
		}  
		else  
		{  
			for (int i = (sizeof(pyvalue)/sizeof(pyvalue[0]) - 1); i >= 0; i--)  
			{  
				if (pyvalue[i] <= chrasc)  
				{  
					strcpy(returnstr+offset ,pystr[i]);  

					offset +=strlen(pystr[i]);    
					break;  
				}  
			}  
			j += 2;  
		}  
	}  

	if( strlen(returnstr) > 0 )  
	{  
		pcReturnString = new char[strlen(returnstr)+1];  
		memset(pcReturnString,0,strlen(returnstr)+1);  
		strcpy(pcReturnString,returnstr);  
	}  

	delete []returnstr;  
	delete []nowchar;  
	delete []wszString;

	std::string strRet = "";
	
	if( pcReturnString != NULL )
	{
		strRet = pcReturnString;
		delete []pcReturnString;
	}
	
	return strRet; 
}

CString CCommon::GetApplicationVersion()
{
	CString strVersion  = "";
	TCHAR szFullPath[MAX_PATH];
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;
	VS_FIXEDFILEINFO * pFileInfo;

	GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
	if (dwVerInfoSize)
	{
		// If we were able to get the information, process it:
		HANDLE  hMem;
		LPVOID  lpvMem;
		unsigned int uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);

		::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);

		int ret = GetLastError();
		WORD nProdVersion[4];

		// Product version from the FILEVERSION of the version info resource 
		nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS); 
		nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS); 


		strVersion.Format(_T("%d.%d.%d.%d"),nProdVersion[0],
			nProdVersion[1],nProdVersion[2],nProdVersion[3]);

		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}

	return strVersion;
} 

TCHAR* CCommon::StringToChar(CString& str)  
{  
	int len = str.GetLength();  
	TCHAR* tr = str.GetBuffer(len);  
	str.ReleaseBuffer();  
	return tr;   
}  

CString CCommon::ExecuteCmd(CString str)  
{  
	SECURITY_ATTRIBUTES sa;  
	HANDLE hRead,hWrite;  

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);  
	sa.lpSecurityDescriptor = NULL;  
	sa.bInheritHandle = TRUE;  
	if (!CreatePipe(&hRead,&hWrite,&sa,0))  
	{  
		AfxMessageBox("Error on CreatePipe()!");  
		return NULL;  
	}  
	STARTUPINFO si={sizeof(si)};  
	PROCESS_INFORMATION pi;  
	si.hStdError = hWrite;  
	si.hStdOutput = hWrite;  
	si.wShowWindow = SW_HIDE;  
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;  
	TCHAR* cmdline = CCommon::StringToChar(str);  
	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))  
	{  
		AfxMessageBox("Error on CreateProcess()!");  
		return NULL;  
	}  
	CloseHandle(hWrite);  

	char buffer[4096];  
	memset(buffer,0,4096);  
	CString output;  
	DWORD byteRead;  

	while(true)  
	{  
		if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL)  
		{  
			break;  
		}  

		output += buffer;  
	}  

	return output;  
}  

CString CCommon::GetFileVersion(CString strFullPath /*= ""*/)
{
	CString strVersion  = "";
	TCHAR szFullPath[MAX_PATH];
	DWORD dwVerInfoSize = 0;
	DWORD dwVerHnd;
	VS_FIXEDFILEINFO * pFileInfo;

	if( strFullPath.IsEmpty() )
	{
		GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));
	}
	else
	{
		sprintf(szFullPath,strFullPath.GetBuffer(0));
	}

	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);
	if (dwVerInfoSize)
	{
		// If we were able to get the information, process it:
		HANDLE  hMem;
		LPVOID  lpvMem;
		unsigned int uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);

		::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (void**)&pFileInfo, &uInfoSize);

		int ret = GetLastError();
		WORD nProdVersion[4];

		// Product version from the FILEVERSION of the version info resource 
		nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS); 
		nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS); 


		strVersion.Format(_T("%d.%d.%d.%d"),nProdVersion[0],
			nProdVersion[1],nProdVersion[2],nProdVersion[3]);

		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}

	return strVersion;
} 

void CCommon::StringToVector(CString strSrc,CString strSep,std::vector<CString>& vecOut)
{
	//分割key
	char* pTemp = strtok(strSrc.GetBuffer(0),strSep);
	while( pTemp )
	{
		vecOut.push_back(pTemp);
		pTemp = strtok(NULL,strSep);
	}
	strSrc.ReleaseBuffer();
}
