#pragma once


#include <string>
using std::string;

//==================================================================
/**    
    ���ܣ�    ������ת����ƴ��ȫƴ�����罫��������ת���ɡ�mazhaorui����������ma521����ת���ɡ�mama521����
    @param[in]    Chinese��Ҫת���ĺ����ַ�
    @param[out]    PinYin��ת�����ƴ���ַ�
    @par�޸ļ�¼��
**/
//==================================================================

class CLetterConvert
{
public:
	static void GetPinYin(const char* Chinese, string& PinYin); 
};



