#!/bin/sh
make clean
make
pintos --filesys-size=2 -p ../examples/echo -a echo -- -f -q run 'echo x'
