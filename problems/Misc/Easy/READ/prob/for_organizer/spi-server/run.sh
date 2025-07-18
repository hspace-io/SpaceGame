#!/bin/sh
sudo docker build -t spi-firmware .
sudo docker run -d -p 21346:21346 --name spi-container spi-firmware

