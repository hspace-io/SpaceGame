FROM ubuntu:22.04@sha256:0eb0f877e1c869a300c442c41120e778db7161419244ee5cbc6fa5f134e74736

RUN apt update
RUN apt-get install -y socat


RUN useradd -mU ctf
COPY chall /home/ctf
COPY flag.txt /home/ctf

RUN chown ctf:ctf /home/ctf/*
RUN chmod 440 /home/ctf/*
RUN chmod +x /home/ctf/chall

USER ctf
WORKDIR /home/ctf

CMD socat -T 60 TCP-LISTEN:11115,reuseaddr,fork EXEC:"/home/ctf/chall"
EXPOSE 11115