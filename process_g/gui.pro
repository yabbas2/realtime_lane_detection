QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
        fullscreenvideoviewer.cpp \
        videowidget.cpp \
        opencvViewer.cpp \
        d_bus.cpp \
        gui.cpp


HEADERS  += mainwindow.h \
        fullscreenvideoviewer.h \
        videowidget.h \
        opencvViewer.h \
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
LIBS += -lopencv_imgcodecs
LIBS += -llogger
}

RESOURCES += \
    resources.qrc
