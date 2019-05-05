#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>
#include "MyAES.h"
using namespace std;
class Token
{
public:
    //生成：登陆成功后的token
    static string MakeLoginToken(const string & sOrg);
    //unpack登陆成功后的token
    static string UnAesLoginToken(const string & sAesStr);
    static MyAES mMyaes;
    static byte mKey[32];
    static int mApplendLen;

};

#endif // TOKEN_H
