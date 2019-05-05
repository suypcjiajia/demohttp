#ifndef WsSendCOM_H
#define WsSendCOM_H

#include <string>
//apt install libboost-dev
//cp mv libboost_system.so.1.54.0 /tiyoume/lib/
//ln -fs libboost_system.so.1.54.0 libboost_system.so
//CONFIG += c++11

#include <set>
#include <websocketpp/config/asio.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/server.hpp>
//#include <websocketpp/transport/asio/endpoint.hpp>


using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::lib::shared_ptr<boost::asio::ssl::context> context_ptr;

struct testee_config : public websocketpp::config::asio {
    // pull default settings from our core config
    typedef websocketpp::config::asio core;

    typedef core::concurrency_type concurrency_type;
    typedef core::request_type request_type;
    typedef core::response_type response_type;
    typedef core::message_type message_type;
    typedef core::con_msg_manager_type con_msg_manager_type;
    typedef core::endpoint_msg_manager_type endpoint_msg_manager_type;

    typedef core::alog_type alog_type;
    typedef core::elog_type elog_type;
    typedef core::rng_type rng_type;
    typedef core::endpoint_base endpoint_base;

    static bool const enable_multithreading = true;

    struct transport_config : public core::transport_config {
        typedef core::concurrency_type concurrency_type;
        typedef core::elog_type elog_type;
        typedef core::alog_type alog_type;
        typedef core::request_type request_type;
        typedef core::response_type response_type;

        static bool const enable_multithreading = true;
    };

    typedef websocketpp::transport::asio::endpoint<transport_config>
        transport_type;

    static const websocketpp::log::level elog_level =
        websocketpp::log::elevel::all;
    static const websocketpp::log::level alog_level =
        websocketpp::log::alevel::all;
};

//SSL开关
//#define WSSSL
#ifdef WSSSL
typedef websocketpp::server<websocketpp::config::asio_tls> server;
typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
#else
typedef websocketpp::server<websocketpp::config::asio> server;
//typedef websocketpp::server<testee_config> server;
typedef websocketpp::client<websocketpp::config::asio_client> client;
#endif
typedef websocketpp::lib::shared_ptr<boost::asio::ssl::context> context_ptr;
using namespace std;

class WsSendCOM
{
public:
    WsSendCOM();
    ~WsSendCOM();
    //WsSend
    void WsSendByClient(client* pClient, websocketpp::connection_hdl pHdl, const string& psSendData);
    void WsSendByServer(server* pServer, websocketpp::connection_hdl pHdl, const string& psSendData);
    //Receive
    const string WsReceive(const string& psReceiveData);
private:
    const string msAesKey="tym1234567890123tym1234567890123";
};

#endif // WsSendCOM_H
