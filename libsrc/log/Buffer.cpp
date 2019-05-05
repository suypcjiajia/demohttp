#include "Buffer.h"
namespace TTDLogger{


Buffer::Buffer()
{

}

void Buffer::append(string line)
{
    ScopeLock lock( this->mutex_ );

    buf_.push_back(line);

}

string Buffer::getLine()
{
    if( buf_.size() == 0)
        return "";
    string line = *buf_.begin();
    buf_.erase(buf_.begin());
    return line;

}

string Buffer::getLines(int l)
{
    ScopeLock lock( this->mutex_ );
    string lines;
    for( ; l > 0 ; l--){
        if( buf_.size() == 0)
            break;
        string line = *buf_.begin();
        buf_.erase(buf_.begin());
        lines += line;

    }

    return lines;

}




}

