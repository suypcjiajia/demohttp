#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "TMutex.hpp"
#include <json/json.h>
using namespace std;

/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:
 *Others:
*/
class  MyConfig
{
public:  static MyConfig* GetInstance();
private: static MyConfig* mInstance;
private: TMutex mMutex;                  //线程锁
public:
    //获取调试日志
    const string GetDebugLog(){return this->msDebugLog;}
    //是否使用https,true or false
    const string IsUseSSL(){return this->msOpenSSL;}
    //获取Evhtp启动的线程数量
    const string & GetEvhtpThreads(){return this->msEvthtpThreads;}
    //mysql地址
    const string & GetMysqlAddr(){return this->msMySqlAddr;}
    const string & GetMysqlUser(){return this->msMysqlUser;}
    const string & GetMysqlPwd(){return this->msMysqlPwd;}
    //是否打开安全检验,true or false
    const string & IsCheckSafe(){return this->msCheckSafe;}
    //是否打开AES加密,true or false
    const  string  & IsOpenAes(){return this->msAes;}
    //获取推送服务IP
    const  string& GetPushIP(){return this->msPushIP;}

    //初始化
    int Init(const string & serverName);



    const string & GetAppKey(){return this->appKey;}
    const string & GetAppSecret(){return this->appSecret;}



private:    
    //调试日志
    string msDebugLog;
    //是否使用https
    string msOpenSSL;
    //Evhtp启动的线程数量
    string msEvthtpThreads;
    //mysql地址
    string msMySqlAddr;
    string msMysqlUser;
    string msMysqlPwd;
    //是否打开安全检验
    string msCheckSafe;
    //是否打开AES加密
    string msAes;
    //推送服务IP
    string msPushIP;
    string appKey;
    string appSecret;


};


#endif // CONFIG_H
