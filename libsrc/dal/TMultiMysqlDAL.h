#ifndef T_MULTIMYSQL_DAL_H
#define T_MULTIMYSQL_DAL_H

//#include <windows.h>//windows
#include <stdlib.h>//linux
#include <mysql/mysql.h>
#include <string>
#include <vector>
using namespace std;

#include "TMutex.hpp"

class TMultiMysqlDAL
{

public:
    TMultiMysqlDAL();
    TMultiMysqlDAL(string sAddr, string sUser, string sPwd);
    ~TMultiMysqlDAL();
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
    //是否自动提交（事务）
    int AutoCommit(bool bOpen);
    //事务回滚
    int RollBack();
    //事务提交
    int Commit();

private:
    MYSQL* mConnected;
    MYSQL mMysql;
    // 数据库-连接数据库
    int ConnectDB();
    //使用数据库
    int UserDB( const string& sDatebase );
    //创建数据库
    int CreateDatabase(const string& sDatebase);
    // 数据库-激活连接线程
    static void* ActivationLinkDBThread(void*);
    // 重连数据库
    int ReConnectDB(int iSleep = 0);
    //使用数据库
    string msUserDB;

    string mAddr;
    string mUser;
    string mPwd;

public:

    static void SetMysqlAddr(const string & psAddr);
    static void SetMysqlUser(const string & psUser);
    static void SetMysqlPwd(const string & psPwd);
private:
    static string mMysqlAddr;
    static string mMysqlUser;
    static string mMysqlPwd;


};

#endif // T_MULTIMYSQL_DAL_H
