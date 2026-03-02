#!/bin/bash
BALLZOOKA_IP="10.106.96.81" # TODO: I think this IP is just assigned via DHCP
TARGET_DIR="~/test"

echo "TRANSMITTING FILE TO arduino@$BALLZOOKA_IP:$TARGET_DIR"

scp $1 "arduino@$BALLZOOKA_IP:$TARGET_DIR"

echo "TRANSMISSION COMPLETE"

