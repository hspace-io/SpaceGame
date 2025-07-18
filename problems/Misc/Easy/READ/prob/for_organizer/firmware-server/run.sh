#!/bin/bash

qemu-system-arm \
-kernel ./zImage \
-initrd ./rootfs-server.squashfs \
-M virt \
-append "root=/dev/ram console=ttyAMA0" \
-device virtio-net-device,netdev=eth0 \
-netdev user,id=eth0,hostfwd=tcp::21345-:80 \
-nographic \
-m 512M

