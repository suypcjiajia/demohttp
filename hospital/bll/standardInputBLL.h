/*
 *Description:标准录入业务接口
 *Others:
*/

#ifndef STANDARDINPUTBLL_H
#define STANDARDINPUTBLL_H

#include <string>
#include <vector>
#include "TMutex.hpp"
#include <tr1/unordered_map>
#include"json/json.h"
#include "TypeDef.h"
#include "dal/standardInputDAL.h"
#include "com/EvhtpSvr.h"
#include "BaseBLL.h"
using namespace std;



class standardInputBLL:public BaseBLL
{
public:
    standardInputBLL();

    int Process(EvhtpSvr * pEvhtp);
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
    所有信息
    @param: evhtpSvr-
    Return: 0 -success
    */
    int GetList( EvhtpSvr * evhtpSvr);



public:
    standardInputDAL mDal;

};

#endif // STANDARDINPUTBLL_H
