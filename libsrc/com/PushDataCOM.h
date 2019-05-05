#ifndef PushDataCOM_H
#define PushDataCOM_H

#include <string>
using namespace std;

class PushDataCOM
{
public:
    PushDataCOM();
    ~PushDataCOM();
    //PushData
     int PushData(const string& psPushIP, const string& psPushData);
private:

};

#endif // PushDataCOM_H
