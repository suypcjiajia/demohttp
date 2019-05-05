#include "TConvert.h"

#include <wchar.h>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include "log/Logger.h"
#include <ctype.h>
#include <iconv.h>
//#include <Ice/Ice.h>
#ifdef _WIN32
#define GetCurrentTime() GetCurrentTime()
#endif
using namespace std;
//字符串分割函数
int TConvert::Split( const string& sStr, const string& sPattern, std::vector<std::string>& resultVector )
{

    resultVector.clear();

    if(sStr.empty())
        return 0;

    std::string::size_type pos;
    string str = sStr + sPattern;//扩展字符串以方便操作
    std::size_t size=str.size();

    for(std::size_t i=0; i<size; i++)
    {
        pos=str.find(sPattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            resultVector.push_back(s);
            i=pos+sPattern.size()-1;
        }
    }

    return 0;
}
//wstring to string
const std::string TConvert::ws2s(const std::wstring& ws)
{
    //return IceUtil::wstringToString(ws);
    //string result(ws.begin(), ws.end());
    //return result;
    return "";
}
//string to wstring
const std::wstring TConvert::s2ws(const std::string& s)
{
  //  return IceUtil::stringToWstring(s);
    /*
    setlocale(LC_ALL, "chs");
        const char* _Source = s.c_str();
        size_t _Dsize = s.size() + 1;
        wchar_t *_Dest = new wchar_t[_Dsize];
        wmemset(_Dest, 0, _Dsize);
        mbstowcs(_Dest,_Source,_Dsize);
        std::wstring result = _Dest;
        delete []_Dest;
        setlocale(LC_ALL, "C");
        return result;


setlocale(LC_ALL, "chs");
    std::wstring wstr(s.length(),L' ');
          std::copy(s.begin(), s.end(), wstr.begin());
setlocale(LC_ALL, "chs");
          return wstr;
*/
    //wstring result(s.begin(), s.end());
    //return result;
    return L"";
}
//int to string
const string TConvert::IntToStr(int iInt)
{
    std::stringstream ssStream;
    ssStream << iInt;
    return ssStream.str();
}
//long to string
const string TConvert::LongToStr(long lLong)
{
    std::stringstream ssStream;
    ssStream << lLong;
    return ssStream.str();
}
//float to string
const string TConvert::FloatToStr(float fFloat){
    char buf[256] = {0};
    snprintf(buf,256-1,"%0.5f",fFloat);
    string tmp = buf;
    return tmp;
}
//double to string
const string TConvert::DoubleToStr(double dDouble){
    char buf[256] = {0};
    snprintf(buf,256-1,"%0.9f",dDouble);
    string tmp = buf;
    return tmp;
}

//string to int
int TConvert::StrToInt(const string& sString)
{
    int iRet;
    std::stringstream ssStream;
    ssStream << sString;
    ssStream >> iRet;
    return iRet;
}

//string to long
long TConvert::StrToLong(const string& sString)
{
    long lRet;
    std::stringstream ssStream;
    ssStream << sString;
    ssStream >> lRet;
    return lRet;
}
//string to float
float TConvert::StrToFloat(const string & sString){
    float fRet;
    std::stringstream ssStream;
    ssStream << sString;
    ssStream >> fRet;
    return fRet;
}

//string to double
double TConvert::StrToDouble(const string & sString){
    double dRet;
    std::stringstream ssStream;
    ssStream << sString;
    ssStream >> dRet;
    return dRet;
}

//获取当前时间
long TConvert::GetCurrentTime()
{
    struct timeval tv;
    gettimeofday( &tv,NULL );
    return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}
//替换字符串
const string TConvert::Replace(const string& str,const string& pattern,const string& dstPattern,int count)
{
    std::string retStr="";
    string::size_type pos;

    string::size_type szStr=str.length();
    //int szPattern=pattern.size();
    string::size_type i=0;
    int l_count=0;
    if(-1 == count) // replace all
        count = szStr;

    for(i=0; i<szStr; i++)
    {
        pos=str.find(pattern,i);

        if(std::string::npos == pos)
            break;
        if(pos < szStr)
        {
            std::string s=str.substr(i,pos-i);
            retStr += s;
            retStr += dstPattern;
            i=pos+pattern.length()-1;
            if(++l_count >= count)
            {
                i++;
                break;
            }
        }
    }
    retStr += str.substr(i);
    return retStr;
}
//去掉前后空格
string TConvert::Trim(const string& str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}
//转大写
const string TConvert::ToUpper(const string& psContent)
{
    string sContent = psContent;
    transform(sContent.begin(), sContent.end(), sContent.begin(), ::toupper);
    return sContent;
}
//转小写
const string TConvert::ToLower(const string& psContent)
{
    string sContent = psContent;
    transform(sContent.begin(), sContent.end(), sContent.begin(), ::tolower);
    return sContent;
}

//int to vector
vector<unsigned char>   TConvert::ToVector(int iInt){
    string sTmp;
    sTmp = IntToStr(iInt);
    vector<unsigned char> vTmp = vector<unsigned char>(sTmp.begin(),sTmp.end());
    return vTmp;
}

//long to vector
vector<unsigned char>   TConvert::ToVector(long lLong){
    string sTmp;
    sTmp = LongToStr(lLong);
    vector<unsigned char> vTmp = vector<unsigned char>(sTmp.begin(),sTmp.end());
    return vTmp;
}
#ifndef _WIN32


// 将16进制字符串转化为hex内存区值
int TConvert::StrToHex(uchar *hex,int iHexlen, uchar *str, int iStrlen)
{
    if((iStrlen % 2 == 1) || iHexlen  <=  iStrlen/2 )
        return -1;
    uchar h1,h2;
    int i;

    for (i=0; i<iStrlen/2; i++)
    {
        h1 = str[2*i];
        h2 = str[2*i+1];

        if (h1 >= 48 && h1 <=57)
            h1 -= 48;
        else if( h1 >= 65 && h1 <=70)
            h1 -= 55;

        if (h2 >= 48 && h2 <=57)
            h2 -= 48;
        else if( h2 >= 65 && h2 <=70)
            h2 -= 55;

        hex[i] = h1*16 + h2;
    }
    return 0;
}


// 将hex内存区值转化为16进制字符串
int TConvert::HexToStr(uchar * str, int iStrlen,uchar * hex, int iHexLen)
{
    if( iStrlen -1 < iHexLen * 2 )
        return -1
                ;
    char	ddl,ddh;
    int i;

    for (i=0; i<iHexLen; i++)
    {
        ddh =  hex[i] / 16;
        ddl =   hex[i] % 16;
        if (ddh >= 0 && ddh <=9)
            ddh += 48;
        else if( ddh >=10 && ddh <=15)
            ddh += 55;
        if (ddl >= 0 && ddl <=9)
            ddl += 48;
        else if( ddl >=10 && ddl <=15)
            ddl += 55;
        str[i*2] = ddh;
        str[i*2+1] = ddl;
    }

    str[iHexLen*2] = '\0';
    return 0;
}

//将sStr转成16进制字符串
string TConvert::StrToHex(const string &  sStr){
    int iLen = sStr.length();
    uchar	ddl,ddh;
    string sOut;
    for (int i=0; i<iLen; i++)
    {
        uchar tmp = sStr[i];
        ddh =  tmp / 16;
        ddl =   tmp % 16;
        if (ddh >= 0 && ddh <=9)
            ddh += 48;
        else if( ddh >=10 && ddh <=15)
            ddh += 55;
        if (ddl >= 0 && ddl <=9)
            ddl += 48;
        else if( ddl >=10 && ddl <=15)
            ddl += 55;
        sOut.resize(i*2 +2);
        sOut[i*2] = ddh;
        sOut[i*2+1] = ddl;
    }

    sOut[iLen*2] = '\0';
    return sOut;
}

//将sStr转成内存区值
string TConvert::HexToStr(const string & sStr){

    string sOut;
    int iStrlen = sStr.length();
    if(iStrlen%2 ==1)
        return  sOut;

    uchar h1,h2;
    int i;

    for (i=0; i<iStrlen/2; i++)
    {
        h1 = sStr[2*i];
        h2 = sStr[2*i+1];

        if (h1 >= 48 && h1 <=57)
            h1 -= 48;
        else if( h1 >= 65 && h1 <=70)
            h1 -= 55;

        if (h2 >= 48 && h2 <=57)
            h2 -= 48;
        else if( h2 >= 65 && h2 <=70)
            h2 -= 55;

        sOut.resize(i+1);
        sOut[i] = h1*16 + h2;
    }
    return sOut;
}

#endif

//绝对时转成"2015-01-01 18:20:00"格式的字符串
string  TConvert::TimeToStr(long ltime){
    if( ltime <=0)
        return "";
    struct tm *local;
    time_t t = (time_t)ltime;
    local=localtime(&t);
    char buf[128] = {0};
    snprintf(buf,128 -1,"%04d-%02d-%02d %02d:%02d:%02d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);
    return buf;
}

//"2015-01-01 18:20:00"格式的字符串转成绝对时
time_t TConvert::StrToTime(const char*  date,const char* const format)
{
    struct tm tm;
    strptime(date,format, &tm) ;
    time_t ft=mktime(&tm);
    return ft;
}



//获取当前日期 2017-02-01
string TConvert::GetCurrentDay()
{
    long lCur = GetCurrentTime();
    return TimeToStr(lCur/1000000).substr(0,10);
}

//获取yesterday日期 2017-02-01
string TConvert::GetYesterDay()
{
    long lCur = GetCurrentTime();
    return TimeToStr(lCur/1000000 - 3600*24).substr(0,10);
}

//获取当前日期and time 2017-02-01 00:12:12
string TConvert::GetCurrentDayTime()
{
    long lCur = GetCurrentTime();
    return TimeToStr(lCur/1000000);
}


string TConvert::Json2String(const Json::Value & jsonValue){
    string out = "";
    if( jsonValue.isArray() || jsonValue.isBool() || jsonValue.isNull()){
        out = "";
    }else   if( jsonValue.isString()){
        out =  jsonValue.asString();
    }else{

        out = jsonValue.toStyledString();
        if( out[out.length() -1 ] == '\n')
            out = out.substr(0,out.length() -1);
    }
    return out;

}
long TConvert::Json2Long(const Json::Value & jsonValue){
    long out = -1;
    if( jsonValue.isArray() || jsonValue.isBool() || jsonValue.isNull()){
        out = -1;
    }else   if( jsonValue.isString()){
        out =StrToLong( jsonValue.asString());
    } else{
        string str = jsonValue.toStyledString();
        if( str[str.length() -1 ] == '\n')
            out = StrToLong (str.substr(0,str.length() -1));
    }
    return out;
}

int TConvert::Json2Int(const Json::Value & jsonValue){
    int out = -1;
    if( jsonValue.isArray() || jsonValue.isBool() || jsonValue.isNull()){
        out = -1;
    }else   if( jsonValue.isString()){
        out =StrToInt( jsonValue.asString());
    }else if (jsonValue.isInt()){
        out = jsonValue.asInt();
    }else{
        string str = jsonValue.toStyledString();
        if( str[str.length() -1 ] == '\n')
            out = StrToInt(str.substr(0,str.length() -1));
    }
    return out;
}

double TConvert::Json2Double(const Json::Value & jsonValue){
    double out = -1.0;
    if( jsonValue.isArray() || jsonValue.isBool() || jsonValue.isNull()){
        out =  -1.0;
    }else if( jsonValue.isString()){
        out =StrToDouble( jsonValue.asString());
    }else if( jsonValue.isDouble() )
        out = jsonValue.asDouble();
    else{
        string str = jsonValue.toStyledString();
        if( str[str.length() -1 ] == '\n')
            out = StrToDouble(str.substr(0,str.length() -1));
    }
    return out;
}

bool TConvert::Json2Bool(const Json::Value & jsonValue){
    bool out = false;
    if( jsonValue.isString()){
        if( strcasecmp( jsonValue.asString().c_str(),"true") == 0 )
            out = true;
    }
    else if( jsonValue.isBool())
        out = jsonValue.asBool();

    return out;
}

string TConvert::JsonArray2String(const Json::Value & jsonValue){
    string member;
    if( jsonValue.isArray()){

        for( int i = 0;i < jsonValue.size(); i++){
            member +=  Json2String( jsonValue[i])  + ",";
        }
        if( !member.empty()){
            member = member.substr(0,member.length() -1);
        }

    }


    return member;
}


//UNICODE码转为GB2312码
int TConvert::utf8togb2312(const char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen) {

    iconv_t cd;

    if( (cd = iconv_open("gb2312","utf-8")) ==0 )
        return -1;

    memset(destbuf,0,destlen);

    char **source =(char**) &sourcebuf;

    char **dest = (char**)&destbuf;
    if(-1 == iconv(cd,source,&sourcelen,dest,&destlen))
        return -1;

    iconv_close(cd);
    return 0;
}
//GB2312码转为UNICODE码
int TConvert::gb2312toutf8(const char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen) {

    iconv_t cd;

    if( (cd = iconv_open("utf-8","gb2312")) ==0 )
        return -1;

    memset(destbuf,0,destlen);

    char **source =(char**) &sourcebuf;

    char **dest = (char**)&destbuf;
    if(-1 == iconv(cd,source,&sourcelen,dest,&destlen))
        return -1;

    iconv_close(cd);
    return 0;
}



BYTE TConvert::toHex(const BYTE &x)
{
    return x > 9 ? x -10 + 'A': x + '0';
}

BYTE TConvert::fromHex(const BYTE &x)
{
    return isdigit(x) ? x-'0' : x-'A'+10;
}

string TConvert::URLEncode(const string &sIn)
{
    string sOut;
    for( size_t ix = 0; ix < sIn.size(); ix++ )
    {
        BYTE buf[4];
        memset( buf, 0, 4 );
        if( isalnum( (BYTE)sIn[ix] ) )
        {
            buf[0] = sIn[ix];
        }
        else
        {
            buf[0] = '%';
            buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
            buf[2] = toHex( (BYTE)sIn[ix] % 16);
        }
        sOut += (char *)buf;
    }
    return sOut;
};

string TConvert::URLDecode(const string &sIn)
{
    string sOut;
    for( size_t ix = 0; ix < sIn.size(); ix++ )
    {
        BYTE ch = 0;
        if(sIn[ix]=='%')
        {
            ch = (fromHex(sIn[ix+1])<<4);
            ch |= fromHex(sIn[ix+2]);
            ix += 2;
        }
        else
        {
            ch = sIn[ix];
        }
        sOut += (char)ch;
    }
    return sOut;
}
//json字符串转json对像
int TConvert::Str2Json(const string& pPostData,Json::Value  & pJson){
    Json::Reader reader;
    if (!reader.parse(pPostData, pJson)){
        return -1;
    }
    if( pJson.size() == 0){
        return -1;
    }
    return 0;
}

