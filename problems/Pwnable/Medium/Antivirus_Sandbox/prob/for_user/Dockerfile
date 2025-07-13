FROM ubuntu:22.04

RUN apt update
RUN apt-get install -y socat python3


RUN useradd -mU ctf
COPY ./chall.py /home/ctf
COPY ./runner /home/ctf
COPY ./sde-external-9.38.0-2024-04-18-lin /home/ctf/intel-sde

ENV FLAG="hspace{this_is_a_fake_flag}"

USER ctf
WORKDIR /home/ctf

CMD socat -T 60 TCP-LISTEN:11111,reuseaddr,fork EXEC:"python3 ./chall.py"
EXPOSE 11111