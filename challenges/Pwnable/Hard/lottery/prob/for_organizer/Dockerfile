FROM ubuntu:22.04@sha256:77906da86b60585ce12215807090eb327e7386c8fafb5402369e421f44eff17e
RUN apt-get update -y
RUN apt-get install socat -y
RUN adduser lottery
WORKDIR /home/lottery

USER lottery
COPY ./lottery ./lottery
COPY ./flag.txt ./flag.txt
COPY ./helper ./helper
COPY ./start.sh ./start.sh

USER root
RUN chown -R root:root /home/lottery
RUN chmod 755 /home/lottery
RUN chmod 644 ./flag.txt
RUN chmod 744 ./start.sh
RUN chmod 755 ./lottery
RUN chmod 755 ./helper

CMD ./start.sh
EXPOSE 15252
EXPOSE 15353
