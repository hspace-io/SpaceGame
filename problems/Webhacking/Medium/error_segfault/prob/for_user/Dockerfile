FROM php:7.0-apache

RUN sed -i 's/deb.debian.org/archive.debian.org/g' /etc/apt/sources.list && \
    sed -i '/security.debian.org/d' /etc/apt/sources.list && \
    sed -i '/stretch-updates/d' /etc/apt/sources.list && \
    echo 'Acquire::Check-Valid-Until "false";' > /etc/apt/apt.conf.d/99no-check-valid-until

RUN apt update && apt install vim -y

RUN rm -rf /var/www/html/index.html
RUN a2enmod rewrite

COPY ./flag.txt /var/www/html/flag.txt
COPY ./src /var/www/html/
COPY ./init/php.ini /usr/local/etc/php/php.ini
COPY ./init/000-default.conf /etc/apache2/sites-available/000-default.conf
COPY ./init/apache2.conf /etc/apache2/apache2.conf

CMD ["/bin/bash", "-c", "source /etc/apache2/envvars && exec /usr/sbin/apache2 -D FOREGROUND"]
