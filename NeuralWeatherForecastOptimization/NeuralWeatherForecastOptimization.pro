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
INCLUDEPATH += ../include/

LIBS += -lcurl


SOURCES += DBWriterDaemon.cpp \
    ANNTrainingDaemon.cpp \
    ForecastCalculator.cpp \
    src/DataBuffer.cpp \
    src/HTTPRequest.cpp \
    src/OpenWeatherMap.cpp \
    src/WebContent.cpp \
    src/ArtificialNeuroNet.cpp \
    src/AirPressureSensor.cpp \
    src/ANNWrapper.cpp \
    src/DBInterface.cpp \
    src/LogWriter.cpp \
    src/Sensor.cpp \
    src/SLevel.cpp \
    src/TemperatureSensor.cpp \
    ArchiveDataPreprocessor.cpp


HEADERS += \
    include/catch.hpp \
    include/DataBuffer.h \
    include/HTTPRequest.h \
    include/OpenWeatherMap.h \
    include/WebContent.h \
    include/WebContentType.h \
    include/ArtificialNeuroNet.h \
    include/ArtificialNeuroNetWrapper.h \
    include/AirPressureSensor.h \
    include/ANNWrapper.h \
    include/Config.h \
    include/DBInterface.h \
    include/LogWriter.h \
    include/Sensor.h \
    include/SensorType.h \
    include/SLevel.h \
    include/TemperatureSensor.h
