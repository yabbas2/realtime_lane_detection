#!/bin/bash

GUI_DIR="../gui/"
GUI_BUILD_DIR="../build-gui/"

cd ..
echo "[GUI] starting..."
cd $GUI_DIR
#git
cd $GUI_BUILD_DIR
echo "[GUI] cleaning..."
rm *
echo "[GUI] generating Makefile..."
qmake CONFIG-=debug CONFIG+=release "${GUI_DIR}/gui.pro"
echo "[GUI] building files..."
make -j8
echo "[GUI] exiting..."
exit 0

