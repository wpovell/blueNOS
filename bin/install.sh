#!/bin/bash

function help {
  echo -e "\ninstall <img> [device]"
}

if [[ $EUID -ne 0 ]]; then
  echo "This script must be run as root."
  help
  exit 1
fi

if [[ -z $1 ]]; then
  echo "Please provide a kernel image"
  help
  exit 1
fi

if [[ -z $2 ]]; then
  DEVICE=/dev/mmcblk0p1
else
  DEVICE=$2
fi

if [[ ! -e $DEVICE ]]; then
  echo "Insert SD card first."
  help
  exit 1
fi

make clean all >/dev/null

DIR=$(mktemp -d)
mount $DEVICE $DIR

cp $1 $DIR
cp disk/* $DIR
cp $(basename $(dirname $1))/config.txt $DIR

echo "Current boot partition contents:"
ls $DIR

umount $DIR
rm -rf $DIR
make clean >/dev/null

echo "Safe to remove!"
