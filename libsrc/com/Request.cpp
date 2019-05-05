#include<iostream>

#include "Request.h"

#include "com/TCurl.h"
#include "MyAES.h"
#include "TConvert.h"
#include "TMd5.h"
#include "log/Logger.h"
#include "Hmac.h"
#include "Base64EnDecode.h"
Request::Request()
{

}

Request::~Request()
{

}

/**
 * 获取一条商铺信息
 * @param sIP
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::ShopGetEntity(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons)
{
    string sUrl = "http://"  + sIP  + "/?sMethod=GetEntity&sBusiness=ShopInfo";

    return Request::JavaHttpPost(sUrl,jsonPost,jsonRespons);

}

/**
 * 获取一条商铺State
 * @param sIP
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::ShopGetState(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons)
{
    string sUrl = "http://"  + sIP  + "/?sMethod=GetState&sBusiness=ShopInfo";

    return Request::JavaHttpPost(sUrl,jsonPost,jsonRespons);

}

/**
 * 获取MyShop
 * @param sIP
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::ShopGetMyShop(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons)
{
    string sUrl = "http://"  + sIP  + "/?sMethod=GetMyShop&sBusiness=ShopInfo";

    return Request::JavaHttpPost(sUrl,jsonPost,jsonRespons);

}

/**
 * 获取多条商铺信息
 * @param sIP
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::ShopGetLst(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons)
{
    string sUrl = "http://"  + sIP  + "/?sMethod=GetData&sBusiness=ShopInfo";

    return Request::JavaHttpPost(sUrl,jsonPost,jsonRespons);
}


/**
 * 获取一条区代信息
 * @param sIP
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::ZoneAgentGetEntity(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons)
{
    string sUrl = "http://"  + sIP  + "/?sMethod=GetEntity&sBusiness=ZoneAgent";

    return Request::JavaHttpPost(sUrl,jsonPost,jsonRespons);

}


/**
 * 获取平台粉丝
 * @param sIP
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::PlatformFans(const string & sIP,const Json::Value & jsonPost,Json::Value & jsonRespons)
{
    string sUrl = "http://"  + sIP  + ":23241/?sMethod=PlatformFans&sBusiness=UserRegister";

    return Request::CppHttpPost(sUrl,jsonPost,jsonRespons);

}


/**
 * 向钱包服务器发送post请求
 * @param sUrl
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::WalletHttpPost(const string & sUrl, const Json::Value & jsonPost, Json::Value & jsonRespons)
{
    const static string sAesKey = "!@#$%^&*()QWERTY";
    TConvert tConvert;
    //加密
    MyAES myAES;
    myAES.SetKey((unsigned char*)sAesKey.c_str(),sAesKey.size());
    string sAesPostData = myAES.Encrypt(jsonPost.toStyledString(),4);
    string sRealPostData = "sCondition=" + sAesPostData;


    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");

    string sAesReturn;
    TCurl tCurl;

    int iRet =tCurl.Posts(sUrl,httpHeaderVector,sRealPostData,sAesReturn,NULL);

    if( 0 == iRet ){
        string sReturn =  myAES.Decrypt(sAesReturn,4);
        appendlog(TTDLogger::LOG_INFO,"%s",sReturn.c_str());
        Json::Reader reader;
        if (!reader.parse(sReturn, jsonRespons)){
            return -1;
        }
        iRet = tConvert.Json2Int( jsonRespons["iRet"] );
    }
    return iRet ;
}


/**
 * 向余总的新加密服务发送post请求
 * @param sUrl
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::WalletHttpPost2(const string & sUrl,string key, const string & timestamp,const string & sSendData, string & sRespons)
{


    string baseSend = EncryptSignAesData(sSendData,timestamp,key);
    appendlog(TTDLogger::LOG_INFO,"sort baseSend:%s",baseSend.c_str());

    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");

    string sUrlEnReturn;
    TCurl tCurl;

    TConvert t;
    string urlencodeSend = t.URLEncode(baseSend);
    int iRet =tCurl.Posts(sUrl,httpHeaderVector,urlencodeSend,sUrlEnReturn,NULL);

    if( 0 == iRet ){
        string sSignAesRet = t.URLDecode(sUrlEnReturn);
        sRespons =  DecryptSignAesData(sSignAesRet,key);

    }
    appendlog(TTDLogger::LOG_INFO,"WalletHttpPost2 url:%s post:%s respons:%s",sUrl.c_str(),urlencodeSend.c_str(),sUrlEnReturn.c_str());
    return iRet ;

}

/**
 * 向余总的新加密服务发送post请求
 * @param sUrl
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::WalletHttpPost2(const string & sUrl,string key, const string & timestamp,const Json::Value & jSendData, Json::Value & jRespons)
{
    TConvert tConvert;
    string sRes;
    int iRet = WalletHttpPost2(sUrl,key,timestamp,jSendData.toStyledString(),sRes);

    appendlog(TTDLogger::LOG_INFO,"WalletHttpPost2 url:%s post:%s respons:%s",sUrl.c_str(),jSendData.toStyledString().c_str(),sRes.c_str());


    if( iRet == 0){
        Json::Reader reader;
        if (!reader.parse(sRes, jRespons)){
            return -1;
        }
        string state = tConvert.Json2String( jRespons["state"] );
        if( state == "Error"){
            return -1;
        }
    }
    return iRet;
}

string Request::EncryptSignAesData(const string & sSendData,const string & timestamp,string key){

    Hmac hmac; char buf[32] = {0};
    //签名
    hmac.GetSha256(timestamp  + sSendData,key,(unsigned char*)buf);



    //加密
    MyAES myAES;    
    myAES.SetKey2((unsigned char*)key.c_str(),key.size());
    sort(key.begin(), key.end(),cmp);
    string sAesData = myAES.EncryptNoBase(sSendData ,key);
    int size = sAesData.length() + 32;
    char * p =  new char[size];

    memcpy(p,buf,32);
    memcpy(p+32,&sAesData[0],sAesData.length());

    Base64EnDecode base64EnDecode;

    string tmp =  base64EnDecode.Base64Encode((const unsigned char*)p,size);

    if( p != NULL){
        delete p;
        p= NULL;
    }

    return tmp;

}

string Request::DecryptSignAesData(const string & sResponsData,string key)
{
    Base64EnDecode base64EnDecode;int outByte = 0;
    string deBase = base64EnDecode.Base64Decode((const  char*)sResponsData.c_str(),sResponsData.length(),outByte);
    string sign = deBase.substr(0,signLen);
    string aes = deBase.substr(signLen);
    string test = base64EnDecode.Base64Encode((const unsigned char*)((sign+aes).c_str()),(sign+aes).length());

    //de密
    MyAES myAES;
    myAES.SetKey2((unsigned char*)key.c_str(),key.size());
    sort(key.begin(), key.end(),cmp);
    return myAES.DecryptNotBase(aes ,key);
}

bool Request::cmp(char f, char  s)
{
    if( f > s )
        return false;
    return true;
}

/**
 * 向C++服务器发送post请求
 * @param sUrl
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::CppHttpPost( string  sUrl, const Json::Value & jsonPost,Json::Value & jsonRespons)
{
    string sAppid="425388c9c928c3d36856e67b6d86cc11";
    string sAppKey="53b7a63d019de6ba32ed2e7fe38dfa72";
    string sAesKey="tym1234567890123tym1234567890123";
    TConvert tConvert;
    string sTimeStamp = tConvert.LongToStr(tConvert.GetCurrentTime()/1000000);
    //加密
    MyAES myAES;
    myAES.SetKey((unsigned char*)sAesKey.c_str(),32);
    string sPostData = myAES.Encrypt(jsonPost.toStyledString(),0);

    string sSign = Md5ToLower( sAppKey  + sPostData + string(sTimeStamp) + sAppid );

    sUrl = sUrl+"&sAppid="+sAppid+"&sTimeStamp="+sTimeStamp+"&sSign="+sSign;

    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");



    //获取HTTP请求
    string sReturn;
    TCurl tCurl;

    int iRet =tCurl.Posts(sUrl,httpHeaderVector,sPostData,sReturn,NULL);
    if ( 0 != iRet ) {

        return iRet;
    }

    if( 0 == iRet ){
        string sDeReturn =  myAES.Decrypt(sReturn,0);
        Json::Reader reader;
        if (!reader.parse(sDeReturn, jsonRespons)){
            return -1;
        }
        iRet = tConvert.Json2Int( jsonRespons["iRet"] );
    }else{
        appendlog(TTDLogger::LOG_ERROR,"Request::CppHttpPost url=%s,iRet=%d",sUrl.c_str(),iRet);
        appendlog(TTDLogger::LOG_ERROR,"Request::CppHttpPost sUrlPost=%s",sPostData.c_str());
        appendlog(TTDLogger::LOG_ERROR,"Request::CppHttpPost sReturn=%s",sReturn.c_str());
    }
    return iRet ;

}


/**
 * 向Java服务器发送post请求
 * @param sUrl
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::JavaHttpPost( string  sUrl, const Json::Value & jsonPost,Json::Value & jsonRespons)
{
    string sAppid="425388c9c928c3d36856e67b6d86cc11";
    string sAppKey="53b7a63d019de6ba32ed2e7fe38dfa72";
    string sAesKey="tym1234567890123tym1234567890123";
    TConvert tConvert;
    string sTimeStamp = tConvert.LongToStr(tConvert.GetCurrentTime()/1000000);
    //加密
    MyAES myAES;
    myAES.SetKey((unsigned char*)sAesKey.c_str(),32);
    string sPostData = myAES.Encrypt(jsonPost.toStyledString(),0);

    string sSign = Md5ToLower( sAppKey  + sPostData + string(sTimeStamp) + sAppid );

    sUrl = sUrl+"&sAppid="+sAppid+"&sTimeStamp="+sTimeStamp+"&sSign="+sSign;

    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Content-Type:application/x-www-form-urlencoded");


    //获取HTTP请求
    string sReturn;
    TCurl tCurl;


    sPostData = "sCondition=" + tConvert.URLEncode(sPostData);
    httpHeaderVector.push_back("Content-Length:" + tConvert.IntToStr( sPostData.length()));
    int iRet =tCurl.Posts(sUrl,httpHeaderVector,sPostData,sReturn,NULL);
    if ( 0 != iRet ) {

        return iRet;
    }

    if( 0 == iRet ){
        string sDeReturn =  myAES.Decrypt(sReturn,0);
        Json::Reader reader;
        if (!reader.parse(sDeReturn, jsonRespons)){
            return -1;
        }
        iRet = tConvert.Json2Int( jsonRespons["iRet"] );
    }else{
        appendlog(TTDLogger::LOG_ERROR,"Request::JavaHttpPost url=%s,iRet=%d",sUrl.c_str(),iRet);
        appendlog(TTDLogger::LOG_ERROR,"Request::JavaHttpPost sUrlPost=%s",sPostData.c_str());
        appendlog(TTDLogger::LOG_ERROR,"Request::JavaHttpPost sReturn=%s",sReturn.c_str());
    }
    return iRet ;

}


/**
 * 向孙伟服务器发送post请求
 * @param sUrl
 * @param jsonPost
 * @return 0 sucess jsonRespons
 */
int Request::SunWeiHttpPost( const string & sUrl,string key, const string & timestamp,const Json::Value & jSendData, Json::Value & jRespons)
{
    string sSendData = jSendData.toStyledString();string sRespons;
    string baseSend = EncryptSignAesData(sSendData,timestamp,key);


    appendlog(TTDLogger::LOG_INFO,"SunWeiHttpPost send:%s",sSendData.c_str());


    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");

    string sUrlEnReturn;
    TCurl tCurl;

    TConvert t;

    Json::Value jRealSend;

    string urlencodeSend = t.URLEncode(baseSend);

    jRealSend["encryptionkey"] = urlencodeSend;
    jRealSend["timestamp"] = timestamp;

    string sRealUrl = sUrl;
    sRealUrl += "&encryptionkey=" + urlencodeSend;
    sRealUrl += "&timestamp=" + timestamp;

    int iRet =tCurl.Posts(sRealUrl,httpHeaderVector,jRealSend.toStyledString(),sUrlEnReturn,NULL);

    if( 0 == iRet ){

        Json::Reader reader;
        Json::Value jRet;
        if (!reader.parse(sUrlEnReturn, jRet)){
            return -1;
        }
        string encryptionkey = t.Json2String( jRet["encryptionkey"] );


        string sSignAesRet = t.URLDecode(encryptionkey);
        sRespons =  DecryptSignAesData(sSignAesRet,key);


        if (!reader.parse(sRespons, jRespons)){
            return -1;
        }

    }

    appendlog(TTDLogger::LOG_INFO,"SunWeiHttpPost url:%s send:%s respons:%s",sRealUrl.c_str(),jRealSend.toStyledString().c_str(),sRespons.c_str());
    return iRet ;

}

/**
 * 向138请求 验证一个手机号是否虚拟号 0-no -1-yes
 * @param sPhone
 */
 int Request::Phone138( string  sPhone)
{
     //http://www.ip138.com:8080/search.asp?mobile=13411367357&action=mobile
    string sUrl = "http://www.ip138.com:8080/search.asp?mobile=" + sPhone + "&action=mobile";
    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("www.ip138.com:8080");
    httpHeaderVector.push_back("Accept-Encoding: gzip, deflate");
    httpHeaderVector.push_back("Accept-Language: zh-CN,zh;q=0.9");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");
    httpHeaderVector.push_back("Connection: keep-alive");
    httpHeaderVector.push_back("Cache-Control: max-age=0");
    httpHeaderVector.push_back("User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36");
    httpHeaderVector.push_back("Upgrade-Insecure-Requests: 1");
    httpHeaderVector.push_back("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
    httpHeaderVector.push_back("Cookie: pgv_pvi=7117202432; Hm_lvt_407473d433e871de861cf818aa1405a1=1506283595; Hm_lvt_407473d433e871de861cf818aa1405a1=1506410186; ASPSESSIONIDSARQSRCT=GJPKKAKALGFGMKFBLCFMEPEB");


    TCurl tCurl;string sReturn;

    tCurl.Get(sUrl,httpHeaderVector,sReturn);


    TConvert tConvert;

    char destbuf[128] = {0};
    string src = "虚拟";
    tConvert.utf8togb2312(src.c_str(),src.length(),destbuf,128);

    char whitename[128] = {0};
    string sWhitename = "海航通信";
    tConvert.utf8togb2312(sWhitename.c_str(),sWhitename.length(),whitename,128);


    if( (sReturn.find(destbuf) != string::npos)  && (sReturn.find(whitename) == string::npos) ){
        appendlog(TTDLogger::LOG_INFO,"Request::Phone138 phone verify not pass sPhone=%s",sPhone.c_str());
        return -1;
    }
    return 0;

}
