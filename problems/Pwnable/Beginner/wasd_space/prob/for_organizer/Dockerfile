FROM ubuntu:22.04
MAINTAINER circler

RUN apt-get update
RUN apt-get install -y xinetd netcat socat

RUN mkdir /pwn-wasd_space
RUN useradd -d /pwn-wasd_space pwn_user -s /bin/bash 
RUN chown -R pwn_user:pwn_user /pwn-wasd_space/

CMD chown -R root:pwn_user /pwn-wasd_space
RUN chmod 750 /pwn-wasd_space 

ADD ./wasd_space /pwn-wasd_space/wasd_space
RUN chmod +x /pwn-wasd_space/wasd_space

ADD ./flag /pwn-wasd_space/flag
RUN chown root:pwn_user pwn-wasd_space/flag 
RUN chmod 440 pwn-wasd_space/flag 

ADD pwn.xinetd /etc/xinetd/pwn 
ADD ./start.sh /
RUN chmod +x /start.sh
