#!/bin/bash

LOGGER_DIR="../logger/"
LOGGER_BUILD_DIR="../build-logger/"
LOGGER_INSTALL_DIR="/usr/lib/arm-linux-gnueabihf/"

cd ..
echo "[LOGGER] starting..."
cd $LOGGER_DIR
#git
cd $LOGGER_BUILD_DIR
echo "[LOGGER] cleaning..."
rm *
echo "[LOGGER] generating Makefile..."
qmake "${LOGGER_DIR}/logger.pro"
echo "[LOGGER] building files..."
make -j8
echo "[LOGGER] installing lib..."
sudo cp liblogger.so* $LOGGER_INSTALL_DIR
echo "[LOGGER] exiting..."
exit 0

