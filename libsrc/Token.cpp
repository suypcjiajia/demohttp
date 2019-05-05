#include "Token.h"
#include <stdio.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "TConvert.h"

MyAES Token::mMyaes;
byte Token::mKey[32] =  "ttdqwertyu";
int Token::mApplendLen = 8;

//生成：登陆成功后的token
string Token::MakeLoginToken(const string & sOrg)
{
    mMyaes.SetKey(mKey,strlen((char*)mKey));
    return mMyaes.Encrypt(sOrg,mApplendLen);
}

//unpack登陆成功后的token
string Token::UnAesLoginToken(const string & sAesStr)
{
    mMyaes.SetKey(mKey,strlen((char*)mKey));
    return mMyaes.Decrypt(sAesStr,mApplendLen);
}

