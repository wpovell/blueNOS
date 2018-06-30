#!/bin/bash
if [[ -z $1 ]]; then
  1=/dev/ttyUSB0
fi
sudo screen $1 115200
