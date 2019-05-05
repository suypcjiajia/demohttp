/*
 *Description:配置业务接口
 *Others:
*/

#include "com/RetJsonMsg.hpp"
#include "MyUtil.h"
#include "TMd5.h"
#include "SettingBLL.h"
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

using namespace std;

SettingBLL::SettingBLL(){

}


int SettingBLL::Process(EvhtpSvr * pEvhtp){
    if(pEvhtp->GetUrl() == "/setting/add"){
        Add(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/setting/del"){
        Delete(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/setting/update"){
        Update(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/setting/list"){
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
int SettingBLL::Delete( EvhtpSvr * evhtpSvr)
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
int SettingBLL::Add( EvhtpSvr * evhtpSvr){
    const Json::Value & body = evhtpSvr->GetJsonBody();
    Json::Value & response = evhtpSvr->GetJsonResponse();
    const Json::Value & detoken = evhtpSvr->GetJsonDetoken();
    if( body["name"].isNull() || body["type"].isNull() ){
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

    if ( 0 == mDal.IsExist( tConvert.Json2String( body["name"]),"name" ) ){
        RetJsonMsg::SetRetMsg(-1,response,"已存在相同配置");
        return -1;
    }
    SettingEntity entity;
    entity.name = tConvert.Json2String( body["name"]);
    entity.type = tConvert.Json2Int( body["type"]);

    if ( 0 != mDal.Insert(entity)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Add error");
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
int SettingBLL::Update( EvhtpSvr * evhtpSvr)
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

    if ( 0 != mDal.IsExist( tConvert.Json2String( body["id"]),"" ) ){
        RetJsonMsg::SetRetMsg(-1,response,"配置不存在");
        return -1;
    }

    SettingEntity entity;

    if(!body["name"].isNull()){
        entity.name = tConvert.Json2String(body["name"]);
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
int SettingBLL::GetList( EvhtpSvr * evhtpSvr)
{
    TConvert tConvert;
    Json::Value & pResponse = evhtpSvr->GetJsonResponse();
    const Json::Value & body = evhtpSvr->GetJsonBody();
    string where;
    if( !body["type"].isNull() ){
        string type = tConvert.Json2String(body["type"]);
        where = " where type=" + type;
    }


    SettingList lst;
    mDal.GetListByWhere(where,lst);
    Json::Value result;int i = 0;
    result.resize(0);
    for(auto e : lst){

        result[i]["name"] = e.name;
        result[i]["id"] = e.id;
        result[i]["type"] = e.type;
        i++;
    }

    pResponse[RESPONSE_RESULT] = result;

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}




