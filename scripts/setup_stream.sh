#!/bin/bash

STREAM_DIR="../stream/"
STREAM_BUILD_DIR="../build_stream/"

cd ..
cd $STREAM_DIR
#git
cd $STREAM_BUILD_DIR
echo "[STREAM] cleaning..."
rm *
echo "[STREAM] generating Makefile..."
qmake "${STREAM_DIR}/stream.pro"
echo "[STREAM] building files..."
make -j8
echo "[STREAM] exiting..."
exit 0

