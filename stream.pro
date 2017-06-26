QT += core
QT += dbus
QT -= gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stream
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        stream.cpp \
    d_bus.cpp \
    ipm.cpp


HEADERS  += stream.h \
    d_bus.h \
    ipm.h

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
LIBS += -llogger
}
