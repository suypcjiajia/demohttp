#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <string>
#include <vector>
#include <mysql/mysql.h>
using namespace std;


struct BaseEntity
{
    BaseEntity(){
        id = -1;
    }
    int id;
    virtual ~BaseEntity(){
    }
    virtual void Row2Entity(MYSQL_ROW row){

    }
};


#endif // BASEENTITY_H
