
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
    msAllColumn = "select account,nickName,headImage,sex,password";
    msAll = " account,nickName,headImage,sex,password ";
}

//添加
int UserDataDAL::Insert( UserDataEntity& pEntity  )
{
    TConvert tConvert;

    TMultiMysqlDAL tMultiMysqlDAL;
    string sSql;
    // SQL
    sSql = "Insert into "+this->msTableName+"("  +   msAll +   ")" +
            "VALUES( '"+pEntity.sAccount+"','" +
            pEntity.sNickName+"','" +
            pEntity.sHeadImage+"','" +
            tConvert.IntToStr( pEntity.iSex) + "','" +
            pEntity.sPassword+ "')";
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}
//删除
int UserDataDAL::Delete( const string& psPrimaryKey )
{
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    TMultiMysqlDAL tMultiMysqlDAL;

    // SQL
    string sSql = "delete from "+this->msTableName;
    sSql += " where account = '"+tMultiMysqlDAL.RealEscapeString(psPrimaryKey)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }

    return con->Query(sSql);
}
//修改
int UserDataDAL::Update(  const string& sPrimaryKey,const UserDataEntity& entity )
{
    TMultiMysqlDAL tMultiMysqlDAL;
    TConvert tConvert;
    long lCurrentTime = tConvert.GetCurrentTime();
    // SQL
    string sSql = "update "+this->msTableName+" set ";

    if ( entity.sAccount != "-1nochange" ) {
        sSql += ",account = '" + tMultiMysqlDAL.RealEscapeString(entity.sAccount) + "' ";
    }
    if ( entity.sPassword != "-1nochange" ) {
        sSql += ",password = '" + tMultiMysqlDAL.RealEscapeString(entity.sPassword) + "' ";
    }
    if ( entity.sNickName != "-1nochange" ) {
        sSql += ",nickName = '" + tMultiMysqlDAL.RealEscapeString(entity.sNickName) + "' ";
    }
    if ( entity.iSex != -1 ) {
        sSql += ",sex = '" + tConvert.IntToStr(entity.iSex) + "' ";
    }

    sSql += "where account = '"+tMultiMysqlDAL.RealEscapeString(sPrimaryKey)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    return con->Query(sSql);
}

//记录是否存在
int UserDataDAL::IsExist( const string & psPrimaryKey, bool& pbIsBool )
{
    pbIsBool = false;
    TMultiMysqlDAL tMultiMysqlDAL;

    // SQL
    string sSql = "select 1 from "+this->msTableName+" where account='"+tMultiMysqlDAL.RealEscapeString(psPrimaryKey)+"' limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        pbIsBool = true;
    }
    // 释放内存
    con->FreeResult( result );

    return 0;
}

//查询一行数据
//ret : -5 -数据不存在  0 -成功
int UserDataDAL::GetEntity(const string& psAccount, UserDataEntity& pEntity)
{
    TMultiMysqlDAL tMultiMysqlDAL;
    // SQL
    string sSql = "select " + msAll +
                  " from " + this->msTableName + " ";
    sSql += " where ";


   sSql += "  account = '" + tMultiMysqlDAL.RealEscapeString(psAccount) + "' ";



    return ExcutEntity(sSql,pEntity);

}




int UserDataDAL::ExcutEntity(const string  & sSql, UserDataEntity& pEntity){

    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result ) {
        iRet = -5;
        MYSQL_ROW row = NULL;
        if ( NULL != (row = mysql_fetch_row( result ))) {
            iRet = 0;


            int iIndex = 0;
            pEntity.sAccount =row[iIndex++];
            pEntity.sNickName = row[iIndex++];


            pEntity.sHeadImage = row[iIndex++];
            pEntity.iSex = atoi( row[iIndex++] );
            pEntity.sPassword = row[iIndex++];




        }
    }
    // 释放内存
    con->FreeResult( result );

    return iRet;

}


