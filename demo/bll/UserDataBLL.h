#ifndef UserDataBLL_H
#define UserDataBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "TypeDef.h"
#include "dal/UserDataDAL.h"
#include "com/EvhtpSvr.h"
using namespace std;


/*
 *Copyright(C),TTD
 *Author: suyp
 *Description:封装用户业务接口
 *Others:
*/
class UserDataBLL
{
public:
    /*
    Description:初始化
    Input: null
    Output: null
    Return: 0 -success
    */
    int Init();




    /*
    添加
    @param: evhtpSvr-
    Return: 0 -success
    */
    int AddData( EvhtpSvr * evhtpSvr);





    /*删除
    @param: evhtpSvr-
    Return: 0 -success
    */
    int Delete( EvhtpSvr * evhtpSvr);

    /*
    修改普通资料
    @param: evhtpSvr-
    Return: 0 -success
    */
    int Update( EvhtpSvr * evhtpSvr);





    /*
    用户信息
    @param: evhtpSvr-
    Return: 0 -success
    */
    int GetUserInfo( EvhtpSvr * evhtpSvr);



public:
    UserDataDAL mUserDataDAL;




};

#endif // UserDataBLL_H
