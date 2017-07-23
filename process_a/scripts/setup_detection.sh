#!/bin/bash

DETECTION_DIR="../detection/"
DETECTION_BUILD_DIR="../build-detection/"

cd ..
echo "[DETECTION] starting..."
cd $DETECTION_DIR
#git
cd $DETECTION_BUILD_DIR
echo "[DETECTION] cleaning..."
rm *
echo "[DETECTION] generating Makefile..."
qmake CONFIG-=debug CONFIG+=release "${DETECTION_DIR}/detection.pro"
echo "[DETECTION] building files..."
make -j8
echo "[DETECTION] exiting..."
exit 0

