FROM python:3.10-slim-bullseye

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Seoul
Workdir /app

RUN apt update \
 && apt install -y wget gnupg \
 && wget -q -O - https://dl-ssl.google.com/linux/linux_signing_key.pub | apt-key add - \ 
 && echo "deb http://dl.google.com/linux/chrome/deb/ stable main" >> /etc/apt/sources.list.d/google.list \
 && apt update && apt install -y google-chrome-stable \
 && rm -rf /var/lib/apt/lists/*

COPY ./app .
RUN pip3 install -r requirements.txt
EXPOSE 80

CMD ["python3", "app.py"]
