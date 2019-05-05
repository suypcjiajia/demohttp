/*
 * MyAES.h
 *
 *  Created on: 2013-3-6
 *      Author: hust
 */

#ifndef MYAES_H_
#define MYAES_H_

#include <cryptopp/aes.h>
#include <cryptopp/default.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>

#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>


using namespace CryptoPP;
using namespace std;

class MyAES
{
public:
    byte  key[32];
    byte  iv[16];
    int key_length;


    MyAES();
    ~MyAES();

    //use the key to encrypt the plainText and return the cipher
    string Encrypt(const string &plainText, int appendlen);
    //use the same key to decrypt the cipher and return the recover
    string Decrypt(const string &cipher, int appendlen);
    string DecryptNotBase(const string & cipher, string srcIv);
    //use the key to encrypt the file
    bool EncryptFile(const string & inFilename, const string & outFilename);
    //use the key to decyrpt the file
    bool DecryptFile(const string & DecFilename,
            const string & recoverFilename);
    void SetKey(byte * k, int keylen);
    void SetKey2(byte * k, int keylen);
    const string defaultCompleteString = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";


    string EncryptNoBase(const string &plainText, string srcIv);
};

#endif /* MYAES_H_ */
