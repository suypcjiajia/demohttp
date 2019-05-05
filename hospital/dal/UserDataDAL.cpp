
#include "UserDataDAL.h"
#include "dal/MysqlConnect.h"
#include "log/Logger.h"

#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"

#include "dal/TMultiMysqlDAL.h"

#include "TypeDef.h"



UserDataDAL::UserDataDAL(){
    msTableName = "user";
    msAllColumn = "id,name,department,password,level";
}

//添加
int UserDataDAL::Insert( UserDataEntity& pEntity  )
{
    string sSql;
    // SQL
    sSql = "Insert into "+this->msTableName+"(name,department,password,level)" +
            "VALUES( '"+  RES_S( pEntity.name)+"','" +
            RES_S(pEntity.department)+"','" +
            RES_S(pEntity.password)+"','" +
            mtConvert.IntToStr( pEntity.level) +  "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

//修改
int UserDataDAL::Update(  const string& value,const UserDataEntity& entity,const string & key )
{
    string sSql = "update "+this->msTableName+" set ";
    string column;
    string id = "id";
    if( !key.empty()){
        id = key;
    }

    if ( !entity.name.empty() ) {
        column += "name = '" + RES_S(entity.name) + "', ";
    }
    if ( !entity.password.empty() ) {
        column += "password = '" + RES_S(entity.password) + "', ";
    }
    if ( !entity.department.empty() ) {
        column += "department = '" + RES_S(entity.department) + "', ";
    }
    if ( entity.level != -1 ) {
        column += "level = '" + mtConvert.IntToStr(entity.level) + "', ";
    }
    if( !column.empty()){
        column = column.substr(0,column.length() -1);
    }

    sSql += "where " + id + "='"+ RES_S(value)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}



//查询many行数据
//ret :   0 -成功
int UserDataDAL::GetList(UserDataList& lst)
{
    string sSql = "select " + msAllColumn +
                  " from " + this->msTableName + " ";
    return ExcutList(sSql,lst);

}

int UserDataDAL::ExcutList(const string  & sSql,UserDataList& lst){
    int ret = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        ret = -5;
        MYSQL_ROW row = NULL;
        while ( NULL != (row = mysql_fetch_row( result ))) {
            ret = 0;
            UserDataEntity pEntity;
            pEntity.Row2Entity(row);
            lst.push_back(pEntity);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return ret;
}
