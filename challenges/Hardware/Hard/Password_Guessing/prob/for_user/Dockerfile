FROM ubuntu:22.04@sha256:c985bc3f77946b8e92c9a3648c6f31751a7dd972e06604785e47303f4ad47c4c

ENV user guess
ENV chall_port 31337

RUN apt-get update
RUN apt-get install -y python3 python3-pip socat

RUN adduser $user

RUN pip3 install --upgrade pip
RUN pip3 install unicorn capstone pyelftools

ADD ./guess /home/$user/guess
ADD ./guess_unicorn.py /home/$user/guess_unicorn.py
ADD ./run.sh /home/$user/run.sh
ADD ./flag /home/$user/flag

RUN chown -R root:root /home/$user
RUN chown root:$user /home/$user/guess_unicorn.py
RUN chown root:$user /home/$user/guess
RUN chown root:$user /home/$user/run.sh

RUN chmod 777 /home/$user
RUN chmod 744 /home/$user/guess_unicorn.py
RUN chmod 755 /home/$user/guess
RUN chmod 755 /home/$user/run.sh
RUN chmod 444 /home/$user/flag

WORKDIR /home/$user/
USER $user
EXPOSE $chall_port
CMD socat TCP-LISTEN:$chall_port,reuseaddr,fork EXEC:./run.sh,stderr
