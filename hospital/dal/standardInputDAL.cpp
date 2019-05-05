
#include "standardInputDAL.h"
#include "dal/MysqlConnect.h"
#include "log/Logger.h"

#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "TConvert.h"
#include "TFile.h"

#include "dal/TMultiMysqlDAL.h"

#include "TypeDef.h"



standardInputDAL::standardInputDAL(){
    msTableName = "standardInput";
    msAllColumn = "id,type,value0,value1,value2,value3,value4,value5";
}

//添加
int standardInputDAL::Insert( standardInputEntity& pEntity  )
{
    string sSql;
    // SQL
    sSql = "Insert into "+this->msTableName+"(type,value0,value1,value2,value3,value4,value5)" +
            "VALUES( '"+ mtConvert.IntToStr( pEntity.type)+"','" +
            RES_S( pEntity.value0)+"','" +
            RES_S( pEntity.value1)+"','" +
            RES_S( pEntity.value2)+"','" +
            RES_S( pEntity.value3)+"','" +
            RES_S( pEntity.value4)+"','" +
            RES_S( pEntity.value5) +  "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

//修改
int standardInputDAL::Update( const string& value,const standardInputEntity& entity,const string & key  )
{
    string sSql = "update "+this->msTableName+" set ";
    string column;
    string id = "id";
    if( !key.empty()){
        id = key;
    }

    if ( !entity.value0.empty() ) {
        column += "value0 = '" + RES_S(entity.value0) + "', ";
    }
    if ( !entity.value1.empty() ) {
        column += "value1 = '" + RES_S(entity.value1) + "', ";
    }
    if ( !entity.value2.empty() ) {
        column += "value2 = '" + RES_S(entity.value2) + "', ";
    }
    if ( !entity.value3.empty() ) {
        column += "value0 = '" + RES_S(entity.value0) + "', ";
    }
    if ( !entity.value3.empty() ) {
        column += "value3 = '" + RES_S(entity.value3) + "', ";
    }
    if ( !entity.value4.empty() ) {
        column += "value4 = '" + RES_S(entity.value4) + "', ";
    }
    if ( !entity.value5.empty() ) {
        column += "value5 = '" + RES_S(entity.value5) + "', ";
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
int standardInputDAL::GetList(standardInputList& lst)
{
    return GetListByWhere("",lst);
}

//查询many行数据
//ret :   0 -成功
int standardInputDAL::GetListByWhere(const string & where,standardInputList& lst){
    string sSql = "select " + msAllColumn +
                  " from " + this->msTableName + " " + where;
    return ExcutList(sSql,lst);
}

int standardInputDAL::ExcutList(const string  & sSql,standardInputList& lst){
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
            standardInputEntity pEntity;
            pEntity.Row2Entity(row);
            lst.push_back(pEntity);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return ret;
}


