FROM ubuntu:22.04

ENV user spi
ENV chall_port 21346

RUN apt-get update
RUN apt-get -y install socat

RUN adduser $user
ADD ./prob /home/$user/prob

RUN chown root:$user /home/$user/prob

RUN chmod 755 /home/$user/prob

WORKDIR /home/$user
USER $user
EXPOSE $chall_port
CMD socat -T 25 TCP-LISTEN:$chall_port,reuseaddr,fork EXEC:/home/$user/prob
