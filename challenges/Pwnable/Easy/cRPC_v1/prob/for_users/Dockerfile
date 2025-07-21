FROM ubuntu:22.04

ENV user crpc
ENV chall_port 18181
ENV TERM xterm

RUN apt-get update
RUN apt-get -y install socat

RUN adduser $user

ADD ./flag.txt /home/$user/flag.txt
ADD ./crpc_v1 /home/$user/crpc_v1

RUN chmod 555 /home/$user
RUN chown root:root /home/$user

RUN chown root:$user /home/$user/flag.txt
RUN chown root:$user /home/$user/crpc_v1

RUN chmod 755 /home/$user/crpc_v1
RUN chmod 440 /home/$user/flag.txt

WORKDIR /home/$user
USER $user
EXPOSE $chall_port

CMD socat TCP-LISTEN:$chall_port,reuseaddr,fork EXEC:"/home/$user/crpc_v1"