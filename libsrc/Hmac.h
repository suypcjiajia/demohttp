#ifndef T_HMAC_H
#define T_HMAC_H

#include <string>
#include <vector>
#include <time.h>
#include <json/json.h>
#include <iconv.h>
using namespace std;
typedef unsigned char uchar;
typedef unsigned char BYTE;
//类型转换类
class Hmac
{
public:
    string Get(const string& sData, const string& sKey);
    /**
     * HMAC
     *
     * @param sData
     *            待摘要数据
     * @param cKey
     *            密钥
     * @param hash 摘要后数据
     * @return void
     */
    void  GetSha256(const string& sData, const string& sKey,unsigned char * hash);
};

#endif //
