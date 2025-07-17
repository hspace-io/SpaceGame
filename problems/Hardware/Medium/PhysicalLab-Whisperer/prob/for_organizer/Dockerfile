FROM ubuntu:22.04

ENV USER prob
ENV PORT 55666

RUN apt-get update \
    && apt-get install -y socat

RUN adduser --disabled-password --gecos "" $USER \
 && chown -R root:$USER /home/$USER && chmod 750 /home/$USER 

COPY --chown=root:$USER ./prob /home/$USER/prob
COPY --chown=root:$USER ./run.sh /home/$USER/run.sh

RUN chmod 550 /home/$USER/run.sh
RUN chmod 550 /home/$USER/prob
WORKDIR /home/$USER

USER root
EXPOSE $PORT

CMD socat TCP-LISTEN:$PORT,reuseaddr,fork EXEC:/home/$USER/run.sh,su=$USER
