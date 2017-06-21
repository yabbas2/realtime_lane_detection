QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
        viewers/multivideoviewer.cpp \
        sidebar/side_bar.cpp \
        viewers/fullscreenvideoviewer.cpp \
        sidebar/gaussianblurwidget.cpp \
        sidebar/inputmethodwidget.cpp \
        sidebar/lsdwidget.cpp \
        sidebar/curvefitwidget.cpp \
        sidebar/decisionmakewidget.cpp \
        sidebar/regiongrowwidget.cpp \
        videowidget.cpp \
        viewers/opencvViewer.cpp \
        stream.cpp \
        stream_in.cpp \
        stream_out.cpp


HEADERS  += mainwindow.h \
        viewers/multivideoviewer.h \
        sidebar/side_bar.h \
        viewers/fullscreenvideoviewer.h \
        sidebar/gaussianblurwidget.h \
        sidebar/inputmethodwidget.h \
        sidebar/lsdwidget.h \
        sidebar/curvefitwidget.h \
        sidebar/decisionmakewidget.h \
        sidebar/regiongrowwidget.h \
        videowidget.h \
        viewers/opencvViewer.h \
        stream.h \
        stream_in.h \
        stream_out.h

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
#LIBS += -lopencv_ml
#LIBS += -lopencv_video
#LIBS += -lopencv_calib3d
}

RESOURCES += \
    resources.qrc
