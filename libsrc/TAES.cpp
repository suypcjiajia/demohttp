
#include "TAES.h"
#include <time.h>

/*
 * Description: use key to encrypt 'plainText', return the cipher
 * Input:
 * 	plainText: the string need to be encrypted
 * OutPUt:
 * 	return the cipher
 */
string TAES::Encrypt(const string &sData ,  unsigned char* cKey)
{
    string cipher;
    ECB_Mode<AES>::Encryption aesEncryptor(cKey, CryptoPP::AES::DEFAULT_KEYLENGTH);
    StringSource(sData, true, new StreamTransformationFilter(aesEncryptor, new StringSink(cipher)));
    return cipher;
}

/*
 * Description: use the same key to decrypt "cipher" and return the plainText
 * Input:
 * 	cipher: the string to be decrypted
 * Output:
 * 	return the recover
 */
string TAES::Decrypt(const string & sData,  unsigned char* cKey)
{
    string sDecodeData;
    ECB_Mode<AES>::Decryption aesDecryptor(cKey, CryptoPP::AES::DEFAULT_KEYLENGTH);
    StringSource(sData, true, new StreamTransformationFilter(aesDecryptor, new StringSink(sDecodeData)));
    return sDecodeData;
}

