FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e
RUN apt-get update -y
RUN apt-get install socat -y
RUN adduser note
WORKDIR /home/note

USER note
COPY ./note ./note
COPY ./flag.txt ./flag.txt

USER root
RUN chown -R root:root /home/note
RUN chmod 755 /home/note
RUN chmod 644 ./flag.txt
RUN chmod 755 ./note

CMD socat -T 300 TCP-LISTEN:15252,reuseaddr,fork EXEC:/home/note/note
EXPOSE 15252
