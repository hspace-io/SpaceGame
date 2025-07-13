vcl 4.0;

backend default {
    .host = "app";
    .port = "8000";
}

acl whitelist {
    "localhost";
}

sub vcl_recv {
    if ( req.url ~ "^/healthcheck" && !(client.ip ~ whitelist) ) {
        return (synth(403, "Forbidden"));
    }
}

sub vcl_recv {
    if (req.http.upgrade ~ "(?i)websocket") {
        return (pipe);
    }
}

sub vcl_pipe {
    if (req.http.upgrade) {
        set bereq.http.upgrade = req.http.upgrade;
        set bereq.http.connection = req.http.connection;
    }
}