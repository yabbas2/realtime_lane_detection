#!/bin/bash

SCRIPTS_DIR="/home/odroid/project/master/scripts/"

cd

echo "[SETUP] starting setup..."
cd $SCRIPTS_DIR
sh setup_gui.sh
wait $!
sh setup_stream.sh
wait $!
sh setup_master.sh
wait $!
echo "[SETUP] setup is finished!"