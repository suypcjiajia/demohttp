#include "PostsAes.h"

#include "com/TCurl.h"
#include "MyAES.h"
#include "TConvert.h"
#include "TMd5.h"
#include "log/Logger.h"
PostsAes::PostsAes()
{

}

PostsAes::~PostsAes()
{

}
//POST
string PostsAes::HttpsPost(string psUrl, string psPostData)
{
    string sAppid="425388c9c928c3d36856e67b6d86cc11";
    string sAppKey="53b7a63d019de6ba32ed2e7fe38dfa72";
    string sAesKey="tym1234567890123tym1234567890123";
    TConvert tConvert;
    string sTimeStamp = tConvert.LongToStr(tConvert.GetCurrentTime()/1000000);
    //加密
    MyAES myAES;
    myAES.SetKey((unsigned char*)sAesKey.c_str(),32);
    string sPostData = myAES.Encrypt(psPostData,0);

    string sSign = Md5ToLower( sAppKey  + sPostData + string(sTimeStamp) + sAppid );

    string sUrl = psUrl+"&sAppid="+sAppid+"&sTimeStamp="+sTimeStamp+"&sSign="+sSign;

    vector<string> httpHeaderVector;
    httpHeaderVector.push_back("Accept:application/json;");
    httpHeaderVector.push_back("Content-Type:application/json;charset=UTF-8");

    //获取HTTP请求
    string sReturn;
    TCurl tCurl;

    int iRet =tCurl.Posts(sUrl,httpHeaderVector,sPostData,sReturn,NULL);
    if ( 0 != iRet ) {
        appendlog(TTDLogger::LOG_ERROR,"PostsAes::HttpsPost url=%s,iRet=%d",sUrl.c_str(),iRet);
        appendlog(TTDLogger::LOG_ERROR,"PostsAes::HttpsPost sUrlPost=%s",psPostData.c_str());
        appendlog(TTDLogger::LOG_ERROR,"PostsAes::HttpsPost sReturn=%s",sReturn.c_str());
        return "";
    }
    //加密
    return myAES.Decrypt(sReturn,0);
}
