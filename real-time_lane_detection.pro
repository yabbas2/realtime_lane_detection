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
            front-end/viewers/opencv_opengl_viewer.cpp \
            front-end/viewers/multivideoviewer.cpp \
            front-end/sidebar/side_bar.cpp \
            front-end/viewers/fullscreenvideoviewer.cpp \
            back-end/stream_in.cpp \
            back-end/stream_out.cpp \
            back-end/stream.cpp \
            front-end/sidebar/gaussianblurwidget.cpp \
            front-end/sidebar/inputmethodwidget.cpp \
            front-end/sidebar/lsdwidget.cpp \
            front-end/sidebar/curvefitwidget.cpp \
            front-end/sidebar/decisionmakewidget.cpp \
            front-end/sidebar/regiongrowwidget.cpp \
    front-end/videowidget.cpp

HEADERS  += mainwindow.h \
            front-end/viewers/opencv_opengl_viewer.h \
            front-end/viewers/multivideoviewer.h \
            front-end/sidebar/side_bar.h \
            front-end/viewers/fullscreenvideoviewer.h \
            back-end/stream_in.h \
            back-end/stream_out.h \
            back-end/stream.h \
            front-end/sidebar/gaussianblurwidget.h \
            front-end/sidebar/inputmethodwidget.h \
            front-end/sidebar/lsdwidget.h \
            front-end/sidebar/curvefitwidget.h \
            front-end/sidebar/decisionmakewidget.h \
            front-end/sidebar/regiongrowwidget.h \
    front-end/videowidget.h


unix {
CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += opencv
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

RESOURCES += \
    resources.qrc
