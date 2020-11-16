#!/bin/bash

stty 9600 -F /dev/ttyUSB0 raw -echo
echo "" > test
cat /dev/ttyUSB0 >> test &
last_pid=$!
sleep 1
echo sg>>/dev/ttyUSB0
sleep 7
kill -KILL $last_pid
cat test
cat test
rm test
