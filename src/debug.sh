#!/bin/bash

GDB=$([ $(uname -s) = "Darwin" ] && echo "i386-elf-gdb" || echo "gdb")

urxvt -e qemu-system-i386 -m 256 -drive format=raw,file=./img/os.img -no-shutdown -no-reboot -s -S -curses &
$GDB ./img/bootimg -command gdb.init
