FROM ubuntu:22.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirror.kakao.com/g" /etc/apt/sources.list
RUN apt-get update
RUN apt-get install xinetd -y
ENV TERM xterm

RUN useradd -ms /bin/bash hspaceETH

ADD hspaceETH /home/hspaceETH
ADD ./flag /home/hspaceETH/flag

RUN echo "#!/bin/bash\ncd /home/hspaceETH\n/home/hspaceETH/hspaceETH" \
        > /home/hspaceETH/run.sh

RUN chmod -R 750 /home/hspaceETH/
RUN chmod 750 /home/hspaceETH/flag
RUN chown -R root:hspaceETH /home/hspaceETH
RUN chown -R root:hspaceETH /home/hspaceETH/flag

ADD ./hspaceETH.xinetd /etc/xinetd.d/hspaceETH

CMD [ "/usr/sbin/xinetd", "-dontfork" ]