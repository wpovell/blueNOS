#!/bin/bash
if [[ $EUID -ne 0 ]]; then
  echo "This script must be run as root."
  exit 1
fi

if [[ -z $1 ]]; then
  1=/dev/mmcblk0p1
fi

if [[ ! -e $1 ]]; then
  echo "Insert SD card first."
  exit 1
fi

mkdir -p mnt
mount $1 mnt
cp build/kernel8.img mnt/
cp disk/* mnt/
echo "Current boot partition contents:"
ls mnt
umount mnt
rm -rf mnt
echo "Safe to remove!"
