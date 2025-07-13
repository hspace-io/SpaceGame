FROM varnish:latest

COPY default.vcl /etc/varnish/default.vcl

CMD ["varnishd", "-F", "-f", "/etc/varnish/default.vcl"]