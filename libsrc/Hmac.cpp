#include <openssl/hmac.h>
#include "Hmac.h"
/**
 * HMAC
 *
 * @param sData
 *            待摘要数据
 * @param cKey
 *            密钥
 * @return String 摘要后数据
 */
string Hmac::Get(const string& sData, const string& sKey)
{
    unsigned char hash[32];

//    HMAC_CTX hmac;
//    HMAC_CTX_init(&hmac);
//    HMAC_Init_ex(&hmac, &sKey[0], sKey.length(), EVP_sha256(), NULL);
//    HMAC_Update(&hmac, (unsigned char*) &sData[0], sData.length());
//    unsigned int len = 32;
//    HMAC_Final(&hmac, hash, &len);
//    HMAC_CTX_cleanup(&hmac);

    return (char*)hash;
}


/**
 * HMAC
 *
 * @param sData
 *            待摘要数据
 * @param cKey
 *            密钥
 * @param hash 摘要后数据
 * @return void
 */
void  Hmac::GetSha256(const string& sData, const string& sKey, unsigned char * hash)
{

//    HMAC_CTX hmac;
//    HMAC_CTX_init(&hmac);
//    HMAC_Init_ex(&hmac, &sKey[0], sKey.length(), EVP_sha256(), NULL);
//    HMAC_Update(&hmac, (unsigned char*) &sData[0], sData.length());
//    unsigned int len = 32;
//    HMAC_Final(&hmac, hash, &len);
//    HMAC_CTX_cleanup(&hmac);

    return ;
}
