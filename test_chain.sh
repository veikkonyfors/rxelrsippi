#!/bin/bash

##
# test_chain.sh Test the whole chain
# 
# If txelrsip stopped with CTRL-C (instead of CTRL-D) will leave rxelrsippi process running
##

killall -u pappa socat txelrsip rxelrsippi 2>/dev/null

socat -d -d PTY,link=/tmp/ttyV0,raw,echo=0 PTY,link=/tmp/ttyV1,raw,echo=0 &
sleep 1 

./build/cmake.run.linux.x86_64.Local/rxelrsippi -p 22777 -t /tmp/ttyV0 & # Start receiving from UDP and writing to UART

cat /tmp/ttyV1 | hexdump -C &

#./oma_lukija -p /tmp/ttyV1 & # Read from UART and dump

../txelrsip/build/cmake.run.linux.x86_64.Local/txelrsip -i 127.0.0.1 -p 22777 # Start transmitting

killall -u pappa socat txelrsip rxelrsippi 2>/dev/null
