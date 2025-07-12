FROM ubuntu:22.04
MAINTAINER ipwn <ipwn.with@gmail.com>

RUN apt update -y
RUN apt install socat python3 -y
RUN useradd -mU prob

COPY run.py /home/prob/run.py
COPY prob /home/prob/prob

RUN chmod 750 /home/prob /home/prob/run.py
RUN chmod 750 /home/prob /home/prob/prob

RUN chown -R root:prob /home/prob/run.py
RUN chown -R root:prob /home/prob/prob

USER prob
WORKDIR /home/prob

CMD socat tcp-l:10101,reuseaddr,fork EXEC:"python3 run.py"