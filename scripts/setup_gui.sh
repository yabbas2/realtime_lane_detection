#!/bin/bash

GUI_DIR="/home/odroid/project/gui/"
GUI_BUILD_DIR="/home/odroid/project/build_gui/"

WD="/home/odroid/"

cd

cd $GUI_DIR
#git
cd
cd $GUI_BUILD_DIR
echo "[GUI] executing make clean..."
make clean
echo "[GUI] generating Makefile..."
qmake "${GUI_DIR}/gui.pro"
echo "[GUI] building files..."
make -j4
echo "[GUI] copying executable to home..."
cp "${GUI_BUILD_DIR}/gui" $WD
echo "[GUI] exiting..."
exit 0

