FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e
RUN apt-get update -y
RUN apt-get install socat -y
RUN adduser uaf_basic
WORKDIR /home/uaf_basic

USER note
COPY ./uaf_basic ./uaf_basic
COPY ./flag.txt ./flag.txt

USER root
RUN chown -R root:root /home/uaf_basic
RUN chmod 755 /home/uaf_basic
RUN chmod 644 ./flag.txt
RUN chmod 755 ./uaf_basic

CMD socat -T 300 TCP-LISTEN:4021,reuseaddr,fork EXEC:/home/uaf_basic/uaf_basic
EXPOSE 4021
