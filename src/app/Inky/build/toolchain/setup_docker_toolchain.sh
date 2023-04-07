#!/bin/sh

# The URL and SHA of the RPI image you're looking to download
# Get URL and SHA at https://www.raspberrypi.com/software/operating-systems/
RPI_IMAGE_URL=https://downloads.raspberrypi.org/raspios_lite_armhf/images/raspios_lite_armhf-2022-09-26/2022-09-22-raspios-bullseye-armhf-lite.img.xz
RPI_IMAGE_DEST=sysroot
RPI_IMAGE_DEV=/dev/loop0
RPI_IMAGE_MNT=/mnt/sysroot

# Attempt an unmount 
echo "Cleaning up previous sysroot (ok if this fails)..."
umount $RPI_IMAGE_MNT
losetup -d $RPI_IMAGE_DEV

echo "Fetching RPi image..."
if [ -f "$RPI_IMAGE_DEST.img" ] ; then
  echo "Already setup!"
else
  echo "Downloading..."
  curl "$RPI_IMAGE_URL" > "$RPI_IMAGE_DEST.img.xz"
  echo "Extracting..."
  xz -d "$RPI_IMAGE_DEST.img.xz"
fi

# Get the second partition offset and size in the disk
IMG_SECT_SIZE=512
IMG_SECT_OFFSET=`fdisk -l --bytes $RPI_IMAGE_DEST.img | grep img2 | tr -s " " | cut -d " " -f2`
IMG_BYTE_OFFSET=`expr $IMG_SECT_OFFSET \* $IMG_SECT_SIZE`
IMG_BYTE_SIZE=`fdisk -l --bytes $RPI_IMAGE_DEST.img | grep img2 | tr -s " " | cut -d " " -f5`
echo "Sysroot partion has offset $IMG_BYTE_OFFSET and size $IMG_BYTE_SIZE"

echo "Creating loop device..."
mknod $RPI_IMAGE_DEV b 7 0

echo "Mounting $RPI_IMAGE_DEST.img at $RPI_IMAGE_MNT"
if [ ! -d "$RPI_IMAGE_MNT" ] ; then
  mkdir -p "$RPI_IMAGE_MNT"
fi
losetup -o $IMG_BYTE_OFFSET --sizelimit $IMG_BYTE_SIZE --sector-size $IMG_SECT_SIZE $RPI_IMAGE_DEV $RPI_IMAGE_DEST.img

# Mount image read/write and fix some symlinks
echo "Fixing symlinks in sysroot..."
mount $RPI_IMAGE_DEV $RPI_IMAGE_MNT
cd $RPI_IMAGE_MNT/lib/arm-linux-gnueabihf
rm libpthread.so
ln -s libpthread.so.0 libpthread.so
cd $RPI_IMAGE_MNT/usr/lib/arm-linux-gnueabihf
rm libpthread.so
ln -s ../../lib/arm-linux-gnueabihf/libpthread.so.0 libpthread.so
cd /

# Remount read-only
echo "Mounting sysroot..."
umount $RPI_IMAGE_MNT
mount -r $RPI_IMAGE_DEV $RPI_IMAGE_MNT

echo "Done!!"