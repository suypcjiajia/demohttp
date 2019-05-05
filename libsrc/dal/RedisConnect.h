#ifndef REDISCONNECT_H
#define REDISCONNECT_H
#include <string>
#include <map>
#include <TMutex.hpp>
#include "dal/RedisCli.h"
using namespace std;

/*
Description: 管理多个Redis连接客户端。（单件模式）
*/
class RedisConnect
{
public:
    static RedisConnect * GetInstance();
    void SetAddress(string addr);
    int Hset(string key,string field,string value);
    int Hget(string key,string field,string & vlaue);
private:
    //创建多连接，一条线程关联上一条redis连接
    RedisCli *  GetConnect(long pThreadId);
private:
    RedisConnect();
    static RedisConnect * mInstance;
    map<long, RedisCli *> mMapCli;
    TMutex mMutex;//线程锁
    string mAddr;

};


#endif // REDISCONNECT_H
