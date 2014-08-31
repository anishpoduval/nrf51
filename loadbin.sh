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
" >loadbin.jli

JLinkExe -if SWD -device nRF51822 -speed 1000 loadbin.jli
rm loadbin.jli loadbin.bin
