#!/bin/sh

rm -rf initrd sdcard
mkdir -p initrd sdcard
cd initrd
mkdir dev lib proc sys
cd ..

rm -rf kl1
mkdir kl1
cd kl1
curl -s -L -o kl1.deb http://mirrordirector.raspbian.org/raspbian/pool/main/k/klibc/`curl -sL http://mirrordirector.raspbian.org/raspbian/pool/main/k/klibc/ | grep 'klibc-utils_' | sort | tail -n 1 | tr '<' '\n' | grep '^a href="' | grep -F '.deb"' | cut -d '"' -f 2`
ar x kl1.deb
rm debian-binary control*
xzcat data.tar.xz | tar xf -
mv usr/lib/klibc/bin/ipconfig ../initrd/
cd ..
rm -rf kl1

rm -rf kl2
mkdir kl2
cd kl2
curl -s -L -o kl2.deb http://mirrordirector.raspbian.org/raspbian/pool/main/k/klibc/`curl -sL http://mirrordirector.raspbian.org/raspbian/pool/main/k/klibc/ | grep 'libklibc_' | sort | tail -n 1 | tr '<' '\n' | grep '^a href="' | grep -F '.deb"' | cut -d '"' -f 2`
ar x kl2.deb
rm debian-binary control*
xzcat data.tar.xz | tar xf -
mv lib/* ../initrd/lib/
cd ..
rm -rf kl2

CCPREFIX="$HOME/arm-none-linux-gnueabi/bin/arm-none-linux-gnueabi-" EXTRAFLAGS="-Wno-unused-parameter -DEMBEDDED -static" ./make.sh

cd initrd
mv ../proxydns2 init
chown -R 0:0 .
chmod -R 0755 .
find . | cpio -H newc -o > ../sdcard/initrd
gzip ../sdcard/initrd
cd ..
rm -rf initrd

curl -L -o fw.zip https://github.com/Hexxeh/rpi-firmware/archive/master.zip
unzip -q -o fw.zip
mv rpi-firmware-master/* sdcard/
rm -rf fw.zip rpi-firmware-master
cd sdcard
rm -rf modules overlays vc *.symvers start{,_db,_x}.elf git_hash *.md fixup{,_db,_x}.dat
cat << EOF > config.txt
gpu_mem=16
boot_delay=0
disable_splash=1
initramfs initrd.gz followkernel
EOF

echo 'logo.nologo maxcpus=2 elevator=noop nomodule panic=30 oops=panic consoleblank=0 smsc95xx.turbo_mode=N root=/dev/ram0 rootwait init=/init devtmpfs.mount=1 quiet hibernate=no ro' > cmdline.txt

cd ..
chmod -R 0755 .
chown -R $(logname):$(sudo -u $(logname) groups | cut -d ' ' -f 1) .

cd ..
echo Done