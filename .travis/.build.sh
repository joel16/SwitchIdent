#!/bin/sh
set -e
set -x

make clean && make
rm .gitattributes .travis.yml devkitpro-pacman.deb icon.jpg LICENSE Makefile README.md
rm -rf .travis build common include romfs source SwitchIdent.elf SwitchIdent.nacp SwitchIdent.nso SwitchIdent.pfs0
