#ifndef COM_REQUEST_H
#define COM_REQUEST_H

#include <string>
#include "json/json.h"
using namespace std;

class Request
{
public:
    Request();
    ~Request();


    /**
     * 获取一条商铺信息
     * @param sIP
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int ShopGetEntity(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons);

    /**
     * 获取一条商铺State
     * @param sIP
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int ShopGetState(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons);

    /**
     * 获取MyShop
     * @param sIP
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int ShopGetMyShop(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons);

    /**
     * 获取多条商铺信息
     * @param sIP
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int ShopGetLst(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons);


    /**
     * 获取一条区代信息
     * @param sIP
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int ZoneAgentGetEntity(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons);


    /**
     * 获取平台粉丝
     * @param sIP
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int PlatformFans(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons);



    /**
     * 向钱包服务器发送post请求
     * @param sUrl
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int WalletHttpPost(const string & sUrl, const Json::Value & jsonPost, Json::Value & jsonRespons);


    /**
     * 向余总的新加密服务发送post请求
     * @param sUrl
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int WalletHttpPost2(const string & sUrl,string key ,const string & timestamp,const string & sSendData, string & sRespons);
    /**
     * 向余总的新加密服务发送post请求
     * @param sUrl
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int WalletHttpPost2(const string & sUrl,string key, const string & timestamp,const Json::Value & jSendData, Json::Value & jRespons);

    static string EncryptSignAesData(const string & sSendData,const string & timestamp,string key);
    static string DecryptSignAesData(const string & sResponsData,string key);
    static bool cmp(char f, char  s);

    /**
     * 向C++服务器发送post请求
     * @param sUrl
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int CppHttpPost( string  sUrl, const Json::Value & jsonPost,Json::Value & jsonRespons);

    /**
     * 向Java服务器发送post请求
     * @param sUrl
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int JavaHttpPost( string  sUrl, const Json::Value & jsonPost,Json::Value & jsonRespons);


    /**
     * 向孙伟服务器发送post请求
     * @param sUrl
     * @param jsonPost
     * @return 0 sucess jsonRespons
     */
    static int SunWeiHttpPost( const string & sUrl,string key, const string & timestamp,const Json::Value & jSendData, Json::Value & jRespons);


    /**
     * 向138请求 验证一个手机号是否虚拟号 0-no -1-yes
     * @param sPhone
     */
    static int Phone138( string  sPhone);



    static const int signLen = 32;
};

#endif // COM_REQUEST_H
