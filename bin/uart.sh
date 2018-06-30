#!/bin/bash
if [[ -z $1 ]]; then
  DEVICE=/dev/ttyUSB0
else
  DEVICE=$1
fi
sudo minicom -D $DEVICE
