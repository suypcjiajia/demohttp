#ifndef RetJsonMsg_H
#define RetJsonMsg_H

#include"json/json.h"
#include "TConvert.h"

class RetJsonMsg
{
public:

    //错误ID 错误描述
    static void SetRetMsg(int piID, Json::Value & pRetJson,string msg = "")
    {
        if( piID == 0 ){
            pRetJson["state"] = piID;
            if( msg.empty()){
                pRetJson["msg"] = "成功调用";
            }else{
                pRetJson["msg"] = msg;
            }

        }else if( -1 == piID ){
            pRetJson["state"] = piID;

            if( msg.empty()){
                pRetJson["msg"] = "未知错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -2 == piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "数据错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -3 == piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "数据已经存在";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -4 ==piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "数据库访问错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -5 == piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "数据不存在";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -6 == piID ){
            pRetJson["state"] =  piID;


            if( msg.empty()){
                pRetJson["msg"] = "安全检查错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -7 == piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "POST参数错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -8 == piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "错误的数据操作";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -9 == piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "签名错误";
            }else{
                pRetJson["msg"] = msg;
            }

        }else if( -10 == piID ){
            pRetJson["state"] = piID;

            if( msg.empty()){
                pRetJson["msg"] = "方法错误";
            }else{
                pRetJson["msg"] = msg;
            }
        }else if( -11 == piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "JOSN格式错误";
            }else{
                pRetJson["msg"] = msg;
            }

        }else if( -12 == piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "数据不合法";
            }else{
                pRetJson["msg"] = msg;
            }

        }else if( -13 == piID ){
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "获取数据库连接时失败";
            }else{
                pRetJson["msg"] = msg;
            }

        }
        else{
            pRetJson["state"] =  piID;

            if( msg.empty()){
                pRetJson["msg"] = "未知异常";
            }else{
                pRetJson["msg"] = msg;
            }

        }
    }

};

#endif // RetJsonMsg_H
