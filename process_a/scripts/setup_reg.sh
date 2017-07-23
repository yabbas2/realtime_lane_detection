#!/bin/bash

REG_DIR="../reg/"
REG_BUILD_DIR="../build-reg/"

cd ..
echo "[REG] starting..."
cd $REG_DIR
#git
cd $REG_BUILD_DIR
echo "[REG] cleaning..."
rm *
echo "[REG] generating Makefile..."
qmake CONFIG-=debug CONFIG+=release "${REG_DIR}/reg.pro"
echo "[REG] building files..."
make -j8
echo "[REG] exiting..."
exit 0

