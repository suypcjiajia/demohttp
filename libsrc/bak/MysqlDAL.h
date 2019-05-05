#ifndef MYSQL_DAL_H
#define MYSQL_DAL_H

//#include <windows.h>//windows
#include <stdlib.h>//linux
#include <mysql/mysql.h>
#include <string>
using namespace std;

#include "TMutex.hpp"

class MysqlDAL
{
// 定义单键
public:  static MysqlDAL* GetInstance();
private: static MysqlDAL* mInstance;
private: TMutex mMutex;//线程锁
public:
    MysqlDAL();
    ~MysqlDAL();
    // 数据库-关闭数据库
    int StartDB(const string& sDatebase);
    // 数据库-关闭数据库
    int CloseDB();
    // 数据库-连接数据库
    int ConnectDB(const string& sDatebase);

    // 数据库-运行SQL语句
    int Query( const string& psSql );
    // 数据库-返回查询内容
    MYSQL_RES* QueryResult( const string& psSql );
    // 释放内存
    void FreeResult( MYSQL_RES* pResult );
    //二进制转字符串
    char* BinaryToString(char* pBinaryData,long lLen);
private:
    MYSQL* mConnected;
    MYSQL mMysql;
    // 数据库-连接数据库
    int ConnectDB();
    //使用数据库
    int UserDB( const string& sDatebase );
    // 启动PING长连接
    int StartPingDB();
    // 数据库-激活连接线程
    static void* ActivationLinkDBThread(void*);
    // 数据库-激活连接
    int Ping();
    //创建数据库
    int CreateDatabase(const string& sDatebase);
    //使用数据库
    string msUserDB;
    //操作次数
    int miOperationTimes;


};

#endif // MYSQLDAL_H
