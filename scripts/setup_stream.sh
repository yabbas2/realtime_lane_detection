#!/bin/bash

STREAM_DIR="/home/odroid/project/stream/"
STREAM_BUILD_DIR="/home/odroid/project/build_stream/"

WD="/home/odroid/"

cd

cd $STREAM_DIR
#git
cd
cd $STREAM_BUILD_DIR
echo "[STREAM] cleaning..."
rm *
echo "[STREAM] generating Makefile..."
qmake "${STREAM_DIR}/stream.pro"
echo "[STREAM] building files..."
make -j4
echo "[STREAM] copying executable to home..."
cp "${STREAM_BUILD_DIR}/stream" $WD
echo "[STREAM] exiting..."
exit 0

