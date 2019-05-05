
#include "SettingDAL.h"
#include "dal/MysqlConnect.h"
#include "log/Logger.h"

#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"

#include "dal/TMultiMysqlDAL.h"

#include "TypeDef.h"



SettingDAL::SettingDAL(){
    msTableName = "setting";
    msAllColumn = "id,name,type";
}

//添加
int SettingDAL::Insert( SettingEntity& pEntity  )
{
    string sSql;
    // SQL
    sSql = "Insert into "+this->msTableName+"(name,type)" +
            "VALUES( '"+  RES_S( pEntity.name)+"','" +
            mtConvert.IntToStr( pEntity.type) +  "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

//修改
int SettingDAL::Update( const string& value,const SettingEntity& entity,const string & key  )
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
    if ( entity.type != -1 ) {
        column += "type = '" + mtConvert.IntToStr(entity.type) + "', ";
    }
    if( !column.empty()){
        column = column.substr(0,column.length() -2);
    }
    sSql += column;

    sSql += "where "  + id + "='"+ RES_S(value)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}



//查询many行数据
//ret :   0 -成功
int SettingDAL::GetList(SettingList& lst)
{
    return GetListByWhere("",lst);
}

//查询many行数据
//ret :   0 -成功
int SettingDAL::GetListByWhere(const string & where,SettingList& lst){
    string sSql = "select " + msAllColumn +
                  " from " + this->msTableName + " " + where;
    return ExcutList(sSql,lst);
}

int SettingDAL::ExcutList(const string  & sSql,SettingList& lst){
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
            SettingEntity pEntity;
            pEntity.Row2Entity(row);
            lst.push_back(pEntity);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return ret;
}


