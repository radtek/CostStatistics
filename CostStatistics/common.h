#ifndef _COMMON_H_
#define _COMMON_H_

#include <map>


class CCommon
{
public:
	CCommon();
	~CCommon();
public:

	//供应商质量
	static void GetSupplierQuality(std::map<int,CString>& mapHeader);

	//从Array中查找指定元素的下标
	static int FindItemFromArray(CStringArray& strArray, CString& strElement);

	//去掉字符串最后一个字符
	static void DeleteLastChar(CString& str);

	//获取当前路径
	static CString GetCurPath();


	//格式化时间 20160619134010 -> 2016-06-19 13:40:10
	static CString FormatTime(CString strDateTime);

	//格式化时间 2016-03-02 00:00:00 -> 2016-03-02
	static CString FormatDate(CString strDateTime);

	//获取ticketcount
	static __int64 GetMyTicketCount();

	//是否是日期20160606或者2016/06/06格式的字符串
	static bool IsDate(CString strDate);

	static int IncludeChinese(char *str);//返回0：无中文，返回1：有中文

	//获取字符串中第一个数字
	static CString GetIntFromString(CString str);

	//判断是否是小数
	static bool IsFloat(CString strData);

	//处理SQP系统用户名
	static CString GetSQPUser(CString strUser);

	//去掉user的tail
	static CString GetRidoffUserTail(CString strUser);

	//获取文件时间
	static CString GetFileTimeString();

	static bool FileIsExist(LPCTSTR szFileFullPathName);

	//获取密码保存文件的路径
	static CString GetSecretPath();

	//转换汉字为拼音
	static std::string ConvertChineseToPinyin(std::string strSrc);

	//获取当前软件版本
	static CString GetApplicationVersion();

	//字符串格式转换
	static TCHAR* StringToChar(CString& str);

	//执行命令行
	static CString ExecuteCmd(CString str);

	//获取版本号
	static CString GetFileVersion(CString strFullPath = "");

	//分割字符串
	static void StringToVector(CString strSrc,CString strSep,std::vector<CString>& vecOut);

protected:
private:
};


#endif