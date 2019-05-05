#include "RedisConnect.h"


RedisConnect * RedisConnect::mInstance = NULL;


RedisConnect::RedisConnect()
{

}

RedisConnect* RedisConnect::GetInstance()
{
    if( mInstance == NULL)
        mInstance = new RedisConnect;
    return mInstance;
}

void RedisConnect::SetAddress(string addr)
{
    mAddr = addr;
}

int RedisConnect::Hset(string key,string field,string value)
{
    int iRet = -1;
    RedisCli * p = GetConnect(pthread_self());
    if( p != NULL){
        iRet = p->HsetSync(key,field,value);
    }
    return iRet;
}

int RedisConnect::Hget(string key,string field,string & vlaue)
{
    int iRet = -1;
    char buf[1024] = {0};
    RedisCli * p = GetConnect(pthread_self());
    if( p != NULL){
        iRet = p->HgetSync(key,field,buf);
    }
    vlaue = buf;
    return iRet;
}


//创建多连接，一条线程关联上一条redis连接
RedisCli *  RedisConnect::GetConnect(long pThreadId){
    ScopeLock lock( this->mMutex );
    RedisCli * p = mMapCli[pThreadId] ;
    if( p == NULL){
        p = new RedisCli();
        if( p != NULL){
            p->SetAddrs(mAddr);
            mMapCli[pThreadId]  = p;
        }
    }
    return p;
}
