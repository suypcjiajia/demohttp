

#include "Basedal.h"

BaseDal::BaseDal()
{

}

int BaseDal::BaseQuery(const string& sSql)
{
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    return con->Query(sSql);
}

int BaseDal::BatchBaseQuery(const StringLst batchSql)
{
    int iIndex = 0;
    int iRet = -1;
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    while( iIndex < batchSql.size()){
        iRet =  con->Query(batchSql[iIndex++]);
        if(iRet != 0){
            break;
        }
    }
    return iRet;

}

int BaseDal::BaseQueryResult(const string & sSql,MYSQL_RES ** res)
{
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    *res =  con->QueryResult(sSql);
    return 0;
}

void BaseDal::FreeResult(MYSQL_RES * res)
{
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return;
    }
    if( res != NULL)
        con->FreeResult(res);


}

int BaseDal::AutoCommit(bool bOpen)
{
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){

        return -1;
    }

    con->AutoCommit(bOpen);
    return 0;
}

int BaseDal::RollBack()
{
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){

        return -1;
    }

    con->RollBack();
    return 0;

}

int BaseDal::Commit()
{
    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){

        return -1;
    }

    con->Commit();
    return 0;
}

int BaseDal::GetQueryTotalCount(const string & psQuery, int & piCount)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }
    result =  con->QueryResult(psQuery);

    MYSQL_ROW row = NULL;

    if( result != NULL){
        iRet = 0;
        piCount = 0;
        if ( NULL != (row = mysql_fetch_row(result) )){
            piCount = atoi(row[0]);
        }
    }

    con->FreeResult(result);
    return iRet;
}


int BaseDal::GetCount(const string & sWhere, int & iCount)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }

    string sSql = "select count(*) from " + msTableName + " " + sWhere;
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){
        iRet = 0;
        iCount = 0;
        if ( NULL != (row = mysql_fetch_row(result) )){
            iCount = atoi(row[0]);
        }
    }
    return iRet;

}

int BaseDal::GetCountAterGroup(const string & sWhere, int & iCount)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }

    string sSql = "select count(*) from (select count(*) from " + msTableName + " " + sWhere +")t";
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){
        iRet = 0;
        iCount = 0;
        if ( NULL != (row = mysql_fetch_row(result) )){
            iCount = atoi(row[0]);
        }
    }
    return iRet;

}

int BaseDal::GetSum(const string & column ,const string & sWhere, int & iCount)
{
    int iRet = -1;
    MYSQL_RES* result;
    TMultiMysqlDAL * con = NULL;
    con =  MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -22;
    }

    string sSql = "select sum(" + column + ") from " + msTableName + " " + sWhere;
    result =  con->QueryResult(sSql);

    MYSQL_ROW row = NULL;

    if( result != NULL){
        iRet = 0;
        iCount = 0;
        if ( NULL != (row = mysql_fetch_row(result) )){

            iCount = 0;
            if(row[0] != NULL)
                iCount = atoi(row[0]);
        }
    }
    return iRet;

}


int BaseDal::IsExist(const string & sWhere)
{

    // SQL
    string sSql = "select 1 from "+ msTableName + " " +  sWhere +" limit 1;";

    MYSQL_RES* result;

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }
    result =  con->QueryResult(sSql);

    if ( NULL != result && result->row_count > 0) {
        return 0;
    }
    // 释放内存
    con->FreeResult( result );

    return -1;

}

int BaseDal::ExcutEntity(const string  & sSql, BaseEntity& pEntity){

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
        if ( NULL != (row = mysql_fetch_row( result ))) {
            ret = 0;
            pEntity.Row2Entity(row);
        }
    }
    // 释放内存
    con->FreeResult( result );

    return ret;

}


//删除
int BaseDal::Delete( const string value,const string& key )
{
    string sSql = "delete from "+this->msTableName;
    string id = "id";
    if( !key.empty()){
        id = key;
    }
    sSql += " where "  + id + "='"+RES_S(value)+"'";

    TMultiMysqlDAL * con = MysqlConnect::GetInstance()->GetConnect(pthread_self());
    if( con == NULL){
        return -1;
    }

    return con->Query(sSql);
}

//查询一行数据
//ret : -5 -数据不存在  0 -成功
int BaseDal::GetEntity(const string& value, BaseEntity& pEntity,const string & key)
{
    string id = "id";
    if(!key.empty()){
        id = key;
    }
    string sSql = "select " + msAllColumn +
                  " from " + this->msTableName + " ";
    sSql += " where ";

   sSql += id +  "='" + RES_S(value) + "'";

    return ExcutEntity(sSql,pEntity);

}

int BaseDal::IsExist(const string & value,const string & key)
{
    string id = "id";
    if(!key.empty()){
        id = key;
    }
    string where = "where "  + id + "=" + value;
    return BaseDal::IsExist(where);
}
