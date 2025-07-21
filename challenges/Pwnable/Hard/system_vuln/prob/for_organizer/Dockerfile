FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e
RUN apt-get update -y
RUN apt-get install socat -y
RUN adduser sandbox
WORKDIR /home/sandbox

USER sandbox
COPY ./system ./system
COPY ./flag.txt ./flag.txt
COPY ./start.sh ./start.sh

USER root
RUN chown -R root:root /home/sandbox
RUN chmod 755 /home/sandbox
RUN chmod 755 /home/sandbox/system
RUN chmod 644 ./flag.txt
RUN chmod 755 ./start.sh

USER sandbox
CMD ./start.sh
EXPOSE 15252

