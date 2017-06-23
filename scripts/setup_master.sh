#!/bin/bash

MASTER_DIR="/home/odroid/project/master/"
MASTER_BUILD_DIR="/home/odroid/project/build_master/"

WD="/home/odroid/"

cd

cd $MASTER_DIR
#git
cd
cd $MASTER_BUILD_DIR
echo "[MASTER] cleaning..."
rm *
echo "[MASTER] generating Makefile..."
qmake "${MASTER_DIR}/master.pro"
echo "[MASTER] building files..."
make -j4
echo "[MASTER] copying executable to home..."
cp "${MASTER_BUILD_DIR}/master" $WD
echo "[MASTER] exiting..."
exit 0

