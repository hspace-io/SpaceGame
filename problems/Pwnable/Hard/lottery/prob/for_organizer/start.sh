#!/bin/bash
socat -T 300 TCP-LISTEN:15252,reuseaddr,fork EXEC:/home/lottery/lottery,su=lottery,stderr &
socat -T 5 TCP-LISTEN:15353,reuseaddr,fork EXEC:/home/lottery/helper,su=lottery,stderr
