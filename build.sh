#!/bin/bash

set -e

make
make clean
insmod get_packet.ko

mknod mychardev0 c 696 0
chmod 666 mychardev0