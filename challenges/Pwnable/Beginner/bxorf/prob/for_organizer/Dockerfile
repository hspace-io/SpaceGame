FROM ubuntu:22.04

ENV LC_CTYPE=C.UTF-8

ENV user bxorf
ENV chall_port 12345

RUN apt-get update
RUN apt-get -y install socat

RUN useradd -d /home/$user $user -s /bin/bash

WORKDIR /home/$user

ADD $user /home/$user/$user
ADD flag /home/$user/flag

RUN chown -R root:root /home/$user
RUN chown root:$user /home/$user/flag
RUN chown root:$user /home/$user/$user

RUN chmod 755 /home/$user/$user
RUN chmod 440 /home/$user/flag

ADD ./pwn_bxorf /etc/xinetd.d/pwn_bxorf


USER $user
EXPOSE $chall_port
CMD socat TCP-LISTEN:$chall_port,reuseaddr,fork EXEC:/home/$user/$user