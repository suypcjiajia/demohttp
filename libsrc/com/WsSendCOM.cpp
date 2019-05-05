#include "WsSendCOM.h"

#include "MyAES.h"
#include "TConvert.h"
#include "log/Logger.h"

WsSendCOM::WsSendCOM()
{

}

WsSendCOM::~WsSendCOM()
{

}
//Send
void WsSendCOM::WsSendByClient(client* pClient,websocketpp::connection_hdl pHdl,const string& psSendData)
{
    pClient->send(pHdl, psSendData, websocketpp::frame::opcode::value::text);
    return;
    //加密
    MyAES myAES;
    myAES.SetKey((unsigned char*)this->msAesKey.c_str(),32);
    string sSendData = myAES.Encrypt(psSendData,0);

    pClient->send(pHdl, sSendData, websocketpp::frame::opcode::value::text);
}
void WsSendCOM::WsSendByServer(server* pServer, websocketpp::connection_hdl pHdl, const string& psSendData)
{
    //websocketpp::lib::error_code err;
    pServer->send(pHdl, psSendData, websocketpp::frame::opcode::value::text);
    //appendlog(TTDLogger::LOG_DEBUG,"webcocket send retmsg:%s", err.message().c_str());
    return;
    //加密
    MyAES myAES;
    myAES.SetKey((unsigned char*)this->msAesKey.c_str(),32);
    string sSendData = myAES.Encrypt(psSendData,0);

    pServer->send(pHdl, sSendData, websocketpp::frame::opcode::value::text);
}
//Receive
const string WsSendCOM::WsReceive(const string& psReceiveData)
{
    return psReceiveData;
    //加密
    MyAES myAES;
    myAES.SetKey((unsigned char*)this->msAesKey.c_str(),32);
    return myAES.Decrypt(psReceiveData,0);
}
