FROM ubuntu:24.04@sha256:c920ba4cfca05503764b785c16b76d43c83a6df8d1ab107e7e6610000d94315c

RUN apt-get update && apt-get install -y socat adduser qemu-system unzip wget

RUN adduser pwn

WORKDIR /home/pwn

COPY ./flag ./flag
COPY ./rootfs.ext4 ./rootfs.ext4
COPY ./run.sh ./run.sh
COPY ./bzImage ./bzImage

RUN chmod 644 ./flag
RUN chmod 644 ./rootfs.ext4
RUN chmod 755 ./run.sh
RUN chmod 755 ./bzImage

USER pwn

EXPOSE 8080
CMD socat TCP-LISTEN:8080,reuseaddr,fork EXEC:./run.sh,stderr
