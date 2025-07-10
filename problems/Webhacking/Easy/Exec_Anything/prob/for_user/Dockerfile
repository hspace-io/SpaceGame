FROM php:8.0-apache

WORKDIR /var/www/html

COPY index.php /var/www/html/

COPY 000-default.conf /etc/apache2/sites-available/

RUN a2enmod rewrite

EXPOSE 80