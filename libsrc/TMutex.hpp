#ifndef T_MUTEX_HPP
#define T_MUTEX_HPP
/*private:
    Mutex mMutex;

  在有冲突的函数内加上
  ScopeLock lock( this->mMutex );
 */
#include <pthread.h>
class TMutex
{
public:
    TMutex(){
        pthread_mutex_init(&mMutex, NULL);
    }
    ~TMutex(){
        pthread_mutex_destroy(&mMutex);
    }
    void Lock(){
        pthread_mutex_lock(&mMutex);
    }
    void UnLock(){
        pthread_mutex_unlock(&mMutex);
    }

private:
    pthread_mutex_t mMutex;
    TMutex(const TMutex&);
    TMutex& operator=(const TMutex&);
};
class ScopeLock
{
private:
    TMutex& mMutex;
    ScopeLock(const ScopeLock& scopeLock);
    ScopeLock& operator=(const ScopeLock&);
public:
    ScopeLock(TMutex& mutex):mMutex(mutex){
        mMutex.Lock();
    }
    ~ScopeLock(){
        mMutex.UnLock();
    }
};
#endif
