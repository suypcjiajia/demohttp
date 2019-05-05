#ifndef OSSMANAGE_H
#define OSSMANAGE_H
#include <string>
#include "aos_log.h"
#include "aos_util.h"
#include "aos_string.h"
#include "aos_status.h"
#include "oss_auth.h"
#include "oss_util.h"
#include "oss_api.h"
using namespace std;

class OssManage
{
public:

    static OssManage * GetInstance();

    //下载文件到本地文件
    //sBucket : 您的bucket名字
    //sObject : 您的object名字
    //sLocalFile : 本地文件路径
    bool GetObjectToLocalFile(const string & sBucket,const string & sObject,const string & sLocalFile);
    //上传本地文件到OSS
    //sBucket : 您的bucket名字
    //sObject : 您的object名字
    //sFile : 本地文件路径
    bool PutObjectFromFile(const string & sBucket,const string & sObject,const string & sFile);

    //获取文件的元数据
    //sBucket : 您的bucket名字
    //sObject : 您的object名字
    //return @sObjectType :文件类型
    //return @lObjectLen : 长度
    //return bool
    bool HeadObject(const string & sBucket,const string & sObject, string & sObjectType, long & lObjectLen);

    void SetEndPoint(const string sEndPoint);
    void SetAccessKeyId(const string sAccessKeyId);
    void SetAccessKeysecret(const string sAccessKeysecret);


private:
    void init_sample_request_options(oss_request_options_t *options, int is_cname);
    void init_sample_config(oss_config_t *config, int is_cname);
    bool Init();

private:
    static OssManage * mInstance;
    bool mbInit;

    string msEndPoint =  "oss-cn-shenzhen.aliyuncs.com";
    string msAccessKeyId =  "Z6JxwAc0qgKCDJXl";
    string msAccessKeysecret =  "0nQl8PqZcYKtjVK6Fm0rjtnRXEqkmV";
    OssManage();
};

#endif // OSSMANAGE_H
