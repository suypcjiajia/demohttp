#include "Config.h"
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"

#include "UserDataBLL.h"
MyConfig* MyConfig::mInstance = NULL;
MyConfig* MyConfig::GetInstance()
{
        if (mInstance == NULL){
                mInstance = new MyConfig;
        }
        return mInstance;
}
//初始化
int MyConfig::Init()
{
    //配置文件
    const string sConfig = "./demo.conf";
    //读配置文件
    TFile tFile;

    if (tFile.IsFileExist(sConfig)) {
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not file=%s",sConfig.c_str());
        return -1;
    }
    tFile.GetValue(sConfig,"DebugLog",this->msDebugLog);
    if(msDebugLog.empty()){
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not set DebugLog");
        return -1;
    }
    tFile.GetValue(sConfig,"OpenSSL",this->msOpenSSL);
    if(msOpenSSL.empty()){
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not set OpenSSL");
        return -1;
    }
    tFile.GetValue(sConfig,"EvthtpThreads",this->msEvthtpThreads);
    if(msEvthtpThreads.empty()){
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not set EvthtpThreads");
        return -1;
    }
    tFile.GetValue(sConfig,"MySqlAddr",this->msMySqlAddr);
    if(msMySqlAddr.empty()){
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not set MySqlAddr");
        return -1;
    }
    tFile.GetValue(sConfig,"MySqlUser",this->msMysqlUser);
    if(msMysqlUser.empty()){
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not set MySqlUser");
        return -1;
    }
    tFile.GetValue(sConfig,"MySqlPwd",this->msMysqlPwd);
    if(msMysqlPwd.empty()){
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not set msMysqlPwd");
        return -1;
    }
    tFile.GetValue(sConfig,"CheckSafe",this->msCheckSafe);
    if(msCheckSafe.empty()){
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not set CheckSafe");
        return -1;
    }
    tFile.GetValue(sConfig,"Aes",this->msAes);
    if(msAes.empty()){
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not set msAes");
        return -1;
    }

    tFile.GetValue(sConfig,"sPushIP",this->msPushIP);
    if(msPushIP.empty()){
        appendlog(TTDLogger::LOG_ERROR,"MyConfig::Init not set sPushIP");
        return -1;
    }





    tFile.GetValue(sConfig,"appKey",this->appKey);


    tFile.GetValue(sConfig,"appSecret",this->appSecret);


    return 0;
}

