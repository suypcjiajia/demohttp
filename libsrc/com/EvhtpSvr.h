#ifndef EVHTP_H
#define EVHTP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <evhtp.h>
#include <unistd.h>
#include "log/Logger.h"
#include"json/json.h"
#include "MyAES.h"
using namespace std;

class EvhtpSvr;
// 定义回调函数
typedef void(*ProcessCallBack)(EvhtpSvr * pEvhtp);
// 定义回调函数
static ProcessCallBack mProcessCallBack = NULL;

// 定义回调函数
typedef void(*ProcessCallBack2)(EvhtpSvr * pEvhtp );
// 定义回调函数
static ProcessCallBack2 mProcessCallBack2 = NULL;

//权限验证名单
static map<string,string> mAppId_AppKeyAesMap;
//是否验证判断
static string msIsCheckSafe;
//是否AES判断
static string msIsAes;

class EvhtpSvr
{
public:
    EvhtpSvr();
    //初始化
    static int Init(const string& sServiceName);
    static int WhiteInit();
    //启动服务
    static void StartSvr(const int piPort,const string& pbIsSSL,const string& piThreads,const string& psIsAes,const string& psIsCheckSafe);

    static void SetProcessCallBack2(ProcessCallBack2 processCallBack2);
    static void SetProcessCallBack(ProcessCallBack processCallBack);
    //请求的处理函数
    void Process(evhtp_request_t * request, void * pUserParma);
    //处理带token，不加密的情况
    void Process2(evhtp_request_t * request, void * pUserParma);

    //请求回调
    static void  RequestCb(evhtp_request_t * request, void * pUserParma);
    static void * EventBaseLoop(void *);
    static string HttpMehtodEnum2Str(const htp_method  & pMethod );
    static int dump_query(evhtp_kv_t * kv, void * arg);
    static int dump_header(evhtp_kv_t * kv, void * arg);

    static int ToJson( string & s);


    //获取URl Head
    string GetHead(const string & key) const;
    //获取URl参数
    string GetQuery(const string & key) const;
    //获取URl
    string GetUrl() const;
    const Json::Value & GetJsonBody() const;
    Json::Value & GetJsonResponse();
    const Json::Value & GetJsonDetoken()const;


private:
    //请求URl参数
    map<string,string> mapQueryParam;
    //请求URl Head
    map<string,string> mapHeadIn;
    string mUrl;
    //body转成的json格式
    Json::Value mJsonbody;
    //response转成的json格式
    Json::Value mJsonResponse;
    //解密后的令牌
    Json::Value mJsonDetoken;

    MyAES myaes;


    static evhtp_ssl_cfg_t msslcfg;
    static map<string,string> mapWhiteName;

    //安全检验
    int SafeCheck(const string & psAppid,const string & psTimeStamp,const string & psSign,const string & psPostData);

    int SetKey();

    int SetKey2();

    int FireWall(string,evhtp_request_t * request);

};

#endif // EVHTP_H
