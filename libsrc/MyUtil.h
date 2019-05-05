#ifndef MYUTIL_H
#define MYUTIL_H

#include <string>
#include <vector>
using namespace std;
class MyUtil
{
public:
    //获取UUID
    static string GetUUID();
    //获取一个文件夹下的所有文件名
    static int getFilesFromDir(string path, vector<string>& files);

    //产生一个大于等于0小于max的随机数
    static int GetRand(int max);

    //生面随机len de num
    static string MakeRandNum(int len);

    //产生一个 unix秒+6位随机数
    static string MakeUnixtimeAnd6Rand();

    //判断手机号合法
    static bool IsRightPhone(string account);

    // 功能：四舍五入（double），支持正负数
    // dSrc ： 待四舍五入之数
    // iBit ： 保留的小数位数。 0 - 不保留小数、1 - 保留一位小数
    // 返回值：返回计算结果
    //
    static double Round(double src,int bit);



    // 功能：向下舍入（double），支持正负数
    // dSrc ： 待向下舍入之数
    // iBit ： 保留的小数位数。 0 - 不保留小数、1 - 保留一位小数
    // 返回值：返回计算结果
    //
    static double Floor(double src,int bit);

};

#endif // MYUTIL_H
