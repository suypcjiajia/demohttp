//工厂模式，生产业务实例

#include "TFile.h"
#include "factory.h"
#include "UserDataBLL.h"
#include "SettingBLL.h"
#include "standardInputBLL.h"
Factory::Factory()
{

}
BaseBLL * Factory::GetBLL(const string & url){
    TFile tFile;std::vector<string> lst;
    if( url.empty())
        return NULL;
    tFile.Split(url,"/",lst);
    if( lst.size() < 2)
        return NULL;

    string key = lst.at(1);

    std::map<string,BaseBLL*>::const_iterator it   = mBllObject.find(key);
    if( it != mBllObject.end()){
        return it->second;
    }
    return NULL;

}


void Factory::MakeBLL(){
    BaseBLL * p =  new UserDataBLL;
    mBllObject["user"] = p;
    p = new  SettingBLL;
    mBllObject["setting"] = p;
    p = new standardInputBLL;
    mBllObject["standardInput"] = p;
}
