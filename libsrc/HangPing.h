#ifndef HANGPING_H
#define HANGPING_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;
//==================================================================
/**
    功能：    将汉字转换成拼音全拼，例如将“马兆瑞”转换成“mazhaorui”。（“妈ma521”可转换成“mama521”）
    @param[in]    Chinese：要转换的汉字字符
    @param[out]    PinYin：转换后的拼音字符
    @par修改记录：
**/
//==================================================================

 /*
 bool m_blnSimaple=false; //是否简拼
 bool m_blnFirstBig=false; //拼音第一个字母是否大写
 bool m_blnAllBiG=false; //拼音是否大写
 bool m_LetterEnd=false; //每个拼音后是否用分隔符
 bool m_unknowSkip=false; //未知拼音的字是否略过
 bool m_filterPunc=true; //是否过滤标点
 */
 void pinyin_gb2312(char * inbuf, char * outbuf, bool m_blnSimaple = false, bool polyphone_support = false, bool m_blnFirstBig = true, bool m_blnAllBiG = false, bool m_LetterEnd = true, bool m_unknowSkip = true, bool m_filterPunc = true) ;

 //utf8 这个函数有的转出拼音是错的
 void pinyin_utf8(char* inbuf,char* outbuf, bool m_blnSimaple=false,bool polyphone_support=false,bool m_blnFirstBig=false,bool m_blnAllBiG=false,bool m_LetterEnd=false, bool m_unknowSkip=true,bool m_filterPunc=true);

 int is_utf8_string(char *utf);
#define HZ2PY_OUTPUT_BUF_ARRAY_SIZE 5120	//一个字段的拼音最长长度
#define HZ2PY_MAX_PINYIN_SIZE 30			//一个字的拼音最长长度
#define HZ2PY_UTF8_CHECK_LENGTH 20			//检查是否为utf8编码时所检查的字符长度
#endif // HANGPING_H



