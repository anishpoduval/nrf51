#!/bin/sh

SRC_ELF=$1

arm-none-eabi-objcopy -O binary "$SRC_ELF"  loadbin.bin

#w4 4001e504 1

echo "
w4 4001e504 2
w4 4001e50c 1
w4 4001e514 1
sleep 100
r
loadbin loadbin.bin 0x0
r
g
" >loadbin.jl1

echo "
si 1
device nRF51822_xxAA
rx 100
halt
w4 4001e504 1
erase
sleep 1000
loadbin load.bin 0
rx 100
g
" >loadbin.jl2

JLinkExe -if SWD -device nRF51822_xxAA -speed 1000 loadbin.jl1
rm loadbin.jl1 loadbin.jl2 loadbin.bin
