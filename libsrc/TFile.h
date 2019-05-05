#ifndef T_FILE_H
#define T_FILE_H

#include <string>
#include <vector>
#include <map>
using namespace std;
// 操作配置文件类
class TFile
{
public:
    // 删除文件
    int Remove( const string& sFileName );
    // 判断文件是否存在
    int IsFileExist( const string& sFileName );
    // 判断目录存在，并创建目录。
    int IsDirExist( const string& sDirPath );
    // 创建文件
    int CreateFile( const string& sFileName );
    // 创建目录
    int CreateDir( const string& sDirName );
    // 创建文件路径目录
    int CreateDirByPathFile( const string& sPathFile );
    // 获取文件大小
    int GetFileSize( const string& sFileName, long& lFileSize );
    // 设置文件字段值
    int SetValue( const string& sFileName,
                  const string& sKey,
                  const string& sValue );
    // 获取文件字段值
    int GetValue( const string& sFileName,
                  const string& sKey,
                  string& sValue);
    // 获取文件字段值toMap
    int GetValueToMap( const string& sFileName,
                       map<string,string>& pMap );
    // 获取文件第一行内容
    int GetTopLine( const string& sFileName, string& sTopLine );
    // 查找文件文本针对每一行
    int IsFindTxt( const string& sFileName,
                    const string& sText );
    // 替换文件文本针对每一行
    int ReplaceTxt( const string& sFile,
                     const string& sSourceTxt,
                     const string& sTargetTxt );
    // 复制文件
    int CopyFile( const string& sSrcFileName, const string& sTargetFileName );
    //保存文件
    int SaveToFile( const long lOffset, const char* cBuff, long FileSize,  const string& sTargetFileName );
    //加载文件
    int LoadToFile( const string& sFileName,const long lOffset,const long lOffsetLength, char* cBuff );
    // 获取文件创建时间
    int GetCreateFileTime(const string & sFileName);

    //字符串分割函数
    int Split( const string& sStr, const string& sPattern, std::vector<std::string>& resultVector );
};

#endif // TFILE_H

