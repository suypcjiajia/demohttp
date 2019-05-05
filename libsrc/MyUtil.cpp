#include "MyUtil.h"
#include <stdio.h>
#include <uuid/uuid.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include "TConvert.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <iostream>

//sudo apt-get install uuid-dev
//获取UUID
string MyUtil::GetUUID(){
    uuid_t uu;
    uuid_generate( uu );
    char buf[33] = {0};
    for(int i=0;i<16;i++)
    {
        sprintf(buf+ i*2,"%02X",uu[i]);
    }
    return buf;
}


int MyUtil::getFilesFromDir(string path, vector<string>& files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp=opendir(path.c_str()))!=NULL)
    {

        while ((dirp=readdir(dp))!=NULL )
        {
            files.push_back(dirp->d_name);
        }
    }
    closedir(dp);

    return 0;
}


int MyUtil::GetRand(int max)
{
    if( max <= 0)
        return 0;

    struct timeval tv;
    gettimeofday( &tv,NULL );
    long lTime =  tv.tv_sec * 1000 * 1000 + tv.tv_usec;
    srand(lTime);
    return rand()%max;
}

//生面随机len de num
string MyUtil::MakeRandNum(int len)
{
    TConvert tConvert;
    string sPwd;
    int i = len;
    while( (i--) > 0){
        srand(tConvert.GetCurrentTime());
        char c = rand()%10 + 48;
        sPwd += c;
    }
    return sPwd;
}

//产生一个 unix秒+6位随机数
string MyUtil::MakeUnixtimeAnd6Rand()
{
    TConvert tConvert;
    return tConvert.LongToStr( tConvert.GetCurrentTime()/1000000 ) + tConvert.IntToStr(GetRand(1000000));

}

//判断手机号合法
bool MyUtil::IsRightPhone(string account)
{
    if( account.length() != 11 || account[0] != '1'){
        return false;
    }
    for( int i = 0; i < account.length(); i++){
        char c = account[i];
        if(c > '9' || c < '0'){
            return false;
        }
    }
    return true;
}



// 功能：四舍五入（double），支持正负数
// dSrc ： 待四舍五入之数
// iBit ： 保留的小数位数。
// 返回值：返回计算结果
//
double MyUtil::Round(double src,int bit)
{
    return round(src*pow(10,bit))/pow(10.0,bit);
}

// 功能：向下舍入（double），支持正负数
// dSrc ： 待向下舍入之数
// iBit ： 保留的小数位数。
// 返回值：返回计算结果
//
double MyUtil::Floor(double src,int bit)
{
    return floor(src*pow(10,bit))/pow(10.0,bit);
}
