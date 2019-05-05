#ifndef CMDVALUE_H
#define CMDVALUE_H

//推送消息的类型
enum EnumCmdValue
{
    enum_cmd_get_registerinfo = 1,
    enum_cmd_register = 100, //注册
    enum_cmd_offline = 101, //下线
    enum_cmd_clearmsgcount = 102,//IOS推送角标清0

};

#endif
