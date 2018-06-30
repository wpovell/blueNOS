#!/bin/bash
if [[ $EUID -ne 0 ]]; then
  echo "This script must be run as root."
  exit 1
fi

if [[ -z $1 ]]; then
  DEVICE=/dev/mmcblk0p1
else
  DEVICE=$1
fi

if [[ ! -e $DEVICE ]]; then
  echo "Insert SD card first."
  exit 1
fi

make clean all >/dev/null

DIR=$(mktemp -d)
mount $DEVICE $DIR

cp build/kernel8.img $DIR
cp disk/* $DIR

echo "Current boot partition contents:"
ls $DIR

umount $DIR
rm -rf $DIR
make clean >/dev/null

echo "Safe to remove!"
