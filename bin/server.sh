#!/bin/bash
set -e
make server kernel
sudo stty -F /dev/ttyUSB0 115200 raw -echo
sudo bin/server /dev/ttyUSB0 build/kernel/kernel8.img
