#include "MysqlDAL.h"
#include "TLog.h"

MysqlDAL* MysqlDAL::mInstance = NULL;
MysqlDAL* MysqlDAL::GetInstance()
{
        if (mInstance == NULL){
                mInstance = new MysqlDAL;
        }
        return mInstance;
}


MysqlDAL::MysqlDAL()
{
    this->mConnected = NULL;
    this->msUserDB = "";
    this->miOperationTimes = 0;
}
MysqlDAL::~MysqlDAL()
{
    this->CloseDB();
}

// 数据库
int MysqlDAL::StartDB(const string& sDatebase)
{
    //1.连接数据库-没有时会自动创建
    if ( -1 == this->ConnectDB() ) {
        return -1;
    }
    //2.使用数据库
    if ( -1 == this->UserDB(sDatebase) ) {
        return -1;
    }
    //3.启动PING长连接
    if ( -1 == this->StartPingDB() ) {
        return -1;
    }
    return 0;
}
// 数据库-连接数据库
int MysqlDAL::ConnectDB(const string& sDatebase)
{
    //1.连接数据库-没有时会自动创建
    if ( -1 == this->ConnectDB() ) {
        return -1;
    }
    //2.使用数据库
    if ( -1 == this->UserDB(sDatebase) ) {
        return -1;
    }
    return 0;
}
// 数据库-连接数据库
int MysqlDAL::ConnectDB()
{
    ScopeLock lock( this->mMutex );
    try {
        if ( this->mConnected ) {return true;}
        // 连接初始化
        if ( ! mysql_init(&this->mMysql) ) {
            return -1;
        }
        this->mConnected = mysql_real_connect( &this->mMysql,
                                               "127.0.0.1",
                                               "root",
                                               "tym123456",
                                               "mysql",
                                               3306,
                                               NULL,
                                               0);
        if ( ! this->mConnected ) {
            return -1;
        }
    } catch (...){
        tLog( LOG_ERROR, "%s::%s_%s","TMysqlDAL","ConnectDB","CallError" );
        return -1;
    };
    return 0;
}
// 数据库-激活连接线程
void* MysqlDAL::ActivationLinkDBThread(void* ptr)
{
    pthread_detach(pthread_self());

    MysqlDAL *me = (MysqlDAL*)ptr;
    while ( true ) {
        sleep( 5 );
        try {
            me->Ping();
        } catch (...){}
    }

    pthread_exit(NULL);
}
// 数据库-关闭数据库
int MysqlDAL::CloseDB()
{
    ScopeLock lock( this->mMutex );
    if ( this->mConnected != NULL ) {
        try {
            mysql_close( &this->mMysql );
        } catch (...){
            tLog( LOG_ERROR, "%s::%s_%s","TMysqlDAL","CloseDB","CallError" );
            return -1;
        };
        this->mConnected = NULL;
    }
    return 0;
}
// 启动PING长连接
int MysqlDAL::StartPingDB()
{
    // 数据库-激活连接线程
    pthread_t tid1;
    pthread_create( &tid1, NULL, ActivationLinkDBThread, (void*)this );
    return 0;
}
//使用数据库
int MysqlDAL::UserDB( const string& sDatebase )
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

// 数据库-激活连接
int MysqlDAL::Ping()
{    
    if (this->miOperationTimes > 0) {
        this->miOperationTimes = 0;
        return 0;
    }
    try {
        if ( 0 != mysql_ping( &this->mMysql ) ) {
            this->CloseDB();
            if (0 == this->ConnectDB() ) {
                //自动重使用
                if (! this->msUserDB.empty()) {
                    this->UserDB(this->msUserDB);
                }
            }

            tLog( LOG_ERROR, "%s::%s_%s","TMysqlDAL","Ping",mysql_error( &this->mMysql ) );
            return -1;
        }
    } catch (...){
        this->CloseDB();
        if (0 == this->ConnectDB() ) {
            //自动重使用
            if (! this->msUserDB.empty()) {
                this->UserDB(this->msUserDB);
            }
        }
        tLog( LOG_ERROR, "%s::%s_%s","TMysqlDAL","Ping","CallError" );
        return -1;
    };
    return 0;
}
//创建数据库
int MysqlDAL::CreateDatabase(const string& sDatebase)
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
int MysqlDAL::Query( const string& sSql )
{
    ScopeLock lock( this->mMutex );
    //usleep(10);
    if ( NULL == this->mConnected ) {return -1;}
    ++this->miOperationTimes;
    try {
        if ( 0 != mysql_query( &this->mMysql, sSql.c_str() ) ) {
            tLog( LOG_ERROR, "%s\n%s",sSql.c_str(),mysql_error( &this->mMysql ) );
            return -1;
        }
    } catch (...){
        tLog( LOG_ERROR, "%s::%s_%s","TMysqlDAL","Query","CallError" );
        return -1;
    };

    return 0;
}
// 数据库-返回查询内容
MYSQL_RES* MysqlDAL::QueryResult( const string& sSql )
{
    ScopeLock lock( this->mMutex );
    //usleep(10);
    if ( NULL == this->mConnected ) {return NULL;}
    ++this->miOperationTimes;
    try {
        if ( 0 != mysql_query( &this->mMysql, sSql.c_str() ) ) {
            tLog( LOG_ERROR, "TMysqlDAL::QueryResult _%s_%s",sSql.c_str(),mysql_error( &this->mMysql ) );
        } else {
            return mysql_store_result( &this->mMysql );
        }
    } catch (...){
        tLog( LOG_ERROR, "TMysqlDAL::QueryResult _%s","CallError" );
        return NULL;
    };
    return NULL;
}
// 释放内存
void MysqlDAL::FreeResult( MYSQL_RES* pResult )
{
    mysql_free_result( pResult );
}
//二进制转字符串
char* MysqlDAL::BinaryToString(char* pBinaryData,long lLen)
{
    //二进制转字符串
    char sBinaryData[1024*64]={0};//64K
    mysql_real_escape_string(&this->mMysql,sBinaryData,pBinaryData,lLen);
    return sBinaryData;
}
