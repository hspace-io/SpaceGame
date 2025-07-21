#!/bin/sh
/etc/init.d/xinetd restart

/bin/bash 
socat TCP-LISTEN:9876,reuseaddr,fork EXEC:'su pwn_user -c /pwn-wasd_space/wasd_space'
sleep infinity