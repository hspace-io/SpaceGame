FROM python:3.12

# init packages
RUN apt update && apt upgrade -y
RUN apt install -y socat python3-pip nodejs
RUN rm -rf /var/lib/apt/lists/*
RUN apt remove curl wget -y

RUN adduser --disabled-password --gecos "" ctf

WORKDIR /chall

# requirements.txt
COPY requirements.txt .
RUN pip3 install --no-cache-dir -r requirements.txt

# builder for numpy, save in current directory
COPY build ./build
COPY setup.py .
RUN python3 setup.py build_ext --inplace

# deploy
COPY deploy .
RUN chmod -R 711 /chall
RUN chown -R root:root /chall/spell

EXPOSE 1337

# clean spells & run
CMD while true; do find /chall/spell -mindepth 1 -delete; sleep 30s; done & \
    socat -d TCP-LISTEN:1337,reuseaddr,fork EXEC:"python /chall/app.py"