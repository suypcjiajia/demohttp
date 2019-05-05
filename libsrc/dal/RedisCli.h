#ifndef REDISCLI_H
#define REDISCLI_H

#include <hircluster.h>
#include <adapters/libevent.h>
#include <string>
using namespace std;
class RedisCli
{
public:
    RedisCli();
    ~RedisCli();
    //设置Redis集群地址
    void SetAddrs(string sAddrs);


    //异步调用，执行命令
    int AsyncCommand(char *command);


    //同步调用，执行命令
    int SyncCommand(const char *command, char * output);


    int HsetSync(string key,string field, string value);

    int HgetSync(string key,string field,char * output);

    static void redisAsyncCommandCallback(redisClusterAsyncContext *c, void *r, void *privdata);
    static void redisAsyncConnectCallback(const redisAsyncContext *c, int status);
    static void redisAsyncDisconnectCallback(const redisAsyncContext *c, int status);
private:
    int redis_cluster_connect();
    int redis_cluster_async_connect();


private:
    string msAddrs;
    redisClusterAsyncContext *  mAsyncContext = NULL;
    redisClusterContext * mSyncContext = NULL;
    struct event_base * mEventBase = NULL;
};

#endif // REDISCLI_H
