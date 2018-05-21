#!/bin/sh
set -e
set -x

make clean && make
rm .gitattributes .travis.yml devkitpro-pacman.deb icon.jpg LICENSE Makefile README.md SwitchIdent.elf SwitchIdent.nacp SwitchIdent.nso SwitchIdent.pfs0 
rm -rf build include resources source