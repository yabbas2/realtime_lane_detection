#!/bin/bash

IPM_DIR="../ipm/"
IPM_BUILD_DIR="../build-ipm/"

cd ..
echo "[IPM] starting..."
cd $IPM_DIR
#git
cd $IPM_BUILD_DIR
echo "[IPM] cleaning..."
rm *
echo "[IPM] generating Makefile..."
qmake CONFIG-=debug CONFIG+=release "${IPM_DIR}/ipm.pro"
echo "[IPM] building files..."
make -j8
echo "[IPM] exiting..."
exit 0

