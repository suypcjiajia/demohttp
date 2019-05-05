#include "com/EvhtpSvr.h"
#include "com/RetJsonMsg.hpp"
#include "TMd5.h"
#include "TConvert.h"
#include "TFile.h"
#include "log/Logger.h"
#include "TypeDef.h"


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Token.h"


evhtp_ssl_cfg_t EvhtpSvr::msslcfg;
map<string,string> EvhtpSvr::mapWhiteName;

EvhtpSvr::EvhtpSvr()
{
}
//初始化
int EvhtpSvr::Init(const string& sServiceName)
{
    //appid文件
    const string sConfig = "./" + sServiceName + ".appid";
    //读appid文件
    TFile tFile;

    if (tFile.IsFileExist(sConfig)) {
        appendlog(TTDLogger::LOG_ERROR,"EvhtpSvr::Init not file=%s",sConfig.c_str());
        return -1;
    }
    mAppId_AppKeyAesMap.clear();
    tFile.GetValueToMap(sConfig,mAppId_AppKeyAesMap);

    WhiteInit();


    return 0;
}


int EvhtpSvr::WhiteInit()
{
    mapWhiteName["120.76.72.55"] ="";
    mapWhiteName["10.24.251.185"] ="";


    mapWhiteName["112.74.128.68"] ="";
    mapWhiteName["10.45.177.45"] ="";

    mapWhiteName["120.76.78.130"] ="";
    mapWhiteName["10.172.40.92"] ="";

    mapWhiteName["120.76.77.26"] ="";
    mapWhiteName["10.45.191.227"] ="";

    mapWhiteName["120.76.77.26"] ="";
    mapWhiteName["10.45.191.227"] ="";


    mapWhiteName["112.74.16.71"] ="";
    mapWhiteName["10.44.67.42"] ="";

    mapWhiteName["120.25.160.101"] ="";
    mapWhiteName["10.116.146.252"] ="";

    mapWhiteName["120.25.235.85"] ="";
    mapWhiteName["10.116.95.176"] ="";

    mapWhiteName["120.76.84.231"] ="";
    mapWhiteName["10.172.40.69"] ="";

    mapWhiteName["120.76.153.193"] ="";
    mapWhiteName["10.24.244.215"] ="";

    mapWhiteName["120.76.154.15"] ="";
    mapWhiteName["10.24.245.113"] ="";

    mapWhiteName["120.76.154.26"] ="";
    mapWhiteName["10.24.245.80"] ="";

    mapWhiteName["120.76.29.158"] ="";
    mapWhiteName["10.25.202.34"] ="";


    mapWhiteName["120.76.30.201"] ="";
    mapWhiteName["10.25.202.64"] ="";

    mapWhiteName["120.76.190.80"] ="";
    mapWhiteName["10.25.210.152"] ="";

    mapWhiteName["120.76.191.60"] ="";
    mapWhiteName["10.46.90.4"] ="";

    mapWhiteName["120.76.79.76"] ="";
    mapWhiteName["10.45.188.188"] ="";


}

void EvhtpSvr::SetProcessCallBack2(ProcessCallBack2 processCallBack2)
{
    mProcessCallBack2 = processCallBack2;
}

void EvhtpSvr::SetProcessCallBack(ProcessCallBack processCallBack)
{
    mProcessCallBack = processCallBack;
}

//启动服务
void EvhtpSvr::StartSvr(const int piPort,const string& psIsSSL,const string& psThreads,const string& psIsAes,const string& psIsCheckSafe){
    msIsCheckSafe = psIsCheckSafe;
    msIsAes = psIsAes;
    try{

        evbase_t *evbase = event_base_new();
        evhtp_t  *htp    = evhtp_new(evbase, NULL);

        //是否启动https
        if( psIsSSL == "true"){
            appendlog(TTDLogger::LOG_INFO,"EvhtpSvr use SSL");

            memset(&msslcfg, 0, sizeof(evhtp_ssl_cfg_t));
            msslcfg.pemfile        = "/certs/cert.pem";
            msslcfg.privfile       = "/certs/key.pem";
            msslcfg.scache_type    = evhtp_ssl_scache_type_internal;
            msslcfg.scache_timeout = 5000;
            int iRet = evhtp_ssl_init(htp, &msslcfg);
            //printf("evhtp_ssl_init=%d\n",iRet);
            if ( 0 != iRet ) {
                appendlog(TTDLogger::LOG_ERROR,"EvhtpSvr::StartSvr evhtp_ssl_init error");
                exit(0);
            }
        }

        //设置请求回调
        //evhtp_set_cb(htp, "/", EvhtpSvr::RequestCb, NULL);
        evhtp_set_gencb(htp, EvhtpSvr::RequestCb, NULL);

        evhtp_set_max_keepalive_requests(htp,100000);

        //psThreads:处理线程的条数（经观察，线程创建后不会退出，所以线程ID不变，所以给每条线程创建了一个mysql连接）
        int iRet = evhtp_use_threads(htp, NULL, atoi(psThreads.c_str()), NULL);
        if ( 0 != iRet ) {
            appendlog(TTDLogger::LOG_ERROR,"EvhtpSvr::StartSvr evhtp_use_threads error");
            exit(0);
        }

        //绑定端口
        iRet = evhtp_bind_socket(htp, "0.0.0.0", piPort, 1024);

        if ( 0 != iRet ) {
            appendlog(TTDLogger::LOG_ERROR,"Evhtevbase_tpSvr::StartSvr evhtp_bind_socket error");
            exit(0);
        }
        pthread_t tid;
        if (pthread_create(&tid,NULL,EvhtpSvr::EventBaseLoop,evbase)!=0) {
            appendlog(TTDLogger::LOG_ERROR,"EvhtpSvr::StartSvr pthread_create error");
            exit(0);
        };

        //开始接受请求
        //event_base_loop(evbase, 0);

    }catch(...){
        appendlog(TTDLogger::LOG_ERROR,"EvhtpSvr Start error");
        exit(0);
    }
}

void * EvhtpSvr::EventBaseLoop(void * evbase)
{
    //开始接受请求
    event_base_loop((event_base *)evbase, 0);
}


//处理带token，不加密的情况
void EvhtpSvr::Process2(evhtp_request_t * request, void * pUserParma)
{

    long lCallId  = -1;
    int iRet = -1;
    TConvert tConvert;

    char * bufPostData = NULL;
    evhtp_kv_t * hn = NULL;
    evhtp_kv_t * ha = NULL;


    try{

        htp_method eMethod = evhtp_request_get_method(request) ;
        //appendlog(TTDLogger::LOG_DEBUG,"http method:%s",HttpMehtodEnum2Str(eMethod).c_str());
        appendlog(TTDLogger::LOG_DEBUG,"http url:%s",request->uri->path->full);
        //appendlog(TTDLogger::LOG_DEBUG,"content length:%d" ,evhtp_request_content_len(request));
        //appendlog(TTDLogger::LOG_DEBUG, "uri.query_raw=%s", request->uri->query_raw);
        //提取URL参数
        evhtp_kvs_for_each(request->uri->query, dump_query, this);
        evhtp_kvs_for_each(request->headers_in, dump_header, this);


        //提取POST数据
        bufPostData = new char[evhtp_request_content_len(request) + 1];
        memset(bufPostData,0,evhtp_request_content_len(request) + 1);
        evbuffer_copyout(request->buffer_in, bufPostData, evhtp_request_content_len(request));

        bufPostData[evhtp_request_content_len(request)] = 0;



        mJsonbody.clear();
        mJsonResponse.clear();

        string sAppKey = mapHeadIn["appkey"];
        string sToken = mapHeadIn["token"];
        string sDeToken;

        appendlog(TTDLogger::LOG_DEBUG,"token=%s" ,sToken.c_str());

        if( !sToken.empty() ){

            sDeToken = Token::UnAesLoginToken(sToken);
            if(!sDeToken.empty()){
                tConvert.Str2Json(sDeToken,mJsonDetoken );
            }
        }

        string sUrlPath = tConvert.URLDecode(request->uri->path->full);
        mUrl = sUrlPath;


        char * bufPost = bufPostData;
        if( strncmp(bufPostData,"dldata=", strlen("dldata=")) == 0){
            bufPost += strlen("dldata=");
        }
        string sPostData = tConvert.URLDecode(bufPost);


        appendlog(TTDLogger::LOG_INFO,"Url=%s,Body=%s" ,request->uri->path->full,sPostData.c_str() );
        ToJson(sPostData);
        iRet = tConvert.Str2Json(sPostData,mJsonbody );
        if(  0 == iRet){
            if( ! mJsonbody["userId"].isNull()  ){
                lCallId = tConvert.Json2Long(mJsonbody["userId"]);
            }
        }

        if(htp_method_POST ==  eMethod ){

        }else if( (htp_method_GET ==  eMethod ) ){

        }
        // 回调函数
        if (mProcessCallBack2 != NULL) {
            mProcessCallBack2(this);
        }




        string sResponse = "";
        sResponse = mJsonResponse.toStyledString();

        if(  lCallId != -1 ){
            appendlog(TTDLogger::LOG_DEBUG,"lCallId:%ld sResponse is:%s" ,lCallId, sResponse.c_str() );
        }else{

            appendlog(TTDLogger::LOG_DEBUG,"sResponse is:%s" , sResponse.c_str() );
        }

        iRet = evbuffer_add(request->buffer_out, sResponse.c_str(), sResponse.length());




        hn = evhtp_header_new("Access-Control-Allow-Origin","*", 1, 1);
        evhtp_headers_add_header (request->headers_out,hn);

        ha = evhtp_header_new("Access-Control-Allow-Headers","token, appkey", 1, 1);
        evhtp_headers_add_header (request->headers_out,ha);


        if( iRet == 0){
            evhtp_send_reply_start(request, EVHTP_RES_OK);
        }else{
            appendlog(TTDLogger::LOG_ERROR,"evbuffer_add err");
            evhtp_send_reply_start(request, EVHTP_RES_500);
        }


    }catch(...){
        appendlog(TTDLogger::LOG_ERROR,"EvhtpSvr::Process err 500");
        evhtp_send_reply_start(request, EVHTP_RES_500);

    }
    if( bufPostData != NULL){
        delete bufPostData;

    }
    if(!hn){
        evhtp_header_free(hn);
    }
    if(!hn){
        evhtp_header_free(ha);
    }

    return;

}

//请求的处理函数
void EvhtpSvr::Process(evhtp_request_t * request, void * pUserParma){

    long lCallId  = -1;
    int iRet = -1;
    TConvert tConvert;
    if (NULL == request->uri->authority){
        appendlog(TTDLogger::LOG_ERROR,"authority is null");//Q:不清楚这个什么用
    }
    char * bufPostData = NULL;

    evhtp_kv_t * hn = NULL;


    try{

        htp_method eMethod = evhtp_request_get_method(request) ;
        //appendlog(TTDLogger::LOG_DEBUG,"http method:%s",HttpMehtodEnum2Str(eMethod).c_str());
        appendlog(TTDLogger::LOG_DEBUG,"http url:%s",request->uri->path->full);
        //appendlog(TTDLogger::LOG_DEBUG,"content length:%d" ,evhtp_request_content_len(request));
        //appendlog(TTDLogger::LOG_DEBUG, "uri.query_raw=%s", request->uri->query_raw);
        //提取URL参数
        evhtp_kvs_for_each(request->uri->query, dump_query, this);
        evhtp_kvs_for_each(request->headers_in, dump_header, this);






        //提取POST数据
        // char bufPostData[2048*5] = {0};//Q:是不是太小了，改为new好点，但new的开销比较大
        bufPostData = new char[evhtp_request_content_len(request) + 1];
        memset(bufPostData,0,evhtp_request_content_len(request) + 1);
        evbuffer_copyout(request->buffer_in, bufPostData, evhtp_request_content_len(request));

        bufPostData[evhtp_request_content_len(request)] = 0;
        //appendlog(TTDLogger::LOG_DEBUG,"content is:%s" , bufPostData );

        //哪个业务
        string sBusiness = mapQueryParam["sBusiness"];
        //哪个接口
        string sMethod = mapQueryParam["sMethod"];
        string sAppid = mapQueryParam["sAppid"];
        string sTimeStamp= mapQueryParam["sTimeStamp"];
        string sSign  = mapQueryParam["sSign"];
        //printf("sTimeStamp=%s\n",sTimeStamp.c_str());



        int appendlen = SetKey();

        Json::Value jsonRespose;
        jsonRespose.clear();

        string sUrlPath = tConvert.URLDecode(request->uri->path->full);
        mUrl = sUrlPath;

        if( FireWall( sMethod,request) == -1){
            goto RESPONSE;
        }

        if(htp_method_POST ==  eMethod ){// process Post
            char * bufPost = bufPostData;
            if( strncmp(bufPostData,"sCondition=", strlen("sCondition=")) == 0){
                bufPost += strlen("sCondition=");
            }
            string sUrlDecodePost = tConvert.URLDecode(bufPost);

            //appendlog(TTDLogger::LOG_DEBUG,"PostData is:%s" , sUrlDecodePost.c_str() );
            //安全检验
            if(SafeCheck(sAppid,sTimeStamp,sSign,sUrlDecodePost) == -1){
                //RetJsonMsg::SetRetMsg(-14,jsonRespose);
                goto RESPONSE;
            }else{
                string  sPostData;

                if( msIsAes == "true"){
                    sPostData = myaes.Decrypt(sUrlDecodePost,appendlen);
                }else{
                    sPostData = sUrlDecodePost;
                }
                appendlog(TTDLogger::LOG_INFO,"sBusiness=%s,sMethod=%s,PostData=%s" ,sBusiness.c_str(), sMethod.c_str(),sPostData.c_str() );
                Json::Value jsonValue;
                iRet = tConvert.Str2Json(sPostData,jsonValue );
                if(  0 == iRet){
                    if( ! jsonValue["lUserId"].isNull()  ){
                        lCallId = tConvert.Json2Long(jsonValue["lUserId"]);
                    }
                    // 定义回调函数
                    if (mProcessCallBack != NULL) {
                        mProcessCallBack(this);
                    }
                }else{
                    jsonRespose.clear();
                    // RetJsonMsg::SetRetMsg(-20,jsonRespose);
                }
            }
        }else{

        }



RESPONSE:
        string sResponse = "";
        sResponse = jsonRespose.toStyledString();

        if(sResponse.substr(0,4) == "null" ){
            sResponse = "";
        }

        if(  lCallId != -1 ){
            appendlog(TTDLogger::LOG_DEBUG,"lCallId:%ld sResponse is:%s" ,lCallId, sResponse.c_str() );
        }else{

            appendlog(TTDLogger::LOG_DEBUG,"sResponse is:%s" , sResponse.c_str() );
        }
        if( msIsAes == "true"){

            if( !sResponse.empty()){
                sResponse = myaes.Encrypt(sResponse,appendlen);
            }
            //appendlog(TTDLogger::LOG_DEBUG,"Aes sResponse is:%s" , sResponse.c_str() );
        }

        // evbuffer_add_reference(request->buffer_out, sResponse.c_str(), sResponse.length(), NULL, NULL);
        iRet = evbuffer_add(request->buffer_out, sResponse.c_str(), sResponse.length());

        hn = evhtp_header_new("Access-Control-Allow-Origin","*", 1, 1);
        evhtp_headers_add_header (request->headers_out,hn);
        // char buflen[24] = {0};
        //  sprintf(buflen,"%d",sResponse.length());
        // evhtp_kv_t * hlen = evhtp_header_new("Content-Length",buflen, 1, 1);
        // evhtp_headers_add_header (request->headers_out,hlen);


        //evhtp_send_reply(request, EVHTP_RES_OK);//Q:https用这个会崩
        if( iRet == 0 && !sResponse.empty() ){
            evhtp_send_reply_start(request, EVHTP_RES_OK);
        }else{
            //appendlog(TTDLogger::LOG_ERROR,"evbuffer_add err");
            evhtp_send_reply_start(request, EVHTP_RES_500);
        }


    }catch(...){
        appendlog(TTDLogger::LOG_ERROR,"EvhtpSvr::Process err 500");
        evhtp_send_reply_start(request, EVHTP_RES_500);

    }
    if( bufPostData != NULL){
        delete bufPostData;
    }

    if(!hn){
        evhtp_header_free(hn);
    }

    return;
}
//请求回调
void  EvhtpSvr::RequestCb(evhtp_request_t * request, void * pUserParma) {

    EvhtpSvr svr;
    evhtp_request_set_keepalive(request,1);
    request->conn->keepalive = 1;
    string path = request->uri->path->full;
    if( path == "/" || path == "" ){//
        svr.Process(request,pUserParma);
    }else{
        svr.Process2(request,pUserParma);
    }
    evhtp_request_free(request);

}


int EvhtpSvr::dump_query(evhtp_kv_t * kv, void * arg){
    EvhtpSvr * myself =(EvhtpSvr *) arg;
    if( NULL ==  kv->val){
        return 0;
    }
    myself->mapQueryParam[kv->key] = kv->val;

#ifdef FORSMS
    appendlog(TTDLogger::LOG_INFO,"query key:%s,value:%s" , kv->key,kv->val );
#endif

    return 0;
}


int EvhtpSvr::dump_header(evhtp_kv_t * kv, void * arg)
{
    EvhtpSvr * myself =(EvhtpSvr *) arg;
    myself->mapHeadIn[kv->key] = kv->val;

#ifdef FORSMS
    appendlog(TTDLogger::LOG_INFO,"head key:%s,value:%s" , kv->key,kv->val );
#endif

    return 0;
}


string EvhtpSvr::HttpMehtodEnum2Str(const htp_method  & pMethod ){
    switch(pMethod){
    case  htp_method_GET:
        return "GET";
    case htp_method_HEAD:
        return "HEAD";
    case htp_method_POST:
        return "POST";
    case htp_method_PUT:
        return "PUT";
    case htp_method_DELETE:
        return "DELETE";
    default:
        return "UNKNOW";
    }
}

int EvhtpSvr::SetKey()
{
    string sAppKey = "";
    string sAppId = "425388c9c928c3d36856e67b6d86cc11";
    map<string,string>::iterator iter= mAppId_AppKeyAesMap.find(sAppId);
    if( iter != mAppId_AppKeyAesMap.end() ) {
        string* sValue = &iter->second;
        sAppKey = sValue->substr(0,32);
        string sAesKey = sValue->substr(32);
        //printf("sAppKey=%s\n",sAppKey.c_str());
        //printf("sAesKey=%s\n",sAesKey.c_str());
        myaes.SetKey((unsigned char*)sAesKey.c_str(),32);
    }else {
        appendlog(TTDLogger::LOG_ERROR,"SafeCheck SetKey error");
        return -1;
    }
    return 0;

}

int EvhtpSvr::SetKey2()
{
    int len = 0;
    string sBusiness = GetQuery("sBusiness");
    string sMethod = GetQuery("sMethod");
    string versions = GetQuery("versions");
    string clientType = GetQuery("clientType");
    if( sMethod == "CodeTTDv2" && sBusiness == "phonesms" &&  clientType == "01"){
        string sAesKey = "ttd135!@#246";
        myaes.SetKey((unsigned char*)sAesKey.c_str(),32);
        len = 8;
    }else{

        string sAppKey = "";
        string sAppId = "425388c9c928c3d36856e67b6d86cc11";
        map<string,string>::iterator iter= mAppId_AppKeyAesMap.find(sAppId);
        if( iter != mAppId_AppKeyAesMap.end() ) {
            string* sValue = &iter->second;
            sAppKey = sValue->substr(0,32);
            string sAesKey = sValue->substr(32);
            //printf("sAppKey=%s\n",sAppKey.c_str());
            //printf("sAesKey=%s\n",sAesKey.c_str());
            myaes.SetKey((unsigned char*)sAesKey.c_str(),32);
            len = 0;
        }else {
            appendlog(TTDLogger::LOG_ERROR,"SafeCheck SetKey error");
            len = -1;
        }
    }
    return len;

}


int EvhtpSvr::FireWall(string sMethod,evhtp_request_t * request)
{


#ifdef FORSMS

    TConvert tConvert;
    if( sMethod == "CodeTTDv2"){

        return 0;

        sockaddr_in sin;
        memcpy(&sin, request->conn->saddr, sizeof(sin));
        string sip = inet_ntoa(sin.sin_addr);
        appendlog(TTDLogger::LOG_ERROR,"EvhtpSvr::FireWall src:%s",sip.c_str());

        bool bWhitePass = false;
        if( mapWhiteName.find(sip) != mapWhiteName.end()){
            bWhitePass = true;
        }
        MyDAL dal;
        int count; string first,last;
        int iRet = dal.GetEntity(sip,count,first,last);
        if( iRet == 0 ){


            //            int lFirst = tConvert.StrToTime(first.c_str());
            //            int lLast = tConvert.StrToTime(last.c_str());
            //            int lDis =   lLast - lFirst;
            //            if( lDis > 60){
            //                dal.UpFirstTime(sip);
            //            }
            //            else if( lDis > 0 ){
            //                //dal.IncreemOne(sip);
            //                if( count/lDis > 10 ){
            //                    return -1;
            //                }
            //            }else if(lDis == 0){
            //                dal.IncreemOne(sip);
            //            }
            dal.IncreemOne(sip);
            if( count > 10 && !bWhitePass){
                //return -1;
            }


        }else{
            dal.AddData(sip);
        }

//        string Accept_Encoding = mapHeadIn["Accept-Encoding"];
//        string User_Agent = mapHeadIn["User-Agent"];

//        if( User_Agent.find("SAMSUNG") != string::npos || Accept_Encoding == "gbk, GB2312"){
//            return -1;
//        }

    }
#endif
    return 0;

}

//安全检验
int EvhtpSvr::SafeCheck(const string & psAppid,const string & psTimeStamp,const string & psSign,const string & psPostData){


    TConvert tConvert;

    if(msIsCheckSafe == "false") {
        return 0;
    }

    if( psAppid.empty()){
        appendlog(TTDLogger::LOG_ERROR,"SafeCheck param empty");
        return -1;
    }





    if( psAppid.empty() || psTimeStamp.empty() || psSign.empty() ){
        appendlog(TTDLogger::LOG_ERROR,"SafeCheck param empty");
        return -1;
    }


    string sAppKey = "";
    string sAppId = "425388c9c928c3d36856e67b6d86cc11";
    map<string,string>::iterator iter= mAppId_AppKeyAesMap.find(sAppId);
    if( iter != mAppId_AppKeyAesMap.end() ) {
        string* sValue = &iter->second;
        sAppKey = sValue->substr(0,32);
    }



    //    long tSecondeTime = tConvert.GetCurrentTime()/1000000;
    //    if( abs(tSecondeTime - tConvert.StrToLong(psTimeStamp)) > 60){//请求有效时间60秒
    //        appendlog(TTDLogger::LOG_ERROR,"SafeCheck time out err");
    //        return -1;
    //    }


    string sSign = Md5ToLower(sAppKey + psPostData + psTimeStamp + psAppid);
    if( sSign != psSign ){
        appendlog(TTDLogger::LOG_ERROR,"SafeCheck sSign err sAppKey:[%s] psPostData:[%s] psTimeStamp:[%s] psAppid:[%s] MySign:[%s] psSign:[%s]" ,
                  sAppKey.c_str(),psPostData.c_str(),psTimeStamp.c_str() , psAppid.c_str(),sSign.c_str(), psSign.c_str());
        return -1;
    }

    return 0;
}


int EvhtpSvr::ToJson( string & s)
{
    if( s.find("=") == string::npos ){
        return 0;
    }
    if( s.find("{") != string::npos ){
        return 0;
    }
    TConvert tConvert;
    StringLst lst;
    tConvert.Split(s,"&",lst);

    StringLst equlLst;
    tConvert.Split(s,"=",equlLst);

    if( equlLst.size() != lst.size()+ 1){
        return 0;
    }
    Json::Value j;
    for(int i = 0; i < lst.size(); i++){
        string & tmp = lst[i];
        StringLst eles;
        tConvert.Split(tmp,"=",eles);
        j[eles.at(0)] = eles.at(1);

    }
    s = j.toStyledString();
    return 0;
}

string EvhtpSvr::GetHead(const string & key) const
{
    if( mapHeadIn.find(key) != mapHeadIn.end()){
        return mapHeadIn.at(key);
    }
    return "";
}

string EvhtpSvr::GetQuery(const string & key) const
{
    if( mapQueryParam.find(key) != mapQueryParam.end()){
        return mapQueryParam.at(key);
    }
    return "";
}

string EvhtpSvr::GetUrl() const
{

    return mUrl;
}

const Json::Value & EvhtpSvr::GetJsonBody() const
{
    return mJsonbody;
}
Json::Value & EvhtpSvr::GetJsonResponse()
{
    return mJsonResponse;
}

    const Json::Value & EvhtpSvr::GetJsonDetoken()const
    {
        return mJsonDetoken;
    }

