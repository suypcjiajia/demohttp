#include "TCurl.h"

#include <wchar.h>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include <curl/curl.h>
#include "log/Logger.h"
#include <TConvert.h>
using namespace std;

//保存数据
static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
    if( NULL == str || NULL == buffer )
    {
        return -1;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
    return nmemb;
}

int TCurl::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)
{
    CURLcode res;
    res = curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res;//正常下返回0
}
/**
 * @brief HTTP POST请求
 * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com/para1=val1?2=val2&…
 * @param HttpHeaderVector 输入参数,包头信息
 * @param strPost 输入参数,内容
 * @param strResponse 输出参数,返回的内容
 * @return 返回是否Post成功
 */
int TCurl::PostJosn(const std::string & strUrl, const vector<string>& HttpHeaderVector, const std::string & strPost, std::string & strResponse)
 {
     CURLcode res;
     res = curl_global_init(CURL_GLOBAL_ALL);

     CURL* curl = curl_easy_init();
     if(NULL == curl)
     {
         return CURLE_FAILED_INIT;
     }

     curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
     curl_easy_setopt(curl, CURLOPT_POST, 1);
     // 设置http发送的内容类型为JSON
     if ( HttpHeaderVector.size() > 0 ) {
         curl_slist *plist = NULL;
         for (int i = 0; i < HttpHeaderVector.size(); ++i) {
             plist = curl_slist_append(plist,HttpHeaderVector[i].c_str());
         }
         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
     }
     // 设置要POST的JSON数据
     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());

     curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
     curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
     curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
     curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
     curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

     res = curl_easy_perform(curl);
     curl_easy_cleanup(curl);
     return res;//正常下返回0

 }
/**
 * @brief HTTPS POST请求
 * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com/para1=val1?2=val2&…
 * @param HttpHeaderVector 输入参数,包头信息
 * @param strPost 输入参数,内容
 * @param strResponse 输出参数,返回的内容
 * @return 返回是否Post成功
 */
 int TCurl::Posts(const std::string & strUrl, const vector<string>& HttpHeaderVector, const std::string & strPost, std::string & strResponse, const char * pCaPath)
 {
     CURLcode res;
     res = curl_global_init(CURL_GLOBAL_ALL);

     CURL* curl = curl_easy_init();
     if(NULL == curl)
     {
         return CURLE_FAILED_INIT;
     }

     curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
     curl_easy_setopt(curl, CURLOPT_POST, 1);
     // 设置http发送的内容类型为JSON
     if ( HttpHeaderVector.size() > 0 ) {
         curl_slist *plist = NULL;
         for (int i = 0; i < HttpHeaderVector.size(); ++i) {
             plist = curl_slist_append(plist,HttpHeaderVector[i].c_str());
         }
         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
     }
     // 设置要POST的JSON数据
     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());

     curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
     curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
     curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
     if(NULL == pCaPath)
     {
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
     }
     else
     {
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
         curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
     }
     curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
     curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

     res = curl_easy_perform(curl);
     curl_easy_cleanup(curl);
     return res;//正常下返回0

 }

int TCurl::Get(const std::string & strUrl, std::string & strResponse)
{
    CURLcode res;
    res = curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    /**
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
    */
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res;//正常下返回0
}


/**
 * @brief HTTP GET请求
 * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com/para1=val1?2=val2&…
 * @param HttpHeaderVector 输入参数,包头信息
 * @param strResponse 输出参数,返回的内容
 * @return 返回是否成功
 */

int TCurl::Get(const std::string & strUrl, const vector<string>& HttpHeaderVector,std::string & strResponse)
{
    CURLcode res;
    res = curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());


    if ( HttpHeaderVector.size() > 0 ) {
        curl_slist *plist = NULL;
        for (int i = 0; i < HttpHeaderVector.size(); ++i) {
            plist = curl_slist_append(plist,HttpHeaderVector[i].c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, plist);
    }

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res;//正常下返回0
}

int TCurl::Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath)
{
    CURLcode res;
    res = curl_global_init(CURL_GLOBAL_ALL);

    CURL* curl = curl_easy_init();
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    if(NULL == pCaPath)
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    }
    else
    {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
    }
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res;//正常下返回0
}
