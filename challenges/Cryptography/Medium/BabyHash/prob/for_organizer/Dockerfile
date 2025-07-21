# NO_DIGEST_PIN
FROM python:3

RUN apt-get update; \
    apt-get install -y socat

ENV user ctf
ENV port 1971
EXPOSE 1971

RUN adduser --disabled-password --gecos "" $user

WORKDIR /home/$user/
RUN pip install pycryptodome
COPY prob.py /home/$user/
COPY flag.txt /home/$user/

CMD socat \
    TCP-LISTEN:$port,reuseaddr,fork \
    EXEC:"python3 prob.py"
