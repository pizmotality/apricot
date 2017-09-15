#!/bin/bash

GDB=$([ $(uname -s) = "Darwin" ] && echo "i386-elf-gdb" || echo "gdb")

qemu-system-x86_64 -cpu athlon -m 256 -drive format=raw,file=./img/os.img -no-shutdown -no-reboot -s -S &
$GDB ./img/bootimg -command gdb.init
