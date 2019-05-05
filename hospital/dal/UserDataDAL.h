


#ifndef UserDataDAL_H
#define UserDataDAL_H

#include <string>
#include <vector>
#include "UserDataEntity.h"
#include "TypeDef.h"
#include "dal/Basedal.h"
using namespace std;



class UserDataDAL:public BaseDal
{

public:
    UserDataDAL();
    //增加
    int Insert( UserDataEntity& pEntity );

    //修改
    int Update( const string& value,const UserDataEntity& pEntity,const string & key ="" );



    //查询many行数据
    //ret :   0 -成功
    int GetList(UserDataList& lst);




private:
    int ExcutList(const string  & sQql,UserDataList& lst);

};

#endif // UserDataDAL_H
