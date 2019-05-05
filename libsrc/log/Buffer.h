#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <string>
#include <stdlib.h>
#include "TMutex.hpp"
using namespace std;

namespace TTDLogger{


class Buffer
{
public:
    Buffer();
    void append(string line);
    string getLine();
    string getLines(int l);

private:
    vector<string> buf_;
    int readableIndex_;
    int writeableIndex_;
    //int maxBuf;//the buffer len pointed by buf
    TMutex mutex_;
};

}

#endif // BUFFER_H
