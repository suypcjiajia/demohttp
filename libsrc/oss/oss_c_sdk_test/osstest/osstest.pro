#-------------------------------------------------
#
# Project created by QtCreator 2017-02-17T14:42:43
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = osstest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    ../test_oss_progress.c \
    ../test_oss_object.c \
    ../test_oss_multipart.c \
    ../test_oss_live.c \
    ../test_oss_image.c \
    ../test_oss_crc.c \
    ../test_oss_callback.c \
    ../test_oss_bucket.c \
    ../test_aos.c \
    ../test_all.c \
    ../oss_test_util.c \
    ../oss_config.c \
    ../CuTest.c \
    ../cjson_utils.c \
    ../cjson.c

DISTFILES += \
    ../test.h.in \
    ../CMakeLists.txt

HEADERS += \
    ../test.h \
    ../oss_test_util.h \
    ../oss_config.h \
    ../CuTest.h \
    ../cjson_utils.h \
    ../cjson.h
