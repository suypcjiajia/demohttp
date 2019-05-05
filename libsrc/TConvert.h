#ifndef T_CONVERT_H
#define T_CONVERT_H

#include <string>
#include <vector>
#include <time.h>
#include <json/json.h>
#include <iconv.h>
using namespace std;
typedef unsigned char uchar;
typedef unsigned char BYTE;
//类型转换类
class TConvert
{
public:
    //字符串分割函数
    int Split(const string& sStr, const string& sPattern, std::vector<std::string>& resultVector);

    //wstring to string
    const std::string ws2s(const std::wstring& ws);
    //string to wstring
    const std::wstring s2ws(const std::string& s);
    //int to string
    const string IntToStr(int iInt);
    //long to string
    const string LongToStr(long lLong);
    //float to string
    const string FloatToStr(float fFloat);
    //double to string
    const string DoubleToStr(double dDouble);

    //string to int
    int StrToInt(const string& sString);
    //string to long
    long StrToLong(const string& sString);
    //string to float
    float StrToFloat(const string & sString);
    //string to double
    double StrToDouble(const string & sString);

    //int to vector
    vector<unsigned char>   ToVector(int iInt);
    //long to vector
    vector<unsigned char>   ToVector(long lLong);

    // 将16进制字符串转化为hex内存区值
    int StrToHex(uchar *hex,int iHexlen, uchar *str, int iStrlen);
    // 将hex内存区值转化为16进制字符串
    int HexToStr(uchar * str, int iStrlen,uchar * hex, int iHexLen);
   //将sStr内存区值转成16进制字符串
    string StrToHex(const string & sStr);
     //将sStr转成内存区值
    string HexToStr(const string & sStr);


    //替换字符串
    const string Replace(const string& str,const string& pattern,const string& dstPattern,int count = -1);
    //去掉前后空格
    string Trim(const string& str);

    //转大写
    const string ToUpper(const string& psContent);
    //转小写
    const string ToLower(const string& psContent);


    //获取当前时间
    long GetCurrentTime();
    //绝对时转成"2015-01-01 18:20:00"格式的字符串 by suyp
    string  TimeToStr(long ltime);
    //"2015-01-01 18:20:00"格式的字符串转成绝对时 by suyp
    time_t StrToTime(const char*  date,const char* const format="%Y-%m-%d %H:%M:%S");
    //获取当前日期 2017-02-01
    string GetCurrentDay();
    //获取yesterday日期 2017-02-01
    string GetYesterDay();
    //获取当前日期and time 2017-02-01 00:12:12
    string GetCurrentDayTime();

    string  Json2String(const Json::Value & jsonValue);
    long  Json2Long(const Json::Value & jsonValue);
    int Json2Int(const Json::Value & jsonValue);
    double Json2Double(const Json::Value & jsonValue);
    bool  Json2Bool(const Json::Value & jsonValue);
    string JsonArray2String(const Json::Value & jsonValue);

    //UNICODE码转为GB2312码
    int utf8togb2312(const char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen) ;
    //GB2312码转为UNICODE码
    int gb2312toutf8(const char *sourcebuf,size_t sourcelen,char *destbuf,size_t destlen) ;
    //URl编码
    string URLEncode(const string &sIn);
    //URL解码
    string URLDecode(const string &sIn);

    /*json字符串转json对像
     * return 0-success
     * */
    int Str2Json(const string & psPostData,Json::Value  & pJson);
private:
    BYTE toHex(const BYTE &x);
    BYTE fromHex(const BYTE &x);
};

#endif // TCONVERT_H
