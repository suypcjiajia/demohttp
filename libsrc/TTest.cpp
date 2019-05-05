#include "TTest.h"

//#include <wchar.h>
//#include <string.h>
//#include <cstdlib>
#include <cstdio>
//#include <sstream>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
using namespace std;
//最大线程数测试
int TTest::MaxThreadTest()
{
    unsigned int i;
    int rc;
    //pthread_t poolid[655350];
    for(i=0;i<655350+1;i++)
    {
        pthread_t tid;
        rc = pthread_create( &tid, NULL, WorkThread, (void*)this );
        if(0 != rc)
        {
            printf("pthread_create() failure\r\n Max pthread num is %d\r\n",i);
            return -1;
        }
    }
    printf("Max pthread num is 655350\r\n Your system is power full\r\n");
    return 0;
}
//线程
void* TTest::WorkThread(void*ptr)
{
    pthread_detach(pthread_self());//退出线程后自动释放全部资源

    //逻辑
    sleep(60);


    pthread_exit(NULL);;//退出线程
}
