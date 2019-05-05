#include <unistd.h>
#include "log/Logger.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RedisCli.h"

RedisCli::RedisCli()
{
    mEventBase = NULL;
    mEventBase = event_base_new();
}

RedisCli::~RedisCli()
{
    if( mEventBase != NULL)
        event_base_dispatch(mEventBase);
}

void RedisCli::SetAddrs(string sAddrs)
{
    msAddrs = sAddrs;
}


void RedisCli::redisAsyncCommandCallback(redisClusterAsyncContext *c, void *r, void *privdata)
{
    redisReply *reply = (redisReply *)r;

    if(NULL != reply)
    {
        appendlog(TTDLogger::LOG_DEBUG,"redisAsyncCommandCallback: %ld, %s\n", reply->integer, reply->str);
    }
    else
    {
        appendlog(TTDLogger::LOG_ERROR,"redisAsyncCommandCallback: reply is NULL\n");
    }

    //redisClusterAsyncDisconnect(mAsyncContext);
}

void RedisCli::redisAsyncConnectCallback(const redisAsyncContext *c, int status)
{
    if(status != REDIS_OK)
    {
        appendlog(TTDLogger::LOG_ERROR,"redisAsyncConnectCallback failed\n");
    }
    else
    {
        appendlog(TTDLogger::LOG_DEBUG,"redisAsyncConnectCallback success\n");
    }
}

void RedisCli::redisAsyncDisconnectCallback(const redisAsyncContext *c, int status)
{
    if(status != REDIS_OK)
    {
        appendlog(TTDLogger::LOG_ERROR,"redisAsyncDisconnectCallback failed\n");
    }
    else
    {
        appendlog(TTDLogger::LOG_DEBUG,"redisAsyncDisconnectCallback success\n");
    }
}

int RedisCli::redis_cluster_async_connect()
{
    int res = 0;


    // redis cluster connect with timeout
    mAsyncContext = redisClusterAsyncConnect(msAddrs.c_str(), HIRCLUSTER_FLAG_NULL);
    if(NULL == mAsyncContext || mAsyncContext->err)
    {
        //connect failed
        if(mAsyncContext)
        {
            appendlog(TTDLogger::LOG_ERROR,"redisClusterAsyncConnect failed: %s!\n", mAsyncContext->errstr);

            redisClusterAsyncFree(mAsyncContext);
            mAsyncContext = NULL;
        }
        else
        {
            appendlog(TTDLogger::LOG_ERROR,"redisClusterAsyncConnect failed: can't allocate redis contex!\n");
        }
        res = 1;
    }
    else
    {
        appendlog(TTDLogger::LOG_DEBUG,"redisClusterAsyncConnect success\n");
        redisClusterLibeventAttach(mAsyncContext, mEventBase);
        redisClusterAsyncSetConnectCallback(mAsyncContext, redisAsyncConnectCallback);
        redisClusterAsyncSetDisconnectCallback(mAsyncContext, redisAsyncDisconnectCallback);
    }

    return res;
}

int RedisCli::redis_cluster_connect()
{
    int res = 0;

    // redis cluster connect with timeout
    mSyncContext = redisClusterConnect(msAddrs.c_str(), HIRCLUSTER_FLAG_NULL);
    if(NULL == mSyncContext || mSyncContext->err)
    {
        //connect failed
        if(mSyncContext)
        {
            appendlog(TTDLogger::LOG_ERROR,"redisClusterConnect failed: %s!\n", mSyncContext->errstr);

            redisClusterFree(mSyncContext);
            mSyncContext = NULL;
        }
        else
        {
            appendlog(TTDLogger::LOG_ERROR,"redisClusterConnect failed: can't allocate redis contex!\n");
        }
        res = -1;
    }
    else
    {
        appendlog(TTDLogger::LOG_DEBUG,"redisClusterConnect success\n");
    }

    return res;
}

int RedisCli::AsyncCommand(char *command)
{
    if(NULL == command)
    {
        appendlog(TTDLogger::LOG_ERROR,"NULL == command \n");
        return 1;
    }

    //execute redis command
    if(REDIS_OK == redisClusterAsyncCommand(mAsyncContext, redisAsyncCommandCallback, NULL, command))
    {
        appendlog(TTDLogger::LOG_DEBUG,"redisClusterAsyncCommand success: command=%s\n", command);
    }
    else
    {
        appendlog(TTDLogger::LOG_ERROR,"redisClusterAsyncCommand fail: command=%s, errstr=%s\n", command, mAsyncContext->errstr);
    }

    return 0;
}

int RedisCli::SyncCommand(const char *command, char * output)
{
    int iRet = 0;
    if(NULL == command)
    {
        iRet = -1;
        appendlog(TTDLogger::LOG_ERROR,"NULL == command \n");
        return iRet;
    }
    if( 0 != redis_cluster_connect())
    {
        iRet =-1;
        return iRet;
    }

    //execute redis command
    redisReply *reply = (redisReply *)redisClusterCommand(mSyncContext,  command);
    if(NULL != reply)
    {
        appendlog(TTDLogger::LOG_DEBUG,"SyncCommand success: command=%s\n", command);
        if( output != NULL)
            memcpy(output,reply->str, reply->len);
    }
    else
    {
        appendlog(TTDLogger::LOG_ERROR,"SyncCommand fail: command=%s, errstr=%s\n", command, mSyncContext->errstr);
        iRet = -1;
    }
    redisClusterFree(mSyncContext);


    return iRet;
}

int RedisCli::HsetSync(string key,string field, string value)
{
    string sCmd = "hset ";
    sCmd += key + " ";
    sCmd += field + " ";
    sCmd += value;
    return SyncCommand(sCmd.c_str(),NULL);
}

int RedisCli::HgetSync(string key,string field,char * output)
{
    string sCmd = "hget ";
    sCmd += key + " ";
    sCmd += field;
    return SyncCommand(sCmd.c_str(),output);
}


