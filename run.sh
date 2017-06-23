#!/bin/bash

STREAM_DIR="/home/odroid/project/stream"
STREAM_BUILD_DIR="/home/odroid/project/build_stream"

GUI_DIR="/home/odroid/project/gui"
GUI_BUILD_DIR="/home/odroid/project/build_gui"

MASTER_DIR="/home/odroid/project/master"
MASTER_BUILD_DIR="/home/odroid/project/build_master"

WD="/home/odroid"

cd

cp "${STREAM_BUILD_DIR}/stream" $WD
cp "${GUI_BUILD_DIR}/gui" $WD
cp "${MASTER_BUILD_DIR}/master" $WD

cd $MASTER_BUILD_DIR
nohup ./master &
exit 0
