#ifndef LOGGER_H
#define LOGGER_H
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <thread>
#include "Buffer.h"
using namespace std;

namespace TTDLogger{

#define appendlog TTDLogger::Logger::getInstance()->append
// log alarm levels
enum LOG_LEVE_ENUM {
    LOG_FATAL = 0, // 严重错误，无法修复，如果继续运行下去，程序逻辑也是杂乱的
    LOG_ERROR,     // 错误，系统出现的错误
    LOG_WARN,      // 警告，进行一些修复性的工作，系统可以正常运行下去
    LOG_INFO,        // 系统反馈给最终用户的提示性信息，具有实际的意义
    LOG_DEBUG     // 系统实际运行时不输出，在调试程序时，任意输出，可以是个变量的值
};

//订阅
enum EnumSubScriber
{
    SUBSCRIBER_LEVEL = 0,
    SUBSCRIBER_KEY ,
    SUBSCRIBER_REGULAR
};


class Logger
{
public:

    void setPath(string path);
    void setLevel(LOG_LEVE_ENUM level);
    void setMaxFileLen(int max);
    void append(LOG_LEVE_ENUM level, const char* fmt, ... );
    //订阅
    void levelSubscriber(LOG_LEVE_ENUM level);
    //订阅
    void keySubscriber(string key);
    //订阅
    void regularSubscriber(string regular);
    static Logger * getInstance();

private:

    string toLevelString(LOG_LEVE_ENUM level);
    string getLogName();
    string getStringTime();
    void writeFile(string line);
    void rollFile(int linelen);
    void threadFunc(void);
    int getCurDateMaxIndex();
    void open(string fullfile);
    Logger();

private:

    LOG_LEVE_ENUM curlevel_;//the setting level of log
    string path_;//the dir of log saved
    Buffer buffer_; // buffer
    static Logger * instance_;//single instacne
    std::thread thread_;//thread who execute write to file
    FILE * fp_;//the current file handle
    int curfilelen_; //the current file length byte
    int maxfilelen_; //the setting of file max length byte
    string curFulName_;

};












}
#endif // LOGGER_H
