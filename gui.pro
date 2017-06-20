QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
        viewers/opencv_opengl_viewer.cpp \
        viewers/multivideoviewer.cpp \
        sidebar/side_bar.cpp \
        viewers/fullscreenvideoviewer.cpp \
        sidebar/gaussianblurwidget.cpp \
        sidebar/inputmethodwidget.cpp \
        sidebar/lsdwidget.cpp \
        sidebar/curvefitwidget.cpp \
        sidebar/decisionmakewidget.cpp \
        sidebar/regiongrowwidget.cpp \
        videowidget.cpp


HEADERS  += mainwindow.h \
        viewers/opencv_opengl_viewer.h \
        viewers/multivideoviewer.h \
        sidebar/side_bar.h \
        viewers/fullscreenvideoviewer.h \
        sidebar/gaussianblurwidget.h \
        sidebar/inputmethodwidget.h \
        sidebar/lsdwidget.h \
        sidebar/curvefitwidget.h \
        sidebar/decisionmakewidget.h \
        sidebar/regiongrowwidget.h \
        videowidget.h


#unix {
#CONFIG += c++11
#CONFIG += link_pkgconfig
#PKGCONFIG += opencv
#}

unix {
INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_ml
LIBS += -lopencv_video
LIBS += -lopencv_features2d
LIBS += -lopencv_calib3d
LIBS += -lopencv_objdetect
#LIBS += -lopencv_contrib
#LIBS += -lopencv_legacy
#LIBS += -lopencv_flann
#LIBS += -lopencv_nonfree
}

RESOURCES += \
    resources.qrc
