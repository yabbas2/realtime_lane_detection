QT += core
QT -= gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    logger.cpp


HEADERS  += \
    logger.h

unix {
CONFIG += c++11
}
