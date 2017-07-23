#!/bin/bash

SCRIPTS_DIR="."

echo "[SETUP] starting setup..."
cd $SCRIPTS_DIR
sh setup_logger.sh
wait $!
sh setup_gui.sh
wait $!
sh setup_stream.sh
wait $!
sh setup_detection.sh
wait $!
sh setup_reg.sh
wait $!
sh setup_ipm.sh
wait $!
sh setup_track.sh
wait $!
sh setup_master.sh
wait $!
echo "[SETUP] exiting setup..."
