#!/bin/sh
set -e
set -x

make clean && make
mv gui/gui.nro ../
mv console/console.nro ../
rm .gitattributes .travis.yml devkitpro-pacman.deb LICENSE Makefile README.md
rm -rf common console gui resources source