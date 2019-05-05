


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
    //删除
    int Delete( const string& psPrimaryKey );
    //修改
    int Update( const string& psPrimaryKey,const UserDataEntity& pEntity );



    //记录是否存在-主键
    int IsExist( const string& psPrimaryKey, bool& pbIsBool );


    //查询一行数据
    //ret : -5 -数据不存在  0 -成功
    int GetEntity(const string& psAccount, UserDataEntity& pEntity);



private:
    int ExcutEntity(const string  & sQql, UserDataEntity& pEntity);

    string msAll;

};

#endif // UserDataDAL_H
