
#include "MyAES.h"
#include <time.h>
#include "Base64EnDecode.h"
MyAES::MyAES()
{
    memset(iv,0,16);
}

MyAES::~MyAES()
{

}


void MyAES::SetKey(byte * k, int keylen)
{
    memcpy(key,k,keylen);
    memcpy(key + keylen,defaultCompleteString.c_str(),32 - keylen);
    key_length = 32;
}

void MyAES::SetKey2(byte * k, int keylen)
{
    memcpy(key,k,keylen);
    key_length = keylen;
}

/*
 * Description: use key to encrypt 'plainText', return the cipher
 * Input:
 * 	plainText: the string need to be encrypted
 * OutPUt:
 * 	return the cipher
 */
string MyAES::Encrypt(const string &plainText ,  int appendlen)
{

    string append = defaultCompleteString.substr(0,appendlen) + plainText;
    string cipher;
    try{
        CBC_Mode<AES>::Encryption aesEncryptor(key, key_length, iv);
        StringSource(append, true,
                     new StreamTransformationFilter(aesEncryptor,
                                                    new StringSink(cipher)));
    }catch(...){
        return "";
    }
    Base64EnDecode base64EnDecode;
    return base64EnDecode.Base64Encode((const unsigned char*)cipher.c_str(),cipher.length());
}

string MyAES::EncryptNoBase(const string &plainText, string srcIv)
{
    memcpy(iv,srcIv.c_str(),srcIv.length());
    string append = plainText;
    string cipher;
    try{
        CBC_Mode<AES>::Encryption aesEncryptor(key, key_length, iv);
        StringSource(append, true,
                     new StreamTransformationFilter(aesEncryptor,
                                                    new StringSink(cipher)));
    }catch(...){
        return "";
    }
    return cipher;

}

/*
 * Description: use the same key to decrypt "cipher" and return the plainText
 * Input:
 * 	cipher: the string to be decrypted
 * Output:
 * 	return the recover
 */
string MyAES::Decrypt(const string & cipher,  int appendlen)
{

    if(cipher.empty())
        return "";
    string recover;
    int outByte;
    Base64EnDecode base64EnDecode;
    string debase64cipher = base64EnDecode.Base64Decode(cipher.c_str(),cipher.length(),outByte);
    if( debase64cipher.empty()){
        return "";
    }
    try{
        CBC_Mode<AES>::Decryption aesDecryptor(key, key_length, iv);
        StringSource(debase64cipher, true,
                     new StreamTransformationFilter(aesDecryptor,
                                                    new StringSink(recover)));
    }catch(...){
        return "";
    }

    return recover.substr(appendlen);
}

string MyAES::DecryptNotBase(const string & cipher, string srcIv)
{
    memcpy(iv,srcIv.c_str(),srcIv.length());

    if(cipher.empty())
        return "";
    string recover;

    try{
        CBC_Mode<AES>::Decryption aesDecryptor(key, key_length, iv);
        StringSource(cipher, true,
                     new StreamTransformationFilter(aesDecryptor,
                                                    new StringSink(recover)));
    }catch(...){
        return "";
    }

    return recover;
}

/*
 * Description: use the key to encrypt the 'inFilename' and store the cipher in 'outFilname'
 * Input:
 *  inFilename: the file need to be encrypted!
 *  outFilename: the encrypted file
 * OutPut:
 *  if encrypt success, return true, or return false
 * Others: the function should delete the file : 'inFilename', however I note it
 */
bool MyAES::EncryptFile(const string & inFilename, const string & outFilename)
{
    // check if the file 'inFilename' exists.
    if (access(inFilename.c_str(), 0) == -1)
    {
        cout << "The file " << inFilename << " is not exist! " << endl;
        return false;
    }
    // file exists, the encrypt the file
    CBC_Mode<AES>::Encryption aesEncryptor(key, key_length, iv);

    FileSource(inFilename.c_str(), true,
               new StreamTransformationFilter(aesEncryptor,
                                              new FileSink(outFilename.c_str())));
    // remove the file 'inFilename'
    // if(remove(inFilename) == 0) cout << "remove file succeed! " << endl;
    // 		else cout << "fail to remove the file " << inFilname << endl;
    // use function remove(), you have to add #include <cstdio> in the .h file
    return true;
}

/*
 * Description: use the same key to decrypt the 'decFilename' and create recoverFile
 * Input:
 * 	decFilename: the encrypted file name
 * 	recoverFilename: the decrypted file name
 * OutPut:
 * 	if decrypted the file successful, return true, else return false
 * Others: we should also delete the file 'decFilename'
 */
bool MyAES::DecryptFile(const string & decFilename,
                        const string & recoverFilename)
{
    // check if the file 'decFilename' exists!
    if (access(decFilename.c_str(), 0) == -1)
    {
        cout << "The file " << decFilename << " is not exist! " << endl;
        return false;
    }
    // exist , then decrypt the file
    CBC_Mode<AES>::Decryption aesDecryptor(key, key_length, iv);
    FileSource(decFilename.c_str(), true,
               new StreamTransformationFilter(aesDecryptor,
                                              new FileSink(recoverFilename.c_str())));
    return true;
}
