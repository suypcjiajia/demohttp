/*
 *Description:封装用户业务接口
 *Others:
*/

#include "com/RetJsonMsg.hpp"
#include "MyUtil.h"
#include "TMd5.h"
#include "UserDataBLL.h"
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

int UserDataBLL::Process(EvhtpSvr * pEvhtp){
    if(pEvhtp->GetUrl() == "/user/add"){
        Add(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/user/update"){
        Update(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/user/info"){
        GetUserInfo(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/user/login"){
        Login(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/user/del"){
        Delete(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/user/modifyPwd"){
        ModifyPwd(pEvhtp);
    }else if(pEvhtp->GetUrl() == "/user/list"){
        GetUserList(pEvhtp);
    }else{
        RetJsonMsg::SetRetMsg(-1,pEvhtp->GetJsonResponse(),"找不到接口名");
    }
}


/*use login
@param: evhtpSvr-
Return: 0 -success
*/
int UserDataBLL::Login(EvhtpSvr * evhtpSvr){
    const Json::Value & body = evhtpSvr->GetJsonBody();
    Json::Value & response = evhtpSvr->GetJsonResponse();
    if(body["name"].isNull() || body["password"].isNull()){
        RetJsonMsg::SetRetMsg(-1,response,"缺少参数");
        return -1;
    }
    TConvert tConvert;
    string name = tConvert.Json2String( body["name"] );
    string password = tConvert.Json2String( body["password"] );

    UserDataEntity entity;
    mUserDataDAL.GetEntity(name,entity,"name");
    if( entity.password != password){
        RetJsonMsg::SetRetMsg(-1,response,"password error");
        return -1;
    }
    Json::Value result;
    result["level"] = entity.level;
    result["department"] = entity.department;
    result["name"] = entity.name;
    string token = Token::MakeLoginToken(result.toStyledString());//令牌
    result["token"] = token;
    response[RESPONSE_RESULT] = result;
    RetJsonMsg::SetRetMsg(0,response);

    return 0;

}

/*删除
@param: evhtpSvr-
Return: 0 -success
*/
int UserDataBLL::Delete( EvhtpSvr * evhtpSvr)
{
    TConvert tConvert;
    const Json::Value & body = evhtpSvr->GetJsonBody();
    Json::Value & response = evhtpSvr->GetJsonResponse();
    const Json::Value & detoken = evhtpSvr->GetJsonDetoken();
    if( body["name"].isNull() ){
        RetJsonMsg::SetRetMsg(-1,response,"缺少参数");
        return -1;
    }
    string name = tConvert.Json2String(body["name"]);

    if( detoken["name"].isNull() ||  detoken["level"].isNull()){
        RetJsonMsg::SetRetMsg(-1,response,"缺少令牌");
        return -1;
    }else{
        int level = tConvert.Json2Int(detoken["level"]);
        if( level != 999){
            RetJsonMsg::SetRetMsg(-1,response,"权限不足");
            return -1;
        }
        if( level == 999 && (tConvert.Json2String(detoken["name"]) == name)){
            RetJsonMsg::SetRetMsg(-1,response,"管理员不能删除自己");
            return -1;
        }
    }



    if ( 0 != mUserDataDAL.Delete(name,"name")) {
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
int UserDataBLL::Add( EvhtpSvr * evhtpSvr){
    const Json::Value & body = evhtpSvr->GetJsonBody();
    Json::Value & response = evhtpSvr->GetJsonResponse();
    const Json::Value & detoken = evhtpSvr->GetJsonDetoken();
    if( body["name"].isNull() || body["level"].isNull() || body["department"].isNull()
            || body["password"].isNull()){
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

    if ( 0 == mUserDataDAL.IsExist( tConvert.Json2String( body["name"]),"name" ) ){
        RetJsonMsg::SetRetMsg(-1,response,"用户已存在");
        return -1;
    }
    UserDataEntity entity;
    entity.name = tConvert.Json2String( body["name"]);
    entity.department = tConvert.Json2String( body["department"]);
    entity.level = tConvert.Json2Int(body["level"]);
    entity.password = tConvert.Json2String( body["password"]);

    if ( 0 != mUserDataDAL.Insert(entity)) {
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
int UserDataBLL::Update( EvhtpSvr * evhtpSvr)
{
    const Json::Value & body = evhtpSvr->GetJsonBody();
    Json::Value & response = evhtpSvr->GetJsonResponse();
    const Json::Value & detoken = evhtpSvr->GetJsonDetoken();
    if( body["name"].isNull() ){
        RetJsonMsg::SetRetMsg(-1,response,"缺少参数");
        return -1;
    }
    TConvert tConvert;
    string name = tConvert.Json2String(body["name"]);

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

    if ( 0 != mUserDataDAL.IsExist( tConvert.Json2String( body["name"]),"name" ) ){
        RetJsonMsg::SetRetMsg(-1,response,"用户不存在");
        return -1;
    }

    UserDataEntity entity;

    if(!body["department"].isNull())
        entity.department = tConvert.Json2String(body["department"]);
    if(!body["level"].isNull())
        entity.level = tConvert.Json2Int(body["level"]);


    if ( 0 != mUserDataDAL.Update(name,entity,"name")) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Update error");
        RetJsonMsg::SetRetMsg(-4,response);
        return -1;
    }

    RetJsonMsg::SetRetMsg(0,response);
    return 0;
}


/*
修改密码
@param: evhtpSvr-
Return: 0 -success
*/
int UserDataBLL::ModifyPwd( EvhtpSvr * evhtpSvr)
{
    const Json::Value & body = evhtpSvr->GetJsonBody();
    Json::Value & response = evhtpSvr->GetJsonResponse();
    if( body["name"].isNull() || body["password"].isNull() || body["orgPwd"].isNull() ){
        RetJsonMsg::SetRetMsg(-1,response,"缺少参数");
        return -1;
    }
    TConvert tConvert;
    UserDataEntity entity;


    string orgPwd  = tConvert.Json2String(body["orgPwd"]);
    string name  = tConvert.Json2String(body["name"]);
    entity.password = tConvert.Json2String(body["password"]);


    UserDataEntity entity2;
    mUserDataDAL.GetEntity(name,entity2,"name");
    if( entity2.password != orgPwd){
        RetJsonMsg::SetRetMsg(-1,response,"原密码错误");
        return -1;
    }


    if ( 0 != mUserDataDAL.Update(name,entity,"name")) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Update error");
        RetJsonMsg::SetRetMsg(-4,response);
        return -1;
    }

    RetJsonMsg::SetRetMsg(0,response);
    return 0;

}


/*
用户信息
@param: evhtpSvr-
Return: 0 -success
*/
int UserDataBLL::GetUserInfo( EvhtpSvr * evhtpSvr)
{

    const Json::Value & pJson = evhtpSvr->GetJsonBody();
    Json::Value & pResponse = evhtpSvr->GetJsonResponse();
    if(  pJson["name"].isNull()){
        RetJsonMsg::SetRetMsg(-1,pResponse,"缺少参数");
        return -1;
    }
    TConvert tConvert;
    int iRet = -1;

    string sAccount =  tConvert.Json2String(pJson["name"]);



    //获取用户内容
    UserDataEntity entity;
    iRet = this->mUserDataDAL.GetEntity(sAccount,entity,"name");
    if ( 0 != iRet ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::GetUserInfo mUserDataDAL.GetEntity error");
        RetJsonMsg::SetRetMsg(iRet ,pResponse);

        if( iRet == -5){
            RetJsonMsg::SetRetMsg(iRet ,pResponse,"用户不存在");
        }
        return -1;
    }
    Json::Value result;
    result["name"] = entity.name;
    result["department"] = entity.department;
    result["level"] = entity.level;
    pResponse[RESPONSE_RESULT] = result;


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
所有用户信息
@param: evhtpSvr-
Return: 0 -success
*/
int UserDataBLL::GetUserList( EvhtpSvr * evhtpSvr)
{
    Json::Value & pResponse = evhtpSvr->GetJsonResponse();
    UserDataList lst;
    mUserDataDAL.GetList(lst);
    Json::Value result;int i = 0;
    result.resize(0);
    for(auto e : lst){

        result[i]["name"] = e.name;
        result[i]["department"] = e.department;
        result[i]["level"] = e.level;
        i++;
    }
    pResponse[RESPONSE_RESULT] = result;
    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;

}




