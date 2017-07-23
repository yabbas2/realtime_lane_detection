#!/bin/bash

MASTER_DIR="../master/"
MASTER_BUILD_DIR="../build-master/"

cd ..
echo "[MASTER] starting..."
cd $MASTER_DIR
#git
cd $MASTER_BUILD_DIR
echo "[MASTER] cleaning..."
rm *
echo "[MASTER] generating Makefile..."
qmake CONFIG-=debug CONFIG+=release "${MASTER_DIR}/master.pro"
echo "[MASTER] building files..."
make -j8
echo "[MASTER] exiting..."
exit 0

