QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        front-end/mainwindow.cpp \
        front-end/viewers/multivideoviewer.cpp \
        front-end/sidebar/side_bar.cpp \
        front-end/viewers/fullscreenvideoviewer.cpp \
        front-end/sidebar/gaussianblurwidget.cpp \
        front-end/sidebar/inputmethodwidget.cpp \
        front-end/sidebar/lsdwidget.cpp \
        front-end/sidebar/curvefitwidget.cpp \
        front-end/sidebar/decisionmakewidget.cpp \
        front-end/sidebar/regiongrowwidget.cpp \
        front-end/videowidget.cpp \
        front-end/viewers/opencvViewer.cpp \
        d_bus.cpp \
        gui.cpp


HEADERS  += front-end/mainwindow.h \
        front-end/viewers/multivideoviewer.h \
        front-end/sidebar/side_bar.h \
        front-end/viewers/fullscreenvideoviewer.h \
        front-end/sidebar/gaussianblurwidget.h \
        front-end/sidebar/inputmethodwidget.h \
        front-end/sidebar/lsdwidget.h \
        front-end/sidebar/curvefitwidget.h \
        front-end/sidebar/decisionmakewidget.h \
        front-end/sidebar/regiongrowwidget.h \
        front-end/videowidget.h \
        front-end/viewers/opencvViewer.h \
        d_bus.h \
        gui.h

unix {
CONFIG += c++11
INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib
LIBS += -lopencv_objdetect
LIBS += -lopencv_features2d
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_videoio
LIBS += -lopencv_core
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_calib3d
}

RESOURCES += \
    resources.qrc
