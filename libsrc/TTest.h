#ifndef T_TEST_H
#define T_TEST_H

#include <string>
#include <vector>
#include <time.h>
using namespace std;

//测试类
class TTest
{
public:
    //最大线程数测试
    int MaxThreadTest();
private:
    //线程
    static void* WorkThread(void*);
};

#endif // TCONVERT_H
