#ifndef SETTINGDAL_H
#define SETTINGDAL_H

#include <string>
#include <vector>
#include "SettingEntity.h"
#include "TypeDef.h"
#include "dal/Basedal.h"
using namespace std;



class SettingDAL:public BaseDal
{

public:
    SettingDAL();
    //增加
    int Insert( SettingEntity& pEntity );

    //修改
    int Update( const string& value,const SettingEntity& entity,const string & key = "" );


    //查询many行数据
    //ret :   0 -成功
    int GetList(SettingList& lst);

    //查询many行数据
    //ret :   0 -成功
    int GetListByWhere(const string & where,SettingList& lst);


private:
    int ExcutList(const string  & sQql,SettingList& lst);

};

#endif // SETTINGDAL_H
