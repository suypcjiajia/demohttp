#include "TMultiMysqlDAL.h"
#include "log/Logger.h"

 string TMultiMysqlDAL::mMysqlAddr = "127.0.0.1";
 string TMultiMysqlDAL::mMysqlUser = "root";
 string TMultiMysqlDAL::mMysqlPwd = "tym123456";



  void TMultiMysqlDAL::SetMysqlAddr(const string & psAddr){
      mMysqlAddr = psAddr;
 }

  void TMultiMysqlDAL::SetMysqlUser(const string & psUser){
      mMysqlUser = psUser;
  }

  void TMultiMysqlDAL::SetMysqlPwd(const string & psPwd){
      mMysqlPwd = psPwd;
  }

TMultiMysqlDAL::TMultiMysqlDAL()
{
    this->mConnected = NULL;
    this->msUserDB = "";
    mAddr = mMysqlAddr;
    mUser = mMysqlUser;
    mPwd = mMysqlPwd;
}

TMultiMysqlDAL::TMultiMysqlDAL(string sAddr, string sUser, string sPwd)
{
    this->mConnected = NULL;
    this->msUserDB = "";
    mAddr = sAddr;
    mUser = sUser;
    mPwd = sPwd;
}

TMultiMysqlDAL::~TMultiMysqlDAL()
{
    this->CloseDB();
}

// 数据库-连接数据库
int TMultiMysqlDAL::ConnectDB(const string& sDatebase)
{
    //1.连接数据库
    if ( -1 == this->ConnectDB() ) {
        return -1;
    }
    //2.使用数据库-没有时会自动创建
    if ( -1 == this->UserDB(sDatebase) ) {
        return -1;
    }

    return 0;
}
// 数据库-连接数据库
int TMultiMysqlDAL::ConnectDB()
{
    try {
        if ( NULL !=this->mConnected  )
        {
            return 0;
        }
        // 连接初始化
        if ( ! mysql_init(&this->mMysql) ) {
            return -1;
        }
        this->mConnected = mysql_real_connect( &this->mMysql,
                                               mAddr.c_str(),
                                                mUser.c_str(),
                                                mPwd.c_str(),
                                               "",
                                               3306,
                                               NULL,
                                               0);
        if ( ! this->mConnected ) {
           appendlog(TTDLogger:: LOG_ERROR, "TMultiMysql::ConnectDB error: %s",mysql_error(&mMysql));
            return -1;
        }

        int iTimeOut = 30;
        mysql_options(&mMysql, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&iTimeOut);
        mysql_options(&mMysql, MYSQL_OPT_READ_TIMEOUT, (const char *)&iTimeOut);
        mysql_options(&mMysql, MYSQL_OPT_WRITE_TIMEOUT, (const char *)&iTimeOut);
    } catch (...){
        appendlog(TTDLogger:: LOG_ERROR, "%s::%s_%s","TMysqlDAL","ConnectDB","CallError" );
        return -1;
    };
    return 0;
}

// 数据库-关闭数据库
int TMultiMysqlDAL::CloseDB()
{
    if ( this->mConnected != NULL ) {
        try {
            mysql_close( &this->mMysql );
        } catch (...){
            appendlog(TTDLogger:: LOG_ERROR, "%s::%s_%s","TMysqlDAL","CloseDB","CallError" );
            return -1;
        };
        this->mConnected = NULL;
    }
    return 0;
}

//使用数据库
int TMultiMysqlDAL::UserDB( const string& sDatebase )
{
    int iRet = -1;
    if ( NULL == this->mConnected ) {return iRet;}
    if ( 0 != mysql_select_db( &this->mMysql, sDatebase.c_str() ) ) {
        //没有时自动创建
        if ( 0 == this->CreateDatabase(sDatebase) ) {
            iRet = 0;//新建的
        }
    } else {
        iRet = 0;
    }
    if (-1 != iRet) {
        mysql_select_db( &this->mMysql, sDatebase.c_str() );
        mysql_set_character_set( &this->mMysql, "utf8");
        this->msUserDB = sDatebase;
    }
    return iRet;
}

//创建数据库
int TMultiMysqlDAL::CreateDatabase(const string& sDatebase)
{
    if ( NULL == this->mConnected ) {return -1;}
    //SQL
    string sSql = "create database if not exists "+sDatebase+" CHARACTER SET utf8 COLLATE utf8_general_ci";
    if ( 0 != this->Query( sSql ) ) {
        return -1;
    }
    return 0;
}
// 数据库-运行SQL语句
int TMultiMysqlDAL::Query( const string& sSql )
{
    int iRet = -1;
    if ( NULL == this->mConnected )
    {
        appendlog(TTDLogger:: LOG_ERROR, "TMultiMysqlDAL::Query mConnected=NULL" );
        //return iRet;
    }

    //连接报错后重试次数
    int iLoop = 1;
    bool bIsConnected = true;
    while(iLoop >= 0){
        try {
            if ( true == bIsConnected ) {
                if ( 0 != mysql_query( &this->mMysql, sSql.c_str() ) ) {
                    //重复记录
                    if (1062 == mysql_errno(&this->mMysql)) {
                        iRet = 1062;
                        break;
                    }
                    if (1452 == mysql_errno(&this->mMysql)) {
                        iRet = 1452;
                        break;
                    }
                    appendlog(TTDLogger:: LOG_ERROR, "TMultiMysqlDAL::Query=%s,mysql_errno=%d,mysql_error=%s",sSql.c_str(),mysql_errno(&this->mMysql),mysql_error( &this->mMysql ) );
                    if (0 == iLoop) {
                        break;
                    }
                }else{
                    iRet = 0;
                    break;
                }
            }
        } catch (...){
            appendlog(TTDLogger:: LOG_ERROR, "catch TMultiMysqlDAL::Query=%s,mysql_errno=%d,mysql_error=%s",sSql.c_str(),mysql_errno(&this->mMysql),mysql_error( &this->mMysql ) );

        };

        //重连数据库
        bIsConnected = false;
        if(0 == ReConnectDB()){
           bIsConnected = true;
        }

        iLoop--;
    }

    return iRet;
}
// 数据库-返回查询内容
MYSQL_RES* TMultiMysqlDAL::QueryResult( const string& sSql )
{
    int iRet = -1;
    if ( NULL == this->mConnected )
    {
        appendlog(TTDLogger:: LOG_ERROR, "TMultiMysqlDAL::QueryResult mConnected=NULL" );
        //return NULL;
    }

    //连接报错后重试次数
    int iLoop = 1;
    bool bIsConnected = true;
    while(iLoop >= 0){
        try {
            if ( true == bIsConnected ) {
                if ( 0 != mysql_query( &this->mMysql, sSql.c_str() ) ) {
                    //重复记录
                    if (1062 == mysql_errno(&this->mMysql)) {
                        iRet = 1062;
                        break;
                    }
                    appendlog(TTDLogger:: LOG_ERROR, "TMultiMysqlDAL::Query=%s,mysql_errno=%d,mysql_error=%s",sSql.c_str(),mysql_errno(&this->mMysql),mysql_error( &this->mMysql ) );

                    if (0 == iLoop) {
                        break;
                    }
                }else{
                    iRet = 0;
                    break;
                }
            }
        } catch (...){
            appendlog(TTDLogger:: LOG_ERROR, "catch TMultiMysqlDAL::Query=%s,mysql_errno=%d,mysql_error=%s",sSql.c_str(),mysql_errno(&this->mMysql),mysql_error( &this->mMysql ) );

        };

        //重连数据库
        bIsConnected = false;
        if(0 == ReConnectDB()){
           bIsConnected = true;
        }

        iLoop--;
    }
    if (iRet == 0) {
        return mysql_store_result( &this->mMysql );
    }
    return NULL;
}
// 释放内存
void TMultiMysqlDAL::FreeResult( MYSQL_RES* pResult )
{
    if( pResult != NULL)
        mysql_free_result( pResult );
}
//字符串特殊符号转义
string TMultiMysqlDAL::RealEscapeString(const string& psContent)
{
    char cRet[1024*64]={0};//64K
    mysql_escape_string(cRet,psContent.c_str(),psContent.length());
    return cRet;
}
//二进制转字符串
char* TMultiMysqlDAL::BinaryToString(char* pBinaryData,long lLen)
{
    //二进制转字符串
    char sBinaryData[1024*64]={0};//64K
    mysql_escape_string(sBinaryData,pBinaryData,lLen);
    return sBinaryData;
}
//显示所有表
int TMultiMysqlDAL::ShowTables(vector<string>& pTablesVector)
{
    // SQL
    string sSql = "show tables";
    int iRet = -1;
    MYSQL_RES* result;
    result = this->QueryResult( sSql );
    if ( NULL != result ) {
        iRet = 0;
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row( result );
        if ( NULL != row ) {
            while( NULL != row ) {
                pTablesVector.push_back(row[0]);

                row = mysql_fetch_row( result );
            }
        }
    }
    // 释放内存
    this->FreeResult( result );

    return iRet;
}

// 重连数据库
int TMultiMysqlDAL::ReConnectDB(int iSleep)
{
    int iRet = -1;
    try {
        if ( 0 != mysql_ping( &this->mMysql ) ) {
            this->CloseDB();
            if( iSleep > 0 ){
                sleep(iSleep);
            }            
            if (0 == this->ConnectDB() ) {
                //自动重使用
                if (! this->msUserDB.empty()) {
                    if (0 == this->UserDB(this->msUserDB)){
                        iRet = 0;
                    }
                }
            }
        }
    } catch (...){
        this->CloseDB();
        if( iSleep > 0 ){
            sleep(iSleep);
        }
        if (0 == this->ConnectDB() ) {
            //自动重使用
            if (! this->msUserDB.empty()) {
                if (0 == this->UserDB(this->msUserDB)){
                    iRet = 0;
                }
            }
        }
    };
    return iRet;
}

//是否自动提交（事务）
int TMultiMysqlDAL::AutoCommit(bool bOpen)
{
    if(bOpen)
        return mysql_autocommit(&mMysql,1);
    else
       return mysql_autocommit(&mMysql,0);
}
//事务回滚
int TMultiMysqlDAL::RollBack()
{
    return mysql_rollback(&mMysql);
}
//事务提交
int TMultiMysqlDAL::Commit()
{
    return mysql_commit(&mMysql);
}
