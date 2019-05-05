#ifndef STANDARDINPUTDAL_H
#define STANDARDINPUTDAL_H

#include <string>
#include <vector>
#include "standardInputEntity.h"
#include "TypeDef.h"
#include "dal/Basedal.h"
using namespace std;



class standardInputDAL:public BaseDal
{

public:
    standardInputDAL();
    //增加
    int Insert( standardInputEntity& pEntity );

    //修改
    int Update( const string& value,const standardInputEntity& entity,const string & key = "" );



    //查询many行数据
    //ret :   0 -成功
    int GetList(standardInputList& lst);

    //查询many行数据
    //ret :   0 -成功
    int GetListByWhere(const string & where,standardInputList& lst);



private:
    int ExcutList(const string  & sQql,standardInputList& lst);

};

#endif // STANDARDINPUTDAL_H
