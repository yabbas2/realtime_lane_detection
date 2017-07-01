QT += core
QT -= gui
QT += dbus
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ipm
TEMPLATE = app

SOURCES += main.cpp \
    ipm.cpp \
    d_bus.cpp

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    ipm.h \
    d_bus.h

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
