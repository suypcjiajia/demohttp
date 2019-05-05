#include "Logger.h"
#include <functional>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <sys/time.h>
#include "MyUtil.h"
#include "TConvert.h"
namespace TTDLogger{

Logger * Logger::instance_ = NULL;

Logger::Logger():thread_(std::bind(&Logger::threadFunc,this))
{
    path_ = '/';
    fp_ = NULL;
    curlevel_ = LOG_DEBUG;
    maxfilelen_ = 1024*1024*100;
}

Logger * Logger::getInstance()
{
    if( instance_ == NULL)
    {
        instance_ = new Logger;
    }
    return instance_;
}

string Logger::toLevelString(LOG_LEVE_ENUM level)
{
    const char v[][10] = {
        "Fatal",
        "Error",
        "Warn",
        "Info",
        "Debug"
    };
    return v[level];
}

string Logger::getLogName()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *t;
    t = localtime( &tv.tv_sec );
    char logFile[512];
    sprintf(logFile, "%04d%02d%02d.log",
            t->tm_year+1900,
            t->tm_mon+1,
            t->tm_mday);
    return logFile;
}

string Logger::getStringTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *t;
    t = localtime( &tv.tv_sec );


    char buff[2048];
    sprintf(buff, "%02d:%02d:%02d:%03d\t",
            t->tm_hour,
            t->tm_min,
            t->tm_sec,
            (int)tv.tv_usec
            );
    return buff;
}

void Logger::setPath(string path)
{
    path_ = path;
}

void Logger::setLevel(LOG_LEVE_ENUM level)
{
    curlevel_ = level;
}

void Logger::setMaxFileLen(int max)
{
    maxfilelen_ = max;
}

void Logger::append(LOG_LEVE_ENUM level, const char* fmt, ...)
{
    //return;
    if (level > curlevel_) {
        return;
    }
    TConvert tConvert;

    va_list pArg;
    va_start(pArg, fmt);
    int size = 1024*1024*10;//10Mb for buff
    static char * buff = NULL;

    if( buff == NULL){
        buff = (char*)malloc(size);
        if( buff == NULL)
            return;
    }

    memset(buff,size,0);

    vsnprintf(buff, size-2,fmt, pArg);
    va_end(pArg);
    strcat(buff, "\n");

    string line = getStringTime() + " " +  toLevelString(level) + " "  +  tConvert.LongToStr( pthread_self() )  + " " + buff;

    buffer_.append(line);

}


void Logger::threadFunc(void)
{
    int emptyCounts = 0;
    while(true){
        //string line = buffer_.getLine();
        string line = buffer_.getLines(10);

        if( line.empty()){
            emptyCounts ++;
            sleep(1);
            continue;
        }
        writeFile(line);
        usleep(1000*10);
    }

}

void Logger::writeFile(string line)
{
    string fullname = path_ + getLogName();

    if( fp_ == NULL || curFulName_ != fullname){
        open(fullname);
    }

    rollFile(line.length());

    fprintf(fp_, "%s", line.c_str());
    fflush(fp_);
    printf("%s",line.c_str());

}


void Logger::open(string fullfile)
{
    fp_ = fopen(fullfile.c_str(),"a");
    fseek(fp_,0,SEEK_END);
    curfilelen_ = ftell(fp_);
    curFulName_ = fullfile;
}

void Logger::rollFile(int linelen)
{
    if ( curfilelen_ + linelen >=  maxfilelen_)
    {
        int maxIndex = getCurDateMaxIndex();
        maxIndex++;
        string logname = getLogName();
        char dstname[128] = {0};
        sprintf(dstname,"%s%d",logname.c_str(),maxIndex);
        fclose(fp_);
        fp_ = NULL;
        string cmd ="mv " + path_ + logname + " " + path_ + dstname;
        system(cmd.c_str());

        string fullname = path_ + getLogName();
        open(fullname);

    }else{
        curfilelen_ += linelen;
    }
}


int Logger::getCurDateMaxIndex()
{
    vector<string>files;
    MyUtil::getFilesFromDir(path_,files);
    string logname = getLogName();
    int iMaxIndex = 0;
    for( vector<string>::iterator it = files.begin(); it != files.end(); it++)
    {
        string &  file = *it;
        if( file.find(logname) != string::npos){
            size_t pos = file.find("log");
            int len  = file.length();
            if( len - pos >= 4){
                string index = file.substr(pos + 3);
                int iIndex = atoi(index.c_str());
                iMaxIndex = (iIndex > iMaxIndex) ? iIndex : iMaxIndex;

            }
        }

    }
    return iMaxIndex;
}







}

