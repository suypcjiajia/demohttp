#ifndef MEPAYENCODE_H
#define MEPAYENCODE_H

#include <string>
using namespace std;
class Base64EnDecode
{
public:
    Base64EnDecode();
    string Base64Encode(const unsigned char* Data,int DataByte);
    string Base64Decode(const char* Data,int DataByte,int& OutByte);
};

#endif // MEPAYENCODE_H
