/*
 *Description:封装用户业务接口
 *Others:
*/

#ifndef UserDataBLL_H
#define UserDataBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "TypeDef.h"
#include "dal/UserDataDAL.h"
#include "dal/SettingDAL.h"
#include "com/EvhtpSvr.h"
#include "BaseBLL.h"
using namespace std;



class UserDataBLL:public BaseBLL
{
public:

    int Process(EvhtpSvr * pEvhtp);


    /*use login
    @param: evhtpSvr-
    Return: 0 -success
    */
    int Login(EvhtpSvr * evhtpSvr);




    /*删除
    @param: evhtpSvr-
    Return: 0 -success
    */
    int Delete( EvhtpSvr * evhtpSvr);

    /*
    add
    @param: evhtpSvr-
    Return: 0 -success
    */
    int Add( EvhtpSvr * evhtpSvr);


    /*
    修改普通资料
    @param: evhtpSvr-
    Return: 0 -success
    */
    int Update( EvhtpSvr * evhtpSvr);

    /*
    修改密码
    @param: evhtpSvr-
    Return: 0 -success
    */
    int ModifyPwd( EvhtpSvr * evhtpSvr);


    /*
    用户信息
    @param: evhtpSvr-
    Return: 0 -success
    */
    int GetUserInfo( EvhtpSvr * evhtpSvr);

    /*
    所有用户信息
    @param: evhtpSvr-
    Return: 0 -success
    */
    int GetUserList( EvhtpSvr * evhtpSvr);



public:
    UserDataDAL mUserDataDAL;

};

#endif // UserDataBLL_H
