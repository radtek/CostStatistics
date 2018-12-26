#pragma once


#include <string>
using std::string;

//==================================================================
/**    
    功能：    将汉字转换成拼音全拼，例如将“马兆瑞”转换成“mazhaorui”。（“妈ma521”可转换成“mama521”）
    @param[in]    Chinese：要转换的汉字字符
    @param[out]    PinYin：转换后的拼音字符
    @par修改记录：
**/
//==================================================================

class CLetterConvert
{
public:
	static void GetPinYin(const char* Chinese, string& PinYin); 
};



