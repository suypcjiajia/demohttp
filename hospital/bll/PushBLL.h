/*多线程异步推送
*/

#ifndef PUSHBLL_H
#define PUSHBLL_H
#include <string>
#include <list>
#include"json/json.h"
#include "TMutex.hpp"
using namespace std;



typedef void ( *FunSucess)(const Json::Value & value);
typedef void ( *FunFail)(const Json::Value & value);
struct PushEle
{
    Json::Value value;
    FunSucess sucess;
    FunFail fail;
};

class PushBLL
{
public:

    //生成多线程
    void Start();
    static void * ThreadFun(void *);
    //线程运行逻辑
    void RealRun();

    //加入推送队列
    //@value :
    void Push(const Json::Value & value, FunSucess sucess, FunSucess fail);

    static PushBLL* GetInstance();

private:

    PushBLL(){}
    ~PushBLL(){}

    bool  Pop(PushEle & ele);
private:
    list<PushEle> mCacheLst;
    TMutex mMutex;

    static PushBLL* mInstance;

};

#endif // PUSHBLL_H
