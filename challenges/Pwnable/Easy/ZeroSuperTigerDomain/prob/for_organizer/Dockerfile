FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e
RUN apt-get update -y
RUN apt-get install socat libzstd-dev -y
RUN adduser chall
WORKDIR /home/chall

USER chall
COPY ./chall ./chall
COPY ./flag.txt ./flag.txt

USER root
RUN chown -R root:root /home/chall
RUN chmod 755 /home/chall
RUN chmod 644 ./flag.txt
RUN chmod 755 ./chall

CMD ["socat", "TCP-LISTEN:15252,reuseaddr,fork", "EXEC:/home/chall/chall,su=chall,stderr"]
EXPOSE 15252
