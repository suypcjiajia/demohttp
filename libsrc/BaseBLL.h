#ifndef BASEBLL_H
#define BASEBLL_H

#include <string>
#include <vector>
#include "com/EvhtpSvr.h"
using namespace std;


class BaseBLL
{
public:

    //virtual ~BaseBLL();
    virtual int Process(EvhtpSvr * pEvhtp) =0;


public:


};

#endif // BASEBLL_H
