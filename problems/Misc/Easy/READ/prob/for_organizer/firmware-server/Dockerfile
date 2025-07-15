FROM ubuntu:22.04

RUN apt update -y && apt upgrade -y && \
    apt install -y qemu-system-arm socat

ADD zImage /
ADD rootfs-server.squashfs /
ADD run.sh /

RUN chmod +x /run.sh
RUN chmod +x /zImage

CMD ["/run.sh"]
