QT += core
QT -= gui

CONFIG += c++11

TARGET = regiongrowing
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    regiongrowing.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    regiongrowing.h

unix {
CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += opencv
LIBS += -lalglib
}

win32 {
CONFIG += c++11
INCLUDEPATH += C:/opencv/build/include
LIBS += -LC:/opencv/build/bin \
C:/opencv/build/x64/vc14/lib/opencv_world310.lib \
C:/opencv/build/x64/vc14/lib/opencv_world310d.lib \
-LC:/opencv/build/x64/vc14/bin \
C:/opencv/build/x64/vc12/lib/opencv_world310.lib \
C:/opencv/build/x64/vc12/lib/opencv_world310d.lib
-LC:/opencv/build/x64/vc12/bin
}
