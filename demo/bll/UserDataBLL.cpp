/*
 *Copyright(C),TTD
 *Author: suyp
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


/*
Description:初始化
Input: null
Output: null
Return: 0 -success
*/
int UserDataBLL::Init()
{

    return 0;
}

/*
添加
@param: evhtpSvr-
Return: 0 -success
*/
int UserDataBLL::AddData( EvhtpSvr * evhtpSvr )
{
    int iRet = -1;
    const Json::Value & pJson = evhtpSvr->GetJsonBody();
    Json::Value & pResponse = evhtpSvr->GetJsonResponse();

    if(pJson["account"].isNull() || pJson["password"].isNull() ||  pJson["nickName"].isNull() || pJson["sex"].isNull()
            || pJson["headImage"].isNull()
            ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -15;
    }
    TConvert tConvert;


    UserDataEntity entity;


    entity.sAccount = tConvert.Json2String(pJson["account"]);
    entity.sPassword = Md5ToLower( "T" + MyUtil::MakeRandNum(6) );
    entity.sNickName = tConvert.Json2String(pJson["nickName"]);
    entity.iSex = tConvert.Json2Int(pJson["sex"]);
    entity.sHeadImage = tConvert.Json2String(pJson["headImage"]);


    if( !MyUtil::IsRightPhone(entity.sAccount))
    {
        RetJsonMsg::SetRetMsg(-1,pResponse,"手机号不合法");
        return -1;
    }




    UserDataEntity uTmp;




    //帐号是否存在
    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExist( tConvert.Json2String(pJson["account"]),bIsExist) ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::AddData mUserDataDAL.IsExist error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    iRet = this->mUserDataDAL.Insert(entity);
    string tmp = pResponse.toStyledString();

    RetJsonMsg::SetRetMsg(iRet,pResponse);
    string tmp2 = pResponse.toStyledString();
    return 0;
}

/*删除
@param: evhtpSvr-
Return: 0 -success
*/
int UserDataBLL::Delete( EvhtpSvr * evhtpSvr)
{
    const Json::Value & pJson = evhtpSvr->GetJsonBody();
    Json::Value & pResponse = evhtpSvr->GetJsonResponse();
    if( pJson["account"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;


    //参数转换
    string account = tConvert.Json2String(pJson["account"]);

    //操作数据库
    if ( 0 != this->mUserDataDAL.Delete(account)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Delete  error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }

    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}

/*
修改普通资料
@param: evhtpSvr-
Return: 0 -success
*/
int UserDataBLL::Update( EvhtpSvr * evhtpSvr)
{
    const Json::Value & pJson = evhtpSvr->GetJsonBody();
    Json::Value & pResponse = evhtpSvr->GetJsonResponse();
    if( pJson["account"].isNull() ){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }


    TConvert tConvert;

    //是否存在
    bool bIsExist = false;
    if ( 0 != this->mUserDataDAL.IsExist( tConvert.Json2String( pJson["account"]), bIsExist ) ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Update IsExist error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }
    if (false == bIsExist) {
        RetJsonMsg::SetRetMsg(-5,pResponse);
        return -1;
    }


    UserDataEntity entity;

    entity.sAccount = tConvert.Json2String(pJson["account"]);

    if( pJson["sex"].isNull() ){
        entity.iSex = -1;
    }else{
        entity.iSex = tConvert.Json2Int(pJson["sex"]);
    }
    if( pJson["headImage"].isNull() ){
        entity.sHeadImage = "";
    }else{
        entity.sHeadImage = tConvert.Json2String(pJson["sHeadImage"]);
    }
    if( pJson["nickName"].isNull() ){
        entity.sNickName = "";
    }else{
        entity.sNickName = tConvert.Json2String(pJson["nickName"]);
    }


    if ( 0 != this->mUserDataDAL.Update(entity.sAccount,entity)) {
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::Update error");
        RetJsonMsg::SetRetMsg(-4,pResponse);
        return -1;
    }


    RetJsonMsg::SetRetMsg(0,pResponse);
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
    if(  pJson["account"].isNull()){
        RetJsonMsg::SetRetMsg(-15,pResponse);
        return -1;
    }
    TConvert tConvert;
    int iRet = -1;

    string sAccount =  tConvert.Json2String(pJson["account"]);



    //获取用户内容
    UserDataEntity entity;
    iRet = this->mUserDataDAL.GetEntity(sAccount,entity);
    if ( 0 != iRet ){
        appendlog(TTDLogger::LOG_ERROR,"UserDataBLL::GetUserInfo mUserDataDAL.GetEntity error");
        RetJsonMsg::SetRetMsg(iRet ,pResponse);

        if( iRet == -5){
            RetJsonMsg::SetRetMsg(iRet ,pResponse,"目标用户不存在");
        }
        return -1;
    }
    Json::Value result;
    result["account"] = entity.sAccount;
    result["nickName"] = entity.sNickName;
    result["headImage"] = entity.sHeadImage;
    result["sex"] = entity.iSex;
    pResponse[RESPONSE_RESULT] = result;


    RetJsonMsg::SetRetMsg(0,pResponse);
    return 0;
}




