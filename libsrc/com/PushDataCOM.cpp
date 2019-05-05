#include "PushDataCOM.h"

#include "PostsAes.h"
#include "TConvert.h"
#include "log/Logger.h"

PushDataCOM::PushDataCOM()
{

}

PushDataCOM::~PushDataCOM()
{

}
//PushData
 int PushDataCOM::PushData(const string& psPushIP, const string& psPushData)
{
    //URL
    string sUrl = "http://"+psPushIP+":23243/?sBusiness=PushData&sMethod=PushData";
   // appendlog(TTDLogger::LOG_DEBUG, "PushDataCOM::PushData ip:%s, data:%s",psPushIP.c_str(), psPushData.c_str());
    //获取HTTP请求
    PostsAes postsAes;
    string sRet =  postsAes.HttpsPost(sUrl,psPushData);
    Json::Value jsonRet;
    Json::Reader reader;
    if (!reader.parse(sRet, jsonRet)){
        return -1;
    }
    TConvert tConvert;
    return tConvert.Json2Int(jsonRet["iRet"]);

}

