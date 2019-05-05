#-------------------------------------------------
#
# Project created by QtCreator 2017-02-15T15:00:21
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = osssample
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
INCLUDEPATH += /home/cpp/libsrc/oss/oss_c_sdk
INCLUDEPATH += /home/cpp/libsrc/apr
LIBS += -L/usr/local/apr/lib -L/usr/local/apr/lib/apr-util-1
LIBS += -lapr-1 -laprutil-1   /usr/local/lib/liboss_c_sdk_static.a -lmxml -lcurl /usr/local/apr/lib/libapr-1.a /usr/local/apr/lib/libaprutil-1.a

SOURCES += \
    ../oss_sample_util.c \
    ../oss_put_object_sample.c \
    ../oss_multipart_upload_sample.c \
    ../oss_head_object_sample.c \
    ../oss_get_object_sample.c \
    ../oss_delete_object_sample.c \
    ../oss_config.c \
    ../oss_append_object_sample.c \
    ../main.c

HEADERS += \
    ../oss_sample_util.h \
    ../oss_config.h
