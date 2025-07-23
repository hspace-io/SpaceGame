
#!/bin/sh

TIME=1000
timeout --foreground $TIME qemu-system-x86_64 \
    -enable-kvm \
    -m 3G \
    -nographic \
    -kernel bzImage \
    -drive file=rootfs.ext4,if=virtio,format=raw \
    -hda flag \
    -append "rootwait root=/dev/vda console=tty1 console=ttyS0 kpti=on quiet oops=panic loglevel=3 panic=-1 panic_on_warn=1 pti=on" \
    -no-reboot \
    -cpu kvm64,+smap,+smep \
    -monitor /dev/null \
    -net nic,model=virtio \
    -net user \
    -snapshot