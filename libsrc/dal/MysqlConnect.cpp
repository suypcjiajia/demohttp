#include "MysqlConnect.h"
#include "TMultiMysqlDAL.h"
#include "log/Logger.h"

MysqlConnect* MysqlConnect::mInstance = NULL;
MysqlConnect* MysqlConnect::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new MysqlConnect;
    }
    return mInstance;
}
//初始化
int MysqlConnect::Init(const string& psDataBaseName)
{
    ScopeLock lock( this->mMutex );
    this->msDataBaseName = psDataBaseName;
    return 0;
}
//创建多连接，一条线程关联上一条mysql连接
TMultiMysqlDAL *  MysqlConnect::GetConnect(long pThreadId){
    ScopeLock lock( this->mMutex );
    TMultiMysqlDAL * p = mMapConnect[pThreadId] ;
    if( p == NULL){
        p = new TMultiMysqlDAL();
        if( p != NULL){
            if( 0 == p->ConnectDB(this->msDataBaseName)){
                mMapConnect[pThreadId]  = p;
                p->Query("set names utf8mb4");//支持聊天符号表情
            }else{
                delete p;
                p = NULL;
            }
        }
    }
    return p;
}



