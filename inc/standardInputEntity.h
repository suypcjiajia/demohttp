#ifndef STANDARDINPUTENTITY_H
#define STANDARDINPUTENTITY_H

#include <string>
#include <vector>
#include <mysql/mysql.h>
#include "BaseEntity.h"
using namespace std;


struct standardInputEntity:public BaseEntity
{
    standardInputEntity(){
        type = -1;

    }
    int type ; //'类型 0-环境空气 1-物体表面 2-医务人员手 3-消毒剂及器械保存液 4-透析液 5-消毒灭菌效果监测 6-污水排放 7-医疗用品',
    string value0 ; //'录入值0',
    string value1 ;// '录入值1',
    string value2 ;// '录入值2',
    string value3 ;// '录入值3',
    string value4 ;// '录入值4',
    string value5 ;// '录入值5'
    void Row2Entity(MYSQL_ROW row){
        int iIndex = 0;
        id = atoi( row[iIndex++] );
        type = atoi( row[iIndex++] );
        value0 =row[iIndex++];
        value1 =row[iIndex++];
        value2 =row[iIndex++];
        value3 =row[iIndex++];
        value4 =row[iIndex++];
        value5 =row[iIndex++];
    }
};
typedef vector<standardInputEntity> standardInputList;


#endif // STANDARDINPUTENTITY_H
