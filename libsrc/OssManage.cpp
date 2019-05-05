
#include "OssManage.h"



OssManage * OssManage::mInstance = NULL;
OssManage::OssManage()
{
    //默认的
    msEndPoint =  "oss-cn-shenzhen.aliyuncs.com";
    msAccessKeyId =  "AxIqe7mZCp09FXy5";
    msAccessKeysecret =  "zRmdYJdjIMLGFBbF5yVWYBGg0IoDtt";
    mbInit = false;
    Init();
}

OssManage * OssManage::GetInstance()
{
    if( mInstance == NULL)
        mInstance = new OssManage;
    return mInstance;

}

bool OssManage::Init()
{
    if( mbInit)
        return mbInit;
    if (aos_http_io_initialize(NULL, 0) != AOSE_OK) {
        return false;
    }
    mbInit = true;
    return mbInit;

}

void OssManage::SetEndPoint(const string sEndPoint)
{
    msEndPoint = sEndPoint;
}

void OssManage::SetAccessKeyId(const string sAccessKeyId)
{
    msAccessKeyId = sAccessKeyId;
}

void OssManage::SetAccessKeysecret(const string sAccessKeysecret)
{
    msAccessKeysecret = sAccessKeysecret;
}

//下载文件到本地文件
//sBucket : 您的bucket名字
//sObject : 您的object名字
//sLocalFile : 本地文件路径
bool OssManage::GetObjectToLocalFile(const string & sBucket,const string & sObject, const string & sLocalFile)
{
    bool bRet = false;
    if( !Init()){
        bRet = false;
        return bRet;
    }

    aos_pool_t *p = NULL;
    aos_string_t bucket;

    aos_string_t object;
    int is_cname = 0;
    oss_request_options_t *options = NULL;
    aos_table_t *headers = NULL;
    aos_table_t *params = NULL;
    aos_table_t *resp_headers = NULL;
    aos_status_t *s = NULL;
    aos_string_t file;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    aos_str_set(&bucket, sBucket.c_str());
    aos_str_set(&object, sObject.c_str());
    headers = aos_table_make(p, 0);
    aos_str_set(&file, sLocalFile.c_str());

    s = oss_get_object_to_file(options, &bucket, &object, headers,
                               params, &file, &resp_headers);

    if (aos_status_is_ok(s)) {
        printf("get object to local file succeeded\n");
        bRet = true;

    } else {
        printf("get object to local file failed\n");
        bRet = false;
    }

    aos_pool_destroy(p);
    return bRet;
}
//上传本地文件到OSS
//sBucket : 您的bucket名字
//sObject : 您的object名字
//sFile : 本地文件路径
bool OssManage::PutObjectFromFile(const string & sBucket,const string & sObject,const string & sFile)
{
    bool bRet = false;
    if( !Init()){
        bRet = false;
        return bRet;
    }
    aos_pool_t *p = NULL;
    aos_string_t bucket;
    aos_string_t object;
    int is_cname = 0;
    aos_table_t *headers = NULL;
    aos_table_t *resp_headers = NULL;
    oss_request_options_t *options = NULL;
    aos_status_t *s = NULL;
    aos_string_t file;

    aos_pool_create(&p, NULL);
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    headers = aos_table_make(options->pool, 1);
    apr_table_set(headers, OSS_CONTENT_TYPE, "image/jpeg");
    aos_str_set(&bucket, sBucket.c_str());
    aos_str_set(&object, sObject.c_str());
    aos_str_set(&file, sFile.c_str());

    s = oss_put_object_from_file(options, &bucket, &object, &file,
                                 headers, &resp_headers);

    if (aos_status_is_ok(s)) {
        printf("put object from file succeeded\n");
        bRet = true;
    } else {
        printf("put object from file failed, code:%d, error_code:%s, error_msg:%s, request_id:%s\n",
            s->code, s->error_code, s->error_msg, s->req_id);
        bRet = false;
    }

    aos_pool_destroy(p);

    return bRet;
}


bool OssManage::HeadObject(const string & sBucket,const string & sObject, string & sObjectType, long & lObjectLen)
{
    bool bRet = false;
    if( !Init()){
        bRet = false;
        return bRet;
    }

    aos_pool_t *p = NULL;
    aos_string_t bucket;
    aos_string_t object;
    oss_request_options_t *options = NULL;
    aos_table_t *headers = NULL;
    aos_table_t *resp_headers = NULL;
    aos_status_t *s = NULL;
    char *content_length_str = NULL;
    char *object_type = NULL;
    int64_t content_length = 0;
    int is_cname = 0;
    aos_pool_create(&p, NULL);
    /* 创建并初始化options */
    options = oss_request_options_create(p);
    init_sample_request_options(options, is_cname);
    /* 初始化参数 */
    aos_str_set(&bucket, sBucket.c_str());
    aos_str_set(&object, sObject.c_str());
    headers = aos_table_make(p, 0);
    /* 获取文件元数据 */
    s = oss_head_object(options, &bucket, &object, headers, &resp_headers);
    if (aos_status_is_ok(s)) {
        /* 获取文件长度 */
        content_length_str = (char*)apr_table_get(resp_headers, OSS_CONTENT_LENGTH);
        if (content_length_str != NULL) {
            content_length = atoll(content_length_str);
        }
        /* 获取文件的类型 */
        object_type = (char*)apr_table_get(resp_headers, OSS_OBJECT_TYPE);
        printf("head object succeeded, object type:%s, content_length:%ld\n",
               object_type, content_length);
        sObjectType = object_type;
        lObjectLen = content_length;
        bRet = true;
    } else {
        printf("head object failed\n");
        bRet =  false;
    }
    aos_pool_destroy(p);
    return bRet;
}

void OssManage::init_sample_request_options(oss_request_options_t *options, int is_cname)
{
    options->config = oss_config_create(options->pool);
    init_sample_config(options->config, is_cname);
    options->ctl = aos_http_controller_create(options->pool, 0);
}

void OssManage::init_sample_config(oss_config_t *config, int is_cname)
{
    aos_str_set(&config->endpoint, msEndPoint.c_str());
    aos_str_set(&config->access_key_id, msAccessKeyId.c_str());
    aos_str_set(&config->access_key_secret, msAccessKeysecret.c_str());
    config->is_cname = is_cname;
}
