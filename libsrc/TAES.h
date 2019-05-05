/*
 * MyAES.h
 *
 *  Created on: 2013-3-6
 *      Author: hust
 */

#ifndef MYAES_H_
#define TAES_H_

#include <cryptopp/aes.h>
#include <cryptopp/default.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hmac.h>

#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>


using namespace CryptoPP;
using namespace std;

class TAES
{
public:
    //use the key to encrypt the plainText and return the cipher
    string Encrypt(const string &sData, unsigned char* cKey);
    //use the same key to decrypt the cipher and return the recover
    string Decrypt(const string &sData, unsigned char* cKey);

};

#endif /* TAES_H_ */
