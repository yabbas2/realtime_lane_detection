#-------------------------------------------------
#
# Project created by QtCreator 2017-04-15T13:24:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = real-time_lane_detection
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    front-end/opencv_opengl_viewer.cpp \
    front-end/multivideoviewer.cpp \
    back-end/stream_in.cpp \
    back-end/stream_out.cpp \
    back-end/stream.cpp

HEADERS  += mainwindow.h \
    front-end/opencv_opengl_viewer.h \
    front-end/multivideoviewer.h \
    back-end/stream_in.h \
    back-end/stream_out.h \
    back-end/stream.h

unix {
CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += opencv
}

win32 {
CONFIG += c++11
INCLUDEPATH += "C:/opencv/include"
LIBS += -LC:/opencv/opencv_bin2/bin \
    libopencv_core240d \
    libopencv_highgui240d \
    libopencv_imgproc240d \
    libopencv_features2d240d \
    libopencv_calib3d240d \
}
