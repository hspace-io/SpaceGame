FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e

ENV user cli-painter
ENV chall_port 39393
ENV TERM xterm

RUN apt-get update
RUN apt-get -y install socat

RUN adduser $user

ADD ./flag.txt /home/$user/flag.txt
ADD ./$user /home/$user/$user

RUN chown root:$user /home/$user/flag.txt
RUN chown root:$user /home/$user/$user

RUN chmod 755 /home/$user/$user
RUN chmod 440 /home/$user/flag.txt

WORKDIR /home/$user
USER $user
EXPOSE $chall_port
CMD socat TCP-LISTEN:$chall_port,reuseaddr,fork EXEC:"/home/$user/$user",pty,setsid,ctty
