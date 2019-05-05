//工厂模式，生产业务实例
#ifndef FACTORY_H
#define FACTORY_H
#include "BaseBLL.h"

class Factory
{
public:
    Factory();
    BaseBLL * GetBLL(const string & url);
    void MakeBLL();

private:
    std::map<string,BaseBLL*> mBllObject;

};

#endif // FACTORY_H
