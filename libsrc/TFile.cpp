#include "TFile.h"
#include <sys/stat.h>
#include <sys/types.h>
#include "log/Logger.h"
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <sstream>
using namespace std;
// 删除文件
int TFile::Remove( const string& sFileName )
{
    if ( remove( sFileName.c_str() ) != 0 ) {
        return -1;
    }

    return 0;
}
// 判断文件是否存在
int TFile::IsFileExist( const string& sFileName )
{
    struct stat buf;
    int iResult;
    iResult = stat( sFileName.c_str(), &buf );
    if ( 0 != iResult ) {
        return -1;
    }
    return 0;
}
// 判断目录存在，并创建目录。
int TFile::IsDirExist( const string& sDirPath )
{
    DIR* pDir=opendir( sDirPath.c_str() );
    if ( pDir == NULL ) {
        return -1;
    }
    closedir(pDir);
    return 0;
}
// 创建文件
int TFile::CreateFile( const string& sFileName )
{
    fstream rFile( sFileName.c_str(), ios::in|ios::out|ios::trunc );
    if ( rFile ) {
        rFile.close();
        return 0;
    }
    return -1;
}
// 创建目录
int TFile::CreateDir( const string& sDirName )
{
    int iRet;
#ifdef _WIN32
    iRet = mkdir( sDirName.c_str() );
#else
    iRet = mkdir( sDirName.c_str(), NULL );
#endif
    if ( iRet != 0 ) {
        return -1;
    }

    return 0;
}
// 创建文件路径目录
int TFile::CreateDirByPathFile( const string& sPathFile )
{
    // 创建目录
    string sDir = "";
    std::vector<std::string> result;
    this->Split(sPathFile,"/",result);
    for(std::size_t i=0; i<result.size(); i++) {
        if ( i == result.size() - 1 ) { break; } // 不操作最后一项文件名
        //OutPuappendlog(TTDLogger:: LOG_ERROR, "CBFile::CopyFile %s\n",result[i].c_str() );
        sDir += result[i] + "/";
        if ( 0 != this->IsFileExist( sDir ) ) {
            if ( 0 !=  this->CreateDir( sDir.c_str() ) ) {
                appendlog(TTDLogger:: LOG_ERROR, "TFile::CreateDirByPathFile %s\n",sDir.c_str() );
                return -1;
            }
            //OutPuappendlog(TTDLogger:: LOG_ERROR, "CBFile::CopyFile dir no %s\n",sDir.c_str() );
        }
    }
    return 0;
}
// 获取文件大小
int TFile::GetFileSize( const string& sFileName, long& lFileSize )
{
    lFileSize = -1;

    struct stat buf;
    int iResult;
    iResult = stat( sFileName.c_str(), &buf );
    if ( 0 == iResult ) {
        lFileSize = buf.st_size;
    } else {       
        return -1;
    }
    return 0;
}

// 获取文件创建时间
int TFile::GetCreateFileTime(const string & sFileName){

    int iTime = 0;
    struct stat buf;
    int iResult;
    iResult = stat( sFileName.c_str(), &buf );
    if ( 0 == iResult ) {
        iTime = buf.st_ctime;
    } else {
        return -1;
    }
    return iTime;
}

// 设置文件字段值
int TFile::SetValue( const string& sFileName,
                       const string& sKey,
                       const string& sValue )
{
    fstream rFile( sFileName.c_str(), ios::in|ios::out );
    if ( rFile ) {
        string sFileTxt = "";       // 记录文件内容
        string sLine;               // 记录每行内容
        bool bIsKeyExist = false;   // 是否存在KEY
        bool bIsChangeKey = false;  // 是否修改
        while ( std::getline( rFile, sLine ) ) {
            if ( sLine.find( sKey + '=' ) != string::npos ) {
                bIsKeyExist = true;
                if ( sLine != ( sKey + '=' + sValue ) ) {
                    sLine = sKey + '=' + sValue;
                    bIsChangeKey = true;
                }
            }
            sFileTxt += sLine + "\n";
        }
        // 没有就添加
        if ( ! bIsKeyExist ) {
            sLine = sKey + '=' + sValue;
            sFileTxt += sLine + "\n";
        }
        if ( ! bIsKeyExist || bIsChangeKey ) {
            rFile.clear();
            rFile.sync();
            rFile.seekp(0);
            rFile << sFileTxt;
            rFile.sync();
        }
        rFile.close();
        return 0;
    }
    return -1;
}
// 获取文件字段值
int TFile::GetValue( const string& sFileName,
                     const string& sKey,
                     string& sValue )
{
    sValue = "";
    fstream rFile( sFileName.c_str(), ios::in );
    if ( rFile ) {
        string sLine;
        while ( ! rFile.eof() ) {
            std::getline( rFile, sLine );
            string::size_type iPos = sLine.find('=');
            if ( iPos != string::npos &&
                 sLine.substr(0, iPos) == sKey ) {
                sValue = sLine.substr( iPos + 1 );
                break;
            }
        }
        rFile.close();
    }
    return 0;
}
// 获取文件字段值toMap
int TFile::GetValueToMap( const string& sFileName,
                          map<string,string>& pMap )
{
    fstream rFile( sFileName.c_str(), ios::in );
    if ( rFile ) {
        string sLine;
        while ( ! rFile.eof() ) {
            std::getline( rFile, sLine );
            if ("#"==sLine.substr(0, 1)) {
                continue;
            }
            string::size_type iPos = sLine.find('=');
            if ( iPos != string::npos ) {
                pMap[sLine.substr(0, iPos)]=sLine.substr( iPos + 1 );
            }
        }
        rFile.close();
    }
    return 0;
}
// 获取文件第一行内容
int TFile::GetTopLine( const string& sFileName, string& sTopLine )
{
    sTopLine = "";
    fstream rFile( sFileName.c_str(), ios::in );
    if ( rFile ) {
        string sLine;
        if ( ! rFile.eof() ) {
            std::getline( rFile, sLine );
            sTopLine = sLine;
        }
        rFile.close();
    }
    return 0;
}
// 查找文件文本针对每一行
int TFile::IsFindTxt( const string& sFileName,
                        const string& sText )
{
    int iRet = -1;
    fstream rFile( sFileName.c_str(), ios::in );
    if ( rFile ) {
        string sLine;
        while ( ! rFile.eof() ) {
            std::getline( rFile, sLine );
            if ( sLine.find( sText.c_str() ) != string::npos ) {
                iRet = 0;
                break;
            }
        }
        rFile.close();
    }
    return iRet;
}
// 替换文件文本针对每一行
int TFile::ReplaceTxt( const string& sFileName,
                         const string& sSourceTxt,
                         const string& sTargetTxt )
{
    fstream rFile( sFileName.c_str(), ios::in|ios::out );
    if ( rFile ) {
        string sFileTxt = "";       // 记录文件内容
        string sLine;               // 记录每行内容
        bool bIsChangeKey = false; // 是否修改
        string::size_type iPos;
        while ( std::getline( rFile, sLine )) {
            while ( (iPos = sLine.find( sSourceTxt.c_str() )) != string::npos ) {
                sLine.replace( iPos, sSourceTxt.length(), sTargetTxt.c_str() );
                bIsChangeKey = true;
            }
            sFileTxt += sLine + "\n";
        }
        if ( bIsChangeKey ) {
            rFile.clear();
            rFile.sync();
            rFile.seekp(0);
            rFile << sFileTxt;
            rFile.sync();
        }
        rFile.close();
        return 0;
    }
    return -1;
}
// 复制文件
int TFile::CopyFile( const string& sSrcFileName, const string& sTargetFileName )
{
    ifstream fin( sSrcFileName.c_str(), ios::binary );
    if( ! fin ) {
        appendlog(TTDLogger:: LOG_ERROR, "CBFile::CopyFile sSrcFileName File open error!\n" );
        return -1;
    }
    // 创建目录
    if ( 0 != this->CreateDirByPathFile(sTargetFileName) ) {
        return -1;
    }
    //
    ofstream fout( sTargetFileName.c_str(), ios::binary );
    if( ! fout ) {
        fin.close();
        appendlog(TTDLogger:: LOG_ERROR, "CBFile::CopyFile sTargetFileName File open error!\n" );
        return -1;
    }
    char c[1024];
    while(!fin.eof())
    {
        fin.read(c,1024);
        fout.write(c,fin.gcount());
    }
    fin.close();
    fout.close();
    return 0;
}
//保存文件
int TFile::SaveToFile( const long lOffset, const char* cBuff, long lFileSize, const string& sTargetFileName )
{
    try {
        //appendlog(TTDLogger:: LOG_ERROR, "TFile::SaveToFile_%s",sTargetFileName.c_str() );
        // 创建目录
        if ( 0 != this->CreateDirByPathFile(sTargetFileName) ) {
            return -1;
        }
        //保存文件
        ofstream fout( sTargetFileName.c_str(), ios::binary|ios::app );
        if( ! fout ) {
            appendlog(TTDLogger:: LOG_ERROR, "TFile::SaveToFile sTargetFileName File open error! %s\n",sTargetFileName.c_str() );
            return -1;
        }
        fout.write(cBuff,lFileSize);
        fout.close();
    } catch(...) {
        appendlog(TTDLogger:: LOG_ERROR, "CBFile::SaveToFile error" );
        return -1;
    }
    return 0;
}
//加载文件
int TFile::LoadToFile( const string& sFileName,const long lOffset,const long lOffsetLength, char* cBuff )
{
    try {
        ifstream fin( sFileName.c_str(), ios::binary );
        if( ! fin ) {
            appendlog(TTDLogger:: LOG_ERROR, "CBFile::LoadToFile sSrcFileName File open error!" );
            return -1;
        }
        fin.seekg(lOffset, ios::beg);
        fin.read(cBuff,lOffsetLength);
        fin.close();
    } catch(...) {
        appendlog(TTDLogger:: LOG_ERROR, "CBFile::LoadToFile error" );
        return -1;
    }

    return 0;
}
//字符串分割函数
int TFile::Split( const string& sStr, const string& sPattern, std::vector<std::string>& resultVector )
{
    resultVector.clear();

    std::string::size_type pos;
    string str = sStr + sPattern;//扩展字符串以方便操作
    std::size_t size=str.size();

     for(std::size_t i=0; i<size; i++)
     {
         pos=str.find(sPattern,i);
         if(pos<size)
         {
             std::string s=str.substr(i,pos-i);
             resultVector.push_back(s);
             i=pos+sPattern.size()-1;
         }
     }
     return 0;
}
