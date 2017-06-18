#!/bin/bash

mkdir ./tmp

cp ./img/os.img ./tmp/os.img
dd skip=63 bs=512 if=./tmp/os.img of=./tmp/tmp.img

cat > debugfs_commands <<EOF
rm bootimg
rm fsimg
write ./img/bootimg bootimg
write ./img/fsimg fsimg
EOF
debugfs -w -f debugfs_commands ./tmp/tmp.img

dd seek=63 bs=512 if=./tmp/tmp.img of=./tmp/os.img
cp ./tmp/os.img ./img/os.img

rm debugfs_commands
rm -r ./tmp
