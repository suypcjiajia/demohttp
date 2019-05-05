#ifndef UserDataEntity_H
#define UserDataEntity_H

#include <string>
#include <vector>
#include "BaseEntity.h"
using namespace std;


struct UserDataEntity:public BaseEntity
{
    UserDataEntity(){
        name = "";
        department = "";
        password = "";
        level = -1;
    }
    string name;//用户姓名
    string department;//用户科室
    string password;//用户密码
    int level;//用户级别  0-normal  999-admin

    void Row2Entity(MYSQL_ROW row){
        int iIndex = 0;
        id = atoi( row[iIndex++] );
        name =row[iIndex++];
        department = row[iIndex++];
        password = row[iIndex++];
        level = atoi( row[iIndex++] );
    }
};
typedef vector<UserDataEntity> UserDataList;


#endif // UserDataEntity_H
