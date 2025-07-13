FROM debian:bullseye

RUN DEBIAN_FRONTEND=noninteractive apt-get update && \
    apt-get install -y \
        nginx \
        php-fpm \
        gcc \
    && rm -rf /var/lib/apt/lists/
RUN ln -sf /usr/share/zoneinfo/Asia/Seoul /etc/localtime 


COPY setting/default /etc/nginx/sites-enabled/default
COPY setting/www.conf /etc/php/7.4/fpm/pool.d/www.conf

RUN rm -rf /var/www/html/*
COPY ./html /var/www/html
COPY flag.c /var/www/html/flag.c
RUN gcc -o /flag /var/www/html/flag.c && \
    chmod 111 /flag && \
    rm /var/www/html/flag.c

RUN chown -R root:root /tmp /var/tmp /var/lib/php/sessions && \
    chmod -R 000 /tmp /var/tmp /var/lib/php/sessions

RUN ln -sf /dev/stdout /var/log/nginx/access.log && \
    ln -sf /dev/stderr /var/log/nginx/error.log

EXPOSE 80
CMD /etc/init.d/php7.4-fpm start && nginx -g 'daemon off;' 
