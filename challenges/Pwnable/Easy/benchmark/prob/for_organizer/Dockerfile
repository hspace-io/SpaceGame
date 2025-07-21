FROM ubuntu:22.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirror.kakao.com/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get install xinetd -y
ENV TERM xterm

RUN useradd -ms /bin/bash benchmark

ADD benchmark /home/benchmark
ADD ./flag /home/benchmark/flag

RUN echo "#!/bin/bash\n/home/benchmark/benchmark" \
        > /home/benchmark/run.sh

RUN chmod -R 750 /home/benchmark/
RUN chmod 750 /home/benchmark/flag
RUN chown -R root:benchmark /home/benchmark
RUN chown -R root:benchmark /home/benchmark/flag

ADD ./benchmark.xinetd /etc/xinetd.d/benchmark

CMD [ "/usr/sbin/xinetd", "-dontfork" ]