/*多线程异步推送
*/

#include "PushBLL.h"
#include "log/Logger.h"
#include "TConvert.h"
#include "com/RetJsonMsg.hpp"
#include "com/PushDataCOM.h"
#include "bll/Config.h"
#include <stdlib.h>



PushBLL* PushBLL::mInstance = NULL;
PushBLL* PushBLL::GetInstance()
{
    if (mInstance == NULL){
        mInstance = new PushBLL;
    }
    return mInstance;
}


//生成线程池
void PushBLL::Start()
{
    pthread_t tid;
    int iNum = 30;
    //生成线程池
    for( int i = 0; i < iNum ; i++){
        if (pthread_create(&tid,NULL,ThreadFun,this)!=0) {
            appendlog(TTDLogger::LOG_ERROR,"PushBLL::Start pthread_create error");
            exit(0);
        };
    }
}


void * PushBLL::ThreadFun(void *p)
{
    PushBLL * myself =  (PushBLL*) p;
    myself->RealRun();
    return NULL;
}
//线程运行逻辑
void PushBLL::RealRun()
{
    int iRet = -1;
    while(true){

        PushEle  ele;

        if( Pop(ele) ){

            PushDataCOM pushDataCOM;
            iRet = pushDataCOM.PushData( MyConfig::GetInstance()->GetPushIP(), ele.value.toStyledString() );
            if( iRet == 0){
                if( ele.sucess != NULL)
                    ele.sucess(ele.value);//回调
            }else{
                if( ele.fail != NULL)
                    ele.fail(ele.value);//回调
            }
            usleep(10);

        }else{
            usleep(1000*100);
        }
    }

}

bool  PushBLL::Pop(PushEle & ele)
{
    ScopeLock lock( this->mMutex );
    if ( mCacheLst.size() > 0 ){
        ele = *mCacheLst.begin();
        mCacheLst.erase(mCacheLst.begin());
        return true;
    }
    return false;
}
//加入推送队列
void PushBLL::Push(const Json::Value & value,FunSucess sucess, FunSucess fail)
{
    ScopeLock lock( this->mMutex );
    PushEle ele;
    ele.value = value;
    ele.sucess = sucess;
    ele.fail = fail;
    mCacheLst.push_back(ele);
    return;

}


