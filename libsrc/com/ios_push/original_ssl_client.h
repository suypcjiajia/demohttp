#ifndef original_ssl_client_h
#define original_ssl_client_h

#pragma once
#include <iostream>
#include <string>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "openssl/pem.h"
#include "openssl/rsa.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/rand.h"
#include "openssl/x509.h"
#include "openssl/crypto.h"
#include "TMutex.hpp"
#include "bll/TBLL.h"
#include "log/Logger.h"
#include "json/json.h"
using namespace std;

int myssl_verify_callback(int preverify_ok, X509_STORE_CTX *ctx);

class original_ssl_client
{
public:
    original_ssl_client()
    {
        m_pctx          = NULL;
        m_sockfd        = -1;
        m_bsock         = false;
        m_phost_info    = NULL;
        m_pssl          = NULL;
        memset(m_recv_buffer,0,MAX_BUFFER_RECEIVE);

    }
    ~original_ssl_client()
    {

    }

private:
    //SSL_METHOD*       m_pmeth;
    SSL_CTX *       m_pctx;
    int          m_sockfd;
    bool  m_bsock;
    sockaddr_in     m_server_addr;
    struct hostent* m_phost_info;
    SSL*            m_pssl;
    enum
    {
        MAX_BUFFER_RECEIVE = 1024,
    };

    char            m_recv_buffer[MAX_BUFFER_RECEIVE];

public:
    //
    void Close()
    {

        if( m_pssl != NULL){
             // 关闭SSL套接字
            SSL_shutdown(m_pssl);
            // 释放SSL套接字
            SSL_free(m_pssl);
        }



        if( m_pctx != NULL){
          // 释放SSL会话环境
           SSL_CTX_free(m_pctx);
        }

        if( m_sockfd != -1){
            // 关闭tcp 套接字
            close(m_sockfd);
        }

    }

    // 初始化ssl库，Windows下初始化WinSock
    void init_openssl()
    {

        //#ifdef _WIN32 WSADATA wsaData;
        //    WSADATA wsaData;
        //    WSAStartup(MAKEWORD(2, 2), &wsaData);
        //#endif

        static bool bInit = false;

        if( !bInit){
            SSL_library_init();
            ERR_load_BIO_strings();
            SSL_load_error_strings();
            OpenSSL_add_all_algorithms();
        }
        bInit = true;
    }

    //
    bool init_tcp_connect(const char* host, int port)
    {
        if ( !host )
            return false;

        struct hostent *hp;
        //struct sockaddr_in m_server_addr;
        if (!(hp = gethostbyname(host)))        // 解析域名
            return false;

        memset(&m_server_addr, 0, sizeof(m_server_addr));
        m_server_addr.sin_addr  = *(struct in_addr*)hp->h_addr_list[0];
        m_server_addr.sin_family = AF_INET;
        m_server_addr.sin_port  = htons(port);
        if ((m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        {
            cout<<"Could not get Socket"<<endl;
            return false;
        }

        if (connect(m_sockfd, (struct sockaddr*)&m_server_addr, sizeof(m_server_addr)) != 0)
        {
            m_bsock = true;
            if( m_sockfd != -1){
                // 关闭tcp 套接字
                close(m_sockfd);
                m_sockfd = -1;
            }
            throw "socket connect err";
            return false;
        }

        return true;
    }

    // 创建SSL Context
    SSL_CTX* init_ssl_context( const char* clientcert, /* 客户端的证书 */ const char* clientkey, /* 客户端的Key */ const char* keypwd, /* 客户端Key的密码, 如果有的话 */ const char* cacert) /* 服务器CA证书 如果有的话 */
    {
        // set up the ssl context
        m_pctx = SSL_CTX_new((SSL_METHOD*)TLSv1_client_method());
        if (!m_pctx) { return NULL; }

        // 要求校验对方证书
        //SSL_CTX_set_verify(m_pctx,SSL_VERIFY_PEER |SSL_VERIFY_CLIENT_ONCE , myssl_verify_callback);

        // certificate
        if (clientcert && SSL_CTX_use_certificate_file(m_pctx, clientcert, SSL_FILETYPE_PEM) <= 0)
        { return NULL; }

        // key
        if ( clientkey )
        {
            SSL_CTX_set_default_passwd_cb_userdata(m_pctx,(void*)keypwd);

            if (SSL_CTX_use_PrivateKey_file(m_pctx, clientkey, SSL_FILETYPE_PEM) <= 0)
            { return NULL; }

            // make sure the key and certificate file match
            if (SSL_CTX_check_private_key(m_pctx) == 0)
            { return NULL; }
        }


        // load ca if exist
        if ( cacert )
        {
            if (!SSL_CTX_load_verify_locations(m_pctx, cacert, NULL))
            { return NULL; }
        }

        return m_pctx;
    }
    // 实现SSL握手，建立SSL连接
    SSL* ssl_connect( )
    {
        m_pssl = SSL_new(m_pctx);
        //BIO *bio = BIO_new_socket(m_sockfd, BIO_NOCLOSE);
        //SSL_set_bio(m_pssl, bio, bio);
        SSL_set_fd(m_pssl,m_sockfd);
        int ret = SSL_connect(m_pssl);
        if ( ret <= 0)
        {
            int nErr = SSL_get_error(m_pssl,ret);   // SSL_ERROR_SSL 1,  SSL_ERROR_SYSCALL 5

            char err_msg[1024];
            ERR_error_string_n(ERR_get_error(), err_msg, sizeof(err_msg));
            printf("%s\n", err_msg);

            ERR_print_errors_fp(stderr);
            std::cout<<ssl_error_string().c_str()<<endl;
            return NULL;
        }

        return m_pssl;
    }

    // 验证服务器证书
    // 首先要验证服务器的证书有效，其次要验证服务器证书的CommonName(CN)与我们
    // 实际要连接的服务器域名一致
    bool verify_connection(const char* peername)
    {
        // 获取校验结果
        int result = SSL_get_verify_result(m_pssl);
        if (result != X509_V_OK && result != X509_V_ERR_CERT_UNTRUSTED && result != X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY)
        {
            fprintf(stderr, "WARNING! ssl verify failed: %d \n", result);
            std::cout<<ssl_error_string().c_str()<<endl;
            return false;
        }

        //      X509 *peer;
        //      char peer_CN[256] = {0};
        //      peer = SSL_get_peer_certificate(m_pssl);
        //      X509_NAME_get_text_by_NID(X509_get_subject_name(peer), NID_commonName, peer_CN, 255);
        //      if (strcmp(peer_CN, peername) != 0)
        //      {
        //          fprintf(stderr, "WARNING! Server Name Doesn't match, got: %s, required: %s", peer_CN, peername);
        //      }

        return true;
    }

    std::string ssl_error_string( )
    {
        //SSL_get_error();
        unsigned long ulErr = ERR_get_error();  // 获取错误号
        char szErrMsg[1024] = {0};
        char *pTmp = NULL;
        pTmp = ERR_error_string(ulErr,szErrMsg); // 格式：error:errId:库:函数:原因
        return szErrMsg;
    }

    void ssl_send_keyinput_msg( )
    {
        while ( true)
        {
            usleep(100*1000);

            if( true )
            {
                char szInput[100] = {};
                cout<<"commond: "<<endl;
                cin.getline(szInput,sizeof(szInput),'\n');
                if ( strcmp(szInput,"exit") == 0 )
                    break;

                char token[]        = "ca1d31b8534807ef73c3af3e2a13c97bc5e3dc4279a63b4585ca935d4ef488c7";
                char format[]       = "{\"aps\":{\"alert\":\"%s\",\"badge\":1}}";
                char payload[256]   = {};
                sprintf(payload,format,szInput);
                int ret = pushMessage(token, payload);
                cout<<"push ret["<<ret<<"]"<<endl;
                cout<<"1sss\n"<<endl;
            }
            cout<<"1\n"<<endl;
            recv_message();
            cout<<"2\n"<<endl;
        }
    }

    int initializeSSL(  )
    {
        /*/
        char host[]     = "gateway.sandbox.push.apple.com";
        int port        = 2195;
        char password[] = "hc123";
        #const char*     CERTFILE_PATH =          "boost/PushChatCert.pem";
        #const char*     CERTKEY_PATH  =          "boost/PushChatKey.pem";
        #const char*     CACERT_PATH   =          "boost/sandbox.pem";
        /*/
        const char*     CERTFILE_PATH =          NULL;
        const char*     CERTKEY_PATH  =          NULL;
        const char*     CACERT_PATH   =          "boost/ca.pem";
        char host[]     = "localhost";
        int port        = 13;
        char password[] = "test";
        //*/
        char token[]    = "ca1d31b8534807ef73c3af3e2a13c97bc5e3dc4279a63b4585ca935d4ef488c7";
        char payload[]  = "{\"aps\":{\"alert\":\"kkkkkkk\",\"badge\":1,\"sound\":\"default\"},}";

        int err;
        SSL_library_init();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();       // 支持所有算法

        m_pctx = SSL_CTX_new((SSL_METHOD*)SSLv3_client_method());
        if( !m_pctx )   {
            cout<<"Could not get SSL Context"<<endl;
            return false;
        }
        // 要求校验对方证书
        SSL_CTX_set_verify(m_pctx,SSL_VERIFY_PEER/*|SSL_VERIFY_CLIENT_ONCE*/, myssl_verify_callback);

        if(SSL_CTX_load_verify_locations(m_pctx, NULL, CACERT_PATH) <= 0)
        {
            cout<<"Failed to set CA location"<<endl;
            ERR_print_errors_fp(stderr);
            return false;
        }


        if(CERTFILE_PATH && SSL_CTX_use_certificate_file(m_pctx,CERTFILE_PATH,SSL_FILETYPE_PEM) <= 0)
        {
            cout<<"Cannot use Certificate File"<<endl;
            ERR_print_errors_fp(stderr);
            return false;
        }

        if ( CERTKEY_PATH )
        {
            SSL_CTX_set_default_passwd_cb_userdata(m_pctx,password);

            if (SSL_CTX_use_PrivateKey_file(m_pctx, CERTKEY_PATH, SSL_FILETYPE_PEM) <= 0)
            {
                cout<<"Cannot use Private Key"<<endl;
                ERR_print_errors_fp(stderr);
                return false;
            }

            if (!SSL_CTX_check_private_key(m_pctx))
            {
                cout<<"Private key does not match the certificate public key"<<endl;
                return false;
            }
        }

        //WSADATA wsaData;
        //WSAStartup(MAKEWORD(2, 2), &wsaData);
        if ((m_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)     {
            cout<<"Could not get Socket"<<endl;
            return false;
        }

        memset (&m_server_addr, '\0', sizeof(m_server_addr));
        m_server_addr.sin_family      = AF_INET;
        m_server_addr.sin_port        = htons(port);
        m_phost_info                  = gethostbyname(host);
        if( m_phost_info )
        {
            struct in_addr *address = (struct in_addr*)m_phost_info->h_addr_list[0];
            m_server_addr.sin_addr.s_addr = inet_addr(inet_ntoa(*address));
        }
        else
        {
            cout<<"Could not resolve hostname = "<<host<<endl;
            return false;
        }

        err = connect(m_sockfd, (struct sockaddr*)&m_server_addr, sizeof(m_server_addr));
        if(err == -1)
        {
            cout<<"Could not connect"<<endl;
            return false;
        }

        m_pssl = SSL_new(m_pctx);
        if( !m_pssl )   {
            cout<<"Could not get SSL Socket"<<endl;
            return false;
        }

        if( SSL_set_fd(m_pssl, m_sockfd) == -1 )
            return false;

        err = SSL_connect(m_pssl);
        if(err <= 0  )  {
            //ERR_print_errors_fp(stderr);
            cout<<ssl_error_string().c_str()<<endl;
            cout<<"Could not connect to SSL Server"<<endl;
            return false;
        }

        // 获取证书验证结果
        int result = SSL_get_verify_result(m_pssl);
        if (result != X509_V_OK && result != X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN)
        {
            fprintf(stderr, "WARNING! ssl verify failed: %d \n", result);
            std::cout<<ssl_error_string().c_str()<<endl;
            return false;
        }


        return true;
    }

    int pushMessage(const char * token, const char * payload)
    {
        char tokenBytes[128];
        char message[2048];
        unsigned long msgLength;

        token2bytes( token, tokenBytes );
        unsigned short payloadLength    = strlen( payload );
        char * pointer = message;
        unsigned short networkTokenLength   = htons( (unsigned short)32 );
        unsigned short networkPayloadLength = htons( (unsigned short)payloadLength );
        // command
        //*/
        unsigned char command           = 0;
        memcpy(pointer, &command, sizeof(unsigned char));
        pointer += sizeof(unsigned char);
        /*/
        unsigned char command           = 1;
        memcpy(pointer, &command, sizeof(unsigned char));
        pointer += sizeof(unsigned char);
        // identityfer
        boost::uint32_t identityfer = 1;
        memcpy(pointer, &identityfer, 4);
        pointer += 4;
        // expiry
        boost::uint32_t tExpiry = time(NULL) + 24*3600;
        memcpy(pointer, &tExpiry, 4);
        pointer += 4;
        //*/

        // token len
        memcpy(pointer, &networkTokenLength, sizeof(unsigned short));
        pointer += sizeof(unsigned short);
        // token
        memcpy(pointer, tokenBytes, sizeof(tokenBytes));
        pointer += 32;
        // payload len
        memcpy(pointer, &networkPayloadLength, sizeof(unsigned short));
        pointer += sizeof(unsigned short);
        // payload
        memcpy(pointer, payload, payloadLength);
        pointer += payloadLength;
        // clac len
        msgLength = pointer - message;
        return SSL_write( m_pssl, message, (int)msgLength );
    }

    void recv_message( )
    {
        int nRealRead = SSL_read(m_pssl,m_recv_buffer,MAX_BUFFER_RECEIVE);
        if ( nRealRead <= 0 )
        {
            int nErr = SSL_get_error(m_pssl, nRealRead);    // SSL_ERROR_SSL 1,  SSL_ERROR_SYSCALL 5

            char err_msg[1024];
            ERR_error_string_n(ERR_get_error(), err_msg, sizeof(err_msg));
            printf("%s\n", err_msg);
        }
        else
        {
            std::cout<<m_recv_buffer<<endl;
            memset(m_recv_buffer,0,MAX_BUFFER_RECEIVE);
        }
    }

    void token2bytes(const char *token, char *bytes)
    {
        int val;
        while (*token)
        {
            sscanf(token, "%2x", &val);
            *(bytes++) = (char)val;
            token += 2;
            while (*token == ' ') {
                ++token;                // skip space
            }
        }
    }

    int InitSandbox()
    {
        this->init_openssl();
        this->init_tcp_connect("gateway.sandbox.push.apple.com",2195);
        this->init_ssl_context("/tiyoume/push/dev_cer.pem","/tiyoume/push/dev_key.pem","175947400",NULL);
        return 0;
    }
    int InitPush()
    {
        this->init_openssl();
        this->init_tcp_connect("gateway.push.apple.com",2195);
        this->init_ssl_context("/tiyoume/push/pub_cer.pem","/tiyoume/push/pub_key.pem","175947400",NULL);
        return 0;
    }

    int SslSendMsg( const string& token,const string& sMsg )
    {
        int iRet = -1;
        //
        if ("true" == TBLL::GetInstance()->GetIosSandbox()) {
            this->InitSandbox();
        } else {
            this->InitPush();
        }
        //
        this->ssl_connect();
        if ( ! this->verify_connection(NULL) ){
            iRet = -1;
        }

        iRet =  pushMessage(token.c_str(), sMsg.c_str());
        //appendlog(TTDLogger::LOG_DEBUG,"smg:%s",sMsg.c_str());


        this->Close();

        return iRet;
    }


};

#endif
