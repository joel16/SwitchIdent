#!/bin/sh
set -e
set -x

# Build and install devkitARM + ctrulib
mkdir -p $DEVKITPRO
cd $DEVKITPRO
wget -N https://raw.githubusercontent.com/devkitPro/installer/master/perl/devkitA64update.pl
chmod +x ./devkitA64update.pl
./devkitA64update.pl

# Get latest libnx and overwrite bundled one
mkdir libnx-update && cd libnx-update
git clone https://github.com/switchbrew/libnx.git
cd libnx/
make && make install
cd ../../
rm -rf libnx-update/

rm *.bz2
rm devkitA64update.pl