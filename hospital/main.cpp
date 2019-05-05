#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <iostream>
#include <string>
#include<iostream>
#include<iomanip>
#include <math.h>
#include <unistd.h>
#include "log/Logger.h"
#include <dal/MysqlConnect.h>
#include <bll/Config.h>
#include "com/RetJsonMsg.hpp"
#include "bll/PushBLL.h"
#include "com/Request.h"
#include "com/TCurl.h"
#include "Base64EnDecode.h"
#include "MyUtil.h"
#include "bll/Config.h"
#include "bll/UserDataBLL.h"
#include "bll/SettingBLL.h"
#include "dal/TMultiMysqlDAL.h"
#include "dal/MysqlConnect.h"
#include "com/EvhtpSvr.h"
#include "Token.h"
#include "bll/factory.h"
using namespace std;

Factory factory;//

// Evhtp通信回调函数
void ProcessCall(EvhtpSvr * pEvhtp)
{
    string psBusiness = pEvhtp->GetQuery("sBusiness");
    string psMethod = pEvhtp->GetQuery("sMethod");
}

// Evhtp通信回调函数
void ProcessCall2(EvhtpSvr * pEvhtp){
    BaseBLL * p = factory.GetBLL(pEvhtp->GetUrl());
    if(p != NULL){
        //交给具体业务处理
        p->Process(pEvhtp);
    }
    else{
        RetJsonMsg::SetRetMsg(-1,pEvhtp->GetJsonResponse(),"获取业务对像时出错");
    }
}
int main(int argc, const char* argv[]) {
    //服务名称
    string sServiceName = argv[0];
    int iPos = sServiceName.find_last_of("/");
    sServiceName = sServiceName.substr(iPos+1);

    //查看版本，命令行下执行 "程序名 -version -v"
    if(argc == 2){
        if( strcmp(argv[1],"-version" ) == 0 || strcmp(argv[1],"-v" ) == 0){
            printf("1.0\n");
            exit(0);
        }
    }

    //初始化config
    if ( 0 != MyConfig::GetInstance()->Init(sServiceName)) {
        return -1;
    }
    TMultiMysqlDAL::SetMysqlAddr(MyConfig::GetInstance()->GetMysqlAddr());//设置数据库ip
    TMultiMysqlDAL::SetMysqlUser(MyConfig::GetInstance()->GetMysqlUser());//设置数据库user
    TMultiMysqlDAL::SetMysqlPwd(MyConfig::GetInstance()->GetMysqlPwd());//设置数据库password
    MysqlConnect::GetInstance()->Init("hospital");//设置数据库名


    //定义日志输出路径
    TTDLogger::Logger::getInstance()->setPath(string("/log/"+sServiceName + "/").c_str());

    if ("true" == MyConfig::GetInstance()->GetDebugLog()) {

        TTDLogger::Logger::getInstance()->setLevel(TTDLogger::LOG_DEBUG);
    } else {

        TTDLogger::Logger::getInstance()->setLevel(TTDLogger::LOG_INFO);
    }

    //

    //日志
    appendlog(TTDLogger::LOG_INFO,"%s::main",sServiceName.c_str());


    PushBLL::GetInstance()->Start();



    //启动http(s)服务
    EvhtpSvr::Init(sServiceName);
    EvhtpSvr::SetProcessCallBack(ProcessCall);
    EvhtpSvr::SetProcessCallBack2(ProcessCall2);
    EvhtpSvr::StartSvr(1314,MyConfig::GetInstance()->IsUseSSL(),
                       MyConfig::GetInstance()->GetEvhtpThreads(),
                       MyConfig::GetInstance()->IsOpenAes(),
                       MyConfig::GetInstance()->IsCheckSafe());

    factory.MakeBLL();

    while(true){
        sleep(2);
    }
}
