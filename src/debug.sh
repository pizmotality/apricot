#!/bin/bash

GDB=$([ $(uname -s) = "Darwin" ] && echo "i386-elf-gdb" || echo "gdb")

qemu-system-i386 -m 256 -drive format=raw,file=./img/os.img -no-shutdown -no-reboot -s -S &
$GDB ./img/bootimg -command gdb.init
