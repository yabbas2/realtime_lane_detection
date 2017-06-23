#!/bin/bash

STREAM_DIR="/home/odroid/project/stream"
STREAM_BUILD_DIR="/home/odroid/project/build_stream"

GUI_DIR="/home/odroid/project/gui"
GUI_BUILD_DIR="/home/odroid/project/build_gui"

MASTER_DIR="/home/odroid/project/master"
MASTER_BUILD_DIR="/home/odroid/project/build_master"

WD="/home/odroid"

cd

cd $STREAM_DIR
#git
cd
cd $STREAM_BUILD_DIR
make clean
qmake "${STREAM_DIR}/stream.pro"
make -j4
cd

cd $GUI_DIR
#git
cd
cd $GUI_BUILD_DIR
make clean
qmake "${GUI_DIR}/gui.pro"
make -j4
cd

cd $MASTER_DIR
#git
cd
cd $MASTER_BUILD_DIR
make clean
qmake "${MASTER_DIR}/master.pro"
make -j4
cd


