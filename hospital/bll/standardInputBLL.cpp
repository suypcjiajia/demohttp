/*
 *Description:标准录入业务接口
 *Others:
*/

#include "com/RetJsonMsg.hpp"
#include "MyUtil.h"
#include "TMd5.h"

#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include "log/Logger.h"
#include "TFile.h"
#include "TConvert.h"
#include "Config.h"
#include "MyAES.h"

#include "com/PostsAes.h"

#include "com/TCurl.h"
#include "com/Request.h"

#include "Token.h"
#include "standardInputBLL.h"
using namespace std;

standardInputBLL::standardInputBLL(){

}


int standardInputBLL::Process(EvhtpSvr * pEvhtp){
    if(pEvhtp->GetUrl() == "/standardInput/add"){
        Add(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/standardInput/del"){
        Delete(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/standardInput/update"){
        Update(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/standardInput/list"){
        GetList(pEvhtp);
    }
    else{
        RetJsonMsg::SetRetMsg(-1,pEvhtp->GetJsonResponse(),"找不到接口名");
    }
}

/*删除
@param: evhtpSvr-
Return: 0 -success
*/
int standardInputBLL::Delete( EvhtpSvr * evhtpSvr)
{
    TConvert tConvert;
    const Json::Value & body = evhtpSvr->GetJsonBody();
    Json::Value & response = evhtpSvr->GetJsonResponse();
    const Json::Value & detoken = evhtpSvr->GetJsonDetoken();
    if( body["id"].isNull() ){
        RetJsonMsg::SetRetMsg(-1,response,"缺少参数");
        return -1;
    }
    string id = tConvert.Json2String(body["id"]);

    if( detoken["name"].isNull() ||  detoken["level"].isNull()){
        RetJsonMsg::SetRetMsg(-1,response,"缺少令牌");
        return -1;
    }else{
        int level = tConvert.Json2Int(detoken["level"]);
        if( level != 999){
            RetJsonMsg::SetRetMsg(-1,response,"权限不足");
            return -1;
        }
    }



    if ( 0 != mDal.Delete(id,"")) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Delete  error");
        RetJsonMsg::SetRetMsg(-4,response);
        return -1;
    }

    RetJsonMsg::SetRetMsg(0,response);
    return 0;
}

/*
add
@param: evhtpSvr-
Return: 0 -success
*/
int standardInputBLL::Add( EvhtpSvr * evhtpSvr){
    const Json::Value & body = evhtpSvr->GetJsonBody();
    Json::Value & response = evhtpSvr->GetJsonResponse();
    const Json::Value & detoken = evhtpSvr->GetJsonDetoken();
    if( body["type"].isNull() ){
        RetJsonMsg::SetRetMsg(-1,response,"缺少参数");
        return -1;
    }
    TConvert tConvert;


    if( detoken["name"].isNull() ||  detoken["level"].isNull()){
        RetJsonMsg::SetRetMsg(-1,response,"缺少令牌");
        return -1;
    }else{
        int level = tConvert.Json2Int(detoken["level"]);
        if( level != 999){
            RetJsonMsg::SetRetMsg(-1,response,"权限不足");
            return -1;
        }
    }

    if ( 0 == mDal.IsExist( tConvert.Json2String( body["value0"]),"value0" ) ){
        RetJsonMsg::SetRetMsg(-1,response,"已存在相同配置");
        return -1;
    }
    standardInputEntity entity;
    entity.value0 = tConvert.Json2String( body["value0"]);
    entity.value1 = tConvert.Json2String( body["value1"]);
    entity.value2 = tConvert.Json2String( body["value2"]);
    entity.value3 = tConvert.Json2String( body["value3"]);
    entity.value4 = tConvert.Json2String( body["value4"]);
    entity.value5 = tConvert.Json2String( body["value5"]);
    entity.type = tConvert.Json2Int( body["type"]);

    if ( 0 != mDal.Insert(entity)) {
        appendlog(TTDLogger::LOG_ERROR,"standardInputBLL::Add error");
        RetJsonMsg::SetRetMsg(-4,response);
        return -1;
    }
    RetJsonMsg::SetRetMsg(0,response);
    return 0;
}

/*
修改普通资料
@param: evhtpSvr-
Return: 0 -success
*/
int standardInputBLL::Update( EvhtpSvr * evhtpSvr)
{
    const Json::Value & body = evhtpSvr->GetJsonBody();
    Json::Value & response = evhtpSvr->GetJsonResponse();
    const Json::Value & detoken = evhtpSvr->GetJsonDetoken();
    if( body["id"].isNull() ){
        RetJsonMsg::SetRetMsg(-1,response,"缺少参数");
        return -1;
    }
    TConvert tConvert;
    string id = tConvert.Json2String(body["id"]);

    if( detoken["name"].isNull() ||  detoken["level"].isNull()){
        RetJsonMsg::SetRetMsg(-1,response,"缺少令牌");
        return -1;
    }else{
        int level = tConvert.Json2Int(detoken["level"]);
        if( level != 999){
            RetJsonMsg::SetRetMsg(-1,response,"权限不足");
            return -1;
        }
    }

    if ( 0 != mDal.IsExist( tConvert.Json2String( body["id"]),"") ){
        RetJsonMsg::SetRetMsg(-1,response,"配置不存在");
        return -1;
    }

    standardInputEntity entity;

    if(!body["value0"].isNull()){
        entity.value0 = tConvert.Json2String(body["value0"]);
    }
    if(!body["value1"].isNull()){
        entity.value1 = tConvert.Json2String(body["value1"]);
    }
    if(!body["value2"].isNull()){
        entity.value2 = tConvert.Json2String(body["value2"]);
    }
    if(!body["value3"].isNull()){
        entity.value3 = tConvert.Json2String(body["value3"]);
    }
    if(!body["value4"].isNull()){
        entity.value4 = tConvert.Json2String(body["value4"]);
    }
    if(!body["value5"].isNull()){
        entity.value5 = tConvert.Json2String(body["value5"]);
    }
    if(!body["type"].isNull()){
        entity.type = tConvert.Json2Int(body["type"]);
    }


    if ( 0 != mDal.Update(id,entity)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Update error");
        RetJsonMsg::SetRetMsg(-4,response);
        return -1;
    }

    RetJsonMsg::SetRetMsg(0,response);
    return 0;
}


/*
所有信息
@param: evhtpSvr-
Return: 0 -success
*/
int standardInputBLL::GetList( EvhtpSvr * evhtpSvr)
{
    TConvert tConvert;
    Json::Value & pResponse = evhtpSvr->GetJsonResponse();
    const Json::Value & body = evhtpSvr->GetJsonBody();
    string where;
    if( !body["type"].isNull() ){
        string type = tConvert.Json2String(body["type"]);
        where = " where type=" + type;
    }


    standardInputList lst;
    mDal.GetListByWhere(where,lst);
    Json::Value result;int i = 0;
    result.resize(0);
    for(auto e : lst){

        result[i]["value0"] = e.value0;
        result[i]["value1"] = e.value1;
        result[i]["value2"] = e.value2;
        result[i]["value3"] = e.value3;
        result[i]["value4"] = e.value4;
        result[i]["value5"] = e.value5;
        result[i]["id"] = e.id;
        result[i]["type"] = e.type;
        i++;
    }

    pResponse[RESPONSE_RESULT] = result;
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}




