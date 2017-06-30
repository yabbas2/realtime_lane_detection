#!/bin/bash

TRACK_DIR="../track/"
TRACK_BUILD_DIR="../build-track/"

cd ..
echo "[TRACK] starting..."
cd $TRACK_DIR
#git
cd $TRACK_BUILD_DIR
echo "[TRACK] cleaning..."
rm *
echo "[TRACK] generating Makefile..."
qmake CONFIG-=debug CONFIG+=release "${TRACK_DIR}/track.pro"
echo "[TRACK] building files..."
make -j8
echo "[TRACK] exiting..."
exit 0

