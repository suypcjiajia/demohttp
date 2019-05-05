#ifndef T_MYSQL_DAL_H
#define T_MYSQL_DAL_H

//#include <windows.h>//windows
#include <stdlib.h>//linux
#include <mysql/mysql.h>
#include <string>
#include <vector>
using namespace std;

#include "TMutex.hpp"

class TMysqlDAL
{
// 定义单键
public:  static TMysqlDAL* GetInstance();
private: static TMysqlDAL* mInstance;
private: TMutex mMutex;//线程锁
public:
    TMysqlDAL();
    ~TMysqlDAL();
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
    //字符串特殊符号转义
    string RealEscapeString(const string& sContent);
    //二进制转字符串
    char* BinaryToString(char* pBinaryData,long lLen);
    //显示所有表
    int ShowTables(vector<string>& pTablesVector);

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
