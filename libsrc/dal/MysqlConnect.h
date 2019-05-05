#ifndef MYSQLCONNECT_H_
#define MYSQLCONNECT_H_

#include <vector>
#include <string>
#include <map>
#include "TMultiMysqlDAL.h"
using namespace std;
class  MysqlConnect
{
public:  static MysqlConnect* GetInstance();
private: static MysqlConnect* mInstance;
private: TMutex mMutex;//线程锁
public:
    //初始化
    int Init(const string& psDataBaseName);

    //创建多连接，一条线程关联上一条mysql连接
    TMultiMysqlDAL *  GetConnect(long pThreadId);

private:
    //数据库
    string msDataBaseName;
    //连接队列
    map<long, TMultiMysqlDAL *> mMapConnect;
};

#endif // MYSQLCONNECT_H
