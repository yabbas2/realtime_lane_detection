#!/bin/bash

SCRIPTS_DIR="."

echo "[SETUP] starting setup..."
cd $SCRIPTS_DIR
sh setup_gui.sh
wait $!
sh setup_stream.sh
wait $!
sh setup_master.sh
wait $!
echo "[SETUP] setup is finished!"
