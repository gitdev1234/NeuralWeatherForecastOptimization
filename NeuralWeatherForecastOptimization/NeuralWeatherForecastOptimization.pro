#-------------------------------------------------
#
# Project created by QtCreator 2016-08-10T17:33:17
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = NeuralWeatherForecastOptimization

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH += include/

LIBS += -lcurl


SOURCES += main.cpp \
    src/airpressuresensor.cpp \
    src/sensor.cpp \
    src/temperaturesensor.cpp \
    src/DataBuffer.cpp \
    src/dbinterface.cpp \
    src/HTTPRequest.cpp \
    src/logwriter.cpp \
    src/slevel.cpp \
    src/OpenWeatherMap.cpp \
    src/WebContent.cpp

HEADERS += \
    include/airpressuresensor.h \
    include/sensor.h \
    include/sensortype.h \
    include/temperaturesensor.h \
    include/catch.hpp \
    include/config.h \
    include/DataBuffer.h \
    include/dbinterface.h \
    include/HTTPRequest.h \
    include/logwriter.h \
    include/slevel.h \
    include/OpenWeatherMap.h \
    include/WebContent.h \
    include/WebContentType.h
