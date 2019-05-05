#ifndef SETTINGENTITY_H
#define SETTINGENTITY_H

#include <string>
#include <vector>
#include <mysql/mysql.h>
#include "BaseEntity.h"
using namespace std;


struct SettingEntity:public BaseEntity
{
    SettingEntity(){
        name = "";
        type = -1;
    }
    string name;//
    int type;//
    void Row2Entity(MYSQL_ROW row){
        int iIndex = 0;
        id = atoi( row[iIndex++] );
        name =row[iIndex++];
        type = atoi( row[iIndex++] );
    }
};
typedef vector<SettingEntity> SettingList;


#endif // SETTINGENTITY_H
