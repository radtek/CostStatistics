#ifndef _COMMON_H_
#define _COMMON_H_

#include <map>


class CCommon
{
public:
	CCommon();
	~CCommon();
public:

	//��Ӧ������
	static void GetSupplierQuality(std::map<int,CString>& mapHeader);

	//��Array�в���ָ��Ԫ�ص��±�
	static int FindItemFromArray(CStringArray& strArray, CString& strElement);

	//ȥ���ַ������һ���ַ�
	static void DeleteLastChar(CString& str);

	//��ȡ��ǰ·��
	static CString GetCurPath();


	//��ʽ��ʱ�� 20160619134010 -> 2016-06-19 13:40:10
	static CString FormatTime(CString strDateTime);

	//��ʽ��ʱ�� 2016-03-02 00:00:00 -> 2016-03-02
	static CString FormatDate(CString strDateTime);

	//��ȡticketcount
	static __int64 GetMyTicketCount();

	//�Ƿ�������20160606����2016/06/06��ʽ���ַ���
	static bool IsDate(CString strDate);

	static int IncludeChinese(char *str);//����0�������ģ�����1��������

	//��ȡ�ַ����е�һ������
	static CString GetIntFromString(CString str);

	//�ж��Ƿ���С��
	static bool IsFloat(CString strData);

	//����SQPϵͳ�û���
	static CString GetSQPUser(CString strUser);

	//ȥ��user��tail
	static CString GetRidoffUserTail(CString strUser);

	//��ȡ�ļ�ʱ��
	static CString GetFileTimeString();

	static bool FileIsExist(LPCTSTR szFileFullPathName);

	//��ȡ���뱣���ļ���·��
	static CString GetSecretPath();

	//ת������Ϊƴ��
	static std::string ConvertChineseToPinyin(std::string strSrc);

	//��ȡ��ǰ����汾
	static CString GetApplicationVersion();

	//�ַ�����ʽת��
	static TCHAR* StringToChar(CString& str);

	//ִ��������
	static CString ExecuteCmd(CString str);

	//��ȡ�汾��
	static CString GetFileVersion(CString strFullPath = "");

	//�ָ��ַ���
	static void StringToVector(CString strSrc,CString strSep,std::vector<CString>& vecOut);

protected:
private:
};


#endif