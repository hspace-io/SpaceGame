FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e
#use kakao mirror as default
RUN sed -e 's/\/\/archive.ubuntu.com\/ubuntu/\/\/mirror.kakao.com\/ubuntu/g' /etc/apt/sources.list > ~/sources.list && mv ~/sources.list /etc/apt/sources.list
RUN apt-get update -y
RUN apt-get install --fix-missing openssh-server libfuse-dev fuse gcc vim -y
#secret - binary owner
#user - real user
RUN adduser secret
RUN chmod 4750 /bin/su
WORKDIR /home/secret

RUN mkdir /super_secret
RUN useradd -d /super_secret user
RUN echo "user:user" | chpasswd
RUN chown secret:secret /super_secret

USER secret
COPY ./secret_file ./secret_file
COPY ./start.sh ./start.sh
USER root
RUN chown secret:secret /home/secret
RUN chown secret:secret /home/secret/start.sh
RUN chown secret:secret /home/secret/secret_file
RUN chmod 700 /home/secret
RUN chmod 700 /home/secret/start.sh
RUN chmod 4700 /home/secret/secret_file

USER root
RUN echo user_allow_other > /etc/fuse.conf

USER root
RUN sed -i 's/^#\?PasswordAuthentication.*/PasswordAuthentication yes/;s/^#\?PermitRootLogin.*/PermitRootLogin no/' /etc/ssh/sshd_config 
RUN mkdir /run/sshd
CMD ./start.sh
EXPOSE 22

