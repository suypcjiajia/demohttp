TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
SOURCES += main.cpp \
    ../libsrc/TConvert.cpp \
    ../libsrc/TFile.cpp \
    ../libsrc/TMd5.cpp \
    ../libsrc/MyAES.cpp \
    ../libsrc/Base64EnDecode.cpp \
    ../libsrc/dal/MysqlConnect.cpp \
    ../libsrc/dal/TMultiMysqlDAL.cpp \
    dal/UserDataDAL.cpp \
    bll/UserDataBLL.cpp \
    ../libsrc/com/TCurl.cpp \
    ../libsrc/com/EvhtpSvr.cpp \
    ../libsrc/com/PostsAes.cpp \
    ../libsrc/com/PushDataCOM.cpp \
    bll/PushBLL.cpp \
    ../libsrc/log/Logger.cpp \
    ../libsrc/log/Buffer.cpp \
    ../libsrc/MyUtil.cpp \
    ../libsrc/com/Request.cpp \
    ../libsrc/dal/Basedal.cpp \
    ../libsrc/Token.cpp \
    ../libsrc/Hmac.cpp \
    bll/Config.cpp \
    dal/SettingDAL.cpp \
    bll/SettingBLL.cpp \
    bll/factory.cpp \
    bll/standardInputBLL.cpp \
    dal/standardInputDAL.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../libsrc/com/EvhtpSvr.h \
    ../libsrc/com/TCurl.h \
    ../libsrc/TConvert.h \
    ../libsrc/TFile.h \
    ../libsrc/TMd5.h \
    ../libsrc/MyAES.h \
    ../libsrc/Base64EnDecode.h \
    ../libsrc/dal/MysqlConnect.h \
    ../libsrc/dal/TMultiMysqlDAL.h \
    dal/UserDataDAL.h \
    bll/UserDataBLL.h \
    ../libsrc/com/PostsAes.h \
    ../libsrc/com/RetJsonMsg.hpp \
    ../libsrc/com/PushDataCOM.h \
    bll/PushBLL.h \
    ../inc/CmdValue.h \
    ../inc/ErrDef.h \
    ../inc/UserDataEntity.h \
    ../inc/TypeDef.h \
    ../libsrc/log/Logger.h \
    ../libsrc/log/Buffer.h \
    ../libsrc/MyUtil.h \
    .../libsrc/com/Request.h \
    ../libsrc/dal/Basedal.h \
    ../libsrc/Token.h \
    ../libsrc/Hmac.h \
    bll/Config.h \
    dal/SettingDAL.h \
    ../inc/SettingEntity.h \
    ../inc/BaseEntity.h \
    bll/SettingBLL.h \
    ../libsrc/BaseBLL.h \
    bll/factory.h \
    bll/standardInputBLL.h \
    ../inc/standardInputEntity.h \
    dal/standardInputDAL.h

APP_PATH = /home/project/cpp
APP_LIBSRC = $$APP_PATH/libsrc
APP_INC = $$APP_PATH/inc

DESTDIR += $$APP_PATH/bin
INCLUDEPATH += $$APP_PATH/libsrc
INCLUDEPATH += $$APP_PATH/libso
INCLUDEPATH += $$APP_PATH/inc/evhtp
INCLUDEPATH += $$APP_PATH/
INCLUDEPATH += $$APP_PATH/inc
INCLUDEPATH += $$APP_PATH/libsrc/oss/oss_c_sdk
INCLUDEPATH += $$APP_PATH/libsrc/apr

LIBS += -L/usr/lib
LIBS += -levhtp -levent  -levent_pthreads -lcrypto -lcryptopp -lpthread -lssl -ljsoncpp -luuid
LIBS += -lcurl -lmysqlclient

#-levent_openssl
